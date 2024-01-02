//
// Created by vrsp on 28.12.2023.
//

#ifndef UDSP_TANKS_TEXTBOX_H
#define UDSP_TANKS_TEXTBOX_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SFML/Graphics.h>

typedef struct textbox {
    sfText* textBox;
    char* text;
    bool isSelected;
    bool hasLimit;
    int limit;
} TEXTBOX;

void txtbox_create(TEXTBOX* txtbox, int limit, int size, sfColor color, sfFont* font, bool isSelected);
void txtbox_destroy(TEXTBOX* txtbox);
void txtbox_destroy_void(void* txtbox);

void txtbox_set_font(TEXTBOX* txtbox, sfFont* font);
void txtbox_set_position(TEXTBOX* txtbox, sfVector2f position);
void txtbox_set_limit_bool(TEXTBOX* txtbox, bool hasLimit);
void txtbox_set_limit_int(TEXTBOX* txtbox, int limit);
void txtbox_set_selected(TEXTBOX* txtbox, bool isSelected);
void txtbox_set_initial_text(TEXTBOX* txtbox, char* text);
void txtbox_typed(TEXTBOX* txtbox, sfEvent event);
char* txtbox_get_text(TEXTBOX* txtbox);
sfText* txtbox_get_textbox(TEXTBOX* txtbox);
bool txtbox_is_selected(TEXTBOX* txtbox);

#endif //UDSP_TANKS_TEXTBOX_H
