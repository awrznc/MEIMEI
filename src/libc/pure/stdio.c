#include "../stdarg.h"

#define DECIMAL_SECTION 10
#define ASCII_NUMBER_CODE 0x30
#define FORMAT_SPECIFIER_PREFIX '%'
#define FORMAT_SPECIFIER_SIGNED_INTEGER 'd'

int vsprintf(char* result, const char* originalFormat, va_list args) {
    char* format = (char*)originalFormat;
    while ( *format ) {

        // 文字が%だった場合は処理に入る
        if(*format == FORMAT_SPECIFIER_PREFIX) {

            // ポインタを一つ進めて、次の文字判定する
            format++;
            switch (*format) {

                // 10進数
                case FORMAT_SPECIFIER_SIGNED_INTEGER: {
                    int length = 0;
                    int number_buffer[64] = { 0 };

                    int embed_value = va_arg(args, int);
                    for (;;) {
                        number_buffer[length++] = embed_value % DECIMAL_SECTION;
                        if (embed_value < DECIMAL_SECTION) break;
                        embed_value /= DECIMAL_SECTION;
                    }

                    // 符号がマイナスの場合は最後の値の符号を逆転させ、ハイフンを追加する
                    if (embed_value < 0) {
                        number_buffer[length-1] *= -1;
                        *(result++) = '-';
                    }

                    while (length) *(result++) = number_buffer[--length] + ASCII_NUMBER_CODE;

                    break;
                }
                default: {
                    break;
                }
            }
        } else {
            *(result++) = *(format);
        }

        format++;
    }

    *result = '\0';
    return 0;
}

/**
  * ```c
  * #include "stdio.h"
  * #include "string.h"
  *
  * int main() {
  *     char string[100] = { 0 };
  *
  *     sprintf(string, "%d", 123);
  *     if( strcmp(string, "123") != 0 ) return 1;
  *     if( strcmp(string, "124") == 0 ) return 1;
  *
  *     sprintf(string, "%d, %d", 345, 678);
  *     if( strcmp(string, "345, 678") != 0 ) return 1;
  *
  *     sprintf(string, "%d", -1);
  *     if( strcmp(string, "-1") != 0 ) return 1;
  *
  *     return 0;
  * }
  * ```
  **/
int sprintf(char* result, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int return_value = vsprintf(result, format, args);
    va_end(args);
    return return_value;
}
