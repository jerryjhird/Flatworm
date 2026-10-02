#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "instruction.h"
#include "phys_addr.h"
#include "util.h"

#define MEM_SIZE VM_PAGE_SIZE * 4

void main_run(const char *filename) {
	FILE *f = fopen(filename, "rb");
	if (!f) {
		superv_panic(1, "failed to open input file\n");
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (size <= 0) {
		superv_panic(1, "input file empty, invalid or corrupted\n");
	}

	unsigned char *buffer = superv_malloc(size);

	size_t read_bytes = fread(buffer, 1, size, f);
	fclose(f);

	if (read_bytes != (size_t)size) {
		superv_free(buffer);
		superv_panic(1, "failed to read input file fully\n");
	}

	vm_phys_address_space *phys_mem = vm_phys_address_space_alloc(MEM_SIZE);

	if ((size_t)size > phys_mem->length) {
		superv_free(buffer);
		vm_phys_address_space_free(phys_mem);
		superv_panic(1, "program too large for memory\n");
	}

	vm_cpu *cpu = vm_cpu_alloc(phys_mem);

	unsigned char *dest = (unsigned char *)phys_mem->start;
	for (long i = 0; i < size; i++) {
		dest[i] = buffer[i];
	}

	superv_free(buffer);

	// run program
	while (!cpu->is_halted) {
		vm_execute_instruction(cpu);
	}

	vm_cpu_free(cpu);
	vm_phys_address_space_free(phys_mem);
}
