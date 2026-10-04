#pragma once

#include <stdint.h>
#include "cpu.h"
#include "operand.h"

// stop.c
void ins_stop(vm_cpu *cpu, vm_operand arg1);

// interrupt.c
void ins_int(vm_cpu *cpu, vm_operand arg1);
void ins_setint(vm_cpu *cpu, vm_operand vector_op, vm_operand handler_op);

// jmp.c
void ins_jmp(vm_cpu *cpu, vm_operand target);
void ins_jz(vm_cpu *cpu, vm_operand reg, vm_operand target);
