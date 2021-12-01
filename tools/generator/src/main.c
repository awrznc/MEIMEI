#include <stdio.h>
#include <getopt.h>

#include "font/maptip.h"
#include "image/image.h"

#define CRYO_HELP_STRING "\
Usage:\n\
    meimei_generator [options] <path>\n\
Options:\n\
    -h --help           Display this information.\n\
    -t --target         Convert target. ( font | binary )\n\
    -i --import <path>  Specify target for import files.\n\
    -e --export <path>  Specify target for export files.\n"

static int print_help() {
    printf(CRYO_HELP_STRING);
    return 0;
}

static int print_unknown_option() {
    printf("meimei_generator: Unknown option.\n");
    return print_help();
}

static int get_options(int argc, char *argv[], char** target, char** import, char** export) {
    int opt = 0;
    int longindex = 0;
    struct option longopts[] = {
        { "help",          no_argument,        NULL, 'h' },
        { "target",        required_argument,  NULL, 't' },
        { "import",        required_argument,  NULL, 'i' },
        { "export",        required_argument,  NULL, 'e' },
        { 0,               0,                  0,     0  },
    };

    while ((opt = getopt_long(argc, argv, "i:e:t:h", longopts, &longindex)) != -1) {
        switch (opt) {
            case 'h': return print_help();
            case 't': { *target = optarg; break; }
            case 'i': { *import = optarg; break; }
            case 'e': { *export = optarg; break; }
            default: return print_unknown_option();
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("meimei_generator: Not enough arguments.\n");
        return print_help();
    }

    char* target = "";
    char* import = "";
    char* export = "";
    if (get_options(argc, argv, &target, &import, &export) == 0) return 1;

    if ( *import == '\0') {
        printf("meimei_generator: Import file path is not found.\n");
        return 1;
    }

    if ( *export == '\0') {
        printf("meimei_generator: Export file path is not found.\n");
        return 1;
    }

    if ( strcmp(target, "font") == 0 ) {
        if( Maptip_bmpToMaptip(import, export, 8, 16) == 0 ) return 1;
    } else if ( strcmp(target, "binary") == 0 ) {
        if( Image_GenerateBinaryFile(import, export) == 0 ) return 1;
    } else {
        printf("meimei_generator: Unknown target. ( %s )\n", target);
        return 1;
    }

    printf("meimei_generator: Export header file. ( %s -> %s )\n", import, export);
    return 0;
}
