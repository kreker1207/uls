#include "../inc/libmx.h"

bool isonlyalpha(const char *str) {
    for(int i = 0; i < mx_strlen(str); i++) {
        if(!mx_isalpha(str[i])) return false;
    }
    return true;
}


