#include "../inc/libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    uc_t *temp = (uc_t*)malloc(len * sizeof(uc_t));
	temp = mx_memcpy(temp, src, len);
	dst = mx_memcpy(dst, temp, len);
	free(temp);
    temp = NULL;
	return dst;
}
