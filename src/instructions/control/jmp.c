#include "bounds.h"
#include "cpu.h"

void ins_jmp(vm_cpu *cpu, uint64_t target) {
	cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value = target;
}

void ins_jz(vm_cpu *cpu, uint64_t reg, uint64_t target) {
	uint64_t val = 0;
	if (!read_reg(cpu, reg, &val)) {
		return;
	}
	if (val == 0) {
		cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value = target;
	}
}
