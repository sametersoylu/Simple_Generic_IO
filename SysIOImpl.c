#include "SysIO.h"
#include <stdarg.h>

size_t stupidstrlen(void * ptr) {
  const char * str = (const char*)ptr;
  return strlen(str); 
}

inline int sys_read_str(unsigned int fd, char *buf, unsigned int size){
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
    int neg = 1; 
    char digit;
    unsigned ret; 
    unsigned size = sizeof(char);

    while (1) {
        asm volatile (
            "syscall"
            : "=a" (ret)
            : "a" (SYS_READ), "D" (fd), "S" (&digit), "d" (size)
            : "rcx", "r11", "memory"
        );

        if ((digit >= '0' && digit <= '9') || digit == '-') {
            if (digit != '-') {
                num = num * 10 + (digit - '0');
            }
            if (digit == '-')  {
                neg = -1; 
            }
        } else {
            break;
        }
    }
    *buf = num * neg; 
    return ret;
}
int sys_read_float(unsigned int fd, float *buf, unsigned int dummy) {
    float num = 0.0f;
    float decim = 0.0f; 
    int neg = 1; 
    char digit;
    unsigned ret; 
    unsigned size = sizeof(char);
    int dec = 0;
    while (1) {
        asm volatile (
            "syscall"
            : "=a" (ret)
            : "a" (SYS_READ), "D" (fd), "S" (&digit), "d" (size)
            : "rcx", "r11", "memory"
        );
        if ((digit >= '0' && digit <= '9') || digit == '-' || digit == '.') { 
            if (digit == '.') {
                dec = 1;
                continue;
            } 
            if (digit == '-') {
                neg = -1; 
                continue;
            }
            if(dec == 0) {
              num = num * 10.0f + (digit - '0'); 
              continue;
            }
            decim = decim * 10.0f + (digit - '0'); 
        } else {
            break;
        }
    }
    while(decim >= 1) {
      decim = decim / 10.0f; 
    }
    
    *buf = (num + decim);
    *buf *= neg; 
    return ret;
}


int sys_write_double_precision(unsigned int fd, double ch, unsigned int precision) {
    unsigned ret;
    char str[24];
    int num = ch;
    int i = 0;
    int j = 0;
    
    if (ch < 0) {
        str[i++] = '-';
        num = -num;
        ch = -ch;
    }
    
    // First left hand side
    do {
      str[i++] = num % 10 + '0';
      num /= 10; 
    } while(num > 0);
    
    str[i++] = '.';
    
    double frac_part = ch - (int)ch;
    for (int j = 0; j < precision; j++) {
        frac_part *= 10;
        int digit = (int)frac_part;
        str[i++] = digit + '0';
        frac_part -= digit;
    }
    
    
    asm volatile (
        "syscall"
        : "=a" (ret)
        : "a" (SYS_WRITE), "D" (fd), "S" (str), "d" (i)
        : "rcx", "r11", "memory"
    );

    return ret;
}

/* As default only prints 2 decimal point low precision */
int sys_write_double(unsigned fd, double ch, unsigned int size) {
  return sys_write_double_precision(fd, ch, 2);
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

    if (num < 0) {
        is_neg = 1;
        num = -num;
    }

    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    if (is_neg) {
        str[i++] = '-';
    }

    for (j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }

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

int strtointsimple(const char * str) {
  int i = 0; 
  int num = 0; 
  int neg = 1;
  int start = 0; 
  while(str[i] != '\0') {
    if(str[i] == '-') {
      neg = -1; 
      continue;
    }
    if(str[i] >= '0' && str[i] <= '9') {
      start = !start; 
      if(!start) 
        break;
    } 
    if(start) {
      num = num * 10 + (str[i] - '0');
    }
  }
  return num * neg; 
} 
#include<stdio.h>
int _print_f(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = 0; 
    int prec = 2; 
    for (int i = 0; format[i] != '\0'; i++) {
        if(format[i] == '%') {
            ++i;
            if(format[i] == '.') {
              char str[10];
              int j = 0; 
              i++; 
              while(format[i] > '0' && format[i] <= '9' && j < 10) {
                str[j++] = format[i++]; 
              }
              prec = strtointsimple(str); 
            }
            if(format[i] == 'd') {
                int num = va_arg(args, int);
                ret += sys_write_int(1, num, sizeof(num));
            } else if(format[i] == 'c') {
                char ch = va_arg(args, int);
                ret += sys_write_char(1, ch, sizeof(ch));
            } else if(format[i] == 's') {
                char* str = va_arg(args, char*);
                ret += sys_write_str(1, str, strlen(str));
            } else if(format[i] == 'f') {
                double num = va_arg(args, double);
                ret += sys_write_double_precision(1, num, prec);
            }
            continue;
        }
        ret += sys_write_char(1, format[i], sizeof(format[i]));
    }

    va_end(args);
    return ret;    
}
