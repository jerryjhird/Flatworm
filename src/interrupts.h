#pragma once

#include "cpu.h"
#include "util.h"

#define MAX_INTERRUPTS 255
#define IRQ_TABLE_MAGIC "irqT"

typedef struct __attribute__((packed)) {
	char magic[4];
	virtenv_ptr_t entries[MAX_INTERRUPTS];
} vm_interrupt_routing_table;

static inline void vm_check_interrupt_routing_table(vm_cpu *cpu, virtenv_ptr_t ptr) {
	if (ptr + sizeof(vm_interrupt_routing_table) > cpu->phys_memory->length) {
		superv_panic(1, "GAR_INTERRUPT_ROUTING_TABLE (ERR 0) (reg 33) was corrupted while needing interrupt functionality\n");
		return;
	}

	vm_interrupt_routing_table *table = (vm_interrupt_routing_table *)((uint8_t *)cpu->phys_memory->start + ptr);

	if (table->magic[0] != 'i' ||
		table->magic[1] != 'r' ||
		table->magic[2] != 'q' ||
		table->magic[3] != 'T') {
		superv_panic(1, "GAR_INTERRUPT_ROUTING_TABLE (ERR1) (reg 33) was corrupted while needing interrupt functionality\n");
		return;
	}
}

void vm_fire_interrupt(vm_cpu *cpu, uint8_t vector);
