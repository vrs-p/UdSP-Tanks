#include <locale.h>
#include "server_controller/server_controller.h"

int main() {
    setlocale(LC_ALL, "sk_SK.UTF-8");

    SERVER_CONTROLLER controller;
    controller_create(&controller);
    controller_start(&controller);

    controller_destroy(&controller);
    return 0;
}
