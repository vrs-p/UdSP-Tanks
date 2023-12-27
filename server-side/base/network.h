//
// Created by mikulec on 12/27/23.
//

#include <SFML/Network.h>

#ifndef SERVER_SIDE_NETWORK_H
#define SERVER_SIDE_NETWORK_H

typedef struct connection {
    unsigned short port;
    sfIpAddress ipAddress;
} CONNECTION;

typedef enum message_types {
    STATUS,
    KILLED,
    END,
    PLAYER_QUIT
} MESSAGE_TYPES;

#endif //SERVER_SIDE_NETWORK_H
