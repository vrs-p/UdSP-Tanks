//
// Created by vrsp on 28.12.2023.
//

#include "menu.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void menu_create(MENU* menu) {
    float xPosition = 50.0;
    float xSpace = 100.0;
    menu->gameStarted = false;
    menu->appClosed = false;
    menu->font = sfFont_createFromFile("../font/consola.ttf");

    sfText_setFont(menu->textMainMenu, menu->font);
    sfText_setCharacterSize(menu->textMainMenu, 32);
    sfText_setFillColor(menu->textMainMenu, sfWhite);
    sfText_setString(menu->textMainMenu, "UdSP-Tanks");
    sfVector2f vec = {(SCREEN_WIDTH - sfText_getLocalBounds(menu->textMainMenu).width) / 2, SCREEN_HEIGHT / 2
                      - sfText_getLocalBounds(menu->textMainMenu).height * 15};
    sfText_setPosition(menu->textMainMenu, vec);

    sfText_setFont(menu->name, menu->font);
    sfText_setCharacterSize(menu->name, 32);
    sfText_setFillColor(menu->name, sfWhite);
    sfText_setString(menu->name, "Enter name:");
    vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->name).height * 9};
    sfText_setPosition(menu->name, vec);

    sfText_setFont(menu->ipAddressText, menu->font);
    sfText_setCharacterSize(menu->ipAddressText, 32);
    sfText_setFillColor(menu->ipAddressText, sfWhite);
    sfText_setString(menu->ipAddressText, "Enter IP address:");
    vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->ipAddressText).height * 7};
    sfText_setPosition(menu->ipAddressText, vec);

    sfText_setFont(menu->portText, menu->font);
    sfText_setCharacterSize(menu->portText, 32);
    sfText_setFillColor(menu->portText, sfWhite);
    sfText_setString(menu->portText, "Enter port:");
    vec = (sfVector2f){xPosition, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->portText).height * 4};
    sfText_setPosition(menu->portText, vec);

    menu->textboxes = malloc(sizeof(LINKED_LIST));
    ls_create(menu->textboxes, sizeof(TEXTBOX*));
    for (int i = 0; i < 3; ++i) {
        TEXTBOX* tmptxtbox = malloc(sizeof(TEXTBOX));
        txtBox_create(tmptxtbox, 32, sfWhite, false);
//        sfText_setFont(menu->font); //TODO: fix
        ls_push(menu->textboxes, tmptxtbox);
    }

    LINKED_LIST_ITERATOR* iterator;
    ls_iterator_init(iterator, menu->textboxes);
    TEXTBOX* text = ls_iterator_move_next(iterator);
    vec = (sfVector2f){sfText_getLocalBounds(menu->name).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->name).height * 9};
    txtbox_set_position(text, vec);
    txtbox_set_limit_int(text, 15);
    txtbox_set_selected(text, true);
    text = ls_iterator_move_next(iterator);
    vec = (sfVector2f){sfText_getLocalBounds(menu->ipAddressText).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->ipAddressText).height * 7};
    txtbox_set_position(text, vec);
    txtbox_set_limit_int(text, 15);
    txtbox_set_initial_text(text, "158.193.128.160");
    text = ls_iterator_move_next(iterator);
    vec = (sfVector2f){sfText_getLocalBounds(menu->portText).width + xSpace, SCREEN_HEIGHT / 2 - sfText_getLocalBounds(menu->portText).height * 4};
    txtbox_set_position(text, vec);
    txtbox_set_limit_int(text, 5);
    txtbox_set_initial_text(text, "13877");
    ls_iterator_destroy(iterator);
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
    menu->button = NULL;

//    ls_run_function(menu->textboxes, txtBox_destroy); TODO: fix it
    ls_destroy(menu->textboxes);
    menu->textboxes = NULL;
}

bool menu_validate_ip(char* ip) {
    regex_t regex;
    int reti;
    char msgbuf[100];

    reti = regcomp(&regex, "^(?:25[0-5]|2[0-4]\\d|[0-1]?\\d{1,2})(?:\\.(?:25[0-5]|2[0-4]\\d|[0-1]?\\d{1,2})){3}$", REG_EXTENDED);
    if (reti) {
        return false;
    }

    reti = regexec(&regex, ip, 0, NULL, 0);
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

void menu_initialize_window(MENU* menu) {
    sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT};
    menu->window = sfRenderWindow_create(mode, "UdSP-Tanks", sfClose, NULL);
    sfRenderWindow_setFramerateLimit(menu->window, 60);
    sfRenderWindow_setActive(menu->window, sfTrue);
}

bool menu_validate_port(char* port) {
    regex_t regex;
    int reti;
    char msgbuf[100];

    reti = regcomp(&regex, "[0-9]+", REG_EXTENDED);
    if (reti) {
        return false;
    }

    reti = regexec(&regex, port, 0, NULL, 0);
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
    if (strlen(txtbox_get_text(textbox0)) == 0 && menu_validate_ip(txtbox_get_text(textbox1)) &&
        menu_validate_port(txtbox_get_text(textbox2))) {
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

        LINKED_LIST_ITERATOR iterator;
        ls_iterator_init(&iterator, menu->textboxes);
        while (ls_iterator_has_next(&iterator)) {
            sfText* text = *(sfText**)ls_iterator_move_next(&iterator);
            sfRenderWindow_drawText(menu->window, text, NULL);
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
                                ls_access_at(menu->textboxes, (void**)&textbox, i - 1);
                                txtbox_set_selected(textbox, true);
                                break;
                            }
                        }
                        break;

                    case sfKeyDown:
                        for (int i = 0; i < ls_get_size(menu->textboxes); ++i) {
                            TEXTBOX* textbox = NULL;
                            ls_access_at(menu->textboxes, (void**)&textbox, i);
                            if (txtbox_is_selected(textbox) && i < ls_get_size(menu->textboxes) - 1) {
                                txtbox_set_selected(textbox, false);
                                ls_access_at(menu->textboxes, (void**)&textbox, i + 1);
                                txtbox_set_selected(textbox, true);
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
                    TEXTBOX* textbox = ls_iterator_move_next(&iterator);
                    if (txtbox_is_selected(textbox)) {
                        txtbox_typed(textbox, event);
                    }
                }
            } else if (event.type == sfEvtMouseMoved && menu_validate(menu)) {
                if (btn_is_mouse_over(menu->button, menu->window)) {
                    btn_set_bg_color(menu->button, sfColor_fromRGB(0, 153, 0));
                    btn_set_text_color(menu->button, sfWhite);
                } else {
                    btn_set_bg_color(menu->button, sfColor_fromRGB(0, 255, 0));
                    btn_set_text_color(menu->button, sfBlack);
                }
            } else if (event.type == sfEvtMouseButtonPressed) {
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

sfIpAddress menu_get_ip_address(MENU* menu) {
    TEXTBOX* textbox = NULL;
    ls_access_at(menu->textboxes, (void**)&textbox, 1);
    return sfIpAddress_fromString(txtbox_get_text(textbox));
}

int menu_get_port(MENU* menu) {
    TEXTBOX* textbox = NULL;
    ls_access_at(menu->textboxes, (void**)&textbox, 2);
    return atoi(txtbox_get_text(textbox));
}

char* menu_get_name(MENU* menu) {
    TEXTBOX* textbox = NULL;
    ls_access_at(menu->textboxes, (void**)&textbox, 0);
    return txtbox_get_text(textbox);
}


#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT