//
// Created by vrsp on 2.1.2024.
//

#ifndef CLIENT_SIDE_MAIN_MENU_H
#define CLIENT_SIDE_MAIN_MENU_H

#include <stdlib.h>
#include <SFML/Graphics.h>
#include "elements/button.h"

typedef struct mmenu {
    sfRenderWindow* window;

    sfFont* font;
    sfText* tittle;

    BUTTON* btnCrtServer;
    BUTTON* btnJoinServer;
    BUTTON* btnKillServer;

    bool create;
    bool join;
    bool kill;
    bool appClosed;
} MMENU;

void mmenu_create(MMENU* mmenu);
void mmenu_destroy(MMENU* mmenu);

void mmenu_render(MMENU* mmenu);
bool mmenu_get_closed(MMENU* mmenu);
bool mmenu_get_create(MMENU* mmenu);
bool mmenu_get_join(MMENU* mmenu);
bool mmenu_get_kill(MMENU* mmenu);
void mmenu_set_closed(MMENU* mmenu, bool state);
void mmenu_set_create(MMENU* mmenu, bool state);
void mmenu_set_join(MMENU* mmenu, bool state);

#endif //CLIENT_SIDE_MAIN_MENU_H