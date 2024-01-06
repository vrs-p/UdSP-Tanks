//
// Created by vrsp on 6.1.2024.
//

#ifndef CLIENT_SIDE_SHOW_ERROR_H
#define CLIENT_SIDE_SHOW_ERROR_H

#include <stdlib.h>
#include <SFML/Graphics.h>
#include "elements/button.h"
#include "../../common/base/server_message_types.h"

typedef struct showError {
    bool showError;

    sfRenderWindow* window;
    sfFont* font;
    sfText* errorText;
    BUTTON* button;
} SHOWERROR;

void error_create(SHOWERROR* shwErr, SERVER_MESSAGE_TYPE* error);
void error_destroy(SHOWERROR* shwErr);

void error_render(SHOWERROR* shwErr);

#endif //CLIENT_SIDE_SHOW_ERROR_H