#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdbool.h>

#include "window.h"

#define BUTTON_COUNT        20

#define BUTTON_WIDTH        50
#define BUTTON_HEIGHT       50

#define BUTTON_PADDING      10
#define BUTTONS_PER_ROW     4

typedef struct {
    int             x;
    int             y;
    int             w;
    int             h;
    const char            *text;
    int             state;
} Button;

Button *button_create(const char *text);

void button_destroy(Button *b);

void button_render(Button *b, Window *win, int x, int y);

bool button_is_inside(Button *b, int x, int y);

#endif // _BUTTON_H_