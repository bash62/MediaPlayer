#ifndef _MEDIAPLAYER_H_
#define _MEDIAPLAYER_H_

#include <stdbool.h>

#include "window.h"

typedef struct {
    int                 width; 
    int                 height;
    Window              *window;
    bool                is_paused;
    bool                is_key_pressed;
    SDL_KeyboardEvent   last_key;
    const Uint8         *keys;
    float               key_presss_timer;
    int                 fps;
} MediaPlayer;

MediaPlayer *mediaplayer_create(int width, int height);

void mediaplayer_destroy(MediaPlayer *app);

void mediaplayer_run(MediaPlayer *app);

void mediaplayer_update(MediaPlayer *app, float delta_time);

void mediaplayer_render(MediaPlayer *app);

void mediaplayer_input(MediaPlayer *app);

#endif // _MEDIAPLAYER_H_