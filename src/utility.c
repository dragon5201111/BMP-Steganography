#include "utility.h"
#include <stdio.h>

void print_usage(void){
    fprintf(stderr, USAGE_STR);
}

int is_null(void * obj){
    return obj == NULL;
}