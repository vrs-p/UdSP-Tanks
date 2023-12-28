#include "application/application.h"

int main() {
    APPLICATION app;
    app_create(&app);

    app_run(&app);
    return 0;
}
