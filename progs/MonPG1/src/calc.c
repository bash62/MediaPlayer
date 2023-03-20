#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "calc.h"
#include "window.h"
#include "button.h"

#define APP_NAME "calc"

#define _XOPEN_SOURCE 500

static char *BUTTON_TEXTS[] = {
    "C", "%", "<<", "/",
    "7", "8", "9", "*",
    "4", "5", "6", "-",
    "1", "2", "3", "+",
    "00", "0", ".", "="
};

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

    app->state                  = APP_STATE_RUNNING;
    app->mouse_clicked          = false;
    app->mouse_click_timer      = 0;

    app->keys                   = SDL_GetKeyboardState(NULL);
    app->is_key_pressed         = false;

    app->display_text           = malloc(sizeof(char) * 100);
    strcpy(app->display_text, "0");

    app->result                 = 0;
    app->number_left            = 0;
    app->number_right           = 0;
    app->operation              = '\0';
    app->do_evaluate            = false;
    app->display_result         = false;

    // Create buttons
    for (int i = 0; i < BUTTON_COUNT; i++) {
        app->buttons[i] = button_create(BUTTON_TEXTS[i]);
    }

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

    // Destroy the buttons
    for (int i = 0; i < BUTTON_COUNT; i++) {
        button_destroy(app->buttons[i]);
    }

    // Free the app instance
    free(app);
}

void calc_update(Calc *app, float delta_time)
{
    if (app->do_evaluate) {
        calc_evaluate(app);
    }
}

void calc_render(Calc *app)
{
    // Clear the window
    window_clear(app->window);

    // Render the buttons
    calc_display_buttons(app);

    // Render the display
    calc_display_result_screen(app);

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
    // Check if the user has clicked on a button
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (button_is_inside(app->buttons[i], app->mouse_x, app->mouse_y)) {
            calc_handle_button_click(app, app->buttons[i]);
        }
    }
}

void calc_handle_mouse_right_click(Calc *app)
{
    return;
}

void calc_display_buttons(Calc *app)
{
    /** Display the buttons by following the BUTTON_PATTERNS
     *     { "C",  "%",  "<<", "/" },
     *     { "7",  "8",  "9",  "*" },
     *     { "4",  "5",  "6",  "-" },
     *     { "1",  "2",  "3",  "+" },
     *     { "00", "0",  ".",  "=" }
     */
    int x = 10;
    int y = 200;
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (i % BUTTONS_PER_ROW == 0 && i != 0) {
            x = 10;
            y += BUTTON_HEIGHT + BUTTON_PADDING;
        }
        button_render(app->buttons[i], app->window, x, y);
        x += BUTTON_WIDTH + BUTTON_PADDING;
    }
}

void calc_handle_button_click(Calc *app, Button *button)
{
    // Check if the user has clicked on the 'C' button
    if (strcmp(button->text, "C") == 0) {
        app->result = 0;
        app->display_text = "0";
        return;
    }

    // Check if the user has clicked on the '<<' button
    if (strcmp(button->text, "<<") == 0) {
        if (strlen(app->display_text) > 1) {
            app->display_text[strlen(app->display_text) - 1] = '\0';
        } else {
            app->display_text = "0";
        }
        return;
    }

    // Check if the user has clicked on the '=' button
    if (strcmp(button->text, "=") == 0) {
        app->display_result     = true;
        app->operation_clicked  = false;
        app->do_evaluate        = true;
        return;
    }

    // Check if the user has clicked on the '%' button
    if (strcmp(button->text, "%") == 0) {
        strcat(app->display_text, " %");
        app->operation = '%';
        app->operation_clicked = true;
        return;
    }

    // Check if the user has clicked on the '/' button
    if (strcmp(button->text, "/") == 0) {
        strcat(app->display_text, " /");
        app->operation = '/';
        app->operation_clicked = true;
        return;
    }

    // Check if the user has clicked on the '*' button
    if (strcmp(button->text, "*") == 0) {
        strcat(app->display_text, " *");
        app->operation = '*';
        app->operation_clicked = true;
        return;
    }

    // Check if the user has clicked on the '-' button
    if (strcmp(button->text, "-") == 0) {
        strcat(app->display_text, " -");
        app->operation = '-';
        app->operation_clicked = true;
        return;
    }

    // Check if the user has clicked on the '+' button
    if (strcmp(button->text, "+") == 0) {
        strcat(app->display_text, " +");
        app->operation = '+';
        app->operation_clicked = true;
        return;
    }

    // Check if the user has clicked on the '.' button
    if (strcmp(button->text, ".") == 0) {
        if (strchr(app->display_text, '.') == NULL) {
            app->display_text = strcat(app->display_text, ".");
        }
        return;
    }

    // Check if the user has clicked on the '00' button
    if (strcmp(button->text, "00") == 0) {
        strcat(app->display_text, "00");
        return;
    }

    // Check if the user has clicked on the '0' button
    if (strcmp(button->text, "0") == 0) {
        if (strcmp(app->display_text, "0") != 0) {
            strcat(app->display_text, "0");
        }
        return;
    }

    // Check if the user has clicked on a number button
    if (strcmp(button->text, "1") == 0 ||
        strcmp(button->text, "2") == 0 ||
        strcmp(button->text, "3") == 0 ||
        strcmp(button->text, "4") == 0 ||
        strcmp(button->text, "5") == 0 ||
        strcmp(button->text, "6") == 0 ||
        strcmp(button->text, "7") == 0 ||
        strcmp(button->text, "8") == 0 ||
        strcmp(button->text, "9") == 0) {
        if (strcmp(app->display_text, "0") == 0) {
            strcpy(app->display_text, button->text);
        } else {
            strcat(app->display_text, " ");
            strcat(app->display_text, button->text);
        }
        if (app->operation_clicked) {
            app->number_right       = atof(button->text);
        } else {
            app->number_left        = atof(button->text);
        }
        return;
    }
}

void calc_display_result_screen(Calc *app)
{
    SDL_Rect rect = { 10, 10, 215, 180};
    window_draw_rect(app->window, &rect, WHITE_COLOR);

    char *display_text = malloc(sizeof(char) * 100);
    sprintf(display_text, "%s", app->display_text);
    window_draw_text(app->window, 20, 20, display_text, 15, WHITE_COLOR, ALIGN_LEFT);

    char *result_text = malloc(sizeof(char) * 100);
    sprintf(result_text, "%.2f", app->result);
    window_draw_text(app->window, 215 - strlen(result_text), 175, result_text, 30, WHITE_COLOR, ALIGN_RIGHT);

    free(result_text);
    free(display_text);
}

void calc_evaluate(Calc *app)
{
    switch (app->operation)
    {
    case '+':
        app->result = app->number_left + app->number_right;
        break;
    case '-':
        app->result = app->number_left - app->number_right;
        break;
    case '*':
        app->result = app->number_left * app->number_right;
        break;
    case '/':
        app->result = app->number_left / app->number_right;
        break;
    case '%':
        app->result = app->number_left / 100;
        break;
    default:
        break;
    }

    app->do_evaluate        = false;
    app->operation_clicked  = false;
    app->number_left        = app->result;
    sprintf(app->display_text, "%.2f", app->result);
}