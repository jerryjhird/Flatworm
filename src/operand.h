#pragma once

#include <stdbool.h>
#include "bounds.h"

typedef enum {
	OP_TYPE_REG = 0,
	OP_TYPE_IMM = 1,
	OP_TYPE_MEM = 2,
} vm_operand_type;

typedef struct {
	vm_operand_type type;
	uint64_t val;
} vm_operand;

static inline uint64_t vm_read_operand(vm_cpu *cpu, vm_operand op) {
	uint64_t out_val = 0;
	switch (op.type) {
		case OP_TYPE_REG:
			out_val = read_reg(cpu, op.val);
			break;
		case OP_TYPE_IMM:
			out_val = op.val;
			break;
		case OP_TYPE_MEM:
			read_le(cpu, op.val, sizeof(uint64_t), &out_val);
			break;
		default:
			break;
	}
	return out_val;
}

static inline bool vm_write_operand(vm_cpu *cpu, vm_operand op, uint64_t val) {
	switch (op.type) {
		case OP_TYPE_REG:
			return write_reg(cpu, op.val, val);
		case OP_TYPE_MEM:
			return write_le(cpu, op.val, sizeof(uint64_t), val);
		case OP_TYPE_IMM:
		default:
			return false;
	}
}

// checks for address/imm or register holding address
static inline uint64_t vm_get_address(vm_cpu *cpu, vm_operand op) {
	if (op.type == OP_TYPE_MEM || op.type == OP_TYPE_IMM) {
		return op.val;
	} else if (op.type == OP_TYPE_REG) {
		return read_reg(cpu, op.val);
	}
	return 0;
}
