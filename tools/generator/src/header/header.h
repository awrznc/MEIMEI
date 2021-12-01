#ifndef INCLUDED_HEADER
#define INCLUDED_HEADER

#include <stdio.h>
#include <string.h>

#define CONTENT_COUNT_IN_ONE_LINE 16
#define MAX_FILE_PATH 255

static int Header_WriteFile(
    FILE* exportFileObject,
    const char* exportFileName,
    const char* exportContents,
    unsigned int contentSize
) {
    char variable_name[MAX_FILE_PATH] = { 0 };
    char* variable_name_pointer = variable_name;
    strcpy(variable_name, exportFileName);

    {
        char* temporary_search = variable_name;
        unsigned int last_slash_point = 0;
        unsigned int current_point = 0;
        while (*temporary_search != '\0') {
            current_point++;

            // replace `.` to `_`
            if (*temporary_search == '.') *temporary_search = '_';
            if (*temporary_search == '/') last_slash_point = current_point;
            temporary_search++;
        }

        // remove `/`
        variable_name_pointer += last_slash_point;
    }

    {
        unsigned char count = 1;
        unsigned int size = 0;

        fprintf(exportFileObject, "unsigned char __%s[] = {\n    ", variable_name_pointer);
        for (int i = 0; i < contentSize-1; i++) {
            size++;
            fprintf(exportFileObject, "0x%02x, ", (unsigned char)exportContents[i]);
            if (count % CONTENT_COUNT_IN_ONE_LINE == 0) {
                count = 1;
                fprintf(exportFileObject, "\n    ");
                continue;
            }
            count++;
        }
        size++;
        fprintf(exportFileObject, "0x%02x", (unsigned char)exportContents[contentSize-1]);
        fprintf(exportFileObject, "\n};\n");
        fprintf(exportFileObject, "unsigned int __%s_size = %u;\n", variable_name_pointer, size);
    }

    return 1;
}


#endif
