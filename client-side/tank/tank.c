//
// Created by vrsp on 28.12.2023.
//

#include <string.h>
#include "tank.h"

void tank_create(TANK *tank) {
    tank->left = false;

    tank->texture = sfTexture_createFromFile("../../common/img/tankWithoutBG.png", NULL);
    tank->sprite = sfSprite_create();
    sfSprite_setTexture(tank->sprite, tank->texture, sfTrue);
    sfVector2f vec = {0.05, 0.05};
    sfSprite_setScale(tank->sprite, vec);

    tank->bullet = malloc(sizeof(BULLET));
    bullet_create(tank->bullet);
    tank->speed = 5.0;
    tank->reloadTime = 3;
    tank->direction = UP;

    tank->lastFire = time(NULL);
}

void tank_destroy(TANK *tank) {
    free(tank->playerName);
    tank->playerName = NULL;

    sfSprite_destroy(tank->sprite);
    sfTexture_destroy(tank->texture);

    free(tank->mutex);
    tank->mutex = NULL;

    bullet_destroy(tank->bullet);
    tank->bullet = NULL;
}

void tank_destroy_void(void *tank) {
    tank_destroy((TANK*)tank);
}

void tank_move_up(TANK *tank) {
    tank_rotate(tank, UP);
    sfVector2f vec = {0, -tank->speed};
    sfSprite_move(tank->sprite, vec);
}

void tank_move_down(TANK *tank) {
    tank_rotate(tank, DOWN);
    sfVector2f vec = {0, tank->speed};
    sfSprite_move(tank->sprite, vec);
}

void tank_move_left(TANK *tank) {
    tank_rotate(tank, LEFT);
    sfVector2f vec = {-tank->speed, 0};
    sfSprite_move(tank->sprite, vec);
}

void tank_move_right(TANK *tank) {
    tank_rotate(tank, RIGHT);
    sfVector2f vec = {tank->speed, 0};
    sfSprite_move(tank->sprite, vec);
}

void tank_fire(TANK *tank) {
    if (difftime(time(NULL), tank->lastFire) > tank->reloadTime) {
        float xSize = sfTexture_getSize(sfSprite_getTexture(tank->sprite)).x * sfSprite_getScale(tank->sprite).x;

        switch (tank->direction) {
            case UP:
                bullet_shot(tank->bullet, sfSprite_getPosition(tank->sprite).x + xSize / 2,
                            sfSprite_getPosition(tank->sprite).y, tank->direction);
                break;

            case DOWN:
                bullet_shot(tank->bullet, sfSprite_getPosition(tank->sprite).x - xSize / 2,
                            sfSprite_getPosition(tank->sprite).y, tank->direction);
                break;

            case LEFT:
                bullet_shot(tank->bullet, sfSprite_getPosition(tank->sprite).x,
                            sfSprite_getPosition(tank->sprite).y - xSize / 2, tank->direction);
                break;

            case RIGHT:
                bullet_shot(tank->bullet, sfSprite_getPosition(tank->sprite).x,
                            sfSprite_getPosition(tank->sprite).y + xSize / 2, tank->direction);
                break;
        }

        tank->lastFire = time(NULL);
    }
}

void tank_rotate(TANK *tank, DIRECTION dir) {
    float xSize = (float) sfTexture_getSize(sfSprite_getTexture(tank->sprite)).x * sfSprite_getScale(tank->sprite).x;
    float ySize = (float) sfTexture_getSize(sfSprite_getTexture(tank->sprite)).y * sfSprite_getScale(tank->sprite).y;
    sfVector2f vec;
    switch (dir) {
        case UP:
            switch (tank->direction) {
                case DOWN:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x - xSize,
                                        sfSprite_getPosition(tank->sprite).y - ySize * 4 / 3};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case LEFT:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x + ySize - xSize,
                                        sfSprite_getPosition(tank->sprite).y - ySize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case RIGHT:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x - ySize,
                                        sfSprite_getPosition(tank->sprite).y - ySize + xSize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                default:
                    break;
            }

            sfSprite_setRotation(tank->sprite, 0);
            break;

        case DOWN:
            switch (tank->direction) {
                case UP:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x + xSize,
                                        sfSprite_getPosition(tank->sprite).y + ySize * 4 / 3};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case LEFT:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x + ySize,
                                        sfSprite_getPosition(tank->sprite).y + ySize - xSize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case RIGHT:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x - ySize + xSize,
                                        sfSprite_getPosition(tank->sprite).y + ySize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;
                case DOWN:
                    break;
            }

            sfSprite_setRotation(tank->sprite, 180);
            break;

        case LEFT:
            switch (tank->direction) {
                case UP:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x + xSize - ySize,
                                        sfSprite_getPosition(tank->sprite).y + ySize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case DOWN:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x - ySize,
                                        sfSprite_getPosition(tank->sprite).y - ySize + xSize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case RIGHT:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x - ySize * 4 / 3,
                                        sfSprite_getPosition(tank->sprite).y + xSize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;
                case LEFT:
                    break;
            }

            sfSprite_setRotation(tank->sprite, 270);
            break;

        case RIGHT:
            switch (tank->direction) {
                case UP:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x + ySize,
                                        sfSprite_getPosition(tank->sprite).y + ySize - xSize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case DOWN:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x - xSize + ySize,
                                        sfSprite_getPosition(tank->sprite).y - ySize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;

                case LEFT:
                    vec = (sfVector2f) {sfSprite_getPosition(tank->sprite).x + ySize * 4 / 3,
                                        sfSprite_getPosition(tank->sprite).y - xSize};
                    sfSprite_setPosition(tank->sprite, vec);
                    break;
                case RIGHT:
                    break;
            }

            sfSprite_setRotation(tank->sprite, 90);
            break;
    }

    tank->direction = dir;
}

void tank_render(TANK *tank, sfRenderWindow *window, LINKED_LIST *listOfWalls) {
    if (bullet_was_fired(tank->bullet)) {
        bullet_render(tank->bullet, window, listOfWalls);
    }

    sfRenderWindow_drawSprite(window, tank->sprite, NULL);
}

void tank_lock_mutex(TANK *tank) {
    pthread_mutex_lock(tank->mutex);
}

void tank_unlock_mutex(TANK *tank) {
    pthread_mutex_unlock(tank->mutex);
}

void tank_set_direction(TANK *tank, DIRECTION dir) {
    tank->direction = dir;
}

void tank_set_player_id(TANK *tank, int id) {
    tank->playerId = id;
}

void tank_set_player_name(TANK *tank, char *name) {
    tank->playerName = malloc(sizeof(char)*strlen(name));
    strcpy(tank->playerName, name);
}

void tank_set_left(TANK *tank, bool left) {
    tank->left = left;
}

void tank_set_score(TANK *tank, int score) {
    tank->score = score;
}

sfSprite *tank_get_sprite(TANK *tank) {
    return tank->sprite;
}

int tank_get_direction(TANK *tank) {
    return tank->direction;
}

int tank_get_player_id(TANK *tank) {
    return tank->playerId;
}

char *tank_get_player_name(TANK *tank) {
    return tank->playerName;
}

BULLET *tank_get_bullet(TANK *tank) {
    return tank->bullet;
}

bool tank_get_left(TANK *tank) {
    return tank->left;
}

int tank_get_score(TANK *tank) {
    return tank->score;
}