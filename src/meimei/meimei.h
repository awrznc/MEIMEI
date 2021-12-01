#include "effect.h"

#define MEIMEI_New() { 0 }

typedef enum {
    MEIMEI_StatusNone,
    MEIMEI_StatusRun
} MEIMEI_Status;

typedef struct {
    int up;
    int down;
    int left;
    int right;
    int action;
    int color;
} MEIMEI_Key;

typedef struct {
    unsigned char status;
    unsigned char counter;
    unsigned char effect_number;
    unsigned char input_flag;
    unsigned char color_use_flag;
    unsigned char color_set_flag;
    int keybuf;
    MEIMEI_Key key;
} MEIMEI_Config;

typedef struct {
    unsigned int x;
    unsigned int y;
} MEIMEI_Point;

typedef struct {
    MEIMEI_Point start_position;
    MEIMEI_Point goal_position;
    char data[MEIMEI_STAGE_WIDTH * MEIMEI_STAGE_HEIGHT];
} MEIMEI_Stage;

typedef struct {
    unsigned char current_stage_number;
    unsigned char update_stage_flag;
    MEIMEI_Point current_point;
    MEIMEI_Config config;
} MEIMEI_Player;

typedef struct {
    MEIMEI_Stage stage[MEIMEI_STAGE_LEVEL];
} MEIMEI_Data;

typedef struct {
    MEIMEI_Status status;
    MEIMEI_Player player;
    MEIMEI_Data data;
} MEIMEI;
