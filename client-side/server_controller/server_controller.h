//
// Created by mikulec on 12/31/23.
//

#ifndef CLIENT_SIDE_SERVER_CONTROLLER_H
#define CLIENT_SIDE_SERVER_CONTROLLER_H

#include <stdio.h>
#include <stdbool.h>
#include <SFML/Network.h>
#include "../windows/show_score.h"
#include "../application/application.h"
#include "../../common/base/server_message_types.h"

bool controller_create_server(sfIpAddress serverIp, unsigned short serverPort, unsigned short newPort, int numberOfPlayers, TYPE_OF_MAPS mapType, SERVER_MESSAGE_TYPE* error);
bool controller_join_server(sfIpAddress serverIp, unsigned short serverPort, wchar_t* playerName);
bool controller_kill_server(sfIpAddress serverIp, unsigned short serverPort, SERVER_MESSAGE_TYPE* error);
void controller_get_server_statistics(sfIpAddress serverIp, unsigned short serverPort, int* activeGames, int* activePlayers, SERVER_MESSAGE_TYPE* error);

#endif //CLIENT_SIDE_SERVER_CONTROLLER_H