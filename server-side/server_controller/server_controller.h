//
// Created by mikulec on 12/31/23.
//

#ifndef SERVER_SIDE_SERVER_CONTROLLER_H
#define SERVER_SIDE_SERVER_CONTROLLER_H

#include <SFML/Network.h>
#include "../../common/base/server_message_types.h"
#include "../../common/linked_list/linked_list.h"
#include "../application/application.h"

typedef struct server_controller {
    LINKED_LIST* servers;

    sfUdpSocket* socket;

    int numberOfRunningGames;
} SERVER_CONTROLLER;

void controller_create(SERVER_CONTROLLER* controller);
void controller_destroy(SERVER_CONTROLLER* controller);

void controller_start(SERVER_CONTROLLER* controller);

#endif //SERVER_SIDE_SERVER_CONTROLLER_H
