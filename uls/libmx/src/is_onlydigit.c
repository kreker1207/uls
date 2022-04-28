#include "../inc/libmx.h"
bool isonlydigit(const char *str) {
for(int i = 0; i < mx_strlen(str); i++ ){
    if(!mx_isdigit(str[i])) return false;
}
return true;
}




