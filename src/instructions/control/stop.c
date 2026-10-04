#include "cpu.h"
#include "operand.h"

void ins_stop(vm_cpu *cpu, vm_operand arg1) {
	UNUSED(arg1);

	cpu->stop = true;
}
