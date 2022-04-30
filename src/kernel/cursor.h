#ifndef INCLUDED_CURSOR
#define INCLUDED_CURSOR

void Initialize_Cursor(char* mouse, char mouseColor, char backgroundColor) {
    static char cursor[5*5] = {
        0,1,1,1,0,
        1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
        0,1,1,1,0
    };

    int height = 5;
    int width = 5;

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            mouse[y * width + x] = (cursor[y * width + x] == 1) ? mouseColor : backgroundColor;
        }
    }
}

#endif
