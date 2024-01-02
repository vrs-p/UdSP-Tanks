#include "server_controller/server_controller.h"

int main() {
//    controller_create_server(sfIpAddress_fromString("127.0.0.1"), 13877, 13878, 1);
    controller_join_server(sfIpAddress_fromString("127.0.0.1"), 13878, "LogiMou");
//    controller_kill_server(sfIpAddress_fromString("127.0.0.1"), 13877);

    return 0;
}
