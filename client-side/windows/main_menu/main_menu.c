//
// Created by vrsp on 2.1.2024.
//

#include "main_menu.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void mmenu_create(MMENU* mmenu) {
    mmenu->create = false;
    mmenu->join = false;
    mmenu->kill = false;
    mmenu->appClosed = false;
    mmenu->serverIsOff = false;
    mmenu->font = sfFont_createFromFile("../font/consola.ttf");

    mmenu->tittle = sfText_create();
    sfText_setFont(mmenu->tittle, mmenu->font);
    sfText_setCharacterSize(mmenu->tittle, 32);
    sfText_setFillColor(mmenu->tittle, sfWhite);
    sfText_setString(mmenu->tittle, "UdSP-Tanks");
    sfVector2f vec = {(SCREEN_WIDTH - sfText_getLocalBounds(mmenu->tittle).width) / 2, 100.f};
    sfText_setPosition(mmenu->tittle, vec);

    mmenu->activeGames = sfText_create();
    sfText_setFont(mmenu->activeGames, mmenu->font);
    sfText_setCharacterSize(mmenu->activeGames, 22);
    sfText_setFillColor(mmenu->activeGames, sfWhite);

    mmenu->activePlayers = sfText_create();
    sfText_setFont(mmenu->activePlayers, mmenu->font);
    sfText_setCharacterSize(mmenu->activePlayers, 22);
    sfText_setFillColor(mmenu->activePlayers, sfWhite);

    mmenu->controllerStatus = sfText_create();
    sfText_setFont(mmenu->controllerStatus, mmenu->font);
    sfText_setCharacterSize(mmenu->controllerStatus, 22);
    sfText_setFillColor(mmenu->controllerStatus, sfRed);

    mmenu->btnCrtServer = malloc(sizeof(BUTTON));
    mmenu->btnJoinServer = malloc(sizeof(BUTTON));
    vec.x = 200.f;
    vec.y = 50.f;
    btn_create(mmenu->btnCrtServer, vec, sfColor_fromRGB(192, 192, 192), "Create Server", 24, sfBlack);
    btn_create(mmenu->btnJoinServer, vec, sfColor_fromRGB(192, 192, 192), "Join Server", 24, sfBlack);
    btn_set_font(mmenu->btnCrtServer, mmenu->font);
    btn_set_font(mmenu->btnJoinServer, mmenu->font);
    vec.x = 150.f;
    vec.y = sfText_getPosition(mmenu->tittle).y + sfText_getLocalBounds(mmenu->tittle).height + 100;
    btn_set_position(mmenu->btnCrtServer, vec);
    vec.x = SCREEN_WIDTH - sfRectangleShape_getSize(btn_get(mmenu->btnJoinServer)).x - 150.f;
    btn_set_position(mmenu->btnJoinServer, vec);

    mmenu->btnKillServer = malloc(sizeof(BUTTON));
    vec.x = 300.f;
    vec.y = 50.f;
    btn_create(mmenu->btnKillServer, vec, sfColor_fromRGB(192, 192, 192), "Kill Server", 24, sfBlack);
    btn_set_font(mmenu->btnKillServer, mmenu->font);
    vec.x = SCREEN_WIDTH / 2 - sfRectangleShape_getLocalBounds(btn_get(mmenu->btnKillServer)).width / 2;
    vec.y = sfRectangleShape_getPosition(btn_get(mmenu->btnJoinServer)).y + sfRectangleShape_getSize(btn_get(mmenu->btnJoinServer)).y + 50;
    btn_set_position(mmenu->btnKillServer, vec);
}

void mmenu_destroy(MMENU* mmenu) {
    btn_destroy(mmenu->btnCrtServer);
    free(mmenu->btnCrtServer);
    mmenu->btnCrtServer = NULL;
    btn_destroy(mmenu->btnJoinServer);
    free(mmenu->btnJoinServer);
    mmenu->btnJoinServer = NULL;
    btn_destroy(mmenu->btnKillServer);
    free(mmenu->btnKillServer);
    mmenu->btnKillServer = NULL;

    sfText_destroy(mmenu->tittle);
    mmenu->tittle = NULL;

    sfText_destroy(mmenu->activeGames);
    mmenu->activeGames = NULL;

    sfText_destroy(mmenu->activePlayers);
    mmenu->activePlayers = NULL;

    sfText_destroy(mmenu->controllerStatus);
    mmenu->controllerStatus = NULL;

    sfFont_destroy(mmenu->font);
    mmenu->font = NULL;

    sfRenderWindow_destroy(mmenu->window);
    mmenu->window = NULL;
}

static void mmenu_initialize_window(MMENU* mmenu) {
    sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT};
    mmenu->window = sfRenderWindow_create(mode, "UdSP-Tanks", sfClose, NULL);
    sfRenderWindow_setFramerateLimit(mmenu->window, 60);
    sfRenderWindow_setActive(mmenu->window, sfTrue);
//    TODO: Fix window position to center of screen
//    sfVideoMode screen = sfVideoMode_getDesktopMode();
//    sfVector2i vec = {(screen.width - SCREEN_WIDTH) / 2, (screen.height - SCREEN_HEIGHT) / 2};
//    sfRenderWindow_setPosition(mmenu->window, vec);
}

static void mmenu_validate(MMENU* mmenu) {
    if (!mmenu->serverIsOff) {
        if (btn_is_mouse_over(mmenu->btnCrtServer, mmenu->window)) {
            btn_set_bg_color(mmenu->btnCrtServer, sfColor_fromRGB(0, 153, 0));
            btn_set_text_color(mmenu->btnCrtServer, sfWhite);
        } else if (btn_is_mouse_over(mmenu->btnJoinServer, mmenu->window)) {
            btn_set_bg_color(mmenu->btnJoinServer, sfColor_fromRGB(0, 153, 0));
            btn_set_text_color(mmenu->btnJoinServer, sfWhite);
        } else if (btn_is_mouse_over(mmenu->btnKillServer, mmenu->window)) {
            btn_set_bg_color(mmenu->btnKillServer, sfColor_fromRGB(153, 0, 0));
            btn_set_text_color(mmenu->btnKillServer, sfWhite);
        } else {
            btn_set_bg_color(mmenu->btnCrtServer, sfColor_fromRGB(0, 255, 0));
            btn_set_text_color(mmenu->btnCrtServer, sfBlack);
            btn_set_bg_color(mmenu->btnJoinServer, sfColor_fromRGB(0, 255, 0));
            btn_set_text_color(mmenu->btnJoinServer, sfBlack);
            btn_set_bg_color(mmenu->btnKillServer, sfColor_fromRGB(255, 0, 0));
            btn_set_text_color(mmenu->btnKillServer, sfBlack);
        }
    } else {
        btn_set_bg_color(mmenu->btnCrtServer, sfColor_fromRGB(192, 192, 192));
        btn_set_text_color(mmenu->btnCrtServer, sfBlack);
        btn_set_bg_color(mmenu->btnJoinServer, sfColor_fromRGB(192, 192, 192));
        btn_set_text_color(mmenu->btnJoinServer, sfBlack);
        btn_set_bg_color(mmenu->btnKillServer, sfColor_fromRGB(192, 192, 192));
        btn_set_text_color(mmenu->btnKillServer, sfBlack);
    }
}

void mmenu_render(MMENU* mmenu) {
    mmenu_initialize_window(mmenu);

    while (!mmenu->create && !mmenu->join && !mmenu->appClosed && !mmenu->kill) {
        sfRenderWindow_clear(mmenu->window, sfBlack);

        mmenu_validate(mmenu);
        sfRenderWindow_drawText(mmenu->window, mmenu->tittle, NULL);
        if (!mmenu->serverIsOff) {
            sfRenderWindow_drawText(mmenu->window, mmenu->activeGames, NULL);
            sfRenderWindow_drawText(mmenu->window, mmenu->activePlayers, NULL);
        } else {
            sfRenderWindow_drawText(mmenu->window, mmenu->controllerStatus, NULL);
        }
        sfRenderWindow_drawRectangleShape(mmenu->window, btn_get(mmenu->btnCrtServer), NULL);
        sfRenderWindow_drawText(mmenu->window, btn_get_text(mmenu->btnCrtServer), NULL);
        sfRenderWindow_drawRectangleShape(mmenu->window, btn_get(mmenu->btnJoinServer), NULL);
        sfRenderWindow_drawText(mmenu->window, btn_get_text(mmenu->btnJoinServer), NULL);
        sfRenderWindow_drawRectangleShape(mmenu->window, btn_get(mmenu->btnKillServer), NULL);
        sfRenderWindow_drawText(mmenu->window, btn_get_text(mmenu->btnKillServer), NULL);

        sfRenderWindow_display(mmenu->window);

        sfEvent event;
        while (sfRenderWindow_pollEvent(mmenu->window, &event)) {
            if (event.type == sfEvtMouseMoved) {
                mmenu_validate(mmenu);
            } else if (event.type == sfEvtMouseButtonPressed) {
                if (btn_is_mouse_over(mmenu->btnCrtServer, mmenu->window) && !mmenu->serverIsOff) {
                    mmenu->create = true;
                } else if (btn_is_mouse_over(mmenu->btnJoinServer, mmenu->window) && !mmenu->serverIsOff) {
                    mmenu->join = true;
                } else if (btn_is_mouse_over(mmenu->btnKillServer, mmenu->window) && !mmenu->serverIsOff) {
                    mmenu->kill = true;
                }
            } else if (event.type == sfEvtClosed || event.text.unicode == 113) {
                mmenu->appClosed = true;
            }
        }
    }

    sfRenderWindow_setActive(mmenu->window, sfFalse);
    sfRenderWindow_close(mmenu->window);
}

bool mmenu_get_closed(MMENU* mmenu) {
    return mmenu->appClosed;
}

bool mmenu_get_create(MMENU* mmenu) {
    return mmenu->create;
}

bool mmenu_get_join(MMENU* mmenu) {
    return mmenu->join;
}

bool mmenu_get_kill(MMENU* mmenu) {
    return mmenu->kill;
}

void mmenu_set_closed(MMENU* mmenu, bool state) {
    mmenu->appClosed = state;
}

void mmenu_set_create(MMENU* mmenu, bool state) {
    mmenu->create = state;
}

void mmenu_set_join(MMENU* mmenu, bool state) {
    mmenu->join = state;
}

void mmenu_update_stats(MMENU *mmenu, int activeGames, int activePlayers) {
    char activeGamesStr[20] = "Active games: ";
    char activePlayersStr[20] = "Active players: ";
    char numberActiveGames[4];
    char numberActivePlayers[4];

    sprintf(numberActiveGames, "%d", activeGames);
    sprintf(numberActivePlayers, "%d", activePlayers);
    strcat(activeGamesStr, numberActiveGames);
    strcat(activePlayersStr, numberActivePlayers);

    sfText_setString(mmenu->activeGames, activeGamesStr);
    sfText_setString(mmenu->activePlayers, activePlayersStr);
    sfText_setString(mmenu->controllerStatus, "Server controller is off!");

    sfVector2f vecActiveGames = {(SCREEN_WIDTH - sfText_getLocalBounds(mmenu->activeGames).width) / 2, 450.f};
    sfText_setPosition(mmenu->activeGames, vecActiveGames);
    sfVector2f vecControllerStatus = {((SCREEN_WIDTH - sfText_getLocalBounds(mmenu->activeGames).width) / 2) - 30, 450.f};
    sfText_setPosition(mmenu->controllerStatus, vecControllerStatus);
    sfVector2f vecActivePlayers = {(SCREEN_WIDTH - sfText_getLocalBounds(mmenu->activePlayers).width) / 2, 500.f};
    sfText_setPosition(mmenu->activePlayers, vecActivePlayers);
}

void mmenu_set_controller_status(MMENU *mmenu, bool isOff) {
    mmenu->serverIsOff = isOff;
}

#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT