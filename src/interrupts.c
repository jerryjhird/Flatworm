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
	"General Protection Fault", // invalid register / etc...
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

void vm_fire_interrupt(vm_cpu *cpu, uint8_t vector) {
	cpu->is_halted = false;

	if (vector >= MAX_INTERRUPTS) {
		superv_panic(1, "interrupt vector %u exceeds maximum limit of %u", vector, MAX_INTERRUPTS);
		return;
	}

	virtenv_ptr_t table_ptr = cpu->registers.general_access_registers[GAR_INTERRUPT_ROUTING_TABLE].value;

	if (table_ptr == 0) {
		if (vector < 32) {
			superv_panic(1, "%s (vector %u)", exception_reason_strings[vector], vector);
		}
		return;
	}

	vm_check_interrupt_routing_table(cpu, table_ptr);

	vm_interrupt_routing_table *table = (vm_interrupt_routing_table *)((uint8_t *)cpu->phys_memory->start + table_ptr);

	virtenv_ptr_t handler_ptr = table->entries[vector];

	if (handler_ptr == 0) {
		if (vector < 32) {
			superv_panic(1, "exception was thrown while missing exception handler for vector %u\n", vector);
		}
		return;
	}

	cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value = handler_ptr;
}
