#pragma once

#include "cpu.h"
#include "operand.h"
#include <stdint.h>

// mov.c
void ins_mov(vm_cpu *cpu, vm_operand dst, vm_operand src);
void ins_movb(vm_cpu *cpu, vm_operand dst, vm_operand src);
void ins_movw(vm_cpu *cpu, vm_operand dst, vm_operand src);
void ins_movl(vm_cpu *cpu, vm_operand dst, vm_operand src);
void ins_movq(vm_cpu *cpu, vm_operand dst, vm_operand src);

// ports.c
void ins_bufin(vm_cpu *cpu, vm_operand port, vm_operand addr, vm_operand length);
void ins_bufout(vm_cpu *cpu, vm_operand port, vm_operand addr, vm_operand length);
