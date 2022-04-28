#include "../inc/libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    if (n == 0) {
        return 0;
    }
    const uc_t *s1_temp = s1;
    const uc_t *s2_temp = s2;
    size_t i = 0;
    for (; *s1_temp == *s2_temp && i != n; s1_temp++, s2_temp++) {
        if (*s2_temp == '\0') {
            return 0;
        }
        i++;
    }
    return *s1_temp - *s2_temp;
}
