#include "../inc/libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    uc_t *s_temp = (uc_t *)s;
	while (n--) {
		if (*s_temp == (uc_t)c) {
			return s_temp;
        }
		s_temp++;
	}
	return NULL;
}
