//
// Created by vrsp on 6.1.2024.
//

#ifndef CLIENT_SIDE_SHOW_STATUS_H
#define CLIENT_SIDE_SHOW_STATUS_H

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
} STATUS_MODAL;

void status_create(STATUS_MODAL* shwErr, SERVER_MESSAGE_TYPE* error);
void status_destroy(STATUS_MODAL* shwErr);

void status_render(STATUS_MODAL* shwErr);

#endif //CLIENT_SIDE_SHOW_STATUS_H