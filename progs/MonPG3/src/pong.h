#ifndef _PONG_H_
#define _PONG_H_

#include <stdbool.h>

#include "window.h"

#define PRESS_KEY_DELAY             0.1f // seconds
#define PRESS_MOUSE_DELAY           0.1f // seconds

#define PADDLE_WIDTH                10
#define PADDLE_HEIGHT               100

#define PADDLE_SPEED                200 // pixels per second

#define BALL_SPEED                  200 // pixels per second

#define BALL_RADIUS                 5

typedef enum {
    START_BUTTON,
    TWO_PLAYERS_BUTTON,
    EXIT_BUTTON,
    BUTTON_COUNT,
} Button;

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

    int                     selected_button;
    int                     selected_player_count;

    bool                    is_key_pressed;
    SDL_KeyboardEvent       last_key;
    const Uint8             *keys;
    float                   key_presss_timer;

    int                     score_left;
    int                     score_right;

    int                     player_left_x;
    int                     player_left_y;
    int                     player_left_dy;

    int                     player_right_x;
    int                     player_right_y;
    int                     player_right_dy;

    int                     ball_x;
    int                     ball_y;
    int                     ball_dx;
    int                     ball_dy;

    int                     fps;
    AppState                state;

    SDL_MouseButtonEvent    mouse_button;
    Sint32                  mouse_x;
    Sint32                  mouse_y;
    bool                    mouse_clicked;
    float                   mouse_click_timer;
} Pong;

Pong *pong_create(int width, int height);

void pong_destroy(Pong *app);

void pong_run(Pong *app);

void pong_update(Pong *app, float delta_time);

void pong_update_ball(Pong *app, float delta_time);

void pong_update_ai(Pong *app, float delta_time);

void pong_update_player(Pong *app, float delta_time);

void pong_render(Pong *app);

void pong_input(Pong *app);

void pong_handle_keys(Pong *app);

void pong_handle_mouse(Pong *app);

void pong_handle_mouse_left_click(Pong *app);

void pong_handle_mouse_right_click(Pong *app);

void pong_display_init(Pong *app);

void pong_display_running(Pong *app);

void pong_display_score(Pong *app);

void pong_display_map(Pong *app);

void pong_display_ball(Pong *app);

void pong_display_paddle(Pong *app);

void pong_display_menu(Pong *app);

#endif // _PONG_H_