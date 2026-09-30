#pragma once

#include <stdint.h>
#include "util.h"

typedef struct {
    superv_ptr_t start;
    uint64_t length;
} vm_phys_address_space;

vm_phys_address_space* vm_phys_address_space_alloc(uint64_t length);
void vm_phys_address_space_free(vm_phys_address_space *mem);
