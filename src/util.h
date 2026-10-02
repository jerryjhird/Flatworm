#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define VM_PAGE_SIZE 4096

#define virtenv_ptr_t uint64_t

typedef enum {
	LOG_INFO = 0,
	LOG_WARN,
	LOG_ERROR
} superv_log_level;

static inline void vsuperv_log(superv_log_level level, const char *fmt, va_list args) {
	FILE *out = (level == LOG_ERROR) ? stderr : stdout;

	switch (level) {
		case LOG_INFO:
			fprintf(out, "INFO : ");
			break;
		case LOG_WARN:
			fprintf(out, "WARN : ");
			break;
		case LOG_ERROR:
			fprintf(out, "ERROR: ");
			break;
	}

	vfprintf(out, fmt, args);
	fprintf(out, "\n");
}

static inline void superv_log(superv_log_level level, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsuperv_log(level, fmt, args);
	va_end(args);
}

static inline void superv_panic(int err_code, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vsuperv_log(LOG_ERROR, fmt, args);
	va_end(args);

	exit(err_code);
}

static inline void superv_panic_notext(int err_code) {
	exit(err_code);
}

static inline void* superv_malloc(size_t size) {
	void *ptr = malloc(size);
	if (!ptr) {
		superv_panic(1, "(superv_malloc) failed to allocate\n");
	}
	return ptr;
}

static inline void* superv_calloc(size_t num, size_t size) {
	void *ptr = calloc(num, size);
	if (!ptr) {
		superv_panic(1, "(superv_calloc) failed to allocate\n");
	}
	return ptr;
}

static inline void superv_free(void *ptr) {
	if (ptr) {
		free(ptr);
	}
}
