#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "instruction.h"
#include "interrupts.h"
#include "registers.h"
#include "instructions.h"
#include "bounds.h"

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

void vm_execute_instruction(vm_cpu *cpu) {
	uint64_t pc = cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value;
	uint64_t head;

	if (!read_le(cpu, pc, 1, &head)) {
		vm_fire_interrupt(cpu, 14);
		return;
	}

	uint32_t type = head & INSTRUCTION_TYPE_MASK;
	size_t id_len = ((head >> INSTRUCTION_LEN_SHIFT) & INSTRUCTION_LEN_MASK) + 1;
	size_t argc = arg_counts[type];
	uint64_t raw;

	if (!read_le(cpu, pc, id_len, &raw)) {
		vm_fire_interrupt(cpu, 14);
		return;
	}

	uint64_t id = raw >> (INSTRUCTION_HEADER_BITS + (ARG_TYPE_BITS + ARG_LEN_BITS) * argc);

	vm_operand args[INSTRUCTION_MAX_ARGS] = {0};
	uint64_t off = id_len;

	for (size_t i = 0; i < argc; i++) {
		uint64_t arg_desc = (raw >> (INSTRUCTION_HEADER_BITS + ((ARG_TYPE_BITS + ARG_LEN_BITS) * i)));

		vm_operand_type op_type = (vm_operand_type)((arg_desc >> ARG_LEN_BITS) & ARG_TYPE_MASK);
		size_t len = (arg_desc & ARG_LEN_MASK) + 1;

		uint64_t val = 0;
		if (!read_le(cpu, pc + off, len, &val)) {
			vm_fire_interrupt(cpu, 14);
			return;
		}

		args[i] = (vm_operand){.type = op_type, .val = val};
		off += len;
	}

	cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value += off;

	switch (type) {
		case INSTRUCTION_ARG_1: {
			instruction1_handler found = lookup1(id);
			if (!found) { vm_fire_interrupt(cpu, 6); return; }
			found(cpu, args[0]);
			break;
		}
		case INSTRUCTION_ARG_2: {
			instruction2_handler found = lookup2(id);
			if (!found) { vm_fire_interrupt(cpu, 6); return; }
			found(cpu, args[0], args[1]);
			break;
		}
		case INSTRUCTION_ARG_3: {
			instruction3_handler found = lookup3(id);
			if (!found) { vm_fire_interrupt(cpu, 6); return; }
			found(cpu, args[0], args[1], args[2]);
			break;
		}
		case INSTRUCTION_ARG_4: {
			instruction4_handler found = lookup4(id);
			if (!found) { vm_fire_interrupt(cpu, 6); return; }
			found(cpu, args[0], args[1], args[2], args[3]);
			break;
		}
	}

	char *debug_env = getenv("DEBUG");
	if (debug_env && (strcmp(debug_env, "true") == 0 || strcmp(debug_env, "1") == 0)) {
		fprintf(stderr, "[DEBUG] PC: 0x%016lx | Type: ARG_%u | ID: %lu | Args: ", pc, type + 1, id);
		for (size_t i = 0; i < argc; i++) {
			const char *type_str = "UNKNOWN";
			if (args[i].type == OP_TYPE_REG) type_str = "REG";
			else if (args[i].type == OP_TYPE_IMM) type_str = "IMM";
			else if (args[i].type == OP_TYPE_MEM) type_str = "MEM";

			fprintf(stderr, "[%s: 0x%lx] ", type_str, args[i].val);
		}
		fprintf(stderr, "\n");
	}
}
