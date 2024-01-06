//
// Created by mikulec on 12/31/23.
//

#include <unistd.h>
#include "server_controller.h"

static bool controller_bind_port(SERVER_CONTROLLER* controller, const unsigned short port) {
    sfIpAddress ipAddress = sfIpAddress_fromString("0.0.0.0");
    if (sfUdpSocket_bind(controller->socket, port, ipAddress) != sfSocketDone) {
        fprintf(stderr, "Cannot bind port: %d\n", port);
        return false;
    }
    return true;
}

static bool controller_create_server(SERVER_CONTROLLER* controller, const unsigned short port, int numberOfPlayers, TYPE_OF_MAPS mapType) {
    APPLICATION* app = malloc(sizeof(APPLICATION));
    app_create(app, numberOfPlayers, mapType);
    if (!app_initialize_socket(app, port)) {
        fprintf(stderr, "Cannot bind port: %d\n", port);
        app_destroy(app);
        free(app);
        return false;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, app_create_controller, app);

    pthread_detach(thread);

    controller->numberOfRunningGames++;
    ls_push(controller->servers, &app);
    usleep(1 * 100000);
    return true;
}

static void controller_send_client_response(sfUdpSocket* socket, sfPacket* packet, sfIpAddress address, unsigned short port, SERVER_MESSAGE_TYPE messageType) {
    char ipAddress_str[11];
    sfIpAddress_toString(address, ipAddress_str);
    sfPacket_clear(packet);
    messageType++;
    sfPacket_writeInt32(packet, messageType);

    if (sfUdpSocket_sendPacket(socket, packet, address, port) != sfSocketDone) {
        fprintf(stderr, "Cannot send packet to: %s:%d\n", ipAddress_str, port);
    }
}

static bool controller_verify_if_games_are_running(SERVER_CONTROLLER* controller) {
    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, controller->servers);
    bool gameIsOn = false;
    while (ls_iterator_has_next(&iterator)) {
        APPLICATION* application = *(APPLICATION**) ls_iterator_move_next(&iterator);
        if (application->isRunning) {
            gameIsOn = true;
        }
    }
    return gameIsOn;
}

static void controller_get_statistics(SERVER_CONTROLLER* controller, int* activeGames, int* activePlayers) {
    int games = 0;
    int players = 0;
    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, controller->servers);
    while (ls_iterator_has_next(&iterator)) {
        APPLICATION* application = *(APPLICATION**) ls_iterator_move_next(&iterator);
        if (application->isRunning) {
            games++;
            players += application->numberOfPlayers - application->numberOfLeftPlayers;
        }
    }
    *activeGames = games;
    *activePlayers = players;
}

void controller_create(SERVER_CONTROLLER *controller) {
    controller->numberOfRunningGames = 0;
    controller->socket = sfUdpSocket_create();
    controller->servers = malloc(sizeof(LINKED_LIST));
    ls_create(controller->servers, sizeof(APPLICATION*));

    if (!controller_bind_port(controller, 13877)) {
        return;
    }
}

void controller_destroy(SERVER_CONTROLLER *controller) {
    ls_run_function(controller->servers, app_destroy_void);
    ls_destroy(controller->servers);
    free(controller->servers);
    controller->servers = NULL;

    sfUdpSocket_destroy(controller->socket);
    controller->socket = NULL;
}

void controller_start(SERVER_CONTROLLER *controller) {
    sfPacket* packetReceive = sfPacket_create();
    sfPacket* packetSend = sfPacket_create();
    sfIpAddress ipAddress;
    unsigned short port;
    int controller_message, requested_port, numberOfPlayers, mapType, activeGames, activePlayers;

    bool run = true;
    while (run) {
        sfPacket_clear(packetReceive);

        if (sfUdpSocket_receivePacket(controller->socket, packetReceive, &ipAddress, &port) != sfSocketDone) {
            fprintf(stderr, "Failed to receive packet\n");
        }
        controller_message = sfPacket_readInt32(packetReceive);
        controller_message--;

        if (controller_message == CREATE_SERVER) {
            requested_port = sfPacket_readInt32(packetReceive);
            numberOfPlayers = sfPacket_readInt32(packetReceive);
            mapType = sfPacket_readInt32(packetReceive);
            if (controller_create_server(controller, requested_port, numberOfPlayers, mapType - 1)) {
                controller_send_client_response(controller->socket, packetSend, ipAddress, port, SERVER_CREATED);
            } else {
                controller_send_client_response(controller->socket, packetSend, ipAddress, port, PORT_OCCUPIED);
            }
        } else if (controller_message == STATISTICS) {
            controller_get_statistics(controller, &activeGames, &activePlayers);
            sfPacket_clear(packetSend);
            sfPacket_writeInt32(packetSend, activeGames);
            sfPacket_writeInt32(packetSend, activePlayers);
            sfUdpSocket_sendPacket(controller->socket, packetSend, ipAddress, port);
        } else {
            if (controller_verify_if_games_are_running(controller)) {
                controller_send_client_response(controller->socket, packetSend, ipAddress, port, GAMES_ARE_RUNNING);
            } else {
                controller_send_client_response(controller->socket, packetSend, ipAddress, port, SERVER_IS_OFF);
                run = false;
            }
        }
    }

    sfPacket_destroy(packetReceive);
    sfPacket_destroy(packetSend);
}
