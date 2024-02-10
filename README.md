# Simple IO operations using _Generic keyword 

This is a hobby program written in C and handles simple IO opreations, such as printing and reading. Printing done using inline assembly and reading is also same. 

### strlength(x)
This is for getting size of strings. If you init a char * str with the initstr function, there'll be a null terminator character at the end of the string's max size. So strlength will work perfect. 

### get_size(x) 
This is for getting size of all types. Calls strlength for strings and sizeof(x) for others. 

### _read_s(x) 
This macro calls the necessary sys_read_<type> function. 

### _read(x) 
This macro calls the necessary sys_read_<type> with get_size(x). 

### _write_s(x) 
This macro calls the necessary sys_write_<type> function. 

### _write(x) 
This macro calls the necessary sys_write_<type> with get_size(x). 

### _print_f(const char * format, ...) 
This function is the user defined implementation of printf() function. Uses sys_write_<type> functions. 

### _read_f(const char * format, ...) // NOT IMPLEMENTED!
TODO: Impelent this. 
