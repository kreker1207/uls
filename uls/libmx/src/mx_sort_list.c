#include "../inc/libmx.h"

t_list *mx_sort_list(t_list *list, int (*cmp)(const char*, const char*)) {
    if (cmp == NULL || list == NULL) return list;
    t_list *temp = list;
    int count = 0;
    while (temp) {
        temp = temp -> next;
        count++;
    }

    for (int i = 0; i < count; i++) {
        temp = list;
        for (int j = 0; j < count - 1; j++) {
            if (cmp(temp -> data, temp -> next -> data) > 0) {
                char *data = mx_strdup(temp -> data); 
                temp -> data = mx_strdup(temp -> next -> data); 
                temp -> next -> data = mx_strdup(data);     
            }
            temp = temp -> next;
        }
    }
    return list;
}

