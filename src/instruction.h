#pragma once

#include <stdint.h>
#include <stddef.h>
#include "cpu.h"
#include "operand.h"

#define INSTRUCTION_ARG_1 0x0 // 00
#define INSTRUCTION_ARG_2 0x1 // 01
#define INSTRUCTION_ARG_3 0x3 // 11
#define INSTRUCTION_ARG_4 0x2 // 10

#define INSTRUCTION_TYPE_MASK 0x3
#define INSTRUCTION_LEN_SHIFT 2
#define INSTRUCTION_LEN_MASK 0x7
#define INSTRUCTION_HEADER_BITS 5
#define INSTRUCTION_ARGLEN_BITS 5
#define INSTRUCTION_ARGLEN_MASK 0x1f
#define INSTRUCTION_MAX_ARGS 4

// bitfield configuration for argument descriptor
#define ARG_TYPE_BITS 2
#define ARG_LEN_BITS  3 // 3 bits allows lengths up to 8 bytes (0-7 -> 1-8)
#define ARG_LEN_MASK  ((1ULL << ARG_LEN_BITS) - 1)
#define ARG_TYPE_MASK ((1ULL << ARG_TYPE_BITS) - 1)

typedef void (*instruction1_handler)(vm_cpu *cpu, vm_operand arg0);
typedef void (*instruction2_handler)(vm_cpu *cpu, vm_operand arg0, vm_operand arg1);
typedef void (*instruction3_handler)(vm_cpu *cpu, vm_operand arg0, vm_operand arg1, vm_operand arg2);
typedef void (*instruction4_handler)(vm_cpu *cpu, vm_operand arg0, vm_operand arg1, vm_operand arg2, vm_operand arg3);

void vm_execute_instruction(vm_cpu *cpu);
