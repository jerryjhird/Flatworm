#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "instruction.h"

#include "instructions/instruction_ids.h"
#include "instructions/memory/all.h"
#include "instructions/control/all.h"
#include "instructions/math/all.h"

typedef struct {
	const char *mnemonic;
	uint64_t id;
	instruction1_handler handler;
} handler_entry1;

typedef struct {
	const char *mnemonic;
	uint64_t id;
	instruction2_handler handler;
} handler_entry2;

typedef struct {
	const char *mnemonic;
	uint64_t id;
	instruction3_handler handler;
} handler_entry3;

typedef struct {
	const char *mnemonic;
	uint64_t id;
	instruction4_handler handler;
} handler_entry4;

static const uint8_t arg_counts[4] = {1, 2, 3, 4};

static const handler_entry1 handlers1[] = {
	{"stop", stop_insid, ins_stop},
	{"int", int_insid, ins_int},
	{"jmp", jmp_insid, ins_jmp},
	{NULL, 0, NULL}
};

static const handler_entry2 handlers2[] = {
	{"movq", movq_insid, ins_movq},
	{"jz", jz_insid, ins_jz},
	{"movb", movb_insid, ins_movb},
	{"movw", movw_insid, ins_movw},
	{"movl", movl_insid, ins_movl},
	{"setint", setint_insid, ins_setint},
	{NULL, 0, NULL}
};

static const handler_entry3 handlers3[] = {
	{"bufout", bufout_insid, ins_bufout},
	{"bufin", bufin_insid, ins_bufin},
	{"add", add_insid, ins_add},
	{"sub", sub_insid, ins_sub},
	{"mul", mul_insid, ins_mul},
	{"div", div_insid, ins_div},
	{NULL, 0, NULL}
};

static const handler_entry4 handlers4[] = {
	{NULL, 0, NULL}
};
