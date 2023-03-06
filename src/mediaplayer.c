#include <stdio.h>

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

            if (frame_time >= 1.0f) {
                frame_time = 0;
                app->fps = frames;
                frames = 0;
            }
        }

        printf("FPS : %d\n", app->fps);


        if (render) {
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