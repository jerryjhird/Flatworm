#pragma once

#include "cpu.h"
#include "util.h"
#include <string.h>
#include <sys/types.h>
#include <errno.h>

static inline void *vm_get_host_addr(vm_cpu *cpu, virtenv_ptr_t addr) {
	return (uint8_t *)cpu->phys_memory->start + addr;
}

static inline size_t bytes_needed(uint64_t v) {
	size_t n = 1;
	while (v >>= 8) {
		n++;
	}
	return n;
}

static inline char *c_strdup(const char *s) {
	size_t len = strlen(s) + 1;

	char *p = malloc(len);

	if (!p) {
		return NULL;
	}

	memcpy(p, s, len);

	return p;
}

static inline ssize_t c_getline(char **lineptr, size_t *n, FILE *stream) {
	if (!lineptr || !n || !stream) {
		errno = EINVAL;
		return -1;
	}

	if (*lineptr == NULL || *n == 0) {
		*n = 128;
		*lineptr = malloc(*n);
		if (!*lineptr) {
			return -1;
		}
	}

	size_t i = 0;
	int c;

	while ((c = fgetc(stream)) != EOF) {
		if (i + 1 >= *n) {
			size_t new_n = *n * 2;
			char *new_ptr = realloc(*lineptr, new_n);
			if (!new_ptr) {
				return -1;
			}
			*lineptr = new_ptr;
			*n = new_n;
		}

		(*lineptr)[i++] = (char)c;
		if (c == '\n') {
			break;
		}
	}

	if (i == 0 && c == EOF) {
		return -1;
	}

	(*lineptr)[i] = '\0';
	return (ssize_t)i;
}
