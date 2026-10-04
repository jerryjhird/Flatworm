#pragma once

// 1 argument
	#define stop_insid 1
	#define int_insid 2
	#define jmp_insid 3

// 2 arguments
	#define movq_insid 1
	#define jz_insid   2

	#define movb_insid 3
	#define movw_insid 4
	#define movl_insid 5
	#define setint_insid 6

// 3 arguments
	#define bufout_insid 1
	#define bufin_insid  2

	#define add_insid 3
	#define sub_insid 4
	#define mul_insid 5
	#define div_insid 6
