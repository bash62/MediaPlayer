#include <stdio.h>
#include <string.h>
#include <dirent.h>

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

    app->state              = APP_STATE_RUNNING;
    app->current_image      = 0;
    app->images_count       = 0;
    app->auto_play          = false;
    app->auto_play_timer    = 0;

    app->mouse_clicked      = false;
    app->mouse_click_timer  = 0;

    app->keys               = SDL_GetKeyboardState(NULL);
    app->is_key_pressed     = false;

    app->images             = mediaplayer_read_images(app, DEFAULT_IMAGES_FOLDER);
    if (app->images == NULL) {
        fprintf(stderr, "Failed to read images from folder %s\n", DEFAULT_IMAGES_FOLDER);
        return NULL;
    }

    return app;
}

void mediaplayer_run(MediaPlayer *app)
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
            mediaplayer_input(app);

            switch ((int) app->state)
            {
                case APP_STATE_EXIT:
                    mediaplayer_destroy(app);
                    exit(0);
                    break;
                case APP_STATE_RUNNING:
                    mediaplayer_handle_keys(app);
                    mediaplayer_handle_mouse(app);
                    break;
                default:
                    break;
            }

            // Update the app
            mediaplayer_update(app, (float) UPDATE_CAP);

            if (frame_time >= 1.0f) {
                frame_time  = 0;
                app->fps    = frames;
                frames      = 0;
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
    // Destroy the Window instance
    window_destroy(app->window);

    // Free the app instance
    free(app);
}

void mediaplayer_update(MediaPlayer *app, float delta_time)
{
    // Get the current time
    float start_time = SDL_GetTicks() / 1000.0f;

    // Check if the user has enabled the auto play
    if (app->auto_play && start_time - app->auto_play_timer > AUTO_PLAY_DELAY) {
        app->current_image++;
        app->auto_play_timer = start_time;
    }

    // Check if the current image is out of bounds
    // Note: This might be the last check before rendering the image
    if (app->current_image < 0) {
        app->current_image = app->images_count - 1;
    } else if (app->current_image >= app->images_count) {
        app->current_image = 0;
    }
}

void mediaplayer_render(MediaPlayer *app)
{
    // Clear the window
    window_clear(app->window);

    // render the image
    mediaplayer_render_image(app, app->images[app->current_image]);

    // If the auto play is enabled, render the auto play icon
    if (app->auto_play) {
        mediaplayer_display_auto_play(app);
    }

    // Update the window
    window_update(app->window);
}

void mediaplayer_input(MediaPlayer *app)
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

void mediaplayer_render_image(MediaPlayer *app, char *image_path)
{
    // Load the image
    SDL_Surface *image = IMG_Load(image_path);
    if (image == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", image_path);
        exit(1);
    }

    // Create a texture from the image
    SDL_Texture *texture = SDL_CreateTextureFromSurface(app->window->renderer, image);
    if (texture == NULL) {
        fprintf(stderr, "Failed to create texture from image: %s\n", image_path);
        exit(1);
    }

    // Render the texture
    SDL_RenderCopy(app->window->renderer, texture, NULL, NULL);

    // Free the image and texture
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
}

char **mediaplayer_read_images(MediaPlayer *app, char *images_folder)
{
    if (app == NULL) return NULL;

    // Open the folder
    DIR *dir = opendir(images_folder);
    if (dir == NULL) {
        fprintf(stderr, "Failed to open folder: %s\n", images_folder);
        return NULL;
    }

    // Count the number of images in the folder
    int images_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            images_count++;
        }
    }
    app->images_count = images_count;

    // Allocate memory for the images
    char **images = malloc(sizeof(char *) * images_count);
    if (images == NULL) {
        fprintf(stderr, "Failed to allocate memory for images\n");
        return NULL;
    }

    // Read the images
    rewinddir(dir);
    int i = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            images[i] = malloc(sizeof(char) * (strlen(images_folder) + strlen(entry->d_name) + 2));
            if (images[i] == NULL) {
                fprintf(stderr, "Failed to allocate memory for image\n");
                return NULL;
            }

            strcpy(images[i], images_folder);
            strcat(images[i], "/");
            strcat(images[i], entry->d_name);
            i++;
        }
    }

    // Close the folder
    closedir(dir);

    return images;
}

void mediaplayer_handle_keys(MediaPlayer *app)
{
    // Check if the user has pressed a key
    if (!app->is_key_pressed) return;

    // Check if the user has pressed the right key
    if (app->last_key.keysym.sym == SDLK_RIGHT) {
        app->current_image++;
    }

    // Check if the user has pressed the left key
    if (app->last_key.keysym.sym == SDLK_LEFT) {
        app->current_image--;
    }

    // Check if the user has pressed the 'd' key
    if (app->last_key.keysym.sym == SDLK_d) {
        app->auto_play = !app->auto_play;
        printf("Auto play: %s\n", app->auto_play ? "enabled" : "disabled");
    }

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

void mediaplayer_handle_mouse(MediaPlayer *app)
{
    // Check if the user has clicked on the window
    if (!app->mouse_clicked) return;

    if (app->mouse_button.button == SDL_BUTTON_LEFT) {
        mediaplayer_handle_mouse_left_click(app);
    }

    if (app->mouse_button.button == SDL_BUTTON_RIGHT) {
        mediaplayer_handle_mouse_right_click(app);
    }

    app->mouse_clicked = false;
}

void mediaplayer_handle_mouse_left_click(MediaPlayer *app)
{
    // Check if the user has clicked on the right side of the window
    if (app->mouse_x > app->window->width / 2) {
        app->current_image++;
    } else {
        app->current_image--;
    }
}

void mediaplayer_handle_mouse_right_click(MediaPlayer *app)
{
    // Check if the user has clicked on the right side of the window
    if (app->mouse_x > app->window->width / 2) {
        app->auto_play = !app->auto_play;
        printf("Auto play: %s\n", app->auto_play ? "enabled" : "disabled");
    } else {
        window_toggle_fullscreen(app->window);
        printf("Fullscreen: %s\n", app->window->fullscreen ? "enabled" : "disabled");
    }
}


void mediaplayer_display_auto_play(MediaPlayer *app)
{
    char *auto_play         = "Auto play: ";
    char *auto_play_status  = app->auto_play ? "ON" : "OFF";

    char *text              = malloc(sizeof(char) * (strlen(auto_play) + strlen(auto_play_status) + 1));

    strcpy(text, auto_play);
    strcat(text, auto_play_status);

}
