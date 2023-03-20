#ifndef _BUTTON_H_
#define _BUTTON_H_

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int state;
    int (*callback)(void);
} button;