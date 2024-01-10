//
// Created by vrsp on 28.12.2023.
//

#ifndef UDSP_TANKS_APPLICATION_H
#define UDSP_TANKS_APPLICATION_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <SFML/Graphics.h>
#include <SFML/Network.h>
#include "../tank/tank.h"
#include "../map/map.h"

typedef enum {
    STATUS,
    KILLED,
    END,
    PLAYER_QUIT
} MESSAGE_TYPE;

typedef struct application {
    sfIpAddress ipAddress;
    int port;
    sfUdpSocket* socket;
    sfPacket* packetSend;
    unsigned short id;

    int numberOfPlayers;
    bool isRunning;
    bool sendDataBool;
    bool playerWasKilled;
    unsigned short idOfKilledPlayer;

    pthread_mutex_t* mutex;
    pthread_cond_t* sendDataCond;

    sfRenderWindow* window;
    sfFont* font;
    sfText* nameOfPlayer;

    LINKED_LIST* scores;

    TANK* clientTank;
    LINKED_LIST* otherTanks;
    MAP* map;
} APPLICATION;

void app_create(APPLICATION* app);
void app_destroy(APPLICATION* app);

void app_run(APPLICATION* app, sfIpAddress ipAddress, int port, wchar_t* playerName);

int app_get_player_score(APPLICATION* app);
LINKED_LIST* app_get_other_tanks(APPLICATION* app);

#endif //UDSP_TANKS_APPLICATION_H