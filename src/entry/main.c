#include <stdio.h>
#include <string.h>
#include "all.h"

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("usage: vm <command> [<args>]\n");
		return 1;
	}

	if (strcmp(argv[1], "run") == 0) {
		if (argc < 3) {
			printf("usage: vm run <program.bin>\n");
			return 1;
		}
		main_run(argv[2]);
	} else if (strcmp(argv[1], "compile") == 0) {
		if (argc < 3) {
			printf("usage: vm compile <input.bin> [-o <output.bin>]\n");
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
		printf("unknown command: %s\n", argv[1]);
		return 1;
	}

	return 0;
}
