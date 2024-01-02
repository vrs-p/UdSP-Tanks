#include "server_controller/server_controller.h"
#include "windows/main_menu.h"
#include "windows/menu.h"

int main() {
//    controller_create_server(sfIpAddress_fromString("127.0.0.1"), 13877, 13878, 1);
//    controller_join_server(sfIpAddress_fromString("127.0.0.1"), 13878, "LogičMou1");
//    controller_kill_server(sfIpAddress_fromString("127.0.0.1"), 13877);

    MMENU* mmenu = malloc(sizeof(MMENU));
    mmenu_create(mmenu);
    while (!mmenu_get_closed(mmenu) && !mmenu_get_kill(mmenu)) {
        mmenu_set_closed(mmenu, false);
        mmenu_set_create(mmenu, false);
        mmenu_set_join(mmenu, false);
        mmenu_render(mmenu);

        if (!mmenu_get_closed(mmenu)) {
            if (mmenu_get_create(mmenu)) {
                MENU *menu = malloc(sizeof(MENU));
                menu_create(menu, true);
                menu_render(menu);
                if (!menu_get_app_closed(menu) && menu_get_started(menu)) {
                    controller_create_server(menu_get_ip_address(menu), menu_get_port(menu), menu_get_new_port(menu),
                                             menu_get_num_players(menu));
                    controller_join_server(menu_get_ip_address(menu), menu_get_new_port(menu), menu_get_name(menu));
                }
                menu_destroy(menu);
            } else if (mmenu_get_join(mmenu)) {
                MENU *menu = malloc(sizeof(MENU));
                menu_create(menu, false);
                menu_render(menu);
                if (!menu_get_app_closed(menu) && menu_get_started(menu)) {
                    controller_join_server(menu_get_ip_address(menu), menu_get_port(menu), menu_get_name(menu));
                }
                menu_destroy(menu);
            } else if (mmenu_get_kill(mmenu)) {
                controller_kill_server(sfIpAddress_fromString("127.0.0.1"), 13877);
            }
        }
    }

    mmenu_destroy(mmenu);

    return 0;
}