#include "window.h"
#include "button.h"

Button *button_create(const char *text)
{
    Button *button = malloc(sizeof(*button));
    if (button == NULL) {
        fprintf(stderr, "Failed to allocate memory for button\n");
        return NULL;
    }

    button->x         = 0;
    button->y         = 0;
    button->w         = strlen(text) * 10 + (BUTTON_WIDTH / 2);
    button->h         = BUTTON_HEIGHT;

    button->text      = text;

    return button;
}

void button_destroy(Button *button)
{
    free(button);
}

void button_render(Button *button, Window *window, int x, int y)
{
    button->x = x;
    button->y = y;

    // Render a rectangle for the button
    SDL_Rect rect = { button->x, button->y, button->w, button->h };
    window_draw_rect(window, &rect, WHITE_COLOR);

    // Render the text for the button
    window_draw_text(window, button->x + 15, button->y + 20, button->text, 10, WHITE_COLOR, ALIGN_CENTER);
}

bool button_is_inside(Button *button, int x, int y)
{
    return (x >= button->x && x <= button->x + button->w) && (y >= button->y && y <= button->y + button->h);
}


