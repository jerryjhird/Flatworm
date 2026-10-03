#include <stdio.h>
#include <string.h>
#include "all.h"

static inline void print_help(const char *prog_name) {
	printf("usage:\n");
	printf("  %s run <program.bin>\n", prog_name);
	printf("  %s compile [input.asm] [-o <output.bin>]\n", prog_name);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		print_help(argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "run") == 0) {
		if (argc < 3) {
			printf("usage: %s run <program.bin>\n", argv[0]);
			return 1;
		}

		main_run(argv[2]);

	} else if (strcmp(argv[1], "compile") == 0) {
		if (argc < 3) {
			printf("usage: %s compile <input> [-o <output.bin>]\n", argv[0]);
			return 1;
		}

		const char *input_file = argv[2];
		const char *output_file = NULL;

		// check for output flag
		for (int i = 3; i < argc; i++) {
			if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
				output_file = argv[i + 1];
				break;
			}
		}

		main_compile(input_file, output_file);
	} else {
		print_help(argv[0]);
		return 1;
	}

	return 0;
}
