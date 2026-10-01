#pragma once

#include "phys_addr.h"
#include "registers.h"
#include "util.h"
#include <stdbool.h>

typedef struct {
	vm_register_file registers;
	vm_phys_address_space *phys_memory;
	bool is_halted;
} vm_cpu;

vm_cpu* vm_cpu_alloc(vm_phys_address_space *phys_memory);
void vm_cpu_free(vm_cpu *cpu);
