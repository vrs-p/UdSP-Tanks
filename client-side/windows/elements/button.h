//
// Created by vrsp on 28.12.2023.
//

#ifndef UDSP_TANKS_BUTTON_H
#define UDSP_TANKS_BUTTON_H

#include <stdbool.h>
#include <SFML/Graphics.h>

typedef struct button {
    sfText* text;
    sfRectangleShape* button;

    bool clickable;
} BUTTON;

void btn_create(BUTTON* btn, sfVector2f size, sfColor bgColor, char* text, int characterSize, sfColor textColor);
void btn_destroy(BUTTON* btn);

void btn_set_font(BUTTON* btn, sfFont font);
void btn_set_bg_color(BUTTON* btn, sfColor color);
void btn_set_text_color(BUTTON* btn, sfColor color);
void btn_set_position(BUTTON* btn, sfVector2f pos);
void btn_set_clickable(BUTTON* btn, bool isClickable);
sfRectangleShape* btn_get(BUTTON* btn);
sfText* btn_get_text(BUTTON* btn);
bool btn_is_mouse_over(BUTTON* btn, sfRenderWindow window);
bool btn_is_clickable(BUTTON* btn);

#endif //UDSP_TANKS_BUTTON_H
