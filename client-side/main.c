#include "application/application.h"
#include "../common/base/server_message_types.h"

int main() {
    APPLICATION app;
    app_create(&app);

    sfIpAddress ipAddress = sfIpAddress_fromString("127.0.0.1");
    unsigned short port = 13877;


    sfUdpSocket* socket = sfUdpSocket_create();
    sfPacket* packet = sfPacket_create();

    sfPacket_clear(packet);

    sfPacket_writeInt32(packet, CREATE_SERVER + 1);
    sfPacket_writeInt32(packet, 13878);
    sfPacket_writeInt32(packet, 1);

    if (sfUdpSocket_sendPacket(socket, packet, ipAddress, port) != sfSocketDone) {
        fprintf(stderr, "Cannot send packet\n");
    }
    printf("Packet sent\n");

    sfPacket_clear(packet);

    if (sfUdpSocket_receivePacket(socket, packet, &ipAddress, &port) != sfSocketDone) {
        fprintf(stderr, "Cannot send packet\n");
    }
    printf("Packet receive\n");

    int response = sfPacket_readInt32(packet);
    response--;

//    if (response == SERVER_CREATED) {
//        printf("jou\n");
//    }
    printf("%d\n", response);




    app_run(&app, ipAddress, 13878, "LogiMou");
    return 0;
}
