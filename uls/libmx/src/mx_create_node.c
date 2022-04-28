#include "../inc/libmx.h"

t_list *mx_create_node(char *data) {
    t_list *newNode = (t_list*)malloc(sizeof(newNode));
    newNode -> next = NULL;
    newNode -> data = mx_strdup(data);
    return newNode;
}
