#ifndef _CALC_H_
#define _CALC_H_

#include <stdbool.h>

#include "button.h"
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

    Button                  *buttons[BUTTON_COUNT];

    char                    *display_text;
    float                   result;
    int                     number_left;
    int                     number_right;
    char                    operation;
    bool                    display_result;
    bool                    do_evaluate;
    bool                    number_clicked;
    bool                    operation_clicked;

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

void calc_display_buttons(Calc *app);

void calc_handle_button_click(Calc *app, Button *button);

void calc_display_result_screen(Calc *app);

void calc_evaluate(Calc *app);

#endif // _CALC_H_