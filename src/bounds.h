#pragma once

#include "cpu.h"
#include "interrupts.h"

static inline void check_ga_reg_read(vm_cpu *cpu, uint64_t reg) {
    if (reg >= GENERAL_ACCESS_REGISTER_COUNT) {
        vm_fire_interrupt(cpu, 1);
        return;
    }
    if (cpu->registers.general_access_registers[reg].permission == ENV_FAULT_ON_ACTION || cpu->registers.general_access_registers[reg].permission == SUPERV_FAULT_ON_ACTION) {
        vm_fire_interrupt(cpu, 1);
        return;
    }
}

static inline void check_ga_reg_write(vm_cpu *cpu, uint64_t reg) {
    if (reg >= GENERAL_ACCESS_REGISTER_COUNT) { 
        vm_fire_interrupt(cpu, 1); 
        return;
    }

    vm_register_permission perm = cpu->registers.general_access_registers[reg].permission;
    if (perm == SUPERV_RO || perm == ENV_RO || perm == SUPERV_FAULT_ON_ACTION || perm == ENV_FAULT_ON_ACTION) {
        vm_fire_interrupt(cpu, 1);
        return;
    }
}

static inline void check_mem_read(vm_cpu *cpu, uint64_t addr, uint64_t size) {
    if (addr + size > cpu->phys_memory->length) {
        vm_fire_interrupt(cpu, 0);
        return;
    }
}

static inline void check_mem_write(vm_cpu *cpu, uint64_t addr, uint64_t size) {
    if (addr + size > cpu->phys_memory->length) {
        vm_fire_interrupt(cpu, 0);
        return;
    }
}
