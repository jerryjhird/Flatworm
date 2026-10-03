#pragma once

#include <stdint.h>
#include "cpu.h"
#include "operand.h"

// halt.c
void ins_halt(vm_cpu *cpu, vm_operand arg1);

// interrupt.c
void ins_int(vm_cpu *cpu, vm_operand arg1);

// jmp.c
void ins_jmp(vm_cpu *cpu, vm_operand target);
void ins_jz(vm_cpu *cpu, vm_operand reg, vm_operand target);
