#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#define VM_PAGE_SIZE 4096

#define superv_ptr_t uintptr_t
#define virtenv_ptr_t uint64_t

static inline void superv_panic(int err_code, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

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
