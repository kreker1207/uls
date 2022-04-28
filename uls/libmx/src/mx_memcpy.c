#include "../inc/libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((uc_t *)dst)[i] = ((uc_t *)src)[i];
    }
    return dst;
}
