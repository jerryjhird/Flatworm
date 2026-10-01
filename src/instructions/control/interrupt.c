#include "cpu.h"
#include "interrupts.h"

// fire interrupt
void ins_int(vm_cpu *cpu, uint64_t arg1) {
	vm_fire_interrupt(cpu, (uint8_t)arg1);
}
