#include "SysIO.h"
#include <stdarg.h>

size_t size_of_ptr(void* dummy) {
    return sizeof(dummy);
}

size_t size_of_int(int dummy) {
    return sizeof(dummy);
}

size_t size_of_double(double dummy) {
    return sizeof(dummy);
}

size_t size_of_float(float dummy) {
    return sizeof(dummy);
}

size_t size_of_char(char dummy) {
    return sizeof(dummy);
}

int sys_read_str(unsigned int fd, char *buf, unsigned int size){
  unsigned ret;

  asm volatile (
    "syscall"
    : "=a"(ret)
    : "a"(SYS_READ), "D"(fd), "S"(buf), "d"(size)
    : "rcx", "r11", "memory"
  );

  for(unsigned int i = 0; i < size; i++){
    if(buf[i] == '\n'){
      buf[i] = '\0';
      break;
    }
  }

  return ret;
}

int sys_read_int(unsigned int fd, int *buf, unsigned int dummy) {
    int num = 0;
    char digit;
    unsigned ret; 
    unsigned size = sizeof(char);

    // Loop until a non-digit character is encountered
    while (1) {
        // Read a single character
        asm volatile (
            "syscall"
            : "=a" (ret)
            : "a" (SYS_READ), "D" (fd), "S" (&digit), "d" (size)
            : "rcx", "r11", "memory"
        );

        // Check if it's a digit or negative sign
        if ((digit >= '0' && digit <= '9') || digit == '-') {
            // Update the number accordingly
            if (digit != '-') {
                num = num * 10 + (digit - '0');
            }
        } else {
            // Stop reading if it's not a digit or negative sign
            break;
        }
    }
    *buf = num; 
    return ret;
}

int sys_write_double(unsigned int fd, double ch, unsigned int size) {
    unsigned ret;
    char str[12];
    int num = ch;
    int i = 0;
    int j = 0;
    int is_neg = 0;

    // Check if the number is negative
    if (num < 0) {
        is_neg = 1;
        num = -num;
    }

    // Convert the number to a string
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Add the negative sign if necessary
    if (is_neg) {
        str[i++] = '-';
    }

    // Reverse the string
    for (j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    // Write the string to the file descriptor
    asm volatile (
        "syscall"
        : "=a" (ret)
        : "a" (SYS_WRITE), "D" (fd), "S" (str), "d" (i)
        : "rcx", "r11", "memory"
    );

    return ret;
}

int sys_write_float(unsigned int fd, float ch, unsigned int size) {
    unsigned ret;
    char str[12];
    int num = ch;
    int i = 0;
    int j = 0;
    int is_neg = 0;

    // Check if the number is negative
    if (num < 0) {
        is_neg = 1;
        num = -num;
    }

    // Convert the number to a string
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Add the negative sign if necessary
    if (is_neg) {
        str[i++] = '-';
    }

    // Reverse the string
    for (j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    // Write the string to the file descriptor
    asm volatile (
        "syscall"
        : "=a" (ret)
        : "a" (SYS_WRITE), "D" (fd), "S" (str), "d" (i)
        : "rcx", "r11", "memory"
    );

    return ret;
}

int sys_write_str(unsigned int fd, const char *buf, unsigned int size) {
    unsigned ret;

    asm volatile (
        "syscall"
        : "=a" (ret)
        : "a" (SYS_WRITE), "D" (fd), "S" (buf), "d" (size)
        : "rcx", "r11", "memory"
    );

    return ret;
}

int sys_write_int(unsigned int fd, int buf, unsigned int dummy) {
    unsigned ret;
    char str[12];
    int num = buf;
    int i = 0;
    int j = 0;
    int is_neg = 0;

    // Check if the number is negative
    if (num < 0) {
        is_neg = 1;
        num = -num;
    }

    // Convert the number to a string
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    // Add the negative sign if necessary
    if (is_neg) {
        str[i++] = '-';
    }

    // Reverse the string
    for (j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

    // Write the string to the file descriptor
    asm volatile (
        "syscall"
        : "=a" (ret)
        : "a" (SYS_WRITE), "D" (fd), "S" (str), "d" (i)
        : "rcx", "r11", "memory"
    );

    return ret;
}

int sys_write_char(unsigned int fd, char ch, unsigned int size) {
    unsigned ret;

    asm volatile (
        "syscall"
        : "=a" (ret)
        : "a" (SYS_WRITE), "D" (fd), "S" (&ch), "d" (size)
        : "rcx", "r11", "memory"
    );

    return ret;

}

int _print_f(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = 0; 

    for (int i = 0; format[i] != '\0'; i++) {
        if(format[i] == '%') {
            ++i;
            if(format[i] == 'd') {
                int num = va_arg(args, int);
                ret += sys_write_int(1, num, size_of_int(num));
            } else if(format[i] == 'c') {
                char ch = va_arg(args, int);
                ret += sys_write_char(1, ch, size_of_char(ch));
            } else if(format[i] == 's') {
                char* str = va_arg(args, char*);
                ret += sys_write_str(1, str, strlen(str));
            } else if(format[i] == 'f') {
                double num = va_arg(args, double);
                ret += sys_write_double(1, num, size_of_double(num));
            }
            continue;
        }
        ret += sys_write_char(1, format[i], size_of_char(format[i]));
    }

    va_end(args);
    return ret;    
}