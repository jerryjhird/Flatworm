#pragma once

#include <stdint.h>
#include <stddef.h>
#include "cpu.h"

// first byte of every instruction
// bits 0-1 = arg count type
// bits 2-4 = id length in bytes - 1
// bits 5.. = one 3 bit length - 1 per argument then opcode
#define INSTRUCTION_ARG_1 0x0 // 00
#define INSTRUCTION_ARG_2 0x1 // 01
#define INSTRUCTION_ARG_3 0x3 // 11
#define INSTRUCTION_ARG_4 0x2 // 10

#define INSTRUCTION_TYPE_MASK 0x3
#define INSTRUCTION_LEN_SHIFT 2
#define INSTRUCTION_LEN_MASK 0x7
#define INSTRUCTION_HEADER_BITS 5
#define INSTRUCTION_ARGLEN_BITS 3
#define INSTRUCTION_ARGLEN_MASK 0x7
#define INSTRUCTION_MAX_ARGS 4

typedef void (*instruction1_handler)(vm_cpu *cpu, uint64_t arg1);
typedef void (*instruction2_handler)(vm_cpu *cpu, uint64_t arg1, uint64_t arg2);
typedef void (*instruction3_handler)(vm_cpu *cpu, uint64_t arg1, uint64_t arg2, uint64_t arg3);
typedef void (*instruction4_handler)(vm_cpu *cpu, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);

void vm_execute_instruction(vm_cpu *cpu);

// return bytes written
// 0 if opcode doesnt fit
size_t vm_encode_instruction(uint8_t *out, uint8_t type, uint64_t id, const uint64_t *args);
