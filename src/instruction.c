#include <stdbool.h>
#include "instruction.h"
#include "interrupts.h"

typedef struct {
	uint64_t id;
	instruction1_handler handler;
} handler_entry1;

typedef struct {
	uint64_t id;
	instruction2_handler handler;
} handler_entry2;

typedef struct {
	uint64_t id;
	instruction3_handler handler;
} handler_entry3;

typedef struct {
	uint64_t id;
	instruction4_handler handler;
} handler_entry4;

#include "instructions/instruction_ids.h"
#include "instructions/memory/all.h"
#include "instructions/control/all.h"

static const handler_entry1 handlers1[] = {
	{halt_insid, ins_halt},
	{int_insid, ins_int},
	{0, NULL}
};

static const handler_entry2 handlers2[] = {
	{mov_rr_insid, ins_mov_rr},
	{mov_rm_insid, ins_mov_rm},
	{mov_mr_insid, ins_mov_mr},
	{mov_imm_insid, ins_mov_imm},
	{0, NULL}
};

static const handler_entry3 handlers3[] = {
	{bufin_insid, ins_bufin},
	{bufout_insid, ins_bufout},
	{0, NULL}
};

static const handler_entry4 handlers4[] = {
	{0, NULL}
};

#define DEF_LOOKUP(n) static instruction##n##_handler lookup##n(uint64_t id) { \
	for (int i = 0; handlers##n[i].handler != NULL; i++) { \
		if (handlers##n[i].id == id) { \
			return handlers##n[i].handler; \
		} \
	} \
	return NULL; \
}

DEF_LOOKUP(1)
DEF_LOOKUP(2)
DEF_LOOKUP(3)
DEF_LOOKUP(4)

// indexed by type bits
static const uint8_t arg_counts[4] = {1, 2, 4, 3};

static size_t bytes_needed(uint64_t v) {
	size_t n = 1;
	while (v >>= 8) {
		n++;
	}
	return n;
}

static bool read_le(vm_cpu *cpu, uint64_t addr, size_t n, uint64_t *out) {
	if (addr >= cpu->phys_memory->length || n > cpu->phys_memory->length - addr) {
		return false;
	}
	uint8_t *p = (uint8_t *)cpu->phys_memory->start + addr;
	uint64_t v = 0;
	for (size_t i = 0; i < n; i++) {
		v |= (uint64_t)p[i] << (i * 8);
	}
	*out = v;
	return true;
}

void vm_execute_instruction(vm_cpu *cpu) {
	uint64_t pc = cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value;
	uint64_t head;

	if (!read_le(cpu, pc, 1, &head)) {
		vm_fire_interrupt(cpu, 0);
		return;
	}

	uint32_t type = head & INSTRUCTION_TYPE_MASK;
	size_t id_len = ((head >> INSTRUCTION_LEN_SHIFT) & INSTRUCTION_LEN_MASK) + 1;
	size_t argc = arg_counts[type];
	uint64_t raw;

	if (!read_le(cpu, pc, id_len, &raw)) {
		vm_fire_interrupt(cpu, 0);
		return;
	}

	uint64_t id = raw >> (INSTRUCTION_HEADER_BITS + INSTRUCTION_ARGLEN_BITS * argc);
	uint64_t args[INSTRUCTION_MAX_ARGS] = {0};
	uint64_t off = id_len;

	for (size_t i = 0; i < argc; i++) {
		size_t len = ((raw >> (INSTRUCTION_HEADER_BITS + INSTRUCTION_ARGLEN_BITS * i)) & INSTRUCTION_ARGLEN_MASK) + 1;
		if (!read_le(cpu, pc + off, len, &args[i])) {
			vm_fire_interrupt(cpu, 0);
			return;
		}
		off += len;
	}

	cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value += off;

	switch (type) {
		case INSTRUCTION_ARG_1: {
			instruction1_handler found = lookup1(id);
			if (!found) {
				vm_fire_interrupt(cpu, 2);
				return;
			}
			found(cpu, args[0]);
			break;
		}
		case INSTRUCTION_ARG_2: {
			instruction2_handler found = lookup2(id);
			if (!found) {
				vm_fire_interrupt(cpu, 2);
				return;
			}
			found(cpu, args[0], args[1]);
			break;
		}
		case INSTRUCTION_ARG_3: {
			instruction3_handler found = lookup3(id);
			if (!found) {
				vm_fire_interrupt(cpu, 2);
				return;
			}
			found(cpu, args[0], args[1], args[2]);
			break;
		}
		case INSTRUCTION_ARG_4: {
			instruction4_handler found = lookup4(id);
			if (!found) {
				vm_fire_interrupt(cpu, 2);
				return;
			}
			found(cpu, args[0], args[1], args[2], args[3]);
			break;
		}
	}
}

size_t vm_encode_instruction(uint8_t *out, uint8_t type, uint64_t id, const uint64_t *args) {
	type &= INSTRUCTION_TYPE_MASK;
	size_t argc = arg_counts[type];
	size_t meta_bits = INSTRUCTION_HEADER_BITS + INSTRUCTION_ARGLEN_BITS * argc;

	if (id >> (64 - meta_bits)) {
		return 0;
	}

	uint64_t raw = type | (id << meta_bits);
	size_t arg_lens[INSTRUCTION_MAX_ARGS];

	for (size_t i = 0; i < argc; i++) {
		arg_lens[i] = bytes_needed(args[i]);
		raw |= (uint64_t)(arg_lens[i] - 1) << (INSTRUCTION_HEADER_BITS + INSTRUCTION_ARGLEN_BITS * i);
	}

	size_t id_len = bytes_needed(raw);
	raw |= (uint64_t)(id_len - 1) << INSTRUCTION_LEN_SHIFT;

	size_t n = 0;
	for (size_t i = 0; i < id_len; i++) {
		out[n++] = (uint8_t)(raw >> (i * 8));
	}
	for (size_t i = 0; i < argc; i++) {
		for (size_t j = 0; j < arg_lens[i]; j++) {
			out[n++] = (uint8_t)(args[i] >> (j * 8));
		}
	}
	return n;
}
