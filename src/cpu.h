#pragma once

#include "phys_addr.h"
#include "registers.h"
#include <stdbool.h>
#include "interrupts_2.h"

typedef struct {
	vm_register_file registers;
	vm_phys_address_space *phys_memory;
	vm_supervisor_irq_table irq_table;
	bool stop;
} vm_cpu;

vm_cpu* vm_cpu_alloc(vm_phys_address_space *phys_memory);
void vm_cpu_free(vm_cpu *cpu);
