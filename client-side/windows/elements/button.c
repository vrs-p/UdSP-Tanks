//
// Created by vrsp on 28.12.2023.
//

#include "button.h"

void btn_create(BUTTON* btn, sfVector2f size, sfColor bgColor, char* text, int characterSize, sfColor textColor) {
    btn->text = sfText_create();
    sfText_setString(btn->text, text);
    sfText_setFillColor(btn->text, textColor);
    sfText_setCharacterSize(btn->text, characterSize);

    btn->button = sfRectangleShape_create();
    sfRectangleShape_setSize(btn->button, size);
    sfRectangleShape_setFillColor(btn->button, bgColor);

    btn->clickable = false;
}

void btn_destroy(BUTTON* btn) {
    sfText_destroy(btn->text);
    btn->text = NULL;

    sfRectangleShape_destroy(btn->button);
    btn->button = NULL;
}

void btn_set_font(BUTTON* btn, sfFont* font) {
    sfText_setFont(btn->text, font);
}

void btn_set_bg_color(BUTTON* btn, sfColor color) {
    sfRectangleShape_setFillColor(btn->button, color);
}

void btn_set_text_color(BUTTON* btn, sfColor color) {
    sfText_setFillColor(btn->text, color);
}

void btn_set_position(BUTTON* btn, sfVector2f pos) {
    sfRectangleShape_setPosition(btn->button, pos);

    float posX = (pos.x + sfRectangleShape_getLocalBounds(btn->button).width / 2) - sfText_getLocalBounds(btn->text).width / 2;
    float posY = (pos.y + sfRectangleShape_getLocalBounds(btn->button).height / 2) - sfText_getLocalBounds(btn->text).height * 2 / 3;

    sfVector2f vec = {posX, posY};
    sfText_setPosition(btn->text, vec);
}

void btn_set_clickable(BUTTON* btn, bool isClickable) {
    btn->clickable = isClickable;
}

sfRectangleShape* btn_get(BUTTON* btn) {
    return btn->button;
}

sfText* btn_get_text(BUTTON* btn) {
    return btn->text;
}

bool btn_is_mouse_over(BUTTON* btn, sfRenderWindow* window) {
    float mouseX = (float)sfMouse_getPositionRenderWindow(window).x;
    float mouseY = (float)sfMouse_getPositionRenderWindow(window).y;

    float btnX = sfRectangleShape_getPosition(btn->button).x;
    float btnY = sfRectangleShape_getPosition(btn->button).y;
    float btnSizeX = sfRectangleShape_getSize(btn->button).x;
    float btnSizeY = sfRectangleShape_getSize(btn->button).y;

    if (mouseX >= btnX && mouseX <= btnX + btnSizeX && mouseY >= btnY && mouseY <= btnY + btnSizeY) {
        return true;
    }

    return false;
}

bool btn_is_clickable(BUTTON* btn) {
    return btn->clickable;
}