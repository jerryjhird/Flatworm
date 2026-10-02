#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "instruction.h"
#include "interrupts.h"
#include "registers.h"
#include "instructions.h"
#include "bounds.h"

// linear
#define DEF_LOOKUP(n) static instruction##n##_handler lookup##n(uint64_t id) { \
	if (id == 0) return NULL; \
	return handlers##n[id - 1].handler; \
}

// non linear
// #define DEF_LOOKUP(n) static instruction##n##_handler lookup##n(uint64_t id) { \
//	 for (int i = 0; handlers##n[i].handler != NULL; i++) { \
//		 if (handlers##n[i].id == id) { \
//			 return handlers##n[i].handler; \
//		 } \
//	 } \
//	 return NULL; \
// }

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

	uint64_t id = raw >> (INSTRUCTION_HEADER_BITS + INSTRUCTION_ARGLEN_BITS * argc);
	uint64_t args[INSTRUCTION_MAX_ARGS] = {0};
	uint64_t off = id_len;

	for (size_t i = 0; i < argc; i++) {
		size_t len = ((raw >> (INSTRUCTION_HEADER_BITS + INSTRUCTION_ARGLEN_BITS * i)) & INSTRUCTION_ARGLEN_MASK) + 1;
		if (!read_le(cpu, pc + off, len, &args[i])) {
			vm_fire_interrupt(cpu, 14);
			return;
		}
		off += len;
	}

	cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value += off;

	switch (type) {
		case INSTRUCTION_ARG_1: {
			instruction1_handler found = lookup1(id);
			if (!found) {
				vm_fire_interrupt(cpu, 6);
				return;
			}
			found(cpu, args[0]);
			break;
		}
		case INSTRUCTION_ARG_2: {
			instruction2_handler found = lookup2(id);
			if (!found) {
				vm_fire_interrupt(cpu, 6);
				return;
			}
			found(cpu, args[0], args[1]);
			break;
		}
		case INSTRUCTION_ARG_3: {
			instruction3_handler found = lookup3(id);
			if (!found) {
				vm_fire_interrupt(cpu, 6);
				return;
			}
			found(cpu, args[0], args[1], args[2]);
			break;
		}
		case INSTRUCTION_ARG_4: {
			instruction4_handler found = lookup4(id);
			if (!found) {
				vm_fire_interrupt(cpu, 6);
				return;
			}
			found(cpu, args[0], args[1], args[2], args[3]);
			break;
		}
	}
}
