#pragma once

#include "cpu.h"
#include "interrupts.h"
#include "mathop.h"

static inline bool read_reg(vm_cpu *cpu, uint64_t reg, uint64_t *out) {
	if (reg >= GENERAL_ACCESS_REGISTER_COUNT) { // check if reg is out of bounds
		vm_fire_interrupt(cpu, 13);
		return false;
	}
	*out = cpu->registers.general_access_registers[reg].value;
	return true;
}

static inline bool write_reg(vm_cpu *cpu, uint64_t reg, uint64_t val) {
	if (reg >= GENERAL_ACCESS_REGISTER_COUNT) { // check if reg is out of bounds
		vm_fire_interrupt(cpu, 13);
		return false;
	}

	if (cpu->registers.general_access_registers[reg].permission == RO) { // check if reg is read only
		vm_fire_interrupt(cpu, 13);
		return false;
	}

	cpu->registers.general_access_registers[reg].value = val;
	return true;
}

static inline bool read_le(vm_cpu *cpu, uint64_t addr, size_t n, uint64_t *out) {
	if (addr >= cpu->phys_memory->length || n > cpu->phys_memory->length - addr) {
		return false;
	}

	uint8_t *p = (uint8_t *)vm_get_host_addr(cpu, addr);
	uint64_t v = 0;

	for (size_t i = 0; i < n; i++) { v |= (uint64_t)p[i] << (i * 8); }

	*out = v;
	return true;
}

static inline bool write_le(vm_cpu *cpu, uint64_t addr, size_t n, uint64_t val) {
	// check if write exceeds physical memory bounds
	if (addr >= cpu->phys_memory->length || n > cpu->phys_memory->length - addr) {
		vm_fire_interrupt(cpu, 14);
		return false;
	}

	uint8_t *p = (uint8_t *)vm_get_host_addr(cpu, addr);

	for (size_t i = 0; i < n; i++) {
		p[i] = (uint8_t)(val >> (i * 8));
	}
	return true;
}
