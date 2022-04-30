#include <stdio.h>
#include <kernel.h>
#include <MEIMEI_Public.h>

#include "meimei.h"

inline MEIMEI_Status MEIMEI_Run(MEIMEI* meimei) {
    return meimei->status;
}

static int load_stage_data(MEIMEI* meimei) {
    for (int y = 0; y < MEIMEI_STAGE_HEIGHT; y++) {
        for (int x = 0; x < MEIMEI_STAGE_WIDTH; x++) {
            meimei->data.stage[0].data[(y*MEIMEI_STAGE_WIDTH)+x] = stage_0[(y*MEIMEI_STAGE_WIDTH)+x];
        }
    }
    meimei->data.stage[0].start_position.x = stage_0_start_point_x;
    meimei->data.stage[0].start_position.y = stage_0_start_point_y;
    meimei->data.stage[0].goal_position.x = stage_0_goal_point_x;
    meimei->data.stage[0].goal_position.y = stage_0_goal_point_y;
    meimei->data.stage[0].data[meimei->player.current_point.y*MEIMEI_STAGE_WIDTH+meimei->player.current_point.x] = MEIMEI_STAGE_PLAYER;

    for (int y = 0; y < MEIMEI_STAGE_HEIGHT; y++) {
        for (int x = 0; x < MEIMEI_STAGE_WIDTH; x++) {
            meimei->data.stage[1].data[(y*MEIMEI_STAGE_WIDTH)+x] = stage_1[(y*MEIMEI_STAGE_WIDTH)+x];
        }
    }
    meimei->data.stage[1].start_position.x = stage_1_start_point_x;
    meimei->data.stage[1].start_position.y = stage_1_start_point_y;
    meimei->data.stage[1].goal_position.x = stage_1_goal_point_x;
    meimei->data.stage[1].goal_position.y = stage_1_goal_point_y;

    for (int y = 0; y < MEIMEI_STAGE_HEIGHT; y++) {
        for (int x = 0; x < MEIMEI_STAGE_WIDTH; x++) {
            meimei->data.stage[2].data[(y*MEIMEI_STAGE_WIDTH)+x] = stage_0[(y*MEIMEI_STAGE_WIDTH)+x];
        }
    }
    meimei->data.stage[2].start_position.x = stage_2_start_point_x;
    meimei->data.stage[2].start_position.y = stage_2_start_point_y;
    meimei->data.stage[2].goal_position.x = stage_2_goal_point_x;
    meimei->data.stage[2].goal_position.y = stage_2_goal_point_y;
    meimei->data.stage[2].data[10*MEIMEI_STAGE_WIDTH+19] = MEIMEI_STAGE_BACK_WALL;
    meimei->data.stage[2].data[10*MEIMEI_STAGE_WIDTH+20] = MEIMEI_STAGE_FLOOR;
    meimei->data.stage[2].data[10*MEIMEI_STAGE_WIDTH+21] = MEIMEI_STAGE_BACK_WALL;
    meimei->data.stage[2].data[11*MEIMEI_STAGE_WIDTH+19] = MEIMEI_STAGE_BACK_WALL;
    meimei->data.stage[2].data[11*MEIMEI_STAGE_WIDTH+20] = MEIMEI_STAGE_NULL;
    meimei->data.stage[2].data[11*MEIMEI_STAGE_WIDTH+21] = MEIMEI_STAGE_BACK_WALL;
    meimei->data.stage[2].data[8*MEIMEI_STAGE_WIDTH+20] = MEIMEI_STAGE_ITEM;
    meimei->data.stage[2].data[meimei->data.stage[0].goal_position.y*MEIMEI_STAGE_WIDTH+meimei->data.stage[0].goal_position.x] = MEIMEI_STAGE_BACK_WALL;

    return 0;
}

static void clear() {
    printf("\033[H");
}

static int print_black_fill() {
    clear();
    printf("\033[30m\033[40m");
    for (int i = 0; i < MEIMEI_STAGE_HEIGHT; i++) {
        for (int j = 0; j < MEIMEI_STAGE_WIDTH; j++) {
            printf(" ");
        }
        printf("\n");
    }
    clear();
    return 1;
}

static int draw_break_effect(int level) {
    clear();
    printf("\033[30m\033[47m");
    for (int i = 0; i < MEIMEI_STAGE_HEIGHT; i++) {
        for (int j = 0; j < MEIMEI_STAGE_WIDTH; j++) {
            if (is_draw(j,i,level)) {
                printf(" ");
            } else {
                printf("\033[C");
            }
        }
        printf("\n");
    }
    clear();
    return 1;
}

static void Initialize_MEIMEI(MEIMEI* meimei) {
    meimei->status = MEIMEI_StatusRun;
    print_black_fill();
    printf("\033[37m\033[40m");
    draw_break_effect(meimei->player.config.effect_number);
    meimei->player.current_point.x = stage_0_start_point_x;
    meimei->player.current_point.y = stage_0_start_point_y;
    load_stage_data(meimei);
}

/**
  * @brief キーが押されていたらtrue
  **/
static int is_press_key() {
    return __keydata < 128;
}

static int set_keyconfig(MEIMEI* meimei) {

    // キーが押されている場合かつ、一つ前のキーの状態と異なる場合
    if (
        is_press_key() &&
        __keydata != __before_keydata &&
        __keydata != meimei->player.config.key.action &&
        __keydata != meimei->player.config.key.left &&
        __keydata != meimei->player.config.key.right &&
        __keydata != meimei->player.config.key.up &&
        __keydata != meimei->player.config.key.down && (
            meimei->player.config.color_set_flag ||
            !(
                meimei->player.config.key.action &&
                meimei->player.config.key.left &&
                meimei->player.config.key.right &&
                meimei->player.config.key.up &&
                meimei->player.config.key.down
            )
        )
    ) {
        if (meimei->player.config.keybuf == __keydata) {
            meimei->player.config.counter++;
            meimei->player.config.effect_number++;
        } else {
            meimei->player.config.keybuf = __keydata;
            meimei->player.config.effect_number -= meimei->player.config.counter;
            meimei->player.config.counter = 0;
        }

        print_black_fill();
        if ( meimei->player.config.color_set_flag ) {
            draw_break_effect(meimei->player.config.effect_number/3);
            if ( meimei->player.config.counter <= 3) return 0;
        } else  {
            draw_break_effect(meimei->player.config.effect_number/2);
            if ( meimei->player.config.counter <= 3 ) return 0;
        }

        if (meimei->player.config.key.action == 0) {
            meimei->player.config.key.action = meimei->player.config.keybuf;
            meimei->player.config.keybuf = 0;
            meimei->player.config.counter = 0;
            // printf("\033[34m\033[45m");
            // printf("action: %d\n", meimei->player.config.key.action);

        } else if (meimei->player.config.key.left == 0) {
            meimei->player.config.key.left = meimei->player.config.keybuf;
            meimei->player.config.keybuf = 0;
            meimei->player.config.counter = 0;
            // printf("left: %d", meimei->player.config.key.left);
        } else if (meimei->player.config.key.right == 0) {
            meimei->player.config.key.right = meimei->player.config.keybuf;
            meimei->player.config.keybuf = 0;
            meimei->player.config.counter = 0;
            // printf("right: %d\033[A", meimei->player.config.key.right);
        } else if (meimei->player.config.key.up == 0) {
            meimei->player.config.key.up = meimei->player.config.keybuf;
            meimei->player.config.keybuf = 0;
            meimei->player.config.counter = 0;
            // printf("up: %d\n", meimei->player.config.key.up);
        } else if (meimei->player.config.key.down == 0) {
            meimei->player.config.key.down = meimei->player.config.keybuf;
            meimei->player.config.keybuf = 0;
            meimei->player.config.counter = 0;
            meimei->player.config.effect_number = 0;
            // printf("down: %d", meimei->player.config.key.down);
            // clear();
        } else if (meimei->player.config.color_set_flag && meimei->player.config.key.color == 0) {
            meimei->player.config.key.color = meimei->player.config.keybuf;
            meimei->player.config.keybuf = 0;
            meimei->player.config.counter = 0;
            meimei->player.config.effect_number = 0;
            meimei->player.config.color_set_flag = 0;
            meimei->player.config.color_use_flag = 1;
            meimei->player.update_stage_flag = 0;
        }

    } else {
        if (meimei->player.config.color_set_flag) {
            print_black_fill();
            draw_break_effect(meimei->player.config.effect_number/2);
            return 0;
        }

        if (
            meimei->player.config.key.action &&
            meimei->player.config.key.left &&
            meimei->player.config.key.right &&
            meimei->player.config.key.up &&
            meimei->player.config.key.down
        ) return 1;
    }

    return 0;
}


static void print_data(MEIMEI* meimei, char data) {
    switch (data) {
        case MEIMEI_STAGE_NULL: {
            printf("\033[37m\033[40m");
            printf(" ");
            break;
        }

        case MEIMEI_STAGE_FROMT_WALL: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[36m\033[45m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("$");
            break;
        }

        case MEIMEI_STAGE_BACK_WALL: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[35m\033[44m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("$");
            break;
        }

        case MEIMEI_STAGE_FLOOR: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[37m\033[43m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf(" ");
            break;
        }

        case MEIMEI_STAGE_YF: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[96m\033[44m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("*");
            break;
        }

        case MEIMEI_STAGE_RF: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[92m\033[44m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("*");
            break;
        }

        case MEIMEI_STAGE_DRAGON: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[92m\033[43m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("D");
            break;
        }

        case MEIMEI_STAGE_PLAYER: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[97m\033[43m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("@");
            break;
        }

        case MEIMEI_STAGE_ITEM: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[37m\033[43m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("+");
            break;
        }

        case MEIMEI_STAGE_FF: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[92m\033[43m");
            } else {
                printf("\033[37m\033[40m");
            }
            printf("*");
            break;
        }

        case MEIMEI_STAGE_BUSHES: {
            if (meimei->player.config.color_use_flag) {
                printf("\033[34m\033[43m");
                printf("$");
            } else {
                printf("\033[37m\033[40m");
                printf(" ");
            }

            break;
        }

        // Unknown
        default: {
            printf("%d", data);
            break;
        }
    }
}

static void print_stage(MEIMEI* meimei) {
    int current_stage = meimei->player.current_stage_number;
    for (int y = 0; y < MEIMEI_STAGE_HEIGHT; y++) {
        for (int x = 0; x < MEIMEI_STAGE_WIDTH; x++) {
            print_data( meimei, meimei->data.stage[current_stage].data[(y*MEIMEI_STAGE_WIDTH)+x] );
        }
        printf("\n");
    }
    clear();
}

static int get_position( MEIMEI* meimei, char data) {
    int current_stage = meimei->player.current_stage_number;
    for (int y = 0; y < MEIMEI_STAGE_HEIGHT; y++){
        for (int x = 0; x < MEIMEI_STAGE_WIDTH; x++){
            if (meimei->data.stage[current_stage].data[(y*MEIMEI_STAGE_WIDTH)+x] == data) {
                return (y*MEIMEI_STAGE_WIDTH)+x;
            }
        }
    }
    return -1;
}

/**
  * @brief リセットを行うデータはそのステージにおいてユニークなデータである必要がある
  **/
static int reset_stage_1_fruits_only( MEIMEI* meimei ) {
    int reset_positoion = get_position( meimei, MEIMEI_STAGE_FF );
    if( reset_positoion < 0 ) return 0;
    int current_stage = meimei->player.current_stage_number;
    meimei->data.stage[current_stage].data[reset_positoion] = MEIMEI_STAGE_FLOOR;
    meimei->data.stage[current_stage].data[(stage_1_reset_ff_point_y*MEIMEI_STAGE_WIDTH)+stage_1_reset_ff_point_x] = MEIMEI_STAGE_FF;
    return 0;
}

/**
  * @brief 条件が真である場合にステージを移動する
  * @note 出入口が MEIMEI_STAGE_NULL である前提。上下の移動にしか対応していない。
  **/
static int move_stage( MEIMEI* meimei, unsigned int next_position ) {
    unsigned char current_stage = meimei->player.current_stage_number;
    unsigned int goal_position = meimei->data.stage[current_stage].goal_position.y * MEIMEI_STAGE_WIDTH + meimei->data.stage[current_stage].goal_position.x;
    unsigned int start_position = meimei->data.stage[current_stage].start_position.y * MEIMEI_STAGE_WIDTH + meimei->data.stage[current_stage].start_position.x;
    if ( next_position == goal_position ) {
        if (current_stage <= MEIMEI_STAGE_LEVEL) {
            if ( current_stage == 1 ) reset_stage_1_fruits_only(meimei);
            meimei->data.stage[meimei->player.current_stage_number].data[next_position] = MEIMEI_STAGE_NULL;
            meimei->player.current_stage_number += 1;
            meimei->player.current_point.x = meimei->data.stage[meimei->player.current_stage_number].start_position.x;
            meimei->player.current_point.y = meimei->data.stage[meimei->player.current_stage_number].start_position.y - 1;
        } else {
            return 0;
        }

    } else if( next_position == start_position ) {
        if (current_stage > 0) {
            if ( current_stage == 1 ) reset_stage_1_fruits_only(meimei);
            meimei->data.stage[meimei->player.current_stage_number].data[next_position] = MEIMEI_STAGE_NULL;
            meimei->player.current_stage_number -= 1;
            meimei->player.current_point.x = meimei->data.stage[meimei->player.current_stage_number].goal_position.x;
            meimei->player.current_point.y = meimei->data.stage[meimei->player.current_stage_number].goal_position.y + 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
    meimei->data.stage[meimei->player.current_stage_number].data[meimei->player.current_point.y*MEIMEI_STAGE_WIDTH+meimei->player.current_point.x] = MEIMEI_STAGE_PLAYER;

    return 1;
}

static int dragon_process(MEIMEI* meimei, int ff_next_position ) {
    int current_stage = meimei->player.current_stage_number;
    int left_data  = meimei->data.stage[current_stage].data[ff_next_position-1];
    int right_data = meimei->data.stage[current_stage].data[ff_next_position+1];
    int up_data    = meimei->data.stage[current_stage].data[ff_next_position-MEIMEI_STAGE_WIDTH];
    int down_data  = meimei->data.stage[current_stage].data[ff_next_position+MEIMEI_STAGE_WIDTH];
    if ( left_data == MEIMEI_STAGE_DRAGON ) {
        meimei->data.stage[current_stage].data[ff_next_position-1] = MEIMEI_STAGE_FLOOR;
        meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_DRAGON;
    } else if ( right_data == MEIMEI_STAGE_DRAGON ) {
        meimei->data.stage[current_stage].data[ff_next_position+1] = MEIMEI_STAGE_FLOOR;
        meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_DRAGON;
    } else if ( up_data == MEIMEI_STAGE_DRAGON ) {
        meimei->data.stage[current_stage].data[ff_next_position-MEIMEI_STAGE_WIDTH] = MEIMEI_STAGE_FLOOR;
        meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_DRAGON;
    } else if ( down_data == MEIMEI_STAGE_DRAGON ) {
        meimei->data.stage[current_stage].data[ff_next_position+MEIMEI_STAGE_WIDTH] = MEIMEI_STAGE_FLOOR;
        meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_DRAGON;
    }

    return 1;
}

static inline int is_move(char data) {
    return data != MEIMEI_STAGE_NULL && data != MEIMEI_STAGE_FLOOR && data != MEIMEI_STAGE_FF && data != MEIMEI_STAGE_BUSHES;
}

static int move_floor(MEIMEI* meimei) {
    int current_stage = meimei->player.current_stage_number;
    int current_position = (meimei->player.current_point.y * MEIMEI_STAGE_WIDTH) + meimei->player.current_point.x;
    if ( is_press_key() && __keydata != __before_keydata ) {

        // Left
        if (__keydata == meimei->player.config.key.left) {
            int next_position = current_position - 1;
            char move_data = meimei->data.stage[current_stage].data[next_position];
            if ( is_move(move_data) ) return 0;

            if ( move_data == MEIMEI_STAGE_BUSHES && meimei->player.config.color_use_flag ) return 0;
            int overwrite_data = MEIMEI_STAGE_FLOOR;
            int bushes_point = (stage_1_reset_bushes_point_y*MEIMEI_STAGE_WIDTH)+stage_1_reset_bushes_point_x;
            if ( meimei->player.current_stage_number == 1 && current_position == bushes_point ) overwrite_data = MEIMEI_STAGE_BUSHES;

            if ( move_data == MEIMEI_STAGE_FF) {
                int ff_next_position = next_position - 1;
                int ff_next_data = meimei->data.stage[current_stage].data[ff_next_position];
                if ( ff_next_data != MEIMEI_STAGE_FLOOR ) return 0;
                meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_FLOOR;
                meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_FF;
                dragon_process(meimei, ff_next_position);
            }
            meimei->data.stage[current_stage].data[current_position] = overwrite_data;
            meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_PLAYER;
            meimei->player.current_point.x -= 1;
            meimei->player.update_stage_flag = 0;

        // Right
        } else if (__keydata == meimei->player.config.key.right) {
            int next_position = current_position + 1;
            int move_data = meimei->data.stage[current_stage].data[next_position];
            if ( is_move(move_data) ) return 0;

            if ( move_data == MEIMEI_STAGE_BUSHES && meimei->player.config.color_use_flag ) return 0;
            int overwrite_data = MEIMEI_STAGE_FLOOR;
            int bushes_point = (stage_1_reset_bushes_point_y*MEIMEI_STAGE_WIDTH)+stage_1_reset_bushes_point_x;
            if ( meimei->player.current_stage_number == 1 && current_position == bushes_point ) overwrite_data = MEIMEI_STAGE_BUSHES;

            if ( move_data == MEIMEI_STAGE_FF) {
                int ff_next_position = next_position + 1;
                int ff_next_data = meimei->data.stage[current_stage].data[ff_next_position];
                if ( ff_next_data != MEIMEI_STAGE_FLOOR ) return 0;
                meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_FLOOR;
                meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_FF;
                dragon_process(meimei, ff_next_position);
            }
            meimei->data.stage[current_stage].data[current_position] = overwrite_data;
            meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_PLAYER;
            meimei->player.current_point.x += 1;
            meimei->player.update_stage_flag = 0;

        // Up
        } else if (__keydata == meimei->player.config.key.up) {
            int next_position = current_position - MEIMEI_STAGE_WIDTH;
            int move_data = meimei->data.stage[current_stage].data[next_position];
            if ( is_move(move_data) ) return 0;

            if ( move_data == MEIMEI_STAGE_BUSHES && meimei->player.config.color_use_flag ) return 0;
            int overwrite_data = MEIMEI_STAGE_FLOOR;
            int bushes_point = (stage_1_reset_bushes_point_y*MEIMEI_STAGE_WIDTH)+stage_1_reset_bushes_point_x;
            if ( meimei->player.current_stage_number == 1 && current_position == bushes_point ) overwrite_data = MEIMEI_STAGE_BUSHES;

            if ( move_data == MEIMEI_STAGE_FF) {
                int ff_next_position = next_position - MEIMEI_STAGE_WIDTH;
                int ff_next_data = meimei->data.stage[current_stage].data[ff_next_position];
                if ( ff_next_data != MEIMEI_STAGE_FLOOR ) return 0;
                meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_FLOOR;
                meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_FF;
                dragon_process(meimei, ff_next_position);
            }
            meimei->data.stage[current_stage].data[current_position] = overwrite_data;
            meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_PLAYER;
            meimei->player.current_point.x -= MEIMEI_STAGE_WIDTH;
            meimei->player.update_stage_flag = 0;
            move_stage(meimei, next_position);

        // Down
        } else if (__keydata == meimei->player.config.key.down) {
            int next_position = current_position + MEIMEI_STAGE_WIDTH;
            int move_data = meimei->data.stage[current_stage].data[next_position];
            if ( is_move(move_data) ) return 0;

            if ( move_data == MEIMEI_STAGE_BUSHES && meimei->player.config.color_use_flag ) return 0;
            int overwrite_data = MEIMEI_STAGE_FLOOR;
            int bushes_point = (stage_1_reset_bushes_point_y*MEIMEI_STAGE_WIDTH)+stage_1_reset_bushes_point_x;
            if ( meimei->player.current_stage_number == 1 && current_position == bushes_point ) overwrite_data = MEIMEI_STAGE_BUSHES;

            if ( move_data == MEIMEI_STAGE_FF) {
                int ff_next_position = next_position + MEIMEI_STAGE_WIDTH;
                int ff_next_data = meimei->data.stage[current_stage].data[ff_next_position];
                if ( ff_next_data != MEIMEI_STAGE_FLOOR ) return 0;
                meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_FLOOR;
                meimei->data.stage[current_stage].data[ff_next_position] = MEIMEI_STAGE_FF;
                dragon_process(meimei, ff_next_position);
            }
            meimei->data.stage[current_stage].data[current_position] = overwrite_data;
            meimei->data.stage[current_stage].data[next_position] = MEIMEI_STAGE_PLAYER;
            meimei->player.current_point.x += MEIMEI_STAGE_WIDTH;
            meimei->player.update_stage_flag = 0;
            move_stage(meimei, next_position);

        // Action
        } else if (__keydata == meimei->player.config.key.action) {
            int left_data  = meimei->data.stage[current_stage].data[current_position-1];
            int right_data = meimei->data.stage[current_stage].data[current_position+1];
            int up_data    = meimei->data.stage[current_stage].data[current_position-MEIMEI_STAGE_WIDTH];
            int down_data  = meimei->data.stage[current_stage].data[current_position+MEIMEI_STAGE_WIDTH];
            if (left_data == MEIMEI_STAGE_ITEM) {
                meimei->player.config.color_set_flag = 1;
                meimei->data.stage[current_stage].data[current_position-1] = MEIMEI_STAGE_FLOOR;
            } else if (right_data == MEIMEI_STAGE_ITEM) {
                meimei->player.config.color_set_flag = 1;
                meimei->data.stage[current_stage].data[current_position+1] = MEIMEI_STAGE_FLOOR;
            } else if (up_data == MEIMEI_STAGE_ITEM) {
                meimei->player.config.color_set_flag = 1;
                meimei->data.stage[current_stage].data[current_position-MEIMEI_STAGE_WIDTH] = MEIMEI_STAGE_FLOOR;
            } else if (down_data == MEIMEI_STAGE_ITEM) {
                meimei->player.config.color_set_flag = 1;
                meimei->data.stage[current_stage].data[current_position+MEIMEI_STAGE_WIDTH] = MEIMEI_STAGE_FLOOR;
            } else if (down_data == MEIMEI_STAGE_RF) {
                meimei->data.stage[current_stage].data[current_position-MEIMEI_STAGE_WIDTH] = MEIMEI_STAGE_FF;
                meimei->data.stage[current_stage].data[current_position+MEIMEI_STAGE_WIDTH] = MEIMEI_STAGE_BACK_WALL;
            } else {
                return 0;
            }

            if (meimei->player.current_stage_number == MEIMEI_STAGE_LEVEL - 1 && (
                left_data == 8 || right_data == 8 || up_data == 8 || down_data == 8
            )) {
                meimei->status = MEIMEI_StatusNone;
            }

            meimei->player.update_stage_flag = 0;

        // Color
        } else if (__keydata == meimei->player.config.key.color) {
            meimei->player.config.color_use_flag = meimei->player.config.color_use_flag < 1 ? meimei->player.config.color_use_flag+1 : 0;
            meimei->player.update_stage_flag = 0;
        } else {
            meimei->player.config.color_use_flag = 0;
        }
    }
    return 1;
}

int MEIMEI_Main() {

    MEIMEI meimei = MEIMEI_New();
    Initialize_MEIMEI(&meimei);

    while ( Kernel_Status() && meimei.status == MEIMEI_StatusRun ) {
        if(!set_keyconfig(&meimei)) continue;
        if(!meimei.player.update_stage_flag) {
            print_stage(&meimei);
            meimei.player.update_stage_flag = 1;
            continue;
        }
        move_floor(&meimei);
    }

    return 0;
}
