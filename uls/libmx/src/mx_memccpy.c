#include "../inc/libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    for (size_t i = 0; i < n; i++) {
		if (((const uc_t *)src)[i] == (uc_t)c) {
			((uc_t *)dst)[i] = ((const uc_t *)src)[i];
			return (void *)&(((uc_t *)dst)[i + 1]);
		} else {
			((uc_t *)dst)[i] = ((const uc_t *)src)[i];
        }
	}
	return NULL;
}
