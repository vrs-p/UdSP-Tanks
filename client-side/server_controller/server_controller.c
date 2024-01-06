//
// Created by mikulec on 12/31/23.
//

#include "server_controller.h"

bool controller_create_server(sfIpAddress serverIp, unsigned short serverPort, unsigned short newPort, int numberOfPlayers, TYPE_OF_MAPS mapType, SERVER_MESSAGE_TYPE* error) {
    unsigned short tmpPort;
    sfUdpSocket* socket = sfUdpSocket_create();
    sfPacket* packet = sfPacket_create();

    sfPacket_clear(packet);

    sfPacket_writeInt32(packet, CREATE_SERVER + 1);
    sfPacket_writeInt32(packet, newPort);
    sfPacket_writeInt32(packet, numberOfPlayers);
    sfPacket_writeInt32(packet, (int)mapType + 1);

    if (sfUdpSocket_sendPacket(socket, packet, serverIp, serverPort) != sfSocketDone) {
        fprintf(stderr, "Cannot send packet\n");
    }
    printf("Request for creation of server was sent.\n");

    sfPacket_clear(packet);
    if (sfUdpSocket_receivePacket(socket, packet, &serverIp, &tmpPort) != sfSocketDone) {
        fprintf(stderr, "Cannot send receive\n");
    }
    printf("Packet receive\n");

    int response = sfPacket_readInt32(packet);
    response--;

    sfUdpSocket_destroy(socket);
    sfPacket_destroy(packet);

    if (response == SERVER_CREATED) {
        printf("Server created.\n");
        *error = SERVER_CREATED;
        return true;
    } else if (response == PORT_OCCUPIED) {
        printf("Selected port is occupied. Failed\n");
        *error = PORT_OCCUPIED;
        return false;
    } else {
        printf("Unknown error.\n");
        *error = UNKNOWN;
        return false;
    }
}

bool controller_join_server(sfIpAddress serverIp, unsigned short serverPort, wchar_t* playerName) {
    APPLICATION app;
    app_create(&app);
    app_run(&app, serverIp, serverPort, playerName);

    SHOWSCORE shwScore;
    score_create(&shwScore, app_get_player_score(&app), app_get_other_tanks(&app));
    score_render(&shwScore);
    score_destroy(&shwScore);
    app_destroy(&app);

    return true;
}

bool controller_kill_server(sfIpAddress serverIp, unsigned short serverPort, SERVER_MESSAGE_TYPE* error) {
    unsigned short tmpPort;
    sfUdpSocket* socket = sfUdpSocket_create();
    sfPacket* packet = sfPacket_create();

    sfPacket_clear(packet);

    sfPacket_writeInt32(packet, KILL_SERVER + 1);

    if (sfUdpSocket_sendPacket(socket, packet, serverIp, serverPort) != sfSocketDone) {
        fprintf(stderr, "Cannot send packet\n");
    }
    printf("Request for creation of server was sent.\n");

    sfPacket_clear(packet);
    if (sfUdpSocket_receivePacket(socket, packet, &serverIp, &tmpPort) != sfSocketDone) {
        fprintf(stderr, "Cannot send receive\n");
    }
    printf("Packet receive\n");

    int response = sfPacket_readInt32(packet);
    response--;

    sfUdpSocket_destroy(socket);
    sfPacket_destroy(packet);

    if (response == SERVER_IS_OFF) {
        printf("Server was successfully killed.\n");
        *error = KILL_SERVER;
        return true;
    } else if (response == GAMES_ARE_RUNNING) {
        printf("Games are still running. Cannot kill server. Failed\n");
        *error = GAMES_ARE_RUNNING;
        return false;
    } else {
        printf("Unknown error.\n");
        *error = UNKNOWN;
        return false;
    }
}

void controller_get_server_statistics(sfIpAddress serverIp, unsigned short serverPort, int *activeGames,
                                      int *activePlayers, SERVER_MESSAGE_TYPE* error) {
    unsigned short tmpPort;
    sfUdpSocket* socket = sfUdpSocket_create();
    sfPacket* packet = sfPacket_create();

    sfPacket_clear(packet);

    sfPacket_writeInt32(packet, STATISTICS + 1);

    if (sfUdpSocket_sendPacket(socket, packet, serverIp, serverPort) != sfSocketDone) {
        fprintf(stderr, "Cannot send packet\n");
    }
    printf("Request for server statistics was sent.\n");

    sfPacket_clear(packet);
    if (sfUdpSocket_receivePacket(socket, packet, &serverIp, &tmpPort) != sfSocketDone) {
        fprintf(stderr, "Cannot send receive\n");
        *error = SERVER_IS_OFF; //TODO: fix this!
    }
    printf("Packet received\n");

    *activeGames = sfPacket_readInt32(packet);
    *activePlayers = sfPacket_readInt32(packet);

    sfUdpSocket_destroy(socket);
    sfPacket_destroy(packet);

    *error = STATISTICS;
}
