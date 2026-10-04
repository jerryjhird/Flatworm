#include "interrupts.h"

#include "cpu.h"
#include <stdbool.h>

// Interrupt Routing Table (0 - 31)
const char *exception_reason_strings[32] = {
	"Divide Error",
	"Debug",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Invalid Instruction",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Invalid Operand",
	"Page Fault",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Floating Point Exception",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void vm_register_interrupt_handler(vm_cpu *cpu, uint8_t vector, virtenv_ptr_t virtenv_handler_ptr) {
	if (vector >= MAX_INTERRUPTS) {
		superv_panic(1, "interrupt vector %u exceeds maximum limit of %u", vector, MAX_INTERRUPTS);
		return;
	}

	cpu->irq_table.entries[vector] = virtenv_handler_ptr;
}

void vm_fire_interrupt(vm_cpu *cpu, uint8_t vector) {
	if (vector >= MAX_INTERRUPTS) {
		superv_panic(1, "interrupt vector %u exceeds maximum limit of %u", vector, MAX_INTERRUPTS);
		return;
	}

	virtenv_ptr_t handler_ptr = cpu->irq_table.entries[vector];

	// if firing unhandled exception
	if (handler_ptr == 0) {
		if (vector < 32) {
			superv_panic(1, "exception was thrown while missing exception handler for vector %u (%s)\n", vector, exception_reason_strings[vector]);
		}
		return;
	}

	cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value = handler_ptr;
}
