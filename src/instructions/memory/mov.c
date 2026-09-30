#include "bounds.h"
#include "cpu.h"

// register -> register
void ins_mov_rr(vm_cpu *cpu, uint64_t dst, uint64_t src) {
    check_ga_reg_read(cpu, src);
    check_ga_reg_write(cpu, dst);
    cpu->registers.general_access_registers[dst].value = cpu->registers.general_access_registers[src].value;
}

// memory -> register
void ins_mov_rm(vm_cpu *cpu, uint64_t dst, uint64_t addr) {
    check_mem_read(cpu, addr, sizeof(uint64_t));
    check_ga_reg_write(cpu, dst);
    cpu->registers.general_access_registers[dst].value = *(uint64_t *)((uint8_t *)cpu->phys_memory->start + addr);
}

// register -> memory
void ins_mov_mr(vm_cpu *cpu, uint64_t addr, uint64_t src) {
    check_ga_reg_read(cpu, src);
    check_mem_write(cpu, addr, sizeof(uint64_t));
    *(uint64_t *)((uint8_t *)cpu->phys_memory->start + addr) = cpu->registers.general_access_registers[src].value;
}

// immediate value -> register
void ins_mov_imm(vm_cpu *cpu, uint64_t dst, uint64_t imm) {
    check_ga_reg_write(cpu, dst);
    cpu->registers.general_access_registers[dst].value = imm;
}
