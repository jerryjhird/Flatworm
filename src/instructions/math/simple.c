#include "bounds.h"
#include "cpu.h"

// format: src1, src2 -> destination

// add src1 and src2 into destination
void ins_add_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst) {
	uint64_t val1 = 0;
	uint64_t val2 = 0;
	if (!read_reg(cpu, src1, &val1) || !read_reg(cpu, src2, &val2)) {
		return;
	}
	write_reg(cpu, dst, val1 + val2);
}

// add immediate value to src1 into destination
void ins_add_imm(vm_cpu *cpu, uint64_t src1, uint64_t imm, uint64_t dst) {
	uint64_t val1 = 0;
	if (!read_reg(cpu, src1, &val1)) {
		return;
	}
	write_reg(cpu, dst, val1 + imm);
}

// subtract src2 from src1 into destination
void ins_sub_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst) {
	uint64_t val1 = 0;
	uint64_t val2 = 0;
	if (!read_reg(cpu, src1, &val1) || !read_reg(cpu, src2, &val2)) {
		return;
	}
	write_reg(cpu, dst, val1 - val2);
}

// subtract immediate value from src1 into destination
void ins_sub_imm(vm_cpu *cpu, uint64_t src1, uint64_t imm, uint64_t dst) {
	uint64_t val1 = 0;
	if (!read_reg(cpu, src1, &val1)) {
		return;
	}
	write_reg(cpu, dst, val1 - imm);
}

// multiply src1 by src2 into destination
void ins_mul_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst) {
	uint64_t val1 = 0;
	uint64_t val2 = 0;
	if (!read_reg(cpu, src1, &val1) || !read_reg(cpu, src2, &val2)) {
		return;
	}
	write_reg(cpu, dst, val1 * val2);
}

// divide src1 by src2 into destination
void ins_div_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst) {
	uint64_t val1 = 0;
	uint64_t val2 = 0;
	if (!read_reg(cpu, src1, &val1) || !read_reg(cpu, src2, &val2)) {
		return;
	}
	if (val2 == 0) {
		vm_fire_interrupt(cpu, 0); // Divide By Zero
		return;
	}
	write_reg(cpu, dst, val1 / val2);
}
