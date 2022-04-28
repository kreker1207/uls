#include "../inc/libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    uc_t *temp_b = (uc_t *)b;
    for (size_t i = 0; i < len; i++) {
        temp_b[i] = (uc_t)c;
    }
    return b;
}
