#include "cpu.h"
#include "operand.h"
#include "registers.h"

void ins_jmp(vm_cpu *cpu, vm_operand addr) {
	uint64_t target = vm_get_address(cpu, addr);
	cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value = target;
}

void ins_jz(vm_cpu *cpu, vm_operand src, vm_operand addr) {
	uint64_t val = vm_read_operand(cpu, src);

	if (val == 0) {
		uint64_t target = vm_get_address(cpu, addr);
		cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value = target;
	}
}
