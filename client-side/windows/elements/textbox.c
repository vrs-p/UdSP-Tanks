//
// Created by vrsp on 28.12.2023.
//

#include "textbox.h"

void txtBox_create(TEXTBOX* txtbox, int size, sfColor color, bool isSelected) {
//    sfText_setCharacterSize(txtbox->textBox, size);
//    sfText_setFillColor(txtbox->textBox, color);
//    txtbox->isSelected = isSelected;
//
//    if (txtbox->isSelected) {
//        sfText_setString(txtbox->textBox, "_");
//    } else {
//        sfText_setString(txtbox->textBox, "");
//    }
//
//    txtbox->textBox.setCharacterSize(size);
//    this->textBox_.setFillColor(color);
//    this->isSelected_ = isSelected;
//
//    if (isSelected) {
//        this->textBox_.setString("_");
//    } else {
//        this->textBox_.setString("");
//    }
}

void txtBox_destroy(TEXTBOX* txtbox) {
    sfText_destroy(txtbox->textBox);
    txtbox->textBox = NULL;

    free(txtbox->text);
    txtbox->text = NULL;
}

void txtbox_set_font(TEXTBOX* txtbox, sfFont* font) {
    sfText_setFont(txtbox->textBox, font);
}

void txtbox_set_position(TEXTBOX* txtbox, sfVector2f position) {
    sfText_setPosition(txtbox->textBox, position);
}

void txtbox_set_limit_bool(TEXTBOX* txtbox, bool hasLimit) {
    txtbox->hasLimit = hasLimit;
}

void txtbox_set_limit_int(TEXTBOX* txtbox, int limit) {
    txtbox->hasLimit = true;
    txtbox->limit = limit - 1;
}

void txtbox_set_selected(TEXTBOX* txtbox, bool isSelected) {
    txtbox->isSelected = isSelected;

    if (!isSelected) {
        char oldText[100];
        strcpy(oldText, txtbox->text);
        char newText[100] = "";

        for (int i = 0; i < strlen(oldText); ++i) {
            newText[i] = oldText[i];
        }

        sfText_setString(txtbox->textBox, newText);
    } else {
        char tmpText[100];
        strcpy(tmpText, txtbox->text);
        strcat(tmpText, "_");
        sfText_setString(txtbox->textBox, tmpText);
    }
}

void txtbox_set_initial_text(TEXTBOX* txtbox, char* text) {
    strcat(txtbox->text, text);
    sfText_setString(txtbox->textBox, txtbox->text);
}

void txtbox_delete_last_character(TEXTBOX* txtbox) {
    char oldText[100];
    strcpy(oldText, txtbox->text);
    char newText[100] = "";

    for (int i = 0; i < strlen(oldText) - 1; ++i) {
        newText[i] = oldText[i];
    }

    strcpy(txtbox->text, newText);
    sfText_setString(txtbox->textBox, txtbox->text);
}

void txtbox_input_logic(TEXTBOX* txtbox, int charTyped) {
//    if (charTyped != sfKeyBackspace && charTyped != sfKeyEnter && charTyped != sfKeyEscape) {
//        strcat(txtbox->text, charTyped);
//    } else if (charTyped == sfKeyBackspace) {
//        if (strlen(txtbox->text) > 0) {
//            txtbox_delete_last_character(txtbox);
//        }
//    }
//    sfText_setString(txtbox->textBox, txtbox->text + "_");
}

void txtbox_typed(TEXTBOX* txtbox, sfEvent event) {
    if (txtbox->isSelected) {
        int charTyped = event.text.unicode;
        if (charTyped < 128) {
            if (txtbox->hasLimit) {
                if (strlen(txtbox->text) <= txtbox->limit) {
                    txtbox_input_logic(txtbox, charTyped);
                } else if (strlen(txtbox->text) > txtbox->limit && charTyped == sfKeyBackspace) {
                    txtbox_delete_last_character(txtbox);
                }
            } else {
                txtbox_input_logic(txtbox, charTyped);
            }
        }
    }
}

char* txtbox_get_text(TEXTBOX* txtbox) {
    return txtbox->text;
}

sfText* txtbox_get_textbox(TEXTBOX* txtbox) {
    return txtbox->textBox;
}

bool txtbox_is_selected(TEXTBOX* txtbox) {
    return txtbox->isSelected;
}

