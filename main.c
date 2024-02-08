#include "SysIO.h"
#include "Sum.h"
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
    char * str = init_str(1024);
    _read(str); 
    _print_f("You entered: %s\n", str);
    int x = 0, y = 0;
    _print_f("Enter two numbers: ");    
    _read(&x); _read(&y);
    _print_f("Sum: %d", sum(x,y));


    return 0; 
}