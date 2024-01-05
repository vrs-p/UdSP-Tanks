//
// Created by mikulec on 12/27/23.
//

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#include <unistd.h>
#include "application.h"

void app_create(APPLICATION *app, int numberOfPlayers, TYPE_OF_MAPS mapType) {
    app->isRunning = true;
    app->sendData = false;
    app->numberOfLeftPlayers = 0;
    app->numberOfPlayers = numberOfPlayers;
    app->mapType = mapType;

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
    free(app->players);
    app->players = NULL;

    pthread_mutex_destroy(app->mutex);
    free(app->mutex);
    app->mutex = NULL;

    pthread_cond_destroy(app->sendDataCond);
    free(app->sendDataCond);
    app->sendDataCond = NULL;

    sfPacket_destroy(app->packetReceive);
    app->packetReceive = NULL;

    sfPacket_destroy(app->packetSend);
    app->packetSend = NULL;

    sfUdpSocket_destroy(app->socket);
    app->socket = NULL;
}

void* app_send_data(void *app) {
    APPLICATION* appl = (APPLICATION*)app;

    LINKED_LIST_ITERATOR iterator;
    LINKED_LIST_ITERATOR iteratorInfo;

    ls_iterator_init(&iterator, appl->players);
    ls_iterator_init(&iteratorInfo, appl->players);
    while (appl->isRunning) {
        pthread_mutex_lock(appl->mutex);
        while (!appl->sendData) {
            pthread_cond_wait(appl->sendDataCond, appl->mutex);
        }
        pthread_mutex_unlock(appl->mutex);

        ls_iterator_reset(&iterator);

        while (ls_iterator_has_next(&iterator)) {
            PLAYER* player = *(PLAYER**)ls_iterator_move_next(&iterator);

            sfPacket_clear(appl->packetSend);
            if (!player_killed_get(player) && !player_left_get(player)) {
                sfPacket_writeInt32(appl->packetSend, (int)STATUS + 1);
                ls_iterator_reset(&iteratorInfo);

                while (ls_iterator_has_next(&iteratorInfo)) {
                    PLAYER* playerInfo = *(PLAYER**)ls_iterator_move_next(&iteratorInfo);
                    if (player_id_get(playerInfo) != player_id_get(player)) {
                        player_lock_mutex(playerInfo);

                        sfPacket_writeInt32(appl->packetSend, player_id_get(playerInfo));
                        sfPacket_writeFloat(appl->packetSend, player_get_position(playerInfo)->xPosition);
                        sfPacket_writeFloat(appl->packetSend, player_get_position(playerInfo)->yPosition);
                        sfPacket_writeInt32(appl->packetSend, (int)player_get_position(playerInfo)->direction);
                        sfPacket_writeBool(appl->packetSend, player_fired_get(playerInfo));

                        player_unlock_mutex(playerInfo);
                    }
                }
                if (sfUdpSocket_sendPacket(appl->socket, appl->packetSend, player_get_connection(player)->ipAddress, player_get_connection(player)->port) != sfSocketDone) {
//                    printf("Status update to player: %s failed\n", player_name(player));
                    wprintf(L"Status update to player: %ls failed\\n\"", player_name(player));
                }
            } else if (player_killed_get(player) && !player_left_get(player)) {
                player_lock_mutex(player);
                player_reset_position(player);

                sfPacket_writeInt32(appl->packetSend, (int)KILLED + 1);
                sfPacket_writeInt32(appl->packetSend, player_id_get(player));
                sfPacket_writeFloat(appl->packetSend, player_get_position(player)->xPosition);
                sfPacket_writeFloat(appl->packetSend, player_get_position(player)->yPosition);
                sfPacket_writeInt32(appl->packetSend, (int)player_get_position(player)->direction);
                sfPacket_writeInt32(appl->packetSend, (int)player_killed_by_get(player));

                player_unlock_mutex(player);

                ls_iterator_reset(&iteratorInfo);
                while (ls_iterator_has_next(&iteratorInfo)) {
                    PLAYER* playerKilled = *(PLAYER**)ls_iterator_move_next(&iteratorInfo);
                    if (sfUdpSocket_sendPacket(appl->socket, appl->packetSend, player_get_connection(playerKilled)->ipAddress, player_get_connection(playerKilled)->port) != sfSocketDone) {
//                        printf("Notify dead user to the user: %s failed\n", player_name(playerKilled));
                        wprintf(L"Notify dead user to the user: %ls failed\n", player_name(playerKilled));
                    }
                }
            } else if (player_left_get(player) && !player_sent_score_get(player)) {
//                printf("Player: %s, left the game.\n", player_name(player));
                wprintf(L"Player: %ls, left the game.\n", player_name(player));
                player_lock_mutex(player);

                sfPacket_writeInt32(appl->packetSend, (int)PLAYER_QUIT + 1);
                sfPacket_writeInt32(appl->packetSend, player_id_get(player));

                player_unlock_mutex(player);
                ls_iterator_reset(&iteratorInfo);
                while (ls_iterator_has_next(&iteratorInfo)) {
                    PLAYER* playerInfo = *(PLAYER**)ls_iterator_move_next(&iteratorInfo);
                    if (sfUdpSocket_sendPacket(appl->socket, appl->packetSend, player_get_connection(playerInfo)->ipAddress, player_get_connection(playerInfo)->port) != sfSocketDone) {
//                        printf("Notify user: %s that user left, failed.\n", player_name(playerInfo));
                        wprintf(L"Notify user: %ls that user left, failed.\n", player_name(playerInfo));
                    }
                }

                player_score_sent(player, true);
                sfPacket_clear(appl->packetSend);
                sfPacket_writeInt32(appl->packetSend, (int)END + 1);

                ls_iterator_reset(&iteratorInfo);
                while (ls_iterator_has_next(&iteratorInfo)) {
                    PLAYER* playerInfo = *(PLAYER**)ls_iterator_move_next(&iteratorInfo);
                    sfPacket_writeInt32(appl->packetSend, player_id_get(playerInfo));
                    sfPacket_writeInt32(appl->packetSend, player_score_get(playerInfo));
                }

                sfUdpSocket_sendPacket(appl->socket, appl->packetSend, player_get_connection(player)->ipAddress, player_get_connection(player)->port);
            }
        }
        ls_iterator_reset(&iteratorInfo);
        while (ls_iterator_has_next(&iteratorInfo)) {
            PLAYER* playerInfo = *(PLAYER**)ls_iterator_move_next(&iteratorInfo);
            player_set_fired(playerInfo, false);
        }
        appl->sendData = false;
        usleep(20000);
    }
    return 0;
}

void* app_receive_data(void *app) {
    APPLICATION* appl = (APPLICATION*)app;
    sfIpAddress ipAddress = sfIpAddress_Any;
    unsigned short port;
    float tmpX = 0, tmpY = 0;
    int tmpDir, pId;
    bool pFIred;
    int messageType;
    int killer;

    LINKED_LIST_ITERATOR iterator;
    LINKED_LIST_ITERATOR iteratorInfo;

    ls_iterator_init(&iterator, appl->players);
    ls_iterator_init(&iteratorInfo, appl->players);

    while (appl->isRunning) {
        sfPacket_clear(appl->packetReceive);

        if (sfUdpSocket_receivePacket(appl->socket, appl->packetReceive, &ipAddress, &port) == sfSocketDone) {
            messageType = sfPacket_readInt32(appl->packetReceive);
            messageType--;
        }

        if (messageType == STATUS) {
            pId = (int)sfPacket_readInt32(appl->packetReceive);
            tmpX = (float)sfPacket_readFloat(appl->packetReceive);
            tmpY = (float)sfPacket_readFloat(appl->packetReceive);
            tmpDir = (int)sfPacket_readInt32(appl->packetReceive);
            pFIred = (bool)sfPacket_readBool(appl->packetReceive);
            ls_iterator_reset(&iterator);
            while (ls_iterator_has_next(&iterator)) {
                PLAYER* player = *(PLAYER**) ls_iterator_move_next(&iterator);
                if (player_id_get(player) == pId) {
                    player_lock_mutex(player);
                    player_update_position(player, tmpX, tmpY, tmpDir);
                    player_set_fired(player, pFIred);

                    player_unlock_mutex(player);
                    break;
                }
            }
        } else if (messageType == KILLED) {
            pId = (int)sfPacket_readInt32(appl->packetReceive);
            killer = (int)sfPacket_readInt32(appl->packetReceive);

            ls_iterator_reset(&iterator);
            while (ls_iterator_has_next(&iterator)) {
                PLAYER* player = *(PLAYER**) ls_iterator_move_next(&iterator);
                if (player_id_get(player) == pId) {
                    player_lock_mutex(player);
                    player_killed(player);
                    player_killed_by(player, killer);
                    player_unlock_mutex(player);
                } else if (player_id_get(player) == killer) {
                    player_increase_score(player);
                }
            }
        } else if (messageType == END) {
            pId = (int)sfPacket_readInt32(appl->packetReceive);
            ls_iterator_reset(&iterator);
            while (ls_iterator_has_next(&iterator)) {
                PLAYER* player = *(PLAYER**) ls_iterator_move_next(&iterator);
                if (player_id_get(player) == pId) {
                    player_lock_mutex(player);
                    player_left(player, true);
                    player_unlock_mutex(player);
                    break;
                }
            }
            appl->numberOfLeftPlayers++;
            if (ls_get_size(appl->players) == appl->numberOfLeftPlayers) {
                appl->isRunning = false;
            }
        }
        appl->sendData = true;
        pthread_cond_signal(appl->sendDataCond);

        usleep(20000);
    }
    return 0;
}

void app_run(APPLICATION *app, unsigned short port) {
    app_initialize_socket(app, port);
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

bool app_initialize_socket(APPLICATION *app, unsigned short port) {
    app->packetSend = sfPacket_create();
    app->packetReceive = sfPacket_create();
    app->ipAddress = sfIpAddress_fromString("0.0.0.0");

//    13877
    app->socket = sfUdpSocket_create();
    if (sfUdpSocket_bind(app->socket, port, app->ipAddress) != sfSocketDone) {
        printf("Cannot bind port 13877\n");
        return false;
    }
    return true;
}

void app_wait_for_clients(APPLICATION *app) {
    srand(time(NULL));
    int count = 0;
    int randomSeed = rand();

    while (count < app->numberOfPlayers) {
        float positionX, positionY;
        unsigned short tmpPort;
        sfIpAddress tmpIp = sfIpAddress_None; // Initialize to None
        DIRECTION tmpDir;
        wchar_t pName[25];

        sfPacket_clear(app->packetReceive);
        if (sfUdpSocket_receivePacket(app->socket, app->packetReceive, &tmpIp, &tmpPort) == sfSocketDone) {
            sfPacket_readWideString(app->packetReceive, pName);
            wprintf(L"Client was connected. Name is: %ls\n", pName);
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
        sfPacket_writeInt32(app->packetSend, app->numberOfPlayers);
        sfPacket_writeInt32(app->packetSend, (int)app->mapType + 1);
        if (app->mapType == RANDOM) {
            sfPacket_writeInt32(app->packetSend, randomSeed);
        }

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
        PLAYER* player = *(PLAYER**)ls_iterator_move_next(&iterator);
        sfPacket_clear(app->packetSend);
        LINKED_LIST_ITERATOR iteratorInfo;
        ls_iterator_init(&iteratorInfo, app->players);

        while (ls_iterator_has_next(&iteratorInfo)) {
            PLAYER *playerInfo = *(PLAYER **) ls_iterator_move_next(&iteratorInfo);
            if (player_id_get(player) != player_id_get(playerInfo)) {
                sfPacket_writeInt32(app->packetSend, player_id_get(playerInfo));
                sfPacket_writeWideString(app->packetSend, player_name(playerInfo));
                sfPacket_writeFloat(app->packetSend, player_get_position(playerInfo)->xPosition);
                sfPacket_writeFloat(app->packetSend, player_get_position(playerInfo)->yPosition);
                sfPacket_writeInt32(app->packetSend, (int)player_get_position(playerInfo)->direction);
            }
        }
        if (sfUdpSocket_sendPacket(app->socket, app->packetSend, player_get_connection(player)->ipAddress, player_get_connection(player)->port) != sfSocketDone) {
            wprintf(L"Sending of initial info to player: %ls\n", player_name(player));
        }
    }
}

void app_update_position_of_tanks(APPLICATION *app) {
    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, app->players);
    while (ls_iterator_has_next(&iterator)) {
        sfPacket_clear(app->packetSend);
        PLAYER* player = *(PLAYER**)ls_iterator_move_next(&iterator);
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
            PLAYER* player = *(PLAYER**)ls_access_at_2(app->players, i);
            if (player_id_get(player) == pId) {
                player_update_position(player, tmpX, tmpY, (DIRECTION)tmpDir);
                player_set_initial_position(player, tmpX, tmpY, (DIRECTION)tmpDir);
            }
        }

        count++;
    }
}

void app_destroy_void(void *app) {
    APPLICATION* application = *(APPLICATION**)app;
    app_destroy(application);
    free(application);
}


void app_start(APPLICATION *app) {
    app_wait_for_clients(app);
    app_update_position_of_tanks(app);
    app_initialize_game(app);
    if (app->isRunning) {
        printf("Game is running with: %d players\n", app->numberOfPlayers);
        pthread_t sendDataThread, receiveDataThread;
        pthread_create(&receiveDataThread, NULL, app_receive_data, app);
        pthread_create(&sendDataThread, NULL, app_send_data, app);

        // wait for the end of the threads
        pthread_detach(receiveDataThread);
        pthread_detach(sendDataThread);
    }
}

void* app_create_controller(void* data){
    APPLICATION * app = (APPLICATION *)data;
    app_start(app);
    return 0;
}

#undef SCREEN_HEIGHT
#undef SCREEN_WIDTH

