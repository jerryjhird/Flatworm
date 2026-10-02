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

static const uint8_t arg_counts[4] = {1, 2, 4, 3};

static const handler_entry1 handlers1[] = {
	{"halt", halt_insid, ins_halt},
	{"int", int_insid, ins_int},
	{"jmp", jmp_insid, ins_jmp},
	{NULL, 0, NULL}
};

static const handler_entry2 handlers2[] = {
	{"mov_rr", mov_rr_insid, ins_mov_rr},
	{"mov_rm", mov_rm_insid, ins_mov_rm},
	{"mov_mr", mov_mr_insid, ins_mov_mr},
	{"mov_imm", mov_imm_insid, ins_mov_imm},
	{"mov_im", mov_im_insid, ins_mov_im},
	{"jz", jz_insid, ins_jz},
	{NULL, 0, NULL}
};

static const handler_entry3 handlers3[] = {
	{"bufout", bufout_insid, ins_bufout},
	{"bufin", bufin_insid, ins_bufin},

	{"add_rr", add_rr_insid, ins_add_rr},
	{"sub_rr", sub_rr_insid, ins_sub_rr},
	{"mul_rr", mul_rr_insid, ins_mul_rr},
	{"div_rr", div_rr_insid, ins_div_rr},
	{"add_imm", add_imm_insid, ins_add_imm},
	{"sub_imm", sub_imm_insid, ins_sub_imm},
	{NULL, 0, NULL}
};

static const handler_entry4 handlers4[] = {
	{NULL, 0, NULL}
};
