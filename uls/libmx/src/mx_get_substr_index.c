#include "../inc/libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    int index = 0;
    int sub_length = 0;
    
    if (sub && str) {
        sub_length = mx_strlen(sub);
        while (*str) {
            if (mx_strncmp(str++, sub, sub_length) == 0)
                return index;
            index++;
        }
        return -1;
    }
    return -2;
}

