#ifndef SYSIO_H
#define SYS_READ 0
#define SYS_WRITE 1
#include <string.h>


/* Implement READ */
int sys_read_str(unsigned fd, char *buf, unsigned size);

/* Implement READ for int */
int sys_read_int(unsigned fd, int * buf, unsigned dummy);

int sys_read_float(unsigned int fd, float *buf, unsigned int dummy);

/* Implement WRITE */
int sys_write_str(unsigned fd, const char *buf, unsigned size);
int sys_write_char(unsigned fd, char ch, unsigned size);
int sys_write_double(unsigned fd, double ch, unsigned size);
int sys_write_double_precision(unsigned fd, double ch, unsigned int size);
/* Implement WRITE for int */
int sys_write_int(unsigned fd, int buf, unsigned dummy);

size_t stupidstrlen(void * ptr); 
size_t size(int dummy); 
#define strlength(x) _Generic((x), char*: stupidstrlen, const char*:stupidstrlen, default: size)(x)

#define get_size(x) _Generic((x), \
                              char*: strlength(x),\
                              const char*: strlength(x),\
                              default: sizeof(x)\
                            )

#define _read_s(x, size) _Generic((x), \
                        int* : sys_read_int, \
                        char* : sys_read_str, \
                        float* : sys_read_float,\
                        double* : sys_read_float\
                        )(SYS_READ, x, size)

#define _read(x) _read_s(x, get_size(x))
                        

#define _write(x) _Generic((x), \
                        int : sys_write_int, \
                        double : sys_write_double, \
                        float : sys_write_double, \
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

