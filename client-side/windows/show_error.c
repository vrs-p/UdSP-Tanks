//
// Created by vrsp on 6.1.2024.
//

#include "show_error.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 200

void error_create(SHOWERROR* shwErr, SERVER_MESSAGE_TYPE* error) {
    shwErr->font = sfFont_createFromFile("../font/consola.ttf");

    shwErr->errorText = sfText_create();
    sfText_setFont(shwErr->errorText, shwErr->font);
    sfText_setCharacterSize(shwErr->errorText, 24);
    sfText_setFillColor(shwErr->errorText, sfWhite);
    switch (*error) {
        case UNKNOWN:
            sfText_setString(shwErr->errorText, "Unknown error.");
            break;

        case GAMES_ARE_RUNNING:
            sfText_setString(shwErr->errorText, "Server can not be killed, because games are running.");
            break;

        case KILL_SERVER:
            sfText_setString(shwErr->errorText, "Server was successfully killed.");
            break;

        case PORT_OCCUPIED:
            sfText_setString(shwErr->errorText, "Server can not be created, because port is occupied.");
            break;

        case SERVER_CREATED:
            sfText_setString(shwErr->errorText, "Server was successfully created.");
            break;

        case SERVER_IS_OFF:
            sfText_setString(shwErr->errorText, "Server is turned off.");
            break;
    }
    sfVector2f vec = {(SCREEN_WIDTH - sfText_getLocalBounds(shwErr->errorText).width) / 2,
                      (SCREEN_HEIGHT - sfText_getLocalBounds(shwErr->errorText).height) / 2};
    sfText_setPosition(shwErr->errorText, vec);

    shwErr->button = malloc(sizeof(BUTTON));
    vec.x = 100.f;
    vec.y = 50.f;
    btn_create(shwErr->button, vec, sfColor_fromRGB(192, 192, 192), "OK", 24, sfBlack);
    btn_set_font(shwErr->button, shwErr->font);
    vec.x = SCREEN_WIDTH - sfRectangleShape_getLocalBounds(btn_get(shwErr->button)).width - 50;
    vec.y = sfText_getPosition(shwErr->errorText).y + sfText_getLocalBounds(shwErr->errorText).height + 50;
    btn_set_position(shwErr->button, vec);
}

void error_destroy(SHOWERROR* shwError) {
    btn_destroy(shwError->button);
    shwError->button = NULL;

    sfText_destroy(shwError->errorText);
    shwError->errorText = NULL;

    sfFont_destroy(shwError->font);
    shwError->font = NULL;

    sfRenderWindow_destroy(shwError->window);
    shwError->window = NULL;
}

static void error_initialize_window(SHOWERROR* shwErr) {
    sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT};
    shwErr->window = sfRenderWindow_create(mode, "UdSP-Tanks", sfClose, NULL);
    sfRenderWindow_setFramerateLimit(shwErr->window, 60);
    sfRenderWindow_setActive(shwErr->window, sfTrue);
    sfVector2i vec = {0, 0};
    sfRenderWindow_setPosition(shwErr->window, vec);
}

static void error_validate(SHOWERROR* shwError) {
    if (btn_is_mouse_over(shwError->button, shwError->window)) {
        btn_set_bg_color(shwError->button, sfColor_fromRGB(153, 153, 0));
        btn_set_text_color(shwError->button, sfWhite);
    } else {
        btn_set_bg_color(shwError->button, sfColor_fromRGB(255, 255, 0));
        btn_set_text_color(shwError->button, sfBlack);
    }
}

void error_render(SHOWERROR* shwError) {
    error_initialize_window(shwError);

    while (!shwError->showError) {
        sfRenderWindow_clear(shwError->window, sfBlack);

        error_validate(shwError);

        sfRenderWindow_drawText(shwError->window, shwError->errorText, NULL);
        sfRenderWindow_drawRectangleShape(shwError->window, btn_get(shwError->button), NULL);
        sfRenderWindow_drawText(shwError->window, btn_get_text(shwError->button), NULL);

        sfRenderWindow_display(shwError->window);

        sfEvent event;
        while (sfRenderWindow_pollEvent(shwError->window, &event)) {
            if (event.type == sfEvtMouseMoved) {
                error_validate(shwError);
            } else if (event.type == sfEvtMouseButtonPressed) {
                if (btn_is_mouse_over(shwError->button, shwError->window)) {
                    shwError->showError = true;
                }
            } else if (event.type == sfEvtClosed || event.text.unicode == 113) {
                shwError->showError = true;
            }
        }
    }

    sfRenderWindow_setActive(shwError->window, sfFalse);
    sfRenderWindow_close(shwError->window);
}