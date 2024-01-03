//
// Created by mikulec on 12/31/23.
//

#ifndef CLIENT_SIDE_SERVER_CONTROLLER_H
#define CLIENT_SIDE_SERVER_CONTROLLER_H

#include <stdbool.h>
#include <SFML/Network.h>
#include "../application/application.h"
#include "../../common/base/server_message_types.h"

bool controller_create_server(sfIpAddress serverIp, unsigned short serverPort, unsigned short newPort, int numberOfPlayers, TYPE_OF_MAPS mapType);
bool controller_join_server(sfIpAddress serverIp, unsigned short serverPort, char* playerName);
bool controller_kill_server(sfIpAddress serverIp, unsigned short serverPort);

#endif //CLIENT_SIDE_SERVER_CONTROLLER_H
