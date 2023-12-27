//
// Created by mikulec on 12/27/23.
//

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#include "application.h"

void app_create(APPLICATION *app) {
    app->isRunning = true;
    app->sendData = false;
    app->numberOfLeftPlayers = 0;

    app->players = malloc(sizeof(LINKED_LIST));
    ls_create(app->players, sizeof(PLAYER*));

    app->mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(app->mutex, NULL);

    app->sendDataCond = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(app->sendDataCond, NULL);
}

void app_destroy(APPLICATION *app) {
    ls_run_function(app->players, player_destroy_void);
    ls_destroy(app->players);
    app->players = NULL;

    free(app->mutex);
    app->mutex = NULL;

    free(app->sendDataCond);
    app->sendDataCond = NULL;

//    free(app->packetReceive);
    sfPacket_destroy(app->packetReceive);
    app->packetReceive = NULL;

//    free(app->packetSend);
    sfPacket_destroy(app->packetSend);
    app->packetSend = NULL;
}

void* app_send_data(void *app) {
    //TODO: finish this function
}

void* app_receive_data(void *app) {
    //TODO: finish this function
}

void app_run(APPLICATION *app) {
    app_initialize_socket(app);
    app_wait_for_clients(app);
    app_update_position_of_tanks(app);
    app_initialize_game(app);
    if (app->isRunning) {
        pthread_t sendDataThread, receiveDataThread;
        pthread_create(&receiveDataThread, NULL, app_receive_data, app);
        pthread_create(&sendDataThread, NULL, app_send_data, app);

        // wait for the end of the threads
        pthread_join(receiveDataThread, NULL);
        pthread_join(sendDataThread, NULL);
    }
}

void app_initialize_socket(APPLICATION *app) {
    app->packetSend = sfPacket_create();
    app->packetReceive = sfPacket_create();
    app->ipAddress = sfIpAddress_fromString("0.0.0.0");

    app->socket = sfUdpSocket_create();
}

void app_wait_for_clients(APPLICATION *app) {
    printf("Enter number of players (max 4): ");
    int numberOfPlayers, count = 0;
    scanf("%d", &numberOfPlayers);

    while (numberOfPlayers < 1 || numberOfPlayers > 4) {
        printf("Incorrect input. Allowed number of players is in range 1-4\n");
        printf("Enter number of players (max 4): ");
        scanf("%d", &numberOfPlayers);
    }

    while (count < numberOfPlayers) {
        float positionX, positionY;
        unsigned short tmpPort;
        sfIpAddress tmpIp = sfIpAddress_None; // Initialize to None
        DIRECTION tmpDir;
        char pName[25];

        sfPacket_clear(app->packetReceive);
        if (sfUdpSocket_receivePacket(app->socket, app->packetReceive, &tmpIp, &tmpPort) == sfSocketDone) {
            sfPacket_readString(app->packetReceive, pName);
            printf("Client was connected. Name is: %s\n", pName);
        }

        if (count == 0) {
            positionX = SCREEN_WIDTH / 2.0f;
            positionY = SCREEN_HEIGHT;
            tmpDir = UP;
        } else if (count == 1) {
            positionX = SCREEN_WIDTH / 2.0f;
            positionY = 0;
            tmpDir = DOWN;
        } else if (count == 2) {
            positionX = SCREEN_WIDTH;
            positionY = SCREEN_HEIGHT / 2.0f;
            tmpDir = LEFT;
        } else {
            positionX = 0;
            positionY = SCREEN_HEIGHT / 2.0f;
            tmpDir = RIGHT;
        }

        sfPacket_clear(app->packetSend);
        sfPacket_writeFloat(app->packetSend, positionX);
        sfPacket_writeFloat(app->packetSend, positionY);
        sfPacket_writeInt32(app->packetSend, count + 1);
        sfPacket_writeInt32(app->packetSend, (int)tmpDir);
        sfPacket_writeInt32(app->packetSend, numberOfPlayers);

        char tmpIpStr[50];
        sfIpAddress_toString(tmpIp, tmpIpStr);
        printf("Sending packet on IP: %s With port: %u\n", tmpIpStr, tmpPort);

        PLAYER* player = malloc(sizeof(PLAYER));
        player_create(player, count + 1, pName, positionX, positionY, tmpDir, tmpPort, tmpIp);
        ls_push(app->players, &player);
        count++;

        if (sfUdpSocket_sendPacket(app->socket, app->packetSend, tmpIp, tmpPort) != sfSocketDone) {
            printf("Sending failed\n");
        }
    }
}

void app_initialize_game(APPLICATION *app) {
    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, app->players);
    while (ls_iterator_has_next(&iterator)) {
        PLAYER* player = (PLAYER*)ls_iterator_move_next(&iterator);
        sfPacket_clear(app->packetSend);
        LINKED_LIST_ITERATOR iteratorInfo;
        ls_iterator_init(&iteratorInfo, app->players);

        while (ls_iterator_has_next(&iteratorInfo)) {
            PLAYER *playerInfo = (PLAYER *) ls_iterator_move_next(&iteratorInfo);
            if (player_id_get(player) != player_id_get(playerInfo)) {
                sfPacket_writeInt32(app->packetSend, player_id_get(playerInfo));
                sfPacket_writeString(app->packetSend, player_name(playerInfo));
                sfPacket_writeFloat(app->packetSend, player_get_position(playerInfo)->xPosition);
                sfPacket_writeFloat(app->packetSend, player_get_position(playerInfo)->yPosition);
                sfPacket_writeInt32(app->packetSend, (int)player_get_position(playerInfo)->direction);
            }
        }
        if (sfUdpSocket_sendPacket(app->socket, app->packetSend, player_get_connection(player)->ipAddress, player_get_connection(player)->port) != sfSocketDone) {
            printf("Sending of initial info to player: %s\n", player_name(player));
        }
    }
}

void app_update_position_of_tanks(APPLICATION *app) {
    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, app->players);
    while (ls_iterator_has_next(&iterator)) {
        sfPacket_clear(app->packetSend);
        PLAYER* player = (PLAYER*) ls_iterator_move_next(&iterator);
        sfUdpSocket_sendPacket(app->socket, app->packetSend, player_get_connection(player)->ipAddress, player_get_connection(player)->port);
    }

    // Receiving packets from clients
    int count = 0;
    unsigned short port;
    float tmpX = 0, tmpY = 0;
    sfIpAddress ipAddress = sfIpAddress_Any;
    int tmpDir, pId;

    while (count < ls_get_size(app->players)) {
        sfPacket_clear(app->packetReceive);

        if (sfUdpSocket_receivePacket(app->socket, app->packetReceive, &ipAddress, &port) == sfSocketDone) {
            pId = (int)sfPacket_readInt32(app->packetReceive);
            tmpX = (float)sfPacket_readFloat(app->packetReceive);
            tmpY = (float)sfPacket_readFloat(app->packetReceive);
            tmpDir = (int)sfPacket_readInt32(app->packetReceive);
        }

        for (int i = 0; i < ls_get_size(app->players); ++i) {
            PLAYER* player = NULL;
            ls_access_at(app->players, (void**)&player, i);
            if (player_id_get(player) == pId) {
                player_update_position(player, tmpX, tmpY, (DIRECTION)tmpDir);
                player_set_initial_position(player, tmpX, tmpY, (DIRECTION)tmpDir);
            }
        }

        count++;
    }
}

#undef SCREEN_HEIGHT
#undef SCREEN_WIDTH

