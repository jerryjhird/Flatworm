#include "cpu.h"
#include "operand.h"

static inline bool mov_sz(vm_cpu *cpu, vm_operand dst, vm_operand src, size_t size) {
	uint64_t val = 0;

	if (!vm_read_operand_chk(cpu, src, size, &val)) {
		return false;
	}

	return vm_write_operand_sz(cpu, dst, val, size);
}

// 1 byte / 8 bit
void ins_movb(vm_cpu *cpu, vm_operand dst, vm_operand src) {
	mov_sz(cpu, dst, src, 1);
}

// 2 byte / 16 bit
void ins_movw(vm_cpu *cpu, vm_operand dst, vm_operand src) {
	mov_sz(cpu, dst, src, 2);
}

// 4 byte / 32 bit
void ins_movl(vm_cpu *cpu, vm_operand dst, vm_operand src) {
	mov_sz(cpu, dst, src, 4);
}

// 8 byte / 64 bit
void ins_movq(vm_cpu *cpu, vm_operand dst, vm_operand src) {
	mov_sz(cpu, dst, src, 8);
}
