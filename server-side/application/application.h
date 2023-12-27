//
// Created by mikulec on 12/27/23.
//

#ifndef SERVER_SIDE_APPLICATION_H
#define SERVER_SIDE_APPLICATION_H

#include <stdbool.h>
#include "../linked_list/linked_list.h"
#include "../player/player.h"

typedef struct application {
    bool isRunning;
    int numberOfLeftPlayers;

    LINKED_LIST* players;

    bool sendData;
    pthread_mutex_t* mutex;
    pthread_cond_t* sendDataCond;

    sfUdpSocket* socket;
    sfPacket* packetSend;
    sfPacket* packetReceive;
    sfIpAddress ipAddress;
} APPLICATION;

void app_create(APPLICATION* app);
void app_destroy(APPLICATION* app);

void app_run(APPLICATION* app);
void app_initialize_socket(APPLICATION* app);
void app_initialize_game(APPLICATION* app);
void app_update_position_of_tanks(APPLICATION* app);
void app_wait_for_clients(APPLICATION* app);

void* app_send_data(void* app);
void* app_receive_data(void* app);

#endif //SERVER_SIDE_APPLICATION_H
