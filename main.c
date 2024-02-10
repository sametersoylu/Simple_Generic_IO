#include "SysIO.h"
#include "Sum.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char * init_str(size_t size) {
    char *str = malloc(size);
    if (str == NULL) {
        _print_f("Error: Unable to allocate memory for string\n");
        exit(1);
    }
    for (size_t i = 0; i < size; i++) {
        str[i] = ' ';
    }
    str[size - 1] = '\0';
    return str;
}

int main() {
    float f = -3.1445;  
    _write(f);
    _print_f("\nFloat .4f = %.4f\n", f); 

    return 0; 
}
