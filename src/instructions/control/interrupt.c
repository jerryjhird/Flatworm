#include "cpu.h"
#include "interrupts.h"
#include "operand.h"

// fire interrupt
void ins_int(vm_cpu *cpu, vm_operand vector) {
	uint64_t val = vm_read_operand(cpu, vector);
	vm_fire_interrupt(cpu, (uint8_t)val);
}

// set handler for interrupt
void ins_setint(vm_cpu *cpu, vm_operand vector_op, vm_operand handler_op) {
	uint64_t vector = vm_read_operand(cpu, vector_op);
	uint64_t handler_ptr = vm_read_operand(cpu, handler_op);

	if (vector >= MAX_INTERRUPTS - 1) {
		vm_fire_interrupt(cpu, 13); // Invalid Operand
		return;
	}

	cpu->irq_table.entries[vector] = (virtenv_ptr_t)handler_ptr;
}
