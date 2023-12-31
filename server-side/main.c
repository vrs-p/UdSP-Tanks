#include "application/application.h"
#include "server_controller/server_controller.h"

int main() {
//    APPLICATION app;
//    app_create(&app);

//    app_run(&app, 13877);

    SERVER_CONTROLLER controller;
    controller_create(&controller);
    controller_start(&controller);
    return 0;
}
