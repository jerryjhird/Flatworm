#pragma once

#include <stdint.h>

// RW - read/write
// RO - read only (exception 1 if written to)
typedef enum {
	RW,
	RO
} vm_register_permission;

typedef struct {
	uint64_t value;
	vm_register_permission permission;
} vm_register;

#define GENERAL_ACCESS_REGISTER_COUNT 33

#define GAR_ZERO 0
#define GAR_PROGRAM_COUNTER 32

typedef struct {
	const char *name;
	uint64_t id;
} special_register;

static const special_register SPECIAL_REGISTERS[] = {
	{ "pc",  GAR_PROGRAM_COUNTER }
};

typedef struct {
	// 0	  = zero register						  (supervisor enforced ro)
	// 1 - 31 = general purpose registers (GPR)		  (rw)
	// 32	 = program counter / instruction pointer  (rw)
	vm_register general_access_registers[GENERAL_ACCESS_REGISTER_COUNT];
} vm_register_file;
