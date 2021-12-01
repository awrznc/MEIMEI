#ifndef INCLUDED_STDIO
#define INCLUDED_STDIO

#include <stdarg.h>

#define NULL 0

typedef struct {
    char* pointer;
    unsigned int position_x;
    unsigned int position_y;
    char foreground_color;
    char background_color;
} FILE;

FILE* stdin;
FILE* stdout;
FILE* stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

int vsprintf(char* result, const char* originalFormat, va_list args);

int vfprintf(FILE *stream, const char *format, va_list args);

int vprintf(const char* format, va_list args);

int sprintf(char* result, const char* format, ...);

int fprintf(FILE* stream, const char *format, ...);

int printf(const char *format, ...);

#endif
