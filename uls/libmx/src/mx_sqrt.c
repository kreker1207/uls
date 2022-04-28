#include "../inc/libmx.h"

int mx_sqrt(int x) {
    if (x <= 0) return 0;
    int count = 1;
    int sqr_count = count * count;
    while (x > sqr_count && sqr_count > 0) {
        count ++;
        sqr_count = count * count;
    }
    if (sqr_count == x) return count;
    return 0;
}

