#include "cpu.h"
#include "util.h"

#define MEM_SIZE (VM_PAGE_SIZE * 4)

vm_cpu* vm_cpu_alloc(vm_phys_address_space *phys_memory) {
	vm_cpu *cpu = superv_malloc(sizeof(vm_cpu));
	cpu->phys_memory = phys_memory;

	for (int i = 0; i < GENERAL_ACCESS_REGISTER_COUNT; i++) {
		cpu->registers.general_access_registers[i].value = 0;
		cpu->registers.general_access_registers[i].permission = ENV_RW;
	}

	cpu->registers.general_access_registers[0].permission = SUPERV_RO;
	cpu->is_halted = false;

	return cpu;
}

void vm_cpu_free(vm_cpu *cpu) {
	if (cpu) {
		superv_free(cpu);
	}
}
