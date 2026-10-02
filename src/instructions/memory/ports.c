#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "bounds.h"

// write to port (outgoing)
void ins_bufout(vm_cpu *cpu, uint64_t port, uint64_t addr, uint64_t length) {
	for (uint64_t i = 0; i < length; i++) {
		uint64_t val = 0;
		if (!read_le(cpu, addr + i, 1, &val)) {
			return;
		}
		if (port == 1) {
			putchar((uint8_t)val);
		}
	}
}

// read from port (incoming)
void ins_bufin(vm_cpu *cpu, uint64_t port, uint64_t addr, uint64_t length) {
	for (uint64_t i = 0; i < length; i++) {
		uint64_t val = 0;
		if (port == 0) {
			int c = getchar();
			val = (c == EOF) ? 0 : (uint8_t)c;
		} else {
			val = 0;
		}
		if (!write_le(cpu, addr + i, 1, val)) {
			return;
		}
	}
}
