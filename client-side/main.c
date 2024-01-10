#include <locale.h>
#include "server_controller/server_controller.h"
#include "windows/show_status/show_status.h"
#include "windows/main_menu/main_menu.h"
#include "windows/menu/menu.h"

//#define SERVER_IP "158.193.154.105"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 13877

int main() {
    setlocale(LC_ALL, "sk_SK.UTF-8");

    int activeGames, activePlayers;

    MMENU* mmenu = malloc(sizeof(MMENU));
    mmenu_create(mmenu);
    while (!mmenu_get_closed(mmenu) && !mmenu_get_kill(mmenu)) {
        mmenu_set_closed(mmenu, false);
        mmenu_set_create(mmenu, false);
        mmenu_set_join(mmenu, false);

        SERVER_MESSAGE_TYPE status = UNKNOWN;
        controller_get_server_statistics(sfIpAddress_fromString(SERVER_IP), SERVER_PORT, &activeGames, &activePlayers, &status);
        STATUS_MODAL* statusModal = malloc(sizeof(STATUS_MODAL));

        mmenu_set_controller_status(mmenu, status != SUCCESS);
        mmenu_update_stats(mmenu, activeGames, activePlayers);
        mmenu_render(mmenu);

        if (!mmenu_get_closed(mmenu)) {
            if (mmenu_get_create(mmenu)) {
                MENU *menu = malloc(sizeof(MENU));
                menu_create(menu, true);
                menu_render(menu);
                if (!menu_get_app_closed(menu) && menu_get_started(menu)) {
                    controller_create_server(menu_get_ip_address(menu), menu_get_port(menu), menu_get_new_port(menu),
                                             menu_get_num_players(menu), menu_get_id_map(menu) - 1, &status);
                    status_create(statusModal, &status);
                    status_render(statusModal);
                    status_destroy(statusModal);
                    if (status == PORT_OCCUPIED || status == UNKNOWN) {
                        mmenu_set_closed(mmenu, true);
                    } else {
                        controller_join_server(menu_get_ip_address(menu), menu_get_new_port(menu), menu_get_name(menu));
                    }
                }
                menu_destroy(menu);
                free(menu);
            } else if (mmenu_get_join(mmenu)) {
                MENU *menu = malloc(sizeof(MENU));
                menu_create(menu, false);
                menu_render(menu);
                if (!menu_get_app_closed(menu) && menu_get_started(menu)) {
                    controller_join_server(menu_get_ip_address(menu), menu_get_port(menu), menu_get_name(menu));
                }
                menu_destroy(menu);
                free(menu);
            } else if (mmenu_get_kill(mmenu)) {
                controller_kill_server(sfIpAddress_fromString(SERVER_IP), SERVER_PORT, &status);
                status_create(statusModal, &status);
                status_render(statusModal);
                status_destroy(statusModal);
            }
        }
        if (statusModal != NULL) {
            free(statusModal);
            statusModal = NULL;
        }
    }

    mmenu_destroy(mmenu);
    free(mmenu);

    return 0;
}