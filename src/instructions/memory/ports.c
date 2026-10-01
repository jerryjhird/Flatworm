#include <stdint.h>
#include <stdio.h>

#include "cpu.h"
#include "bounds.h"

// write to port (outgoing)
void ins_bufout(vm_cpu *cpu, uint64_t port, uint64_t addr, uint64_t length) {
	check_mem_read(cpu, addr, length);

	uint8_t *src = (uint8_t *)cpu->phys_memory->start + addr;

	for (uint64_t i = 0; i < length; i++) {
		if (port == 1) {
			putchar(src[i]);
		}
	}
}

// read from port (incoming)
void ins_bufin(vm_cpu *cpu, uint64_t port, uint64_t addr, uint64_t length) {
	check_mem_write(cpu, addr, length);

	uint8_t *dest = (uint8_t *)cpu->phys_memory->start + addr;

	for (uint64_t i = 0; i < length; i++) {
		if (port == 0) {
			int c = getchar();
			dest[i] = (c == EOF) ? 0 : (uint8_t)c;
		} else {
			dest[i] = 0;
		}
	}
}
