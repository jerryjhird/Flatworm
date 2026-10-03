#include "cpu.h"
#include "interrupts.h"
#include "operand.h"

// fire interrupt
void ins_int(vm_cpu *cpu, vm_operand vector) {
	uint64_t val = vm_read_operand(cpu, vector);
	vm_fire_interrupt(cpu, (uint8_t)val);
}
