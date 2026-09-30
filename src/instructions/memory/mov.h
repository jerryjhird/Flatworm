#pragma once

#include "cpu.h"

void ins_mov_rr(vm_cpu *cpu, uint64_t dst, uint64_t src);
void ins_mov_rm(vm_cpu *cpu, uint64_t dst, uint64_t addr);
void ins_mov_mr(vm_cpu *cpu, uint64_t addr, uint64_t src);
void ins_mov_imm(vm_cpu *cpu, uint64_t dst, uint64_t imm);
