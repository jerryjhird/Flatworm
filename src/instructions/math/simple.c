#include "cpu.h"
#include "operand.h"

// add src1 and src2 into dest
void ins_add(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst) {
	uint64_t val1 = vm_read_operand(cpu, src1);
	uint64_t val2 = vm_read_operand(cpu, src2);

	if (!vm_write_operand(cpu, dst, val1 + val2)) {
		return;
	}
}

// subtract src2 from src1 into dest
void ins_sub(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst) {
	uint64_t val1 = vm_read_operand(cpu, src1);
	uint64_t val2 = vm_read_operand(cpu, src2);

	if (!vm_write_operand(cpu, dst, val1 - val2)) {
		return;
	}
}

// multiply src1 by src2 into dest
void ins_mul(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst) {
	uint64_t val1 = vm_read_operand(cpu, src1);
	uint64_t val2 = vm_read_operand(cpu, src2);

	if (!vm_write_operand(cpu, dst, val1 * val2)) {
		return;
	}
}

// divide src1 by src2 into dest
void ins_div(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst) {
	uint64_t val1 = vm_read_operand(cpu, src1);
	uint64_t val2 = vm_read_operand(cpu, src2);

	if (val2 == 0) {
		vm_fire_interrupt(cpu, 0); // Divide By Zero
		return;
	}

	if (!vm_write_operand(cpu, dst, val1 / val2)) {
		return;
	}
}
