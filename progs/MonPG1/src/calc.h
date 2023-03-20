#ifndef _CALC_H_
#define _CALC_H_

#include <stdbool.h>

#include "window.h"

#define PRESS_KEY_DELAY     0.1f // seconds
#define PRESS_MOUSE_DELAY   0.1f // seconds

typedef enum {
    APP_STATE_EXIT,
    APP_STATE_RUNNING
} AppState;

typedef struct {
    int                     width; 
    int                     height;
    Window                  *window;
    bool                    is_paused;

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
} Calc;

Calc *calc_create(int width, int height);

void calc_destroy(Calc *app);

void calc_run(Calc *app);

void calc_update(Calc *app, float delta_time);

void calc_render(Calc *app);

void calc_input(Calc *app);

void calc_handle_keys(Calc *app);

void calc_handle_mouse(Calc *app);

void calc_handle_mouse_left_click(Calc *app);

void calc_handle_mouse_right_click(Calc *app);

#endif // _CALC_H_