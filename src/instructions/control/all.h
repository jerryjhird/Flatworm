#pragma once

#include <stdint.h>
#include "cpu.h"

// halt.c
void ins_halt(vm_cpu *cpu, uint64_t arg1);

// interrupt.c
void ins_int(vm_cpu *cpu, uint64_t arg1);

// jmp.c
void ins_jmp(vm_cpu *cpu, uint64_t target);
void ins_jz(vm_cpu *cpu, uint64_t reg, uint64_t target);
