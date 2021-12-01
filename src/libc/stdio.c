#include "stdio.h"
#include "graphic.h"

#undef stdin
#undef stdout
#undef stderr

FILE __stdin_entity = { 0 };
FILE __stdout_entity = { 0 };
FILE __stderr_entity = { 0 };

FILE* stdin = &__stdin_entity;
FILE* stdout = &__stdout_entity;
FILE* stderr = &__stderr_entity;

static int prefix_match(char* a, char* b) {
    int flag = 1;
    while (*b != '\0') {
        if(*a != *b) {
            flag = 0;
            break;
        }
        a++;
        b++;
    }
    return flag;
}

/**
  * @brief 色やカーソル位置などを制御しながら文字列を出力する
  * @param [in] stream 出力の制御に利用する情報
  * @param [in] print_string 出力文字列
  **/
static int ansi_puts( FILE *stream, char* print_string ) {
    BootInfo* boot_info = BiosInitializeBootInfo();

    for(int count = 0; *print_string != 0x00; print_string++, count++) {

        // newline
        if (prefix_match(print_string, (char*)"\n")) {
            print_string += 1;
            stream->position_y++;
            stream->position_x = 0;

        // return
        } else if (prefix_match(print_string, (char*)"\r")) {
            print_string += 1;
            stream->position_x = 0;

        // move the cursor up
        } else if (prefix_match(print_string, (char*)"\033[A")) {
            print_string += 3;
            if ( 0 < stream->position_y ) {
                stream->position_y--;
                stream->position_x = 0;
            }

        // move the cursor right
        } else if (prefix_match(print_string, (char*)"\033[C")) {
            print_string += 3;
            stream->position_x++;

        // reset the cursor point
        } else if (prefix_match(print_string, (char*)"\033[H")) {
            print_string += 3;
            stream->position_y = 0;
            stream->position_x = 0;

        // change color
        } else if (prefix_match(print_string, (char*)"\033[")) {
            int move_count = 2;
            if ( *(print_string+move_count) == '3' ) {
                move_count++;
                if ( *(print_string+move_count) >= '0' || *(print_string+move_count) <= '7' ) {
                    char color = *(print_string+move_count) - '0';
                    move_count++;
                    if ( *(print_string+move_count) == 'm') {
                        print_string += move_count;
                        stdout->foreground_color = color;
                    }
                }
            } else if ( *(print_string+move_count) == '9' ) {
                move_count++;
                if ( *(print_string+move_count) >= '0' || *(print_string+move_count) <= '7' ) {
                    char color = *(print_string+move_count) - '0' + 8;
                    move_count++;
                    if ( *(print_string+move_count) == 'm') {
                        print_string += move_count;
                        stdout->foreground_color = color;
                    }
                }
            } else if ( *(print_string+move_count) == '4' ) {
                move_count++;
                if ( *(print_string+move_count) >= '0' || *(print_string+move_count) <= '7' ) {
                    char color = *(print_string+move_count) - '0';
                    move_count++;
                    if ( *(print_string+move_count) == 'm') {
                        print_string += move_count;
                        stdout->background_color = color;
                    }
                }
            }

        // put
        } else {
            GraphicFillSquare2( boot_info->vram, boot_info->screen_x, stream->background_color, stream->position_x * 8,  stream->position_y * 16, 8, 16 );
            GraphicPut( boot_info, stream->position_x * 8,  stream->position_y * 16, stream->foreground_color, *(unsigned char*)print_string );
            stream->position_x += 1;
        }
    }

    return 1;
}

int vfprintf(FILE *stream, const char *format, va_list args) {
    char print_string[0xff] = { 0 };
    vsprintf(print_string, format, args);
    return ansi_puts(stream, print_string);
}

int vprintf(const char* format, va_list args) {
    return vfprintf(stdout, format, args);
}

int fprintf(FILE* stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int return_value = vfprintf(stream, format, args);
    va_end(args);
    return return_value;
}

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int return_value = vprintf(format, args);
    va_end(args);
    return return_value;
}
