#include "phys_addr.h"
#include "util.h"

vm_phys_address_space* vm_phys_address_space_alloc(uint64_t length) {
    vm_phys_address_space *memspace = superv_malloc(sizeof(vm_phys_address_space));
    void *mem = superv_calloc(1, length);
    memspace->start = (superv_ptr_t)mem;
    memspace->length = length;
    return memspace;
}

void vm_phys_address_space_free(vm_phys_address_space *mem) {
    if (mem) {
        superv_free((void*)mem->start);
        superv_free(mem);
    }
}
