#pragma once

#include "cpu.h"
#include <stdint.h>

// simple.c
void ins_add_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst);
void ins_add_imm(vm_cpu *cpu, uint64_t src1, uint64_t imm, uint64_t dst);
void ins_sub_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst);
void ins_sub_imm(vm_cpu *cpu, uint64_t src1, uint64_t imm, uint64_t dst);
void ins_mul_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst);
void ins_div_rr(vm_cpu *cpu, uint64_t src1, uint64_t src2, uint64_t dst);
