#pragma once

#include "util.h"

#define MAX_INTERRUPTS 255

typedef struct {
	virtenv_ptr_t entries[MAX_INTERRUPTS];
} vm_supervisor_irq_table;
