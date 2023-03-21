#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "pong.h"
#include "window.h"

#define APP_NAME "pong"

#define _XOPEN_SOURCE 500

static const int PLAYER_COUNTS[] = { 1, 2 };

Pong *pong_create(int width, int height)
{
    Pong *app = malloc(sizeof(*app));
    if (app == NULL) {
        fprintf(stderr, "Failed to allocate memory for Pong\n");
        return NULL;
    }

    app->width  = width;
    app->height = height;

    app->window = window_create(APP_NAME, width, height);
    if (app->window == NULL) return NULL;

    app->state                  = APP_STATE_INIT;
    app->mouse_clicked          = false;
    app->mouse_click_timer      = 0;

    app->keys                   = SDL_GetKeyboardState(NULL);
    app->is_key_pressed         = false;

    app->score_left             = 0;
    app->score_right            = 0;

    app->player_left_x          = 5;
    app->player_left_y          = height / 2 - PADDLE_HEIGHT / 2;
    app->player_left_dy         = PADDLE_SPEED;

    app->player_right_x         = width - 5 - PADDLE_WIDTH;
    app->player_right_y         = height / 2 - PADDLE_HEIGHT / 2;
    app->player_right_dy        = PADDLE_SPEED;

    app->ball_x                 = width / 2 - BALL_RADIUS / 2;
    app->ball_y                 = height / 2 - BALL_RADIUS / 2;
    app->ball_dx                = BALL_SPEED;
    app->ball_dy                = BALL_SPEED;

    app->selected_button        = START_BUTTON;
    app->selected_player_count  = 1;

    return app;
}

void pong_run(Pong *app)
{
    bool render             = true;

    float first_time        = 0;
    float last_time         = SDL_GetTicks() / 1000.0f;
    float passed_time       = 0;
    float unprocessed_time  = 0;

    float frame_time        = 0;
    int frames              = 0;

    while (true) {
        first_time  = SDL_GetTicks() / 1000.0f;
        passed_time = first_time - last_time;
        last_time   = first_time;

        unprocessed_time    += passed_time;
        frame_time          += passed_time;

        while (unprocessed_time >= UPDATE_CAP) {
            unprocessed_time -= UPDATE_CAP;

            // Input the app
            pong_input(app);

            switch ((int) app->state)
            {
                case APP_STATE_EXIT:
                    pong_destroy(app);
                    exit(0);
                    break;
                case APP_STATE_INIT:
                case APP_STATE_RUNNING:
                    pong_handle_keys(app);
                    pong_handle_mouse(app);
                    break;
                default:
                    break;
            }

            // Update the app
            pong_update(app, (float) UPDATE_CAP);

            if (frame_time >= 1.0f) {
                frame_time  = 0;
                app->fps    = frames;
                frames      = 0;
            }
        }

        //printf("FPS : %d\n", app->fps);

        if (render) {
            // Render the app
            pong_render(app);
            frames++;
        } else {
            SDL_Delay(1);
        }
    }
}

void pong_destroy(Pong *app)
{
    // Destroy the Window instance
    window_destroy(app->window);

    // Free the app instance
    free(app);
}

void pong_update(Pong *app, float delta_time)
{
    // If the app is running
    if (app->state == APP_STATE_RUNNING) {
        // Check if the game is over
        if (app->score_left == 10 || app->score_right == 10) {
            app->state = APP_STATE_MENU;
        }

        // Update the ball
        pong_update_ball(app, delta_time);

        // If the game is in single player mode
        if (PLAYER_COUNTS[app->selected_player_count] == 1) {
            // Update the AI
            pong_update_ai(app, delta_time);
        }

        // Update the players
        pong_update_player(app, delta_time);
    }
}

void pong_update_ai(Pong *app, float delta_time)
{
    // If the ball is moving towards the AI
    if (app->ball_dx > 0) {
        // If the ball is above the AI
        if (app->ball_y < app->player_right_y + PADDLE_HEIGHT / 2) {
            // Move the AI up
            app->player_right_y -= app->player_right_dy * delta_time;
        }

        // If the ball is below the AI
        if (app->ball_y > app->player_right_y + PADDLE_HEIGHT / 2) {
            // Move the AI down
            app->player_right_y += app->player_right_dy * delta_time;
        }
    }
}

void pong_update_player(Pong *app, float delta_time)
{
    // If the player is moving up
    if (app->keys[SDL_SCANCODE_W]) {
        // Move the player up
        app->player_left_y -= app->player_left_dy * delta_time;
    }

    // If the player is moving down
    if (app->keys[SDL_SCANCODE_S]) {
        // Move the player down
        app->player_left_y += app->player_left_dy * delta_time;
    }

    // If the player is moving up
    if (app->keys[SDL_SCANCODE_UP] && PLAYER_COUNTS[app->selected_player_count] == 2) {
        // Move the player up
        app->player_right_y -= app->player_right_dy * delta_time;
    }

    // If the player is moving down
    if (app->keys[SDL_SCANCODE_DOWN] && PLAYER_COUNTS[app->selected_player_count] == 2) {
        // Move the player down
        app->player_right_y += app->player_right_dy * delta_time;
    }
}

void pong_update_ball(Pong *app, float delta_time)
{
    // Update the ball position
    app->ball_x += app->ball_dx * delta_time;
    app->ball_y += app->ball_dy * delta_time;

    // Check if the ball is colliding with the top or bottom of the screen
    if (app->ball_y >= app->height - BALL_RADIUS || app->ball_y <= BALL_RADIUS) {
        app->ball_dy = -app->ball_dy;
    }

    // Check if the ball is colliding with the left or right of the screen
    if (app->ball_x >= app->width - BALL_RADIUS || app->ball_x <= BALL_RADIUS) {
        // If the ball is colliding with the left of the screen
        if (app->ball_x <= BALL_RADIUS) {
            // Increment the player 2 score
            app->score_right++;
        }

        // If the ball is colliding with the right of the screen
        if (app->ball_x >= app->width - BALL_RADIUS) {
            // Increment the player 1 score
            app->score_left++;
        }

        // Reset the ball position
        app->ball_x = app->width / 2;
        app->ball_y = app->height / 2;
        app->ball_dx = -app->ball_dx;
    }

    // Check if the ball is colliding with the player 1 paddle
    if (
        app->ball_x <= app->player_left_x + PADDLE_WIDTH + (BALL_RADIUS / 2) &&
        app->ball_x >= app->player_left_x + (BALL_RADIUS / 2) &&
        app->ball_y >= app->player_left_y - (BALL_RADIUS / 2) &&
        app->ball_y <= app->player_left_y + PADDLE_HEIGHT + (BALL_RADIUS / 2)
    ) {
        app->ball_dx = -app->ball_dx;
    }

    // Check if the ball is colliding with the player 2 paddle
    if (
        app->ball_x >= app->player_right_x - (BALL_RADIUS / 2) &&
        app->ball_x <= app->player_right_x + PADDLE_WIDTH + (BALL_RADIUS / 2) &&
        app->ball_y >= app->player_right_y - (BALL_RADIUS / 2) &&
        app->ball_y <= app->player_right_y + PADDLE_HEIGHT + (BALL_RADIUS / 2)
    ) {
        app->ball_dx = -app->ball_dx;
    }
}

void pong_render(Pong *app)
{
    // Clear the window
    window_clear(app->window);

    if (app->state == APP_STATE_INIT) {
        // Render the menu
        pong_display_init(app);
    }

    if (app->state == APP_STATE_RUNNING) {
        pong_display_running(app);
    }

    if (app->state == APP_STATE_MENU) {
        // Render the end screen
        pong_display_menu(app);
    }

    // Update the window
    window_update(app->window);
}

void pong_input(Pong *app)
{
    // get the keyboard state
    app->keys = SDL_GetKeyboardState(NULL);

    // get the last key pressed
    float start_time = SDL_GetTicks() / 1000.0f;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Check if the user has pressed a key
        if (event.type == SDL_KEYDOWN && !app->is_key_pressed) {
            app->last_key       = event.key;
            app->is_key_pressed = true;
            //printf("Key pressed: %s\n", SDL_GetKeyName(app->last_key.keysym.sym));
        }

        // Check if the user has released a key
        if (event.type == SDL_KEYUP && start_time - app->key_presss_timer > PRESS_KEY_DELAY) {
            app->is_key_pressed     = false;
            app->key_presss_timer   = start_time;
        }

        // Check if the user has closed the window
        if (event.type == SDL_QUIT) {
            app->state = APP_STATE_EXIT;
        }

        // Check if the user has clicked on the window
        if (event.type == SDL_MOUSEBUTTONDOWN && !app->mouse_clicked) {
            app->mouse_button   = event.button;
            app->mouse_x        = event.button.x;
            app->mouse_y        = event.button.y;
            app->mouse_clicked  = true;
            //printf("Mouse button pressed: %d\n", app->mouse_button.button);
        }

        // Check if the user has released the mouse button
        if (event.type == SDL_MOUSEBUTTONUP && start_time - app->mouse_click_timer > PRESS_MOUSE_DELAY) {
            app->mouse_clicked      = false;
            app->mouse_click_timer  = start_time;
        }
    }
}

void pong_handle_keys(Pong *app)
{
    // Check if the user has pressed a key
    if (!app->is_key_pressed) return;

    // Check if the user has pressed the 'f' key
    if (app->last_key.keysym.sym == SDLK_f) {
        window_toggle_fullscreen(app->window);

        app->width  = app->window->width;
        app->height = app->window->height;

        // Recalculate the paddle positions
        app->player_left_x          = 5;
        app->player_left_y          = app->height / 2 - PADDLE_HEIGHT / 2;

        app->player_right_x         = app->width - 5 - PADDLE_WIDTH;
        app->player_right_y         = app->height / 2 - PADDLE_HEIGHT / 2;

        app->ball_x                 = app->width / 2 - BALL_RADIUS / 2;
        app->ball_y                 = app->height / 2 - BALL_RADIUS / 2;
        app->ball_dx                = BALL_SPEED;
        app->ball_dy                = BALL_SPEED;

        printf("Fullscreen: %s\n", app->window->fullscreen ? "enabled" : "disabled");
    }

    // Check if the user has pressed the 'q' key
    if (app->last_key.keysym.sym == SDLK_q) {
        app->state = APP_STATE_EXIT;
    }

    if (app->last_key.keysym.sym == SDLK_DOWN && app->state == APP_STATE_INIT) {
        app->selected_button = (app->selected_button + 1) % BUTTON_COUNT;
    }

    if (app->last_key.keysym.sym == SDLK_UP && app->state == APP_STATE_INIT) {
        app->selected_button = (app->selected_button + BUTTON_COUNT - 1) % BUTTON_COUNT;
    }

    if (app->last_key.keysym.sym == SDLK_RETURN && app->state == APP_STATE_INIT) {
        if (app->selected_button == START_BUTTON) {
            app->state = APP_STATE_RUNNING;
        }

        if (app->selected_button == EXIT_BUTTON) {
            app->state = APP_STATE_EXIT;
        }
    }

    if (app->last_key.keysym.sym == SDLK_LEFT && app->state == APP_STATE_INIT) {
        if (app->selected_button == TWO_PLAYERS_BUTTON) {
            app->selected_player_count = (app->selected_player_count + 2 - 1) % 2;
        }
    }

    if (app->last_key.keysym.sym == SDLK_RIGHT && app->state == APP_STATE_INIT) {
        if (app->selected_button == TWO_PLAYERS_BUTTON) {
            app->selected_player_count = (app->selected_player_count + 1) % 2;
        }
    }

    app->is_key_pressed = false;
}

void pong_handle_mouse(Pong *app)
{
    // Check if the user has clicked on the window
    if (!app->mouse_clicked) return;

    if (app->mouse_button.button == SDL_BUTTON_LEFT) {
        pong_handle_mouse_left_click(app);
    }

    if (app->mouse_button.button == SDL_BUTTON_RIGHT) {
        pong_handle_mouse_right_click(app);
    }

    app->mouse_clicked = false;
}

void pong_handle_mouse_left_click(Pong *app)
{
    if (app->state == APP_STATE_RUNNING) {
    }

    if (app->state == APP_STATE_MENU) {
        app->state = APP_STATE_INIT;
    }
}

void pong_handle_mouse_right_click(Pong *app)
{
    return;
}

void pong_display_init(Pong *app)
{
    // Display "START" button
    char *text = "START";
    window_draw_text(app->window, app->window->width / 2, app->window->height / 2, text, 30, WHITE_COLOR, ALIGN_CENTER);

    // Display target size setting
    char text2[100];
    sprintf(text2, "TARGET SIZE < %d >", PLAYER_COUNTS[app->selected_player_count]);
    window_draw_text(app->window, app->window->width / 2, app->window->height / 2 + 50, text2, 30, WHITE_COLOR, ALIGN_CENTER);

    // Display "EXIT" button
    char *text3 = "EXIT";
    window_draw_text(app->window, app->window->width / 2, app->window->height / 2 + 100, text3, 30, WHITE_COLOR, ALIGN_CENTER);

    // Display the selected button
    SDL_Rect rect;
    rect.x = app->window->width / 2 - 100;
    rect.y = app->window->height / 2 + 50 * app->selected_button - 20;
    rect.w = 250;
    rect.h = 40;
    window_draw_rect(app->window, &rect, WHITE_COLOR);
}

void pong_display_running(Pong *app)
{
    pong_display_map(app);
    pong_display_ball(app);
    pong_display_paddle(app);
    pong_display_score(app);
}

void pong_display_ball(Pong *app)
{
    SDL_Rect rect;
    rect.x = app->ball_x - BALL_RADIUS;
    rect.y = app->ball_y - BALL_RADIUS;
    rect.w = BALL_RADIUS * 2;
    rect.h = BALL_RADIUS * 2;
    window_draw_filled_rect(app->window, &rect, WHITE_COLOR);
}

void pong_display_paddle(Pong *app)
{
    SDL_Rect rect;
    rect.x = app->player_left_x;
    rect.y = app->player_left_y;
    rect.w = PADDLE_WIDTH;
    rect.h = PADDLE_HEIGHT;
    window_draw_filled_rect(app->window, &rect, WHITE_COLOR);

    rect.x = app->player_right_x;
    rect.y = app->player_right_y;
    rect.w = PADDLE_WIDTH;
    rect.h = PADDLE_HEIGHT;
    window_draw_filled_rect(app->window, &rect, WHITE_COLOR);
}

void pong_display_map(Pong *app)
{
    // Display the middle line
    SDL_Rect rect;
    rect.x = app->window->width / 2 - 2;
    rect.y = 0;
    rect.w = 2;
    rect.h = app->window->height;
    window_draw_filled_rect(app->window, &rect, WHITE_COLOR);

    // Display the score box
    rect.x = app->window->width / 2 - 100;
    rect.y = 0;
    rect.w = 200;
    rect.h = 50;
    window_draw_rect(app->window, &rect, WHITE_COLOR);
}

void pong_display_score(Pong *app)
{
    char text[100];
    sprintf(text, "%d      %d", app->score_left, app->score_right);
    window_draw_text(app->window, app->window->width / 2, 30, text, 45, WHITE_COLOR, ALIGN_CENTER);
}

void pong_display_menu(Pong *app)
{
    char *text = "THE WINNER IS";
    window_draw_text(app->window, app->window->width / 2, app->window->height / 2 - 100, text, 30, WHITE_COLOR, ALIGN_CENTER);

    char text2[100];
    sprintf(text2, "%s", app->score_left > app->score_right ? "LEFT PLAYER" : "RIGHT PLAYER");
    window_draw_text(app->window, app->window->width / 2, app->window->height / 2, text2, 30, WHITE_COLOR, ALIGN_CENTER);
}