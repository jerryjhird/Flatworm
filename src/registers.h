#pragma once

#include <stdint.h>

// ENV_* meaning code running inside environment assigned the rule. can be changed
// SUPERV_* meaning the vm/supervisor assigned the rule and cannot be changed from inside environment

// RW - read + write
// RO - read only (exception 1 if written to)
// FAULT_ON_ACTION (exception 1 when any action is taken related to register)
typedef enum {
    ENV_RW,
    ENV_RO,
    ENV_FAULT_ON_ACTION, 

    SUPERV_RW,
    SUPERV_RO,
    SUPERV_FAULT_ON_ACTION
} vm_register_permission;

typedef struct {
    uint64_t value;
    vm_register_permission permission;
} vm_register;

#define GENERAL_ACCESS_REGISTER_COUNT 34

#define GAR_PROGRAM_COUNTER 32
#define GAR_INTERRUPT_ROUTING_TABLE 33

typedef struct {
    // 0      = zero register                          (supervisor enforced ro)
    // 1 - 31 = general purpose registers (GPR)        (rw)
    // 32     = program counter / instruction pointer  (rw)
    // 33     = interrupt routing table                (rw)
    vm_register general_access_registers[GENERAL_ACCESS_REGISTER_COUNT]; // general access meaning standard instructions like mov_rr and etc work
} vm_register_file;
