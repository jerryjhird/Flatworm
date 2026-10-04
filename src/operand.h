#pragma once

#include <stdint.h>
#include <stddef.h>
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

static inline uint64_t vm_size_mask(size_t size) {
	if (size >= sizeof(uint64_t)) {
		return UINT64_MAX;
	}

	return (1ULL << (size * 8)) - 1;
}

static inline bool vm_read_operand_chk(vm_cpu *cpu, vm_operand op, size_t size, uint64_t *out) {
	uint64_t val = 0;

	if (size == 0 || size > sizeof(uint64_t)) {
		vm_fire_interrupt(cpu, 13);
		return false;
	}

	switch (op.type) {
		case OP_TYPE_REG: {
			if (op.val >= GENERAL_ACCESS_REGISTER_COUNT) {
				vm_fire_interrupt(cpu, 13);
				return false;
			}

			val = cpu->registers.general_access_registers[op.val].value;
			break;
		}
		case OP_TYPE_IMM: {
			val = op.val;
			break;
		}
		case OP_TYPE_MEM: {
			if (!read_le(cpu, op.val, size, &val)) {
				vm_fire_interrupt(cpu, 14);
				return false;
			}

			break;
		}
		default: {
			vm_fire_interrupt(cpu, 13);
			return false;
		}
	}

	*out = val & vm_size_mask(size);
	return true;
}

static inline uint64_t vm_read_operand_sz(vm_cpu *cpu, vm_operand op, size_t size) {
	uint64_t out_val = 0;

	if (!vm_read_operand_chk(cpu, op, size, &out_val)) {
		return 0;
	}

	return out_val;
}

static inline bool vm_write_operand_sz(vm_cpu *cpu, vm_operand op, uint64_t val, size_t size) {
	if (size == 0 || size > sizeof(uint64_t)) {
		vm_fire_interrupt(cpu, 13);
		return false;
	}

	val &= vm_size_mask(size);

	switch (op.type) {
		case OP_TYPE_REG: {
			return write_reg(cpu, op.val, val);
		}
		case OP_TYPE_MEM: {
			return write_le(cpu, op.val, size, val);
		}
		case OP_TYPE_IMM:
		default: {
			vm_fire_interrupt(cpu, 13);
			return false;
		}
	}
}

// (defaults to 8 bytes / 64-bit)
static inline uint64_t vm_read_operand(vm_cpu *cpu, vm_operand op) {
	return vm_read_operand_sz(cpu, op, sizeof(uint64_t));
}

// (defaults to 8 bytes / 64-bit)
static inline bool vm_write_operand(vm_cpu *cpu, vm_operand op, uint64_t val) {
	return vm_write_operand_sz(cpu, op, val, sizeof(uint64_t));
}

// checks for address/imm or register holding address
static inline uint64_t vm_get_address(vm_cpu *cpu, vm_operand op) {
	if (op.type == OP_TYPE_MEM || op.type == OP_TYPE_IMM) {
		return op.val;
	} else if (op.type == OP_TYPE_REG) {
		return read_reg(cpu, op.val);
	}

	vm_fire_interrupt(cpu, 13);
	return 0;
}
