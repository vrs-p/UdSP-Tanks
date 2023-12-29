//
// Created by vrsp on 28.12.2023.
//

#include "application.h"

void app_create(APPLICATION* app) {
    app->socket = sfUdpSocket_create();
    app->map = malloc(sizeof(MAP));
    map_create(app->map);

    app->packetSend = sfPacket_create();
    app->id = 0;

    app->isRunning = true;
    app->playerWasKilled = false;

//    app->font = sfFont_createFromFile("../font/consola.ttf");
//    sfText_setFont(app->nameOfPlayer, app->font);
//    sfText_setCharacterSize(app->nameOfPlayer, 11);
//    sfText_setFillColor(app->nameOfPlayer, sfWhite);

    app->clientTank = malloc(sizeof(TANK));
    tank_create(app->clientTank);
    app->otherTanks = malloc(sizeof(LINKED_LIST));
    ls_create(app->otherTanks, sizeof(TANK*));
}

void app_destroy(APPLICATION* app) {
    pthread_mutex_destroy(app->mutex);
    pthread_cond_destroy(app->sendDataCond);

    sfFont_destroy(app->font);
    sfPacket_destroy(app->packetSend);
    sfText_destroy(app->nameOfPlayer);
    sfRenderWindow_destroy(app->window);
    sfUdpSocket_destroy(app->socket);

    tank_destroy(app->clientTank);
    ls_run_function(app->otherTanks, tank_destroy_void);
    ls_destroy(app->otherTanks);

    map_destroy(app->map);
    free(app->map);
}



int app_get_player_score(APPLICATION* app) {
    return tank_get_score(app->clientTank);
}

LINKED_LIST* app_get_other_tanks(APPLICATION* app) {
    return app->otherTanks;
}

void app_connect_to_server(APPLICATION* app) {
    sfPacket_clear(app->packetSend);
    sfPacket_writeString(app->packetSend, tank_get_player_name(app->clientTank));
    if (sfUdpSocket_sendPacket(app->socket, app->packetSend, app->ipAddress, app->port) != sfSocketDone) {
        fprintf(stderr, "Sending failed.");
    }

    sfIpAddress ipAddress = sfIpAddress_Any;
    unsigned short port;
    float tmpX, tmpY;
    int tmpDir, tmpID, numberOfPlayers;
    sfPacket* packetReceive = sfPacket_create();
    sfPacket_clear(packetReceive);

    if (sfUdpSocket_receivePacket(app->socket, packetReceive, &ipAddress, &port) == sfSocketDone) {
        tmpX = sfPacket_readFloat(packetReceive);
        tmpY = sfPacket_readFloat(packetReceive);
        tmpID = sfPacket_readInt32(packetReceive);
        tmpDir = sfPacket_readInt32(packetReceive);
        numberOfPlayers = sfPacket_readInt32(packetReceive);
    }

    sfPacket_destroy(packetReceive);

    switch (tmpDir) {
        case UP:
            tmpX = tmpX - sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).x
                    * sfSprite_getScale(tank_get_sprite(app->clientTank)).x / 2;
            tmpY = tmpY - sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).y
                    * sfSprite_getScale(tank_get_sprite(app->clientTank)).y - 20;
            break;

        case DOWN:
            tmpX = tmpX - sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).x
                    * sfSprite_getScale(tank_get_sprite(app->clientTank)).x / 2;
            tmpY = tmpY - sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).y
                    * sfSprite_getScale(tank_get_sprite(app->clientTank)).y / 3 + 20;
            break;

        case LEFT:
            tmpX = tmpX - sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).x
                    * sfSprite_getScale(tank_get_sprite(app->clientTank)).x;
            tmpY = tmpY - sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).y
                    * sfSprite_getScale(tank_get_sprite(app->clientTank)).y * 2 / 3;
            break;

        case RIGHT:
            tmpY = tmpY - sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).y
                    * sfSprite_getScale(tank_get_sprite(app->clientTank)).y * 2 / 3;
            break;
    }

    switch (tmpID) {
        case 2:
            sfSprite_setColor(tank_get_sprite(app->clientTank), sfColor_fromRGBA(0, 255, 0, 192));
            break;

        case 3:
            sfSprite_setColor(tank_get_sprite(app->clientTank), sfColor_fromRGBA(255, 0, 0, 192));
            break;

        case 4:
            sfSprite_setColor(tank_get_sprite(app->clientTank), sfColor_fromRGBA(0, 255, 255, 192));
            break;

        default:
            break;
    }

    app->id = tmpID;
    app->numberOfPlayers = numberOfPlayers;
    sfVector2f vec = {tmpX, tmpY};
    sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
    tank_rotate(app->clientTank, tmpDir);
    tank_set_direction(app->clientTank, tmpDir);
    tank_set_player_id(app->clientTank, tmpID);

    printf("X: %.2f Y: %.2f ID: %d\n", tmpX, tmpY, tmpID);
}

void app_wait_for_game_settings(APPLICATION* app) {
    sfPacket* packetReceive = sfPacket_create();
    sfIpAddress ipAddress = sfIpAddress_Any;
    unsigned short port;
    int pID, dir;
    float posX, posY;
    char name[50];

    sfPacket_clear(packetReceive);

    if (sfUdpSocket_receivePacket(app->socket, packetReceive, &ipAddress, &port) == sfSocketDone)

    for (int i = 0; i < app->numberOfPlayers - 1; ++i) {
        pID = sfPacket_readInt32(packetReceive);
        sfPacket_readString(packetReceive, name);
        posX = sfPacket_readFloat(packetReceive);
        posY = sfPacket_readFloat(packetReceive);
        dir = sfPacket_readInt32(packetReceive);


        TANK* tmpTank = malloc(sizeof(TANK));
        tank_create(tmpTank);
        tank_set_player_id(tmpTank, pID);
        tank_set_player_name(tmpTank, name);

        switch (dir) {
            case UP:
                break;

            case DOWN: {
                tank_set_direction(tmpTank, DOWN);
                tank_rotate(tmpTank, UP);
                tank_set_direction(tmpTank, DOWN);
                break;
            }
            case LEFT: {
                float sizeX = (float)sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).x;
                float scaleX = sfSprite_getScale(tank_get_sprite(app->clientTank)).x;
                float sizeY = (float)sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).y;
                float scaleY = sfSprite_getScale(tank_get_sprite(app->clientTank)).y;

                posX = posX + sizeY * scaleY - sizeX * scaleX;
                posY = posY - sizeY * scaleY;
                break;
            }
            case RIGHT: {
                float sizeX = (float)sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).x;
                float scaleX = sfSprite_getScale(tank_get_sprite(app->clientTank)).x;
                float sizeY = (float)sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).y;
                float scaleY = sfSprite_getScale(tank_get_sprite(app->clientTank)).y;

                posX = posX - sizeY * scaleY;
                posY = posY - sizeY * scaleY + sizeX + scaleX;
                break;
            }
        }

        switch (tank_get_player_id(tmpTank)) {
            case 2:
                sfSprite_setColor(tank_get_sprite(tmpTank), sfColor_fromRGBA(0, 255, 0, 192));
                break;

            case 3:
                sfSprite_setColor(tank_get_sprite(tmpTank), sfColor_fromRGBA(255, 0, 0, 192));
                break;

            case 4:
                sfSprite_setColor(tank_get_sprite(tmpTank), sfColor_fromRGBA(0, 255, 255, 192));
                break;
        }

        sfVector2f vec = {posX, posY};
        sfSprite_setPosition(tank_get_sprite(tmpTank), vec);
        tank_rotate(tmpTank, dir);

        ls_push(app->otherTanks, &tmpTank);
    }
    sfPacket_destroy(packetReceive);
}



#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void app_initialize_window(APPLICATION* app) {
    sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT, 32};
    app->window = sfRenderWindow_create(mode, "UdSP-Tanks", sfClose, NULL);
    sfRenderWindow_setFramerateLimit(app->window, 60);
    sfRenderWindow_setActive(app->window, sfTrue);
}

void app_read_client_input(APPLICATION* app) {
    sfEvent event;
    while (sfRenderWindow_pollEvent(app->window, &event)) {
        if (event.type == sfEvtClosed) {
            tank_set_left(app->clientTank, true);
            pthread_mutex_lock(app->mutex);
            app->sendDataBool = true;
            pthread_cond_signal(app->sendDataCond);
            pthread_mutex_unlock(app->mutex);
        } else if (event.type == sfEvtKeyPressed) {
            switch (event.key.code) {
                case sfKeyUp:
                    tank_move_up(app->clientTank);
                    break;

                case sfKeyDown:
                    tank_move_down(app->clientTank);
                    break;

                case sfKeyLeft:
                    tank_move_left(app->clientTank);
                    break;

                case sfKeyRight:
                    tank_move_right(app->clientTank);
                    break;

                case sfKeySpace:
                    tank_fire(app->clientTank);
                    break;

                case sfKeyQ:
                case sfKeyEscape:
                    tank_set_left(app->clientTank, true);
                    break;
                default:
                    break;
            }

            pthread_mutex_lock(app->mutex);
            app->sendDataBool = true;
            pthread_cond_signal(app->sendDataCond);
            pthread_mutex_unlock(app->mutex);
        }
    }
}

void app_check_borders(APPLICATION* app) {
    float posX = sfSprite_getPosition(tank_get_sprite(app->clientTank)).x;
    float posY = sfSprite_getPosition(tank_get_sprite(app->clientTank)).y;

    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, map_get_list_of_walls(app->map));
    while (ls_iterator_has_next(&iterator)) {
        sfRectangleShape* wall = *(sfRectangleShape**)ls_iterator_move_next(&iterator);
        float wallPosX = sfRectangleShape_getPosition(wall).x;
        float wallPosY = sfRectangleShape_getPosition(wall).y;
        float wallSizeX = sfRectangleShape_getSize(wall).x;
        float wallSizeY = sfRectangleShape_getSize(wall).y;


        sfFloatRect tankBounds = sfSprite_getGlobalBounds(tank_get_sprite(app->clientTank));
        sfFloatRect wallBounds = sfRectangleShape_getGlobalBounds(wall);
        sfBool intersection = sfFloatRect_intersects(&tankBounds, &wallBounds, NULL);
//        sfFloatRect_intersects(&sfSprite_getGlobalBounds(tank_get_sprite(app->clientTank)), sfRectangleShape_getGlobalBounds(wall), NULL)
        if (intersection) {
            switch (tank_get_direction(app->clientTank)) {
                case UP: {
                    sfVector2f vec = {posX, wallPosY + wallSizeY + 1};
                    sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
                    break;
                }
                case DOWN: {
                    sfVector2f vec = {posX, wallPosY - 1};
                    sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
                    break;
                }
                case LEFT: {
                    sfVector2f vec = {wallPosX + wallSizeX + 1, posY};
                    sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
                    break;
                }
                case RIGHT: {
                    sfVector2f vec = {wallPosX - 1, posY};
                    sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
                    break;
                }
            }
        }
    }

    if (posX > SCREEN_WIDTH) {
        sfVector2f vec = {SCREEN_WIDTH, posY};
        sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
    }

    if (posX < 0) {
        sfVector2f vec = {0, posY};
        sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
    }

    if (posY > SCREEN_HEIGHT) {
        sfVector2f vec = {posX, SCREEN_HEIGHT};
        sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
    }

    if (posY < 0) {
        sfVector2f vec = {posX, 0};
        sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
    }
}

void app_update_positions_of_tanks(APPLICATION* app) {
    sfPacket* packetReceive = sfPacket_create();
    sfPacket* packetSend = sfPacket_create();
    sfIpAddress ipAddress = sfIpAddress_Any;
    unsigned short port;
    float posX, posY;

    if (sfUdpSocket_receivePacket(app->socket, packetReceive, &ipAddress, &port) == sfSocketDone) {}
    sfPacket_clear(packetReceive);
    sfPacket_clear(packetSend);

    posX = sfSprite_getPosition(tank_get_sprite(app->clientTank)).x;
    posY = sfSprite_getPosition(tank_get_sprite(app->clientTank)).y;

    sfPacket_writeInt32(packetSend, tank_get_player_id(app->clientTank));
    sfPacket_writeFloat(packetSend, posX);
    sfPacket_writeFloat(packetSend, posY);
    sfPacket_writeInt32(packetSend, tank_get_direction(app->clientTank));

    if (sfUdpSocket_sendPacket(app->socket, packetSend, app->ipAddress, app->port) != sfSocketDone) {
        fprintf(stderr, "Sending failed.\n");
    }

    sfPacket_destroy(packetReceive);
    sfPacket_destroy(packetSend);
}

void app_check_bullet_collision(APPLICATION* app) {
    BULLET* bullet = tank_get_bullet(app->clientTank);

    if (bullet != NULL) {
        if (bullet_was_fired(bullet)) {
            float bulletPosX = bullet_get_position(bullet).x;
            float bulletPosY = bullet_get_position(bullet).y;
            float bulletSizeX = bullet_get_size(bullet).x;
            float bulletSizeY = bullet_get_size(bullet).y;

            LINKED_LIST_ITERATOR iterator;
            ls_iterator_init(&iterator, app->otherTanks);
            while (ls_iterator_has_next(&iterator)) {
                TANK* tank = *(TANK**)ls_iterator_move_next(&iterator);
                if (!tank_get_left(tank)) {
                    float tankPosX = sfSprite_getPosition(tank_get_sprite(tank)).x;
                    float tankPosY = sfSprite_getPosition(tank_get_sprite(tank)).y;
                    float tankSizeX = sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(tank))).x
                                      * sfSprite_getScale(tank_get_sprite(tank)).x;
                    float tankSizeY = sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(tank))).y
                                      * sfSprite_getScale(tank_get_sprite(tank)).y;

                    if (bullet_was_fired(bullet)) {
                        switch (tank_get_direction(tank)) {
                            case UP:
                                if (bulletPosX + bulletSizeX >= tankPosX && bulletPosX <= tankPosX + tankSizeX &&
                                    bulletPosY + bulletSizeY >= tankPosY && bulletPosY <= tankPosY + tankSizeY) {
                                    bullet_set_fired(bullet, false);
                                    app->playerWasKilled = true;
                                    app->idOfKilledPlayer = tank_get_player_id(tank);
                                    pthread_mutex_lock(app->mutex);
                                    app->sendDataBool = true;
                                    pthread_cond_signal(app->sendDataCond);
                                    pthread_mutex_unlock(app->mutex);
                                }
                                break;

                            case DOWN:
                                if (bulletPosX + bulletSizeX >= tankPosX - tankSizeX && bulletPosX <= tankPosX &&
                                    bulletPosY + bulletSizeY >= tankPosY - tankSizeY && bulletPosY <= tankPosY) {
                                    bullet_set_fired(bullet, false);
                                    app->playerWasKilled = true;
                                    app->idOfKilledPlayer = tank_get_player_id(tank);
                                    pthread_mutex_lock(app->mutex);
                                    app->sendDataBool = true;
                                    pthread_cond_signal(app->sendDataCond);
                                    pthread_mutex_unlock(app->mutex);
                                }
                                break;

                            case LEFT:
                                if (bulletPosX + bulletSizeX >= tankPosX && bulletPosX <= tankPosX + tankSizeY &&
                                    bulletPosY + bulletSizeY >= tankPosY - tankSizeX && bulletPosY <= tankPosY) {
                                    bullet_set_fired(bullet, false);
                                    app->playerWasKilled = true;
                                    app->idOfKilledPlayer = tank_get_player_id(tank);
                                    pthread_mutex_lock(app->mutex);
                                    app->sendDataBool = true;
                                    pthread_cond_signal(app->sendDataCond);
                                    pthread_mutex_unlock(app->mutex);
                                }
                                break;

                            case RIGHT:
                                if (bulletPosX + bulletSizeX >= tankPosX - tankSizeY && bulletPosX <= tankPosX &&
                                    bulletPosY + bulletSizeY >= tankPosY && bulletPosY <= tankPosY + tankSizeX) {
                                    bullet_set_fired(bullet, false);
                                    app->playerWasKilled = true;
                                    app->idOfKilledPlayer = tank_get_player_id(tank);
                                    pthread_mutex_lock(app->mutex);
                                    app->sendDataBool = true;
                                    pthread_cond_signal(app->sendDataCond);
                                    pthread_mutex_unlock(app->mutex);
                                }
                                break;
                        }
                    }
                }
            }
        }
    }
}

void app_communication_with_server(APPLICATION* app) {
    app_connect_to_server(app);
    app_update_positions_of_tanks(app);
    app_wait_for_game_settings(app);
}

void app_draw(APPLICATION* app) {
    sfRenderWindow_clear(app->window, sfBlack);

    tank_render(app->clientTank, app->window, map_get_list_of_walls(app->map));

//    float tankPosX = sfSprite_getPosition(tank_get_sprite(app->clientTank)).x;
//    float tankPosY = sfSprite_getPosition(tank_get_sprite(app->clientTank)).y;
//    float tankSizeX = (float)sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).x
//                      * sfSprite_getScale(tank_get_sprite(app->clientTank)).x;
//    float tankSizeY = (float)sfTexture_getSize(sfSprite_getTexture(tank_get_sprite(app->clientTank))).y
//                      * sfSprite_getScale(tank_get_sprite(app->clientTank)).y;

//    switch (tank_get_direction(app->clientTank)) {
//        case UP: {
//            sfVector2f vec = {tankPosX + tankSizeX / 2 - sfText_getLocalBounds(app->nameOfPlayer).width / 2,
//                              tankPosY + tankSizeY + 5};
//            sfText_setPosition(app->nameOfPlayer, vec);
//            break;
//        }
//        case DOWN: {
//            sfVector2f vec = {tankPosX - tankSizeX / 2 - sfText_getLocalBounds(app->nameOfPlayer).width / 2,
//                              tankPosY - tankSizeY - sfText_getLocalBounds(app->nameOfPlayer).height - 10};
//            sfText_setPosition(app->nameOfPlayer, vec);
//            break;
//        }
//        case LEFT: {
//            if (sfText_getLocalBounds(app->nameOfPlayer).width <= tankSizeY) {
//                sfVector2f vec = {tankPosX + tankSizeY - sfText_getLocalBounds(app->nameOfPlayer).width, tankPosY + 5};
//                sfText_setPosition(app->nameOfPlayer, vec);
//            } else {
//                sfVector2f vec = {tankPosX + tankSizeY / 2 - sfText_getLocalBounds(app->nameOfPlayer).width / 2,
//                                  tankPosY + 5};
//                sfText_setPosition(app->nameOfPlayer, vec);
//            }
//            break;
//        }
//        case RIGHT: {
//            if (sfText_getLocalBounds(app->nameOfPlayer).width <= tankSizeY) {
//                sfVector2f vec = {tankPosX - tankSizeY, tankPosY + tankSizeX + 5};
//                sfText_setPosition(app->nameOfPlayer, vec);
//            } else {
//                sfVector2f vec = {tankPosX - tankSizeY / 2 - sfText_getLocalBounds(app->nameOfPlayer).width / 2,
//                                  tankPosY + tankSizeX + 5};
//                sfText_setPosition(app->nameOfPlayer, vec);
//            }
//            break;
//        }
//    }

//    sfRenderWindow_drawText(app->nameOfPlayer); TODO: fix it

    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(&iterator, app->otherTanks);
    while (ls_iterator_has_next(&iterator)) {
        TANK* tank = *(TANK**)ls_iterator_move_next(&iterator);
        tank_lock_mutex(tank);
        if (!tank_get_left(tank)) {
            tank_render(tank, app->window, map_get_list_of_walls(app->map));
        }
        tank_unlock_mutex(tank);
    }

    LINKED_LIST_ITERATOR iteratorOther;
    ls_iterator_init(&iteratorOther, map_get_list_of_walls(app->map));
    while (ls_iterator_has_next(&iteratorOther)) {
        sfRectangleShape* wall = *(sfRectangleShape**)ls_iterator_move_next(&iteratorOther);
        sfRenderWindow_drawRectangleShape(app->window, wall, NULL);
    }

    sfRenderWindow_display(app->window);
}

void* app_render(void* application) {
    APPLICATION* app = application;
    app_initialize_window(app);

    while (app->isRunning) {
        app_read_client_input(app);
        app_check_borders(app);
        app_check_bullet_collision(app);
        app_draw(app);
    }

    sfRenderWindow_setActive(app->window, sfFalse);
    sfRenderWindow_close(app->window);
    return 0;
}



#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT





void* app_receive_data(void* application) {
    APPLICATION* app = application;
    sfPacket* packetReceive = sfPacket_create();
    sfIpAddress ipAddress = sfIpAddress_Any;
    unsigned short port;
    int pID, dir;
    float posX, posY;
    bool fired;
    int msgType, killerID, score;

    while (app->isRunning) {
        sfPacket_clear(packetReceive);
        if (sfUdpSocket_receivePacket(app->socket, packetReceive, &ipAddress, &port) == sfSocketDone) {
            msgType = sfPacket_readInt32(packetReceive);
            msgType--;

            if (msgType == STATUS) {
                for (int i = 0; i < app->numberOfPlayers - 1; ++i) {
                    pID = sfPacket_readInt32(packetReceive);
                    posX = sfPacket_readFloat(packetReceive);
                    posY = sfPacket_readFloat(packetReceive);
                    dir = sfPacket_readInt32(packetReceive);
                    fired = sfPacket_readBool(packetReceive);

                    LINKED_LIST_ITERATOR iterator;
                    ls_iterator_init(&iterator, app->otherTanks);
                    while (ls_iterator_has_next(&iterator)) {
                        TANK* tank = *(TANK**)ls_iterator_move_next(&iterator);
                        tank_lock_mutex(tank);
                        if (tank_get_player_id(tank) == pID) {
                            if (tank_get_direction(tank) != dir) {
                                tank_rotate(tank, dir);
                            }

                            sfVector2f vec = { posX, posY};
                            sfSprite_setPosition(tank_get_sprite(tank), vec);

                            if (fired) {
                                tank_fire(tank);
                            }
                        }
                        tank_unlock_mutex(tank);
                    }
                }
            } else if (msgType == KILLED) {
                pID = sfPacket_readInt32(packetReceive);
                posX = sfPacket_readFloat(packetReceive);
                posY = sfPacket_readFloat(packetReceive);
                dir = sfPacket_readInt32(packetReceive);
                killerID = sfPacket_readInt32(packetReceive);

                if (pID == app->id) {
                    bullet_set_fired(tank_get_bullet(app->clientTank), false);
                    tank_rotate(app->clientTank, dir);
                    sfVector2f vec = {posX, posY};
                    sfSprite_setPosition(tank_get_sprite(app->clientTank), vec);
                } else {
                    LINKED_LIST_ITERATOR iterator;
                    ls_iterator_init(&iterator, app->otherTanks);
                    while (ls_iterator_has_next(&iterator)) {
                        TANK* tank = *(TANK**)ls_iterator_move_next(&iterator);
                        tank_lock_mutex(tank);
                        if (tank_get_player_id(tank) == pID) {
                            bullet_set_fired(tank_get_bullet(tank), false);

                            if (tank_get_direction(tank) != dir) {
                                tank_rotate(tank, dir);
                            }

                            sfVector2f vec = {posX, posY};
                            sfSprite_setPosition(tank_get_sprite(tank), vec);
                        }
                        tank_unlock_mutex(tank);
                    }
                }
                LINKED_LIST_ITERATOR iterator;
                ls_iterator_init(&iterator, app->otherTanks);
                while (ls_iterator_has_next(&iterator)) {
                    TANK* tank = *(TANK**)ls_iterator_move_next(&iterator);
                    if (tank_get_player_id(tank) == killerID) {
                        bullet_set_fired(tank_get_bullet(tank), false);
                    }
                }
            } else if (msgType == END) {
                for (int i = 0; i < app->numberOfPlayers; ++i) {
                    pID = sfPacket_readInt32(packetReceive);
                    score = sfPacket_readInt32(packetReceive);

                    if (tank_get_player_id(app->clientTank) == pID) {
                        tank_set_score(app->clientTank, score);
                    } else {
                        LINKED_LIST_ITERATOR iterator;
                        ls_iterator_init(&iterator, app->otherTanks);
                        while (ls_iterator_has_next(&iterator)) {
                            TANK* tank = *(TANK**)ls_iterator_move_next(&iterator);
                            if (tank_get_player_id(tank) == pID) {
                                tank_set_score(tank, score);
                            }
                        }
                    }
                }
                app->isRunning = false;
            } else if (msgType == PLAYER_QUIT) {
                pID = sfPacket_readInt32(packetReceive);

                LINKED_LIST_ITERATOR iterator;
                ls_iterator_init(&iterator, app->otherTanks);
                while (ls_iterator_has_next(&iterator)) {
                    TANK* tank = *(TANK**)ls_iterator_move_next(&iterator);
                    tank_lock_mutex(tank);
                    if (tank_get_player_id(tank) == pID) {
                        tank_set_left(tank, true);
                    }
                    tank_unlock_mutex(tank);
                }
            }
        }
        usleep(20 * 1000);
    }
    sfPacket_destroy(packetReceive);
    return 0;
}

void* app_send_data(void* application) {
    APPLICATION* app = application;
    sfPacket* packetSend = sfPacket_create();
    float posX, posY;
    bool continueWithSending = true;

    while (continueWithSending) {
        pthread_mutex_lock(app->mutex);
        while (!app->sendDataBool) {
            pthread_cond_wait(app->sendDataCond, app->mutex);
        }
        pthread_mutex_unlock(app->mutex);

        if (app->window != NULL && app->clientTank != NULL) {
            sfPacket_clear(packetSend);

            if (tank_get_left(app->clientTank)) {
                sfPacket_writeInt32(packetSend, END + 1);
                sfPacket_writeInt32(packetSend, tank_get_player_id(app->clientTank));
                continueWithSending = false;
            } else if (app->playerWasKilled) {
                sfPacket_writeInt32(packetSend, KILLED + 1);
                sfPacket_writeInt32(packetSend, app->idOfKilledPlayer);
                sfPacket_writeInt32(packetSend, tank_get_player_id(app->clientTank));
                app->playerWasKilled = false;
                bullet_reset_was_fired_and_sent(tank_get_bullet(app->clientTank));
            } else if (app->isRunning) {
                posX = sfSprite_getPosition(tank_get_sprite(app->clientTank)).x;
                posY = sfSprite_getPosition(tank_get_sprite(app->clientTank)).y;

                sfPacket_writeInt32(packetSend, STATUS + 1);
                sfPacket_writeInt32(packetSend, tank_get_player_id(app->clientTank));
                sfPacket_writeFloat(packetSend, posX);
                sfPacket_writeFloat(packetSend, posY);
                sfPacket_writeInt32(packetSend, tank_get_direction(app->clientTank));
                sfPacket_writeBool(packetSend, bullet_was_fired(tank_get_bullet(app->clientTank)) &&
                                    !bullet_was_fired_and_sent(tank_get_bullet(app->clientTank)));

                if (bullet_was_fired(tank_get_bullet(app->clientTank)) &&
                    !bullet_was_fired_and_sent(tank_get_bullet(app->clientTank))) {
                    bullet_set_was_fired_and_sent(tank_get_bullet(app->clientTank));
                }
            }

            if (sfUdpSocket_sendPacket(app->socket, packetSend, app->ipAddress, app->port) != sfSocketDone) {
                fprintf(stderr, "Sending failed.\n");
            }
            usleep(20 * 1000);
        }
        app->sendDataBool = false;
    }
    sfPacket_destroy(packetSend);
    return 0;
}

void app_run(APPLICATION* app, sfIpAddress ipAddress, int port, char* playerName) {
    app->ipAddress = ipAddress;
    app->port = port;
    app->sendDataBool = false;
//    sfText_setString(app->nameOfPlayer, playerName);
    tank_set_player_name(app->clientTank, playerName);

    app_communication_with_server(app);

    app->mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(app->mutex, NULL);
    app->sendDataCond = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(app->sendDataCond, NULL);

    if (app->isRunning) {
        pthread_t renderingThread, receivingThread, sendingThread;

        pthread_create(&renderingThread, NULL, app_render, app);
        pthread_create(&receivingThread, NULL, app_receive_data, app);
        pthread_create(&sendingThread, NULL, app_send_data, app);

        pthread_join(receivingThread, NULL);
        pthread_join(sendingThread, NULL);
        pthread_join(renderingThread, NULL);
    }
}
