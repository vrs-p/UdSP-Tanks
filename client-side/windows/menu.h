//
// Created by vrsp on 28.12.2023.
//

#ifndef UDSP_TANKS_MENU_H
#define UDSP_TANKS_MENU_H

#include <regex.h>
#include <arpa/inet.h>
#include <SFML/Graphics.h>
#include <SFML/Network.h>
#include "../../common/linked_list/linked_list.h"
#include "elements/button.h"
#include "elements/textbox.h"

typedef struct menu {
    bool gameStarted;
    bool appClosed;
    bool create;

    sfFont* font;
    sfText* textMainMenu;
    sfText* name;
    sfText* ipAddressText;
    sfText* portText;
    sfText* portServer;
    sfText* numOfPlayers;
    sfText* idMap;

    sfRenderWindow* window;
    sfIpAddress ipAddress;
    unsigned short int port;

    LINKED_LIST* textboxes;
    BUTTON* button;
} MENU;

void menu_create(MENU* menu, bool create);
void menu_destroy(MENU* menu);

void menu_render(MENU* menu);

bool menu_get_app_closed(MENU* menu);
bool menu_get_started(MENU* menu);
sfIpAddress menu_get_ip_address(MENU* menu);
int menu_get_port(MENU* menu);
int menu_get_new_port(MENU* menu);
wchar_t* menu_get_name(MENU* menu);
int menu_get_num_players(MENU* menu);
int menu_get_id_map(MENU* menu);

#endif //UDSP_TANKS_MENU_H
