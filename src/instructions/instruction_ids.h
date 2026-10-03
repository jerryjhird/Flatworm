#pragma once

// 1 argument
	#define halt_insid 1
	#define int_insid 2
	#define jmp_insid 3

// 2 arguments
	// memory/mov
		#define mov_insid 1
		#define jz_insid 2

// 3 arguments
	// memory/ports
		#define bufout_insid 1
		#define bufin_insid 2

	// math/simple
		#define add_insid 3
		#define sub_insid 4
		#define mul_insid 5
		#define div_insid 6
