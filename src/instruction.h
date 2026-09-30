#pragma once

#include <stdint.h>
#include "cpu.h"

// the first 2 bits of each instruction id will contain one of these identifiers of these to make it clear how many arguments the instruction can take
#define INSTRUCTION_ARG_1 0x0 // 00
#define INSTRUCTION_ARG_2 0x1 // 01
#define INSTRUCTION_ARG_3 0x3 // 11
#define INSTRUCTION_ARG_4 0x2 // 10

typedef void (*instruction1_handler)(vm_cpu *cpu, uint64_t arg1);
typedef void (*instruction2_handler)(vm_cpu *cpu, uint64_t arg1, uint64_t arg2);
typedef void (*instruction3_handler)(vm_cpu *cpu, uint64_t arg1, uint64_t arg2, uint64_t arg3);
typedef void (*instruction4_handler)(vm_cpu *cpu, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);

typedef struct {
    uint32_t id;
    uint64_t arg1;
} __attribute__((packed)) instruction1;

typedef struct {
    uint32_t id;
    uint64_t arg1;
    uint64_t arg2;
} __attribute__((packed)) instruction2;

typedef struct {
    uint32_t id;
    uint64_t arg1;
    uint64_t arg2;
    uint64_t arg3;
} __attribute__((packed)) instruction3;

typedef struct {
    uint32_t id;
    uint64_t arg1;
    uint64_t arg2;
    uint64_t arg3;
    uint64_t arg4;
} __attribute__((packed)) instruction4;

void vm_execute_instruction(vm_cpu *cpu);
