#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "human_bench.h"
#include "window.h"

#define APP_NAME "human_bench"

#define _XOPEN_SOURCE 500

HumanBench *human_bench_create(int width, int height)
{
    HumanBench *app = malloc(sizeof(*app));
    if (app == NULL) {
        fprintf(stderr, "Failed to allocate memory for HumanBench\n");
        return NULL;
    }

    app->width  = width;
    app->height = height;

    app->window = window_create(APP_NAME, width, height);
    if (app->window == NULL) return NULL;

    app->state              = APP_STATE_INIT;
    app->mouse_clicked      = false;
    app->mouse_click_timer  = 0;

    app->score              = 0;
    app->errors             = 0;

    app->best_time          = 0.0f;
    app->current_time       = 0.0f;
    app->start_time         = 0.0f;
    app->end_time           = 0.0f;
    app->is_inside_target   = false;
    app->target_color       = (SDL_Color) { 255, 0, 0, 255 };

    app->keys               = SDL_GetKeyboardState(NULL);
    app->is_key_pressed     = false;

    return app;
}

void human_bench_run(HumanBench *app)
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
            human_bench_input(app);

            switch ((int) app->state)
            {
                case APP_STATE_EXIT:
                    human_bench_destroy(app);
                    exit(0);
                    break;
                case APP_STATE_INIT:
                case APP_STATE_RUNNING:
                    human_bench_handle_keys(app);
                    human_bench_handle_mouse(app);
                    break;
                default:
                    break;
            }

            // Update the app
            human_bench_update(app, (float) UPDATE_CAP);

            if (frame_time >= 1.0f) {
                frame_time  = 0;
                app->fps    = frames;
                frames      = 0;
            }
        }

        //printf("FPS : %d\n", app->fps);

        if (render) {
            // Render the app
            human_bench_render(app);
            frames++;
        } else {
            SDL_Delay(1);
        }
    }
}

void human_bench_destroy(HumanBench *app)
{
    // Destroy the Window instance
    window_destroy(app->window);

    // Free the app instance
    free(app);
}

void human_bench_update(HumanBench *app, float delta_time)
{
    if (app->state == APP_STATE_RUNNING) {
    }
}

void human_bench_render(HumanBench *app)
{
    // Clear the window
    window_clear(app->window);

    if (app->state == APP_STATE_INIT) {
        // Render the menu
        human_bench_display_init(app);
    }

    if (app->state == APP_STATE_RUNNING) {
        // Render the target
        human_bench_display_target(app);

        // Render the best time
        human_bench_display_best_time(app);

        // Render the current time
        human_bench_display_current_time(app);

        // Render the score
        human_bench_display_score(app);
    }

    if (app->state == APP_STATE_MENU) {
        // Render the end screen
        human_bench_display_menu(app);
    }

    // Update the window
    window_update(app->window);
}

void human_bench_input(HumanBench *app)
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

void human_bench_handle_keys(HumanBench *app)
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

    if (app->last_key.keysym.sym == SDLK_SPACE && app->state == APP_STATE_INIT) {
        app->state      = APP_STATE_RUNNING;
        app->start_time = SDL_GetTicks() / 1000.0f;

        human_bench_generate_target(app);
    }

    app->is_key_pressed = false;
}

void human_bench_handle_mouse(HumanBench *app)
{
    // Check if the user has clicked on the window
    if (!app->mouse_clicked) return;

    if (app->mouse_button.button == SDL_BUTTON_LEFT) {
        human_bench_handle_mouse_left_click(app);
    }

    if (app->mouse_button.button == SDL_BUTTON_RIGHT) {
        human_bench_handle_mouse_right_click(app);
    }

    app->mouse_clicked = false;
}

void human_bench_handle_mouse_left_click(HumanBench *app)
{
    if (app->state == APP_STATE_RUNNING) {

        app->end_time = SDL_GetTicks() / 1000.0f;
        app->is_inside_target = app->mouse_x >= app->target_x - TARGET_RADIUS && 
            app->mouse_x <= app->target_x + TARGET_RADIUS &&
            app->mouse_y >= app->target_y - TARGET_RADIUS && 
            app->mouse_y <= app->target_y + TARGET_RADIUS;

        human_bench_generate_target(app);

        if (app->is_inside_target) {
            app->current_time = app->end_time - app->start_time;

            if (app->current_time < app->best_time || app->best_time == 0.0f) {
                app->best_time = app->current_time;
            }

            app->start_time         = SDL_GetTicks() / 1000.0f;
            app->is_inside_target   = false;

            app->score++;
        } else {
            app->end_time = SDL_GetTicks() / 1000.0f;

            app->errors++;
        }
    }
}

void human_bench_handle_mouse_right_click(HumanBench *app)
{
    return;
}

void human_bench_generate_target(HumanBench *app)
{
    // Generate a random target position
    app->target_x = rand() % (app->window->width - TARGET_RADIUS) + TARGET_RADIUS;
    app->target_y = rand() % (app->window->height - TARGET_RADIUS) + TARGET_RADIUS;
}

void human_bench_display_init(HumanBench *app)
{
    // Display "Right click to start"
    char *text = "Press SPACE to start";
    window_draw_text(app->window, app->window->width / 2, app->window->height / 2, text, 30, WHITE_COLOR, ALIGN_CENTER);
}

void human_bench_display_target(HumanBench *app)
{
    // Display the target
    window_draw_circle(app->window, app->target_x, app->target_y, TARGET_RADIUS, app->is_inside_target ? GREEN_COLOR : RED_COLOR);
}

void human_bench_display_best_time(HumanBench *app)
{
    // Display the best time
    char text[100];
    sprintf(text, "Best time: %.2f ms", app->best_time * 1000);
    window_draw_text(app->window, 2, 10, text, 30, WHITE_COLOR, ALIGN_LEFT);
}

void human_bench_display_current_time(HumanBench *app)
{
    // Display the current time
    char text[100];
    sprintf(text, "Time: %.2f ms", app->current_time * 1000);
    window_draw_text(app->window, 2, 45, text, 30, WHITE_COLOR, ALIGN_LEFT);
}

void human_bench_display_score(HumanBench *app)
{
    // Display the score
    char text[100];
    sprintf(text, "%d / %d / 100", app->score, app->errors);
    window_draw_text(app->window, 2, 80, text, 30, GREEN_COLOR, ALIGN_LEFT);
}

void human_bench_display_menu(HumanBench *app)
{
    // Display the end message
    char text[100];
    sprintf(text, "Your score: %d / 100", app->score);
    window_draw_text(app->window, app->window->width / 2, app->window->height / 2, text, 30, WHITE_COLOR, ALIGN_CENTER);
}