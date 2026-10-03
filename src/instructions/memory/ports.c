#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "bounds.h"
#include "instruction.h"
#include "operand.h"

// write to port (outgoing)
void ins_bufout(vm_cpu *cpu, vm_operand port_op, vm_operand addr_op, vm_operand length_op) {
	uint64_t port = vm_read_operand(cpu, port_op);
	uint64_t base_addr = vm_get_address(cpu, addr_op);
	uint64_t length = vm_read_operand(cpu, length_op);

	for (uint64_t i = 0; i < length; i++) {
		uint64_t val = 0;
		if (!read_le(cpu, base_addr + i, 1, &val)) {
			return;
		}
		if (port == 1) {
			putchar((uint8_t)val);
		}
	}
}

// read from port (incoming)
void ins_bufin(vm_cpu *cpu, vm_operand port_op, vm_operand addr_op, vm_operand length_op) {
	uint64_t port = vm_read_operand(cpu, port_op);
	uint64_t base_addr = vm_get_address(cpu, addr_op);
	uint64_t length = vm_read_operand(cpu, length_op);

	// wip
	UNUSED(port);
	UNUSED(base_addr);
	UNUSED(length);
}
