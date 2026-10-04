#pragma once

#include "cpu.h"
#include "util.h"
#include "interrupts_2.h"

#define IRQ_TABLE_MAGIC "irqT"

void vm_fire_interrupt(vm_cpu *cpu, uint8_t vector);
void vm_register_interrupt_handler(vm_cpu *cpu, uint8_t vector, virtenv_ptr_t handler);
