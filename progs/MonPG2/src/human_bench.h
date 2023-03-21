#ifndef _HUMAN_BENCH_H_
#define _HUMAN_BENCH_H_

#include <stdbool.h>

#include "window.h"

#define PRESS_KEY_DELAY     0.1f // seconds
#define PRESS_MOUSE_DELAY   0.1f // seconds

#define TARGET_RADIUS       20

#define MAX_SCORE           100

typedef enum {
    APP_STATE_EXIT,
    APP_STATE_INIT,
    APP_STATE_RUNNING,
    APP_STATE_MENU,
} AppState;

typedef struct {
    int                     width; 
    int                     height;
    Window                  *window;
    bool                    is_paused;

    int                     target_x;
    int                     target_y;
    SDL_Color               target_color;
    float                   best_time;
    float                   current_time;
    float                   start_time;
    float                   end_time;
    bool                    is_inside_target;

    int                     score;
    int                     errors;

    bool                    is_key_pressed;
    SDL_KeyboardEvent       last_key;
    const Uint8             *keys;
    float                   key_presss_timer;

    int                     fps;
    AppState                state;

    SDL_MouseButtonEvent    mouse_button;
    Sint32                  mouse_x;
    Sint32                  mouse_y;
    bool                    mouse_clicked;
    float                   mouse_click_timer;
} HumanBench;

HumanBench *human_bench_create(int width, int height);

void human_bench_destroy(HumanBench *app);

void human_bench_run(HumanBench *app);

void human_bench_update(HumanBench *app, float delta_time);

void human_bench_render(HumanBench *app);

void human_bench_input(HumanBench *app);

void human_bench_handle_keys(HumanBench *app);

void human_bench_handle_mouse(HumanBench *app);

void human_bench_handle_mouse_left_click(HumanBench *app);

void human_bench_handle_mouse_right_click(HumanBench *app);

void human_bench_generate_target(HumanBench *app);

void human_bench_display_init(HumanBench *app);

void human_bench_display_target(HumanBench *app);

void human_bench_display_best_time(HumanBench *app);

void human_bench_display_current_time(HumanBench *app);

void human_bench_display_score(HumanBench *app);

void human_bench_display_menu(HumanBench *app);

#endif // _HUMAN_BENCH_H_