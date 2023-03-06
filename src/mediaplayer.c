#include <stdio.h>
#include <string.h>

#include "mediaplayer.h"
#include "window.h"

#define APP_NAME "MediaPlayer"

#define _XOPEN_SOURCE 500

MediaPlayer *mediaplayer_create(int width, int height)
{
    MediaPlayer *app = malloc(sizeof(*app));
    if (app == NULL) {
        fprintf(stderr, "Failed to allocate memory for MediaPlayer\n");
        return NULL;
    }

    app->width  = width;
    app->height = height;

    app->window = window_create(APP_NAME, width, height);
    if (app->window == NULL) return NULL;

    app->keys            = SDL_GetKeyboardState(NULL);
    app->is_key_pressed  = false;

    app->state = APP_STATE_RUNNING;

    return app;
}

void mediaplayer_run(MediaPlayer *app)
{
    if (app == NULL) return NULL;

    bool render = true;

    float first_time = 0;
    float last_time = SDL_GetTicks() / 1000.0f;
    float passed_time = 0;
    float unprocessed_time = 0;

    float frame_time = 0;
    int frames = 0;

    while (true) {
        first_time = SDL_GetTicks() / 1000.0f;
        passed_time = first_time - last_time;
        last_time = first_time;

        unprocessed_time += passed_time;
        frame_time += passed_time;

        while (unprocessed_time >= UPDATE_CAP) {
            unprocessed_time -= UPDATE_CAP;

            // Update the app
            mediaplayer_update(app, (float) UPDATE_CAP);

            // Input the app
            mediaplayer_input(app);

            if (frame_time >= 1.0f) {
                frame_time = 0;
                app->fps = frames;
                frames = 0;
            }
        }

        //printf("FPS : %d\n", app->fps);

        if (render) {
            // Render the app
            mediaplayer_render(app);
            frames++;
        } else {
            SDL_Delay(1);
        }
    }
}

void mediaplayer_destroy(MediaPlayer *app)
{
    if (app == NULL) return NULL;

    // Destroy the Window instance
    window_destroy(app->window);

    // Free the app instance
    free(app);
}

void mediaplayer_update(MediaPlayer *app, float delta_time)
{
    if (app == NULL) return NULL;

    switch ((int) app->state)
    {
        case APP_STATE_EXIT:
            mediaplayer_destroy(app);
            exit(0);
            break;
        default:
            break;
    }
}

void mediaplayer_render(MediaPlayer *app)
{
    if (app == NULL) return NULL;

    // Clear the window
    window_clear(app->window);

    // Update the window
    window_update(app->window);
}

void mediaplayer_input(MediaPlayer *app)
{
    if (app == NULL) return NULL;

    // get the keyboard state
    app->keys = SDL_GetKeyboardState(NULL);

    // get the last key pressed
    float start_time = SDL_GetTicks() / 1000.0f;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Check if the user has pressed a key
        if (event.type == SDL_KEYDOWN && !app->is_key_pressed) {
            app->last_key = event.key;
            app->is_key_pressed = true;
            printf("Key pressed: %s\n", SDL_GetKeyName(app->last_key.keysym.sym));
        }

        // Check if the user has released a key
        if (event.type == SDL_KEYUP && start_time - app->key_presss_timer > PRESS_KEY_DELAY) {
            app->is_key_pressed = false;
            app->key_presss_timer = start_time;
        }

        // Check if the user has closed the window
        if (event.type == SDL_QUIT) {
            app->state = APP_STATE_EXIT;
        }
    }
}