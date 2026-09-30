#pragma once

#include "cpu.h"

void ins_bufin(vm_cpu *cpu, uint64_t port, uint64_t addr, uint64_t length);
void ins_bufout(vm_cpu *cpu, uint64_t port, uint64_t addr, uint64_t length);
