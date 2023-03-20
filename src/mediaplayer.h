#ifndef _MEDIAPLAYER_H_
#define _MEDIAPLAYER_H_

#include <stdbool.h>

#include "window.h"

#define PRESS_KEY_DELAY     0.1f // seconds
#define PRESS_MOUSE_DELAY   0.1f // seconds
#define AUTO_PLAY_DELAY     5.0f // seconds

#define DEFAULT_IMAGES_FOLDER "images"

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

    int                     current_image;
    char                    **images;
    int                     images_count;

    bool                    auto_play;
    int                     auto_play_timer;

    SDL_MouseButtonEvent    mouse_button;
    Sint32                  mouse_x;
    Sint32                  mouse_y;
    bool                    mouse_clicked;
    float                   mouse_click_timer;
} MediaPlayer;

MediaPlayer *mediaplayer_create(int width, int height);

void mediaplayer_destroy(MediaPlayer *app);

void mediaplayer_run(MediaPlayer *app);

void mediaplayer_update(MediaPlayer *app, float delta_time);

void mediaplayer_render(MediaPlayer *app);

void mediaplayer_input(MediaPlayer *app);

void mediaplayer_render_image(MediaPlayer *app, char *image_path);


char **mediaplayer_read_images(MediaPlayer *app, char *images_folder);



void mediaplayer_handle_keys(MediaPlayer *app);

void mediaplayer_handle_mouse(MediaPlayer *app);

void mediaplayer_handle_mouse_left_click(MediaPlayer *app);

void mediaplayer_handle_mouse_right_click(MediaPlayer *app);

void mediaplayer_display_auto_play(MediaPlayer *app);

#endif // _MEDIAPLAYER_H_