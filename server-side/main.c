#include "application/application.h"
#include "server_controller/server_controller.h"

int main() {
    SERVER_CONTROLLER controller;
    controller_create(&controller);
    controller_start(&controller);

    controller_destroy(&controller);
    return 0;
}
