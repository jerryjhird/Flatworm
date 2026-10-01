#include "halt.h"

#include "cpu.h"

void ins_halt(vm_cpu *cpu, uint64_t arg1) {
	(void)arg1;
	cpu->is_halted = true;
}
