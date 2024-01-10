//
// Created by vrsp on 28.12.2023.
//

#include "menu.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void menu_create(MENU* menu, bool create) {
    float xPosition = 50.0;
    float xSpace = 100.0;
    menu->gameStarted = false;
    menu->appClosed = false;
    menu->create = create;
    menu->font = sfFont_createFromFile("../font/consola.ttf");

    menu->textMainMenu = sfText_create();
    sfText_setFont(menu->textMainMenu, menu->font);
    sfText_setCharacterSize(menu->textMainMenu, 32);
    sfText_setFillColor(menu->textMainMenu, sfWhite);
    sfText_setString(menu->textMainMenu, "UdSP-Tanks");
    sfVector2f vec = {(SCREEN_WIDTH - sfText_getLocalBounds(menu->textMainMenu).width) / 2, SCREEN_HEIGHT / 2
                                                                                            - sfText_getLocalBounds(menu->textMainMenu).height * 15};
    sfText_setPosition(menu->textMainMenu, vec);

    menu->name = sfText_create();
    sfText_setFont(menu->name, menu->font);
    sfText_setCharacterSize(menu->name, 32);
    sfText_setFillColor(menu->name, sfWhite);
    sfText_setString(menu->name, "Enter name:");
    vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->name).height * 9};
    sfText_setPosition(menu->name, vec);

    menu->ipAddressText = sfText_create();
    sfText_setFont(menu->ipAddressText, menu->font);
    sfText_setCharacterSize(menu->ipAddressText, 32);
    sfText_setFillColor(menu->ipAddressText, sfWhite);
    sfText_setString(menu->ipAddressText, "Enter IP address:");
    vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->ipAddressText).height * 7};
    sfText_setPosition(menu->ipAddressText, vec);

    menu->portText = sfText_create();
    sfText_setFont(menu->portText, menu->font);
    sfText_setCharacterSize(menu->portText, 32);
    sfText_setFillColor(menu->portText, sfWhite);
    sfText_setString(menu->portText, "Enter server port:");
    vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->portText).height * 4};
    sfText_setPosition(menu->portText, vec);

    menu->portServer = sfText_create();
    menu->numOfPlayers = sfText_create();
    menu->idMap = sfText_create();
    if (create) {
        sfText_setFont(menu->portServer, menu->font);
        sfText_setCharacterSize(menu->portServer, 32);
        sfText_setFillColor(menu->portServer, sfWhite);
        sfText_setString(menu->portServer, "Enter new port:");
        vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->portServer).height * 2.5};
        sfText_setPosition(menu->portServer, vec);

        sfText_setFont(menu->numOfPlayers, menu->font);
        sfText_setCharacterSize(menu->numOfPlayers, 32);
        sfText_setFillColor(menu->numOfPlayers, sfWhite);
        sfText_setString(menu->numOfPlayers, "Enter number of players:");
        vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->numOfPlayers).height};
        sfText_setPosition(menu->numOfPlayers, vec);

        sfText_setFont(menu->idMap, menu->font);
        sfText_setCharacterSize(menu->idMap, 32);
        sfText_setFillColor(menu->idMap, sfWhite);
        sfText_setString(menu->idMap, "Choose map (1-4):");
        vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 + sfText_getLocalBounds(menu->idMap).height * 0.5};
        sfText_setPosition(menu->idMap, vec);

        menu->textboxes = malloc(sizeof(LINKED_LIST));
        ls_create(menu->textboxes, sizeof(TEXTBOX*));
        for (int i = 0; i < 6; ++i) {
            TEXTBOX* tmptxtbox = malloc(sizeof(TEXTBOX));
            txtbox_create(tmptxtbox, 15, 32, sfWhite, menu->font, false);
            ls_push(menu->textboxes, &tmptxtbox);
        }

        TEXTBOX* text = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 3);
        vec = (sfVector2f){sfText_getLocalBounds(menu->portServer).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->portServer).height * 2.5};
        txtbox_set_position(text, vec);
        txtbox_set_limit_int(text, 5);
        txtbox_set_initial_text(text, L"13878");

        text = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 4);
        vec = (sfVector2f){sfText_getLocalBounds(menu->numOfPlayers).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->numOfPlayers).height};
        txtbox_set_position(text, vec);
        txtbox_set_limit_int(text, 1);
        txtbox_set_initial_text(text, L"1");

        text = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 5);
        vec = (sfVector2f){sfText_getLocalBounds(menu->idMap).width + xSpace, SCREEN_HEIGHT / 2 + sfText_getLocalBounds(menu->idMap).height * 0.5};
        txtbox_set_position(text, vec);
        txtbox_set_limit_int(text, 1);
        txtbox_set_initial_text(text, L"1");
    } else {
        menu->textboxes = malloc(sizeof(LINKED_LIST));
        ls_create(menu->textboxes, sizeof(TEXTBOX*));
        for (int i = 0; i < 3; ++i) {
            TEXTBOX* tmptxtbox = malloc(sizeof(TEXTBOX));
            txtbox_create(tmptxtbox, 15, 32, sfWhite, menu->font, false);
            ls_push(menu->textboxes, &tmptxtbox);
        }
    }

    TEXTBOX* text = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 0);
    vec = (sfVector2f){sfText_getLocalBounds(menu->name).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->name).height * 9};
    txtbox_set_position(text, vec);
    txtbox_set_initial_text(text, L"LogičMou");
    txtbox_set_selected(text, true);

    text = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 1);
    vec = (sfVector2f){sfText_getLocalBounds(menu->ipAddressText).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->ipAddressText).height * 7};
    txtbox_set_position(text, vec);
    txtbox_set_initial_text(text, L"127.0.0.1");

    text = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 2);
    vec = (sfVector2f){sfText_getLocalBounds(menu->portText).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->portText).height * 4};
    txtbox_set_position(text, vec);
    txtbox_set_limit_int(text, 5);
    txtbox_set_initial_text(text, L"13877");

    menu->button = malloc(sizeof(BUTTON));
    vec = (sfVector2f){200, 50};
    char btnText[7];
    if (create) {
        strcpy(btnText, "Create");
    } else {
        strcpy(btnText, "Join");
    }
    btn_create(menu->button, vec, sfColor_fromRGB(192, 192, 192), btnText, 24, sfBlack);
    btn_set_font(menu->button, menu->font);
    if (create) {
        vec = (sfVector2f){SCREEN_WIDTH - sfRectangleShape_getSize(btn_get(menu->button)).x - 100,
                           sfText_getPosition(menu->idMap).y + sfText_getLocalBounds(menu->idMap).height + 50};
    } else {
        vec = (sfVector2f){SCREEN_WIDTH - sfRectangleShape_getSize(btn_get(menu->button)).x - 100,
                           sfText_getPosition(menu->portText).y + sfText_getLocalBounds(menu->portText).height + 50};
    }
    btn_set_position(menu->button, vec);
}

void menu_destroy(MENU* menu) {
    sfFont_destroy(menu->font);
    menu->font = NULL;

    sfText_destroy(menu->textMainMenu);
    menu->textMainMenu = NULL;
    sfText_destroy(menu->name);
    menu->name = NULL;
    sfText_destroy(menu->ipAddressText);
    menu->ipAddressText = NULL;
    sfText_destroy(menu->portText);
    menu->portText = NULL;

    sfRenderWindow_destroy(menu->window);
    menu->window = NULL;

    btn_destroy(menu->button);
    free(menu->button);
    menu->button = NULL;

    ls_run_function(menu->textboxes, txtbox_destroy_void);
    ls_destroy(menu->textboxes);
    menu->textboxes = NULL;
}

void menu_initialize_window(MENU* menu) {
    sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT};
    menu->window = sfRenderWindow_create(mode, "UdSP-Tanks", sfClose, NULL);
    sfRenderWindow_setFramerateLimit(menu->window, 60);
    sfRenderWindow_setActive(menu->window, sfTrue);
}

#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT

bool menu_validate_ip(wchar_t* ip) {
    char str[16];
    size_t len = wcstombs(str, ip, sizeof(str));
    if (len == (size_t)-1) {
        return false;
    }

    struct sockaddr_in sa;
    return inet_pton(AF_INET, str, &(sa.sin_addr)) != 0;
}

bool menu_validate_port(wchar_t* port) {
    char str[6];
    size_t len = wcstombs(str, port, sizeof(str));
    if (len == (size_t)-1) {
        return false;
    }

    regex_t regex;
    int reti;

    reti = regcomp(&regex, "^(0|[1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$", REG_EXTENDED);
    if (reti) {
        return false;
    }

    reti = regexec(&regex, str, 0, NULL, 0);
    if (!reti) {
        regfree(&regex);
        return true;
    } else {
        regfree(&regex);
        return false;
    }
}

bool menu_validate_num(wchar_t* num) {
    char str[2];
    size_t len = wcstombs(str, num, sizeof(str));
    if (len == (size_t)-1) {
        return false;
    }

    regex_t regex;
    int reti;

    reti = regcomp(&regex, "^[1-4]$", REG_EXTENDED);
    if (reti) {
        return false;
    }

    reti = regexec(&regex, str, 0, NULL, 0);
    if (!reti) {
        regfree(&regex);
        return true;
    } else if (reti == REG_NOMATCH) {
        regfree(&regex);
        return false;
    } else {
        regfree(&regex);
        return false;
    }
}

bool menu_validate(MENU* menu) {
    TEXTBOX* textbox0 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 0);
    TEXTBOX* textbox1 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 1);
    TEXTBOX* textbox2 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 2);
    TEXTBOX* textbox3;
    TEXTBOX* textbox4;
    TEXTBOX* textbox5;
    if (menu->create) {
        textbox3 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 3);
        textbox4 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 4);
        textbox5 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 5);
    }
    if (!menu->create && wcslen(txtbox_get_text(textbox0)) != 0 && menu_validate_ip(txtbox_get_text(textbox1)) &&
        menu_validate_port(txtbox_get_text(textbox2))) {
        if (btn_is_mouse_over(menu->button, menu->window)) {
            btn_set_bg_color(menu->button, sfColor_fromRGB(0, 153, 0));
            btn_set_text_color(menu->button, sfWhite);
        } else {
            btn_set_bg_color(menu->button, sfColor_fromRGB(0, 255, 0));
            btn_set_text_color(menu->button, sfBlack);
        }
        return true;
    } else if (menu->create && wcslen(txtbox_get_text(textbox0)) != 0 && menu_validate_ip(txtbox_get_text(textbox1)) &&
        menu_validate_port(txtbox_get_text(textbox2)) && menu_validate_port(txtbox_get_text(textbox3)) &&
        menu_validate_num(txtbox_get_text(textbox4)) && menu_validate_num(txtbox_get_text(textbox5))) {
        if (btn_is_mouse_over(menu->button, menu->window)) {
            btn_set_bg_color(menu->button, sfColor_fromRGB(0, 153, 0));
            btn_set_text_color(menu->button, sfWhite);
        } else {
            btn_set_bg_color(menu->button, sfColor_fromRGB(0, 255, 0));
            btn_set_text_color(menu->button, sfBlack);
        }
        return true;
    }
    btn_set_bg_color(menu->button, sfColor_fromRGB(192, 192, 192));
    btn_set_text_color(menu->button, sfBlack);
    return false;
}

void menu_render(MENU* menu) {
    menu_initialize_window(menu);

    while (!menu->gameStarted && !menu->appClosed) {
        sfRenderWindow_clear(menu->window, sfBlack);

        sfRenderWindow_drawText(menu->window, menu->textMainMenu, NULL);
        sfRenderWindow_drawText(menu->window, menu->name, NULL);
        sfRenderWindow_drawText(menu->window, menu->ipAddressText, NULL);
        sfRenderWindow_drawText(menu->window, menu->portText, NULL);
        if (menu->create) {
            sfRenderWindow_drawText(menu->window, menu->portServer, NULL);
            sfRenderWindow_drawText(menu->window, menu->numOfPlayers, NULL);
            sfRenderWindow_drawText(menu->window, menu->idMap, NULL);
        }

        LINKED_LIST_ITERATOR iterator;
        ls_iterator_init(&iterator, menu->textboxes);
        while (ls_iterator_has_next(&iterator)) {
            TEXTBOX* text = *(TEXTBOX**)ls_iterator_move_next(&iterator);
            sfRenderWindow_drawText(menu->window, txtbox_get_textbox(text), NULL);
        }

        menu_validate(menu);
        sfRenderWindow_drawRectangleShape(menu->window, btn_get(menu->button), NULL);
        sfRenderWindow_drawText(menu->window, btn_get_text(menu->button), NULL);

        sfRenderWindow_display(menu->window);

        sfEvent event;
        while (sfRenderWindow_pollEvent(menu->window, &event)) {
            if (event.type == sfEvtKeyPressed) {
                switch (event.key.code) {
                    case sfKeyUp:
                        for (int i = 0; i < ls_get_size(menu->textboxes); ++i) {
                            TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, i);
                            if (txtbox_is_selected(textbox) && i > 0) {
                                txtbox_set_selected(textbox, false);
                                TEXTBOX* textbox2 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, i - 1);
                                txtbox_set_selected(textbox2, true);
                                break;
                            }
                        }
                        break;

                    case sfKeyDown:
                        for (int i = 0; i < ls_get_size(menu->textboxes); ++i) {
                            TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, i);
                            if (txtbox_is_selected(textbox) && i < ls_get_size(menu->textboxes) - 1) {
                                txtbox_set_selected(textbox, false);
                                TEXTBOX* textbox2 = *(TEXTBOX**)ls_access_at_2(menu->textboxes, i + 1);
                                txtbox_set_selected(textbox2, true);
                                break;
                            }
                        }
                        break;

                    case sfKeyEscape:
                        menu->appClosed = true;
                        break;
                }
            } else if (event.type == sfEvtTextEntered) {
                ls_iterator_reset(&iterator);
                while (ls_iterator_has_next(&iterator)) {
                    TEXTBOX* textbox = *(TEXTBOX**)ls_iterator_move_next(&iterator);
                    if (txtbox_is_selected(textbox)) {
                        txtbox_typed(textbox, event);
                    }
                }
            } else if (event.type == sfEvtMouseButtonPressed && menu_validate(menu)) {
                if (btn_is_mouse_over(menu->button, menu->window) && menu_validate(menu)) {
                    menu->gameStarted = true;
                }
            } else if (event.type == sfEvtClosed) {
                menu->appClosed = true;
            }
        }
    }
    sfRenderWindow_setActive(menu->window, sfFalse);
    sfRenderWindow_close(menu->window);
}

bool menu_get_app_closed(MENU* menu) {
    return menu->appClosed;
}

bool menu_get_started(MENU* menu) {
    return menu->gameStarted;
}

sfIpAddress menu_get_ip_address(MENU* menu) {
    TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 1);
    char str[16];
    size_t len = wcstombs(str, txtbox_get_text(textbox), sizeof(str));
    if (len == (size_t)-1) {
        return sfIpAddress_None;
    }
    return sfIpAddress_fromString(str);
}

int menu_get_port(MENU* menu) {
    TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 2);
    wchar_t *endptr;
    wchar_t *text = txtbox_get_text(textbox);
    long int num = wcstol(text, &endptr, 10);
    return (int)num;
}

int menu_get_new_port(MENU* menu) {
    TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 3);
    wchar_t *endptr;
    wchar_t *text = txtbox_get_text(textbox);
    long int num = wcstol(text, &endptr, 10);
    return (int)num;
}

wchar_t* menu_get_name(MENU* menu) {
    TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 0);
    return txtbox_get_text(textbox);
}

int menu_get_num_players(MENU* menu) {
    TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 4);
    wchar_t *endptr;
    wchar_t *text = txtbox_get_text(textbox);
    long int num = wcstol(text, &endptr, 10);
    return (int)num;
}

int menu_get_id_map(MENU* menu) {
    TEXTBOX* textbox = *(TEXTBOX**)ls_access_at_2(menu->textboxes, 5);
    wchar_t *endptr;
    wchar_t *text = txtbox_get_text(textbox);
    long int num = wcstol(text, &endptr, 10);
    return (int)num;
}