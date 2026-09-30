#include "interrupts.h"

#include "cpu.h"
#include <stdbool.h>

// Interrupt Routing Table (0 - 31)
// 0  :  Illegal Memory Access
// 1  :  Illegal Register Access
// 2  :  Illegal Instruction
// 3  :  Reserved
// 4  :  Reserved
// 5  :  Reserved
// 6  :  Reserved
// 7  :  Reserved
// 8  :  Reserved
// 9  :  Reserved
// 10  : Reserved
// 11  : Reserved
// 12  : Reserved
// 13  : Reserved
// 14  : Reserved
// 15  : Reserved
// 16  : Reserved
// 17  : Reserved
// 18  : Reserved
// 19  : Reserved
// 20 :  Reserved
// 21  : Reserved
// 22  : Reserved
// 23  : Reserved
// 24  : Reserved
// 25  : Reserved
// 26  : Reserved
// 27  : Reserved
// 28  : Reserved
// 29  : Reserved
// 30  : Reserved
// 31  : Reserved

void vm_fire_interrupt(vm_cpu *cpu, uint8_t vector) {
    cpu->is_halted = false;

    if (vector >= MAX_INTERRUPTS) {
        superv_panic(1, "interrupt vector %u exceeds maximum limit of %u", vector, MAX_INTERRUPTS);
        return;
    }

    virtenv_ptr_t table_ptr = cpu->registers.general_access_registers[GAR_INTERRUPT_ROUTING_TABLE].value;

    vm_check_interrupt_routing_table(cpu, table_ptr);

    vm_interrupt_routing_table *table = (vm_interrupt_routing_table *)((uint8_t *)cpu->phys_memory->start + table_ptr);

    virtenv_ptr_t handler_ptr = table->entries[vector];

    if (handler_ptr == 0) {
        if (vector < 32) {
            superv_panic(1, "exception was thrown while missing exception handler for vector %u\n", vector);
        }
        return;
    }

    cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value = handler_ptr;
}
