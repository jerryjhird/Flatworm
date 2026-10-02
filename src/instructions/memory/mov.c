#include "bounds.h"
#include "cpu.h"
#include "mathop.h"

// format: destination -> source

// register -> register
void ins_mov_rr(vm_cpu *cpu, uint64_t dst, uint64_t src) {
	uint64_t val = 0;
	if (!read_reg(cpu, src, &val)) {
		return;
	}
	write_reg(cpu, dst, val);
}

// memory -> register
void ins_mov_rm(vm_cpu *cpu, uint64_t dst, uint64_t addr) {
	uint64_t val = 0;
	if (!read_le(cpu, addr, sizeof(uint64_t), &val)) {
		return;
	}
	write_reg(cpu, dst, val);
}

// register -> memory
void ins_mov_mr(vm_cpu *cpu, uint64_t addr, uint64_t src) {
	uint64_t val = 0;
	if (!read_reg(cpu, src, &val)) {
		return;
	}
	write_le(cpu, addr, sizeof(uint64_t), val);
}

// immediate value -> register.
// (does not follow format)
void ins_mov_imm(vm_cpu *cpu, uint64_t dst, uint64_t imm) {
	write_reg(cpu, dst, imm);
}

// immediate value -> memory
// (does not follow format)
void ins_mov_im(vm_cpu *cpu, uint64_t addr, uint64_t imm) {
	uint64_t val = imm;
	write_le(cpu, addr, sizeof(uint64_t), val);
}
