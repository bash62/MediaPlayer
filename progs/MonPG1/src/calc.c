#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "calc.h"
#include "window.h"

#define APP_NAME "calc"

#define _XOPEN_SOURCE 500

Calc *calc_create(int width, int height)
{
    Calc *app = malloc(sizeof(*app));
    if (app == NULL) {
        fprintf(stderr, "Failed to allocate memory for calc\n");
        return NULL;
    }

    app->width  = width;
    app->height = height;

    app->window = window_create(APP_NAME, width, height);
    if (app->window == NULL) return NULL;

    app->state              = APP_STATE_RUNNING;
    app->mouse_clicked      = false;
    app->mouse_click_timer  = 0;

    app->keys               = SDL_GetKeyboardState(NULL);
    app->is_key_pressed     = false;

    return app;
}

void calc_run(Calc *app)
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
            calc_input(app);

            switch ((int) app->state)
            {
                case APP_STATE_EXIT:
                    calc_destroy(app);
                    exit(0);
                    break;
                case APP_STATE_RUNNING:
                    calc_handle_keys(app);
                    calc_handle_mouse(app);
                    break;
                default:
                    break;
            }

            // Update the app
            calc_update(app, (float) UPDATE_CAP);

            if (frame_time >= 1.0f) {
                frame_time  = 0;
                app->fps    = frames;
                frames      = 0;
            }
        }

        //printf("FPS : %d\n", app->fps);

        if (render) {
            // Render the app
            calc_render(app);
            frames++;
        } else {
            SDL_Delay(1);
        }
    }
}

void calc_destroy(Calc *app)
{
    // Destroy the Window instance
    window_destroy(app->window);

    // Free the app instance
    free(app);
}

void calc_update(Calc *app, float delta_time)
{
    return;
}

void calc_render(Calc *app)
{
    // Clear the window
    window_clear(app->window);
    // Update the window
    window_update(app->window);
}

void calc_input(Calc *app)
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

void calc_handle_keys(Calc *app)
{
    // Check if the user has pressed a key
    if (!app->is_key_pressed) return;

    // Check if the user has pressed the 'f' key
    if (app->last_key.keysym.sym == SDLK_f) {
        window_toggle_fullscreen(app->window);
        printf("Fullscreen: %s\n", app->window->fullscreen ? "enabled" : "disabled");
    }

    // Check if the user has pressed the 'q' key
    if (app->last_key.keysym.sym == SDLK_q) {
        app->state = APP_STATE_EXIT;
    }

    app->is_key_pressed = false;
}

void calc_handle_mouse(Calc *app)
{
    // Check if the user has clicked on the window
    if (!app->mouse_clicked) return;

    if (app->mouse_button.button == SDL_BUTTON_LEFT) {
        calc_handle_mouse_left_click(app);
    }

    if (app->mouse_button.button == SDL_BUTTON_RIGHT) {
        calc_handle_mouse_right_click(app);
    }

    app->mouse_clicked = false;
}

void calc_handle_mouse_left_click(Calc *app)
{
    return;
}

void calc_handle_mouse_right_click(Calc *app)
{
    return;
}

