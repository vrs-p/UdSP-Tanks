//
// Created by vrsp on 28.12.2023.
//

#include "textbox.h"

void txtbox_create(TEXTBOX* txtbox, int limit, int size, sfColor color, sfFont* font, bool isSelected) {
    txtbox->textBox = sfText_create();
    sfText_setFont(txtbox->textBox, font);
    sfText_setCharacterSize(txtbox->textBox, size);
    sfText_setFillColor(txtbox->textBox, color);
    sfText_setUnicodeString(txtbox->textBox, L"");
    txtbox->isSelected = isSelected;

    txtbox->text = malloc((limit + 1) * sizeof(wchar_t));
    wchar_t tmp[] = L"";
    wcscpy(txtbox->text, tmp);
    txtbox->hasLimit = true;
    txtbox->limit = limit;

    if (txtbox->isSelected) {
        sfText_setUnicodeString(txtbox->textBox, L"_");
    } else {
        sfText_setUnicodeString(txtbox->textBox, L"");
    }
}

void txtbox_destroy(TEXTBOX* txtbox) {
    sfText_destroy(txtbox->textBox);
    txtbox->textBox = NULL;

    free(txtbox->text);
    txtbox->text = NULL;
}

void txtbox_destroy_void(void* txtbox) {
    txtbox_destroy(*(TEXTBOX**)txtbox);
    free(*(TEXTBOX**)txtbox);
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
    txtbox->limit = limit;
    txtbox->text = realloc(txtbox->text, (limit + 1) * sizeof(wchar_t));
}

void txtbox_set_selected(TEXTBOX* txtbox, bool isSelected) {
    txtbox->isSelected = isSelected;

    if (isSelected) {
        wchar_t tmpText[txtbox->limit + 2];
        wcscpy(tmpText, txtbox->text);
        wcscat(tmpText, L"_");
        sfText_setUnicodeString(txtbox->textBox, tmpText);
    } else {
        sfText_setUnicodeString(txtbox->textBox, txtbox->text);
    }
}

void txtbox_set_initial_text(TEXTBOX* txtbox, wchar_t* text) {
    wcscat(txtbox->text, text);
    sfText_setUnicodeString(txtbox->textBox, txtbox->text);
}

static void txtbox_delete_last_character(TEXTBOX* txtbox) {
    size_t len = wcslen(txtbox->text);
    wchar_t tmp[len];
    if (len > 0) {
        wcsncpy(tmp, txtbox->text, len - 1);
        tmp[len - 1] = '\0';
    }
    wcscpy(txtbox->text, tmp);
    sfText_setUnicodeString(txtbox->textBox, txtbox->text);
}

static void txtbox_input_logic(TEXTBOX* txtbox, sfEvent event) {
    if (event.text.unicode != 8 && event.text.unicode != 27) {
        wchar_t str[2];
        str[0] = (wchar_t)event.text.unicode;
        str[1] = '\0';
        wcscat(txtbox->text, str);
    } else if (event.text.unicode == 8) {
        if (wcslen(txtbox->text) > 0) {
            txtbox_delete_last_character(txtbox);
        }
    }

    wchar_t tmpText[txtbox->limit + 2];
    wcscpy(tmpText, txtbox->text);
    wcscat(tmpText, L"_");
    sfText_setUnicodeString(txtbox->textBox, tmpText);
}

void txtbox_typed(TEXTBOX* txtbox, sfEvent event) {
    if (txtbox->isSelected) {
        unsigned int charTyped = event.text.unicode;
        if (charTyped == 8 || charTyped == 27 || (charTyped >= 32 && charTyped <= 122) || (charTyped >= 192 && charTyped <= 382)) {
            if (txtbox->hasLimit) {
                if (wcslen(txtbox->text) < txtbox->limit) {
                    txtbox_input_logic(txtbox, event);
                } else if (wcslen(txtbox->text) >= txtbox->limit && charTyped == 8) {
                    txtbox_delete_last_character(txtbox);
                }
            } else {
                txtbox_input_logic(txtbox, event);
            }
        }
    }
}

wchar_t* txtbox_get_text(TEXTBOX* txtbox) {
    return txtbox->text;
}

sfText* txtbox_get_textbox(TEXTBOX* txtbox) {
    return txtbox->textBox;
}

bool txtbox_is_selected(TEXTBOX* txtbox) {
    return txtbox->isSelected;
}