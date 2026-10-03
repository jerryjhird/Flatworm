#include "cpu.h"
#include "operand.h"

void ins_mov(vm_cpu *cpu, vm_operand dst, vm_operand src) {
	uint64_t val = vm_read_operand(cpu, src);

	if (!vm_write_operand(cpu, dst, val)) {
		return;
	}
}
