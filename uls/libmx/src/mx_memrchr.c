#include "../inc/libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    uc_t *s_temp = (uc_t *)s + mx_strlen(s);
	while (n--) {
		if (*s_temp == (uc_t)c) {
			return s_temp;
        }
		s_temp--;
	}
	return NULL;
}
