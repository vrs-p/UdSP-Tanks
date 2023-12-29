#include "application/application.h"

int main() {
    APPLICATION app;
    app_create(&app);

    sfIpAddress ipAddress = sfIpAddress_fromString("127.0.0.1");
    unsigned short port = 13877;
    app_run(&app, ipAddress, port, "LogiMou");
    return 0;
}
