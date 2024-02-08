#ifndef SYSIO_H
#define SYS_READ 0
#define SYS_WRITE 1
#include <string.h>


/* Implement READ */
int sys_read_str(unsigned fd, char *buf, unsigned size);

/* Implement READ for int */
int sys_read_int(unsigned fd, int * buf, unsigned dummy);

/* Implement WRITE */
int sys_write_str(unsigned fd, const char *buf, unsigned size);
int sys_write_char(unsigned fd, char ch, unsigned size);
int sys_write_double(unsigned fd, double ch, unsigned size);
int sys_write_float(unsigned fd, float ch, unsigned size);
/* Implement WRITE for int */
int sys_write_int(unsigned fd, int buf, unsigned dummy);


size_t size_of_ptr(void *ptr);

size_t size_of_int(int dummy);

size_t size_of_char(char dummy);
size_t size_of_double(double dummy);
size_t size_of_float(float dummy);


#define get_size(x) _Generic((x), \
                        int* : size_of_ptr, \
                        int : size_of_int, \
                        double : size_of_double, \
                        float : size_of_float, \
                        char* : strlen, \
                        char : size_of_char, \
                        const char* : strlen \
                        )(x) 

#define _read(x) _Generic((x), \
                        int* : sys_read_int, \
                        char* : sys_read_str \
                        )(SYS_READ, x, get_size(x))

#define _read_s(x, size) _Generic((x), \
                        int* : sys_read_int, \
                        char* : sys_read_str \
                        )(SYS_READ, x, size)
                        

#define _write(x) _Generic((x), \
                        int : sys_write_int, \
                        double : sys_write_double, \
                        float : sys_write_float, \
                        char* : sys_write_str, \
                        char : sys_write_char, \
                        const char* : sys_write_str \
                        )(SYS_WRITE, x, get_size(x))

#define _write_s(x, size) _Generic((x), \
                        int : sys_write_int, \
                        char* : sys_write_str, \
                        char : sys_write_char, \
                        const char* : sys_write_str \
                        )(SYS_WRITE, x, size)

/* Implement printf using sys_write */
int _print_f(const char *format, ...);

#endif

