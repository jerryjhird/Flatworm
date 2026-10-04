#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "instruction.h"
#include "registers.h"
#include "instructions.h"
#include "mathop.h"

#define REG_PREFIX 'r'

typedef struct {
	char *name;
	uint64_t offset;
} label_t;

static label_t *labels = NULL;
static size_t label_count = 0;
static size_t label_capacity = 0;

static void free_labels(void) {
	for (size_t i = 0; i < label_count; i++) {
		superv_free(labels[i].name);
	}

	superv_free(labels);

	labels = NULL;
	label_count = 0;
	label_capacity = 0;
}

static void add_label(const char *name, uint64_t offset) {
	size_t len = strlen(name);
	char *clean_name = superv_malloc(len + 1);

	if (!clean_name) {
		fprintf(stderr, "error: memory allocation failed\n");
		exit(1);
	}

	strcpy(clean_name, name);

	if (len > 0 && clean_name[len - 1] == ':') { clean_name[len - 1] = '\0'; }

	for (size_t i = 0; i < label_count; i++) {
		if (strcmp(labels[i].name, clean_name) == 0) {
			fprintf(stderr, "error: duplicate label %s\n", clean_name);
			superv_free(clean_name);
			exit(1);
		}
	}

	if (label_count >= label_capacity) {
		label_capacity = label_capacity == 0 ? 16 : label_capacity * 2;
		label_t *new_labels = realloc(labels, label_capacity * sizeof(label_t));

		if (!new_labels) {
			fprintf(stderr, "error: memory allocation failed\n");
			superv_free(clean_name);
			exit(1);
		}

		labels = new_labels;
	}

	labels[label_count].name = clean_name;
	labels[label_count].offset = offset;
	label_count++;
}

static bool find_label(const char *name, uint64_t *out_offset) {
	if (!name) { return false; }

	size_t len = strlen(name);
	char *clean_name = superv_malloc(len + 1);

	if (!clean_name) { return false; }

	strcpy(clean_name, name);

	if (len > 0 && clean_name[len - 1] == ':') { clean_name[len - 1] = '\0'; }

	for (size_t i = 0; i < label_count; i++) {
		if (labels[i].name && strcmp(labels[i].name, clean_name) == 0) {
			*out_offset = labels[i].offset;
			superv_free(clean_name);
			return true;
		}
	}

	superv_free(clean_name);
	return false;
}

static vm_operand parse_operand(const char *op) {
	vm_operand operand = {0, 0};
	if (!op) { return operand; }

	// check if its a gpr (by checking if it starts with REG_PREFIX)
	if (op[0] == REG_PREFIX && op[1] >= '0' && op[1] <= '9') {
		operand.type = OP_TYPE_REG;
		operand.val = strtoull(op + 1, NULL, 0);
		return operand;
	}

	// check special register table
	for (size_t i = 0; i < sizeof(SPECIAL_REGISTERS) / sizeof(SPECIAL_REGISTERS[0]); i++) {
		if (strcmp(op, SPECIAL_REGISTERS[i].name) == 0) {
			operand.type = OP_TYPE_REG;
			operand.val = SPECIAL_REGISTERS[i].id;
			return operand;
		}
	}

	// check for a register indirect memory addr like [r1]
	size_t len = strlen(op);
	if (len >= 3 && op[0] == '[' && op[1] == REG_PREFIX && op[len - 1] == ']') {
		char reg_buf[32];
		size_t reg_len = len - 3; // subtract [ and ]
		if (reg_len < sizeof(reg_buf)) {
			memcpy(reg_buf, op + 2, reg_len);
			reg_buf[reg_len] = '\0';
			operand.type = OP_TYPE_MEM;
			operand.val = strtoull(reg_buf, NULL, 0);
			return operand;
		}
	}

	// check memory addr enclosed in brackets like [0x1000] or [label]
	if (len >= 2 && op[0] == '[' && op[len - 1] == ']') {
		char inner[256];
		size_t inner_len = len - 2;
		if (inner_len < sizeof(inner)) {
			memcpy(inner, op + 1, inner_len);
			inner[inner_len] = '\0';

			uint64_t label_offset = 0;
			if (find_label(inner, &label_offset)) {
				operand.type = OP_TYPE_MEM;
				operand.val = label_offset;
				return operand;
			}

			operand.type = OP_TYPE_MEM;
			operand.val = strtoull(inner, NULL, 0);
			return operand;
		}
	}

	// check for immediate label
	uint64_t label_offset = 0;
	if (find_label(op, &label_offset)) {
		operand.type = OP_TYPE_IMM;
		operand.val = label_offset;
		return operand;
	}

	// default to immediate value
	operand.type = OP_TYPE_IMM;
	operand.val = strtoull(op, NULL, 0);
	return operand;
}

#define CHECK_TABLE(table, type_val, args_val) \
	for (int i = 0; table[i].mnemonic != NULL; i++) { \
		if (strcmp(table[i].mnemonic, mnemonic) == 0) { \
			*out_type = type_val; \
			*out_id = table[i].id; \
			*out_args = args_val; \
			return true; \
		} \
	}

static bool lookup_mnemonic(const char *mnemonic, uint8_t *out_type, uint64_t *out_id, int *out_args) {
	CHECK_TABLE(handlers1, 0, arg_counts[0])
	CHECK_TABLE(handlers2, 1, arg_counts[1])
	CHECK_TABLE(handlers3, 2, arg_counts[2])
	CHECK_TABLE(handlers4, 3, arg_counts[3])
	return false;
}

size_t encode_instruction(uint8_t *out, uint8_t type, uint64_t id, const vm_operand *args) {
	type &= INSTRUCTION_TYPE_MASK;
	size_t argc = arg_counts[type];

	// total bits for header + descriptors
	size_t meta_bits = INSTRUCTION_HEADER_BITS + (ARG_TYPE_BITS + ARG_LEN_BITS) * argc;

	if (id >> (64 - meta_bits)) {
		return 0;
	}

	// initialize raw header with instruction type
	uint64_t raw = type;
	size_t arg_lens[INSTRUCTION_MAX_ARGS];

	for (size_t i = 0; i < argc; i++) {
		arg_lens[i] = bytes_needed(args[i].val);
		// ensure length fits within ARG_LEN_BITS
		uint64_t encoded_len = (arg_lens[i] > 0 ? arg_lens[i] - 1 : 0);
		uint64_t arg_desc = ((uint64_t)args[i].type << ARG_LEN_BITS) | (encoded_len & ARG_LEN_MASK);

		raw |= (arg_desc << (INSTRUCTION_HEADER_BITS + ((ARG_TYPE_BITS + ARG_LEN_BITS) * i)));
	}

	// pack instruction ID above argument descriptors
	raw |= (id << meta_bits);

	// total header size in bytes
	size_t id_len = bytes_needed(raw);
	if (id_len > 8) {
		id_len = 8; // cap header size to 64 bit word max
	}

	// insert header length (minus 1) into header bits shift position
	raw &= ~(INSTRUCTION_LEN_MASK << INSTRUCTION_LEN_SHIFT);
	raw |= ((uint64_t)(id_len - 1) & INSTRUCTION_LEN_MASK) << INSTRUCTION_LEN_SHIFT;

	size_t n = 0;
	for (size_t i = 0; i < id_len; i++) {
		out[n++] = (uint8_t)(raw >> (i * 8));
	}

	// append actual operand payload bytes
	for (size_t i = 0; i < argc; i++) {
		for (size_t j = 0; j < arg_lens[i]; j++) {
			out[n++] = (uint8_t)(args[i].val >> (j * 8));
		}
	}
	return n;
}

static char *read_line(FILE *fin) {
	size_t cap = 128;
	size_t len = 0;
	char *buf = superv_malloc(cap);
	if (!buf) {
		return NULL;
	}

	int c;
	while ((c = fgetc(fin)) != EOF && c != '\n') {
		if (len + 1 >= cap) {
			cap *= 2;
			char *new_buf = realloc(buf, cap);

			if (!new_buf) {
				superv_free(buf);
				return NULL;
			}

			buf = new_buf;
		}
		buf[len++] = (char)c;
	}

	if (c == EOF && len == 0) {
		superv_free(buf);
		return NULL;
	}

	buf[len] = '\0';
	return buf;
}

typedef struct {
	char **tokens;
	size_t count;
} token_list_t;

static token_list_t tokenize_line(const char *line) {
	token_list_t tlist = {NULL, 0};
	if (!line) {
		return tlist;
	}
	size_t cap = 0;
	const char *p = line;
	while (*p) {
		while (*p == ' ' || *p == '\t' || *p == '\r' || *p == ',') {
			p++;
		}
		if (*p == '\0' || *p == '#' || *p == ';') {
			break; // stop at comments
		}

		const char *start = p;
		while (*p && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n' && *p != ',' && *p != '#' && *p != ';') {
			p++;
		}

		size_t len = p - start;
		if (len == 0) {
			break;
		}

		char *token = superv_malloc(len + 1);
		if (!token) {
			break;
		}
		memcpy(token, start, len);
		token[len] = '\0';

		if (tlist.count >= cap) {
			cap = cap == 0 ? 4 : cap * 2;
			char **new_tokens = realloc(tlist.tokens, cap * sizeof(char *));

			if (!new_tokens) {
				superv_free(token);
				break;
			}

			tlist.tokens = new_tokens;
		}
		tlist.tokens[tlist.count++] = token;
	}
	return tlist;
}

static void free_tokens(token_list_t *tlist) {
	if (!tlist) {
		return;
	}

	for (size_t i = 0; i < tlist->count; i++) {
		superv_free(tlist->tokens[i]);
	}

	superv_free(tlist->tokens);
	tlist->tokens = NULL;
	tlist->count = 0;
}

void main_compile(const char *input_filename, const char *output_filename) {
	if (!output_filename) {
		output_filename = "out.bin";
	}

	FILE *fin = fopen(input_filename, "r");

	if (!fin) {
		fprintf(stderr, "error: failed to open input file\n");
		exit(1);
	}

	uint64_t current_offset = 0;
	free_labels();

	char *line;
	while ((line = read_line(fin)) != NULL) {
		token_list_t tokens = tokenize_line(line);

		if (tokens.count == 0) {
			free_tokens(&tokens);
			superv_free(line);
			continue;
		}

		// is line label definiton?
		if (tokens.count == 1 && strchr(tokens.tokens[0], ':') != NULL) {
			add_label(tokens.tokens[0], current_offset);
			free_tokens(&tokens);
			superv_free(line);
			continue;
		}

		const char *mnemonic = tokens.tokens[0];
		uint8_t type = 0;
		uint64_t id = 0;
		int expected_args = 0;

		if (!lookup_mnemonic(mnemonic, &type, &id, &expected_args)) {
			fprintf(stderr, "error: unknown mnemonic %s\n", mnemonic);
			free_tokens(&tokens);
			superv_free(line);
			fclose(fin);
			free_labels();
			exit(1);
		}

		// measure size in pass one
		int arg_count = (int)tokens.count - 1;

		if (arg_count != expected_args) {
			fprintf(stderr, "error: %s expects %d operands, got %d\n", mnemonic, expected_args, arg_count);
			free_tokens(&tokens);
			superv_free(line);
			fclose(fin);
			free_labels();
			exit(1);
		}

		vm_operand dummy_args[4] = {{0, 0}};

		for (int i = 0; i < arg_count && i < 4; i++) {
			dummy_args[i] = parse_operand(tokens.tokens[i + 1]);
		}

		uint8_t encoded[32];
		size_t encoded_len = encode_instruction(encoded, type, id, dummy_args);

		if (encoded_len == 0) {
			fprintf(stderr, "error: failed to encode instruction %s during pass one\n", mnemonic);
			free_tokens(&tokens);
			superv_free(line);
			fclose(fin);
			free_labels();
			exit(1);
		}

		current_offset += encoded_len;
		free_tokens(&tokens);
		superv_free(line);
	}

	fclose(fin);

	fin = fopen(input_filename, "r");

	if (!fin) {
		fprintf(stderr, "error: failed to reopen input file\n");
		free_labels();
		exit(1);
	}

	FILE *fout = fopen(output_filename, "wb");

	if (!fout) {
		fprintf(stderr, "error: failed to open out file\n");
		fclose(fin);
		free_labels();
		exit(1);
	}

	size_t program_size = 0;

	while ((line = read_line(fin)) != NULL) {
		token_list_t tokens = tokenize_line(line);

		if (tokens.count == 0) {
			free_tokens(&tokens);
			superv_free(line);
			continue;
		}

		// skip labels in pass two
		if (tokens.count == 1 && strchr(tokens.tokens[0], ':') != NULL) {
			free_tokens(&tokens);
			superv_free(line);
			continue;
		}

		const char *mnemonic = tokens.tokens[0];
		uint8_t type = 0;
		uint64_t id = 0;
		int expected_args = 0;

		if (!lookup_mnemonic(mnemonic, &type, &id, &expected_args)) {
			free_tokens(&tokens);
			superv_free(line);
			fclose(fin);
			fclose(fout);
			free_labels();
			exit(1);
		}

		int arg_count = (int)tokens.count - 1;
		vm_operand args[4] = {{0, 0}};
		for (int i = 0; i < arg_count && i < 4; i++) {
			args[i] = parse_operand(tokens.tokens[i + 1]);
		}

		uint8_t encoded[32];
		size_t encoded_len = encode_instruction(encoded, type, id, args);
		if (encoded_len == 0) {
			fprintf(stderr, "error: failed to encode instruction %s during pass two\n", mnemonic);
			free_tokens(&tokens);
			superv_free(line);
			fclose(fin);
			fclose(fout);
			free_labels();
			exit(1);
		}

		fwrite(encoded, 1, encoded_len, fout);
		program_size += encoded_len;

		free_tokens(&tokens);
		superv_free(line);
	}

	fclose(fin);
	fclose(fout);
	free_labels();
	printf("compiled %s to %s successfully (size %zu bytes)\n", input_filename, output_filename, program_size);
}
