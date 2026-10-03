#include "cpu.h"
#include "operand.h"

void ins_halt(vm_cpu *cpu, vm_operand arg1) {
	UNUSED(arg1);

	cpu->is_halted = true;
}
