#pragma once

// 1 argument
	#define halt_insid 1
	#define int_insid 2
	#define jmp_insid 3

// 2 arguments
	// memory/mov
		#define mov_rr_insid 1
		#define mov_rm_insid 2
		#define mov_mr_insid 3
		#define mov_imm_insid 4
		#define mov_im_insid 5
		#define jz_insid 6

// 3 arguments
	// memory/ports
		#define bufout_insid 1
		#define bufin_insid 2

	// math/simple
		#define add_rr_insid 3
		#define sub_rr_insid 4
		#define mul_rr_insid 5
		#define div_rr_insid 6
		#define add_imm_insid 7
		#define sub_imm_insid 8
