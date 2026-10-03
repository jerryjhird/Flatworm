#pragma once

#include "cpu.h"
#include "operand.h"
#include <stdint.h>

// simple.c
void ins_add(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst);
void ins_sub(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst);
void ins_mul(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst);
void ins_div(vm_cpu *cpu, vm_operand src1, vm_operand src2, vm_operand dst);
