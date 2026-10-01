#include "instruction.h"
#include "interrupts.h"

typedef struct {
	uint32_t id;
	instruction1_handler handler;
} handler_entry1;

typedef struct {
	uint32_t id;
	instruction2_handler handler;
} handler_entry2;

typedef struct {
	uint32_t id;
	instruction3_handler handler;
} handler_entry3;

typedef struct {
	uint32_t id;
	instruction4_handler handler;
} handler_entry4;

#include "instructions/instruction_ids.h"
#include "instructions/memory/all.h"
#include "instructions/control/all.h"

static const handler_entry1 handlers1[] = {
	{halt_insid, ins_halt},
	{int_insid, ins_int},
	{0, NULL}
};

static const handler_entry2 handlers2[] = {
	{mov_rr_insid, ins_mov_rr},
	{mov_rm_insid, ins_mov_rm},
	{mov_mr_insid, ins_mov_mr},
	{mov_imm_insid, ins_mov_imm},
	{0, NULL}
};

static const handler_entry3 handlers3[] = {
	{bufin_insid, ins_bufin},
	{bufout_insid, ins_bufout},
	{0, NULL}
};

static const handler_entry4 handlers4[] = {
	{0, NULL}
};

void vm_execute_instruction(vm_cpu *cpu) {
	uint64_t pc = cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value;

	if (pc + sizeof(uint32_t) > cpu->phys_memory->length) {
		// instruction's argument's exceeds physical memory roof
		vm_fire_interrupt(cpu, 0);
		return;
	}

	uint32_t raw_id = *(uint32_t *)((uint8_t *)cpu->phys_memory->start + pc);
	uint32_t type = raw_id >> 30;
	uint32_t id = raw_id & 0x3FFFFFFF;

	if (type == INSTRUCTION_ARG_1) {
		if (pc + sizeof(instruction1) > cpu->phys_memory->length) {
			// instruction's argument's exceeds physical memory roof
			vm_fire_interrupt(cpu, 0);
			return;
		}
		cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value += sizeof(instruction1);
		instruction1 *ins = (instruction1 *)((uint8_t *)cpu->phys_memory->start + pc);
		instruction1_handler found = NULL;
		for (int i = 0; handlers1[i].handler != NULL; i++) {
			if (handlers1[i].id == id) {
				found = handlers1[i].handler;
				break;
			}
		}
		if (!found) {
			vm_fire_interrupt(cpu, 2);
			return;
		}
		found(cpu, ins->arg1);
	} else if (type == INSTRUCTION_ARG_2) {
		if (pc + sizeof(instruction2) > cpu->phys_memory->length) {
			// instruction's argument's exceeds physical memory roof
			vm_fire_interrupt(cpu, 0);
			return;
		}
		cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value += sizeof(instruction2);
		instruction2 *ins = (instruction2 *)((uint8_t *)cpu->phys_memory->start + pc);
		instruction2_handler found = NULL;
		for (int i = 0; handlers2[i].handler != NULL; i++) {
			if (handlers2[i].id == id) {
				found = handlers2[i].handler;
				break;
			}
		}
		if (!found) {
			vm_fire_interrupt(cpu, 2);
			return;
		}
		found(cpu, ins->arg1, ins->arg2);
	} else if (type == INSTRUCTION_ARG_3) {
		if (pc + sizeof(instruction3) > cpu->phys_memory->length) {
			// instruction's argument's exceeds physical memory roof
			vm_fire_interrupt(cpu, 0);
			return;
		}
		cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value += sizeof(instruction3);
		instruction3 *ins = (instruction3 *)((uint8_t *)cpu->phys_memory->start + pc);
		instruction3_handler found = NULL;
		for (int i = 0; handlers3[i].handler != NULL; i++) {
			if (handlers3[i].id == id) {
				found = handlers3[i].handler;
				break;
			}
		}
		if (!found) {
			vm_fire_interrupt(cpu, 2);
			return;
		}
		found(cpu, ins->arg1, ins->arg2, ins->arg3);
	} else if (type == INSTRUCTION_ARG_4) {
		if (pc + sizeof(instruction4) > cpu->phys_memory->length) {
			// instruction's argument's exceeds physical memory roof
			vm_fire_interrupt(cpu, 0);
			return;
		}
		cpu->registers.general_access_registers[GAR_PROGRAM_COUNTER].value += sizeof(instruction4);
		instruction4 *ins = (instruction4 *)((uint8_t *)cpu->phys_memory->start + pc);
		instruction4_handler found = NULL;
		for (int i = 0; handlers4[i].handler != NULL; i++) {
			if (handlers4[i].id == id) {
				found = handlers4[i].handler;
				break;
			}
		}
		if (!found) {
			vm_fire_interrupt(cpu, 2);
			return;
		}
		found(cpu, ins->arg1, ins->arg2, ins->arg3, ins->arg4);
	} else {
		vm_fire_interrupt(cpu, 2); // unknown instruction type encoding in upper bits
		return;
	}
}
