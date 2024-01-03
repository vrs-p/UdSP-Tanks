//
// Created by vrsp on 28.12.2023.
//

#ifndef UDSP_TANKS_TANK_H
#define UDSP_TANKS_TANK_H

#include <SFML/Graphics.h>
#include <pthread.h>
#include <wchar.h>
#include "../bullet/bullet.h"

typedef struct tank {
    int playerId;
    wchar_t* playerName;
    bool left;
    int score;
    
    sfTexture* texture;
    sfSprite* sprite;
    pthread_mutex_t* mutex;

    BULLET* bullet;
    float speed;
    double reloadTime;
    DIRECTION direction;

    time_t lastFire;
} TANK;

void tank_create(TANK* tank);
void tank_destroy(TANK* tank);
void tank_destroy_void(void * tank);

void tank_move_up(TANK* tank);
void tank_move_down(TANK* tank);
void tank_move_left(TANK* tank);
void tank_move_right(TANK* tank);
void tank_fire(TANK* tank);
void tank_rotate(TANK* tank, DIRECTION dir);

void tank_render(TANK* tank, sfRenderWindow* window, LINKED_LIST* listOfWalls);

void tank_lock_mutex(TANK* tank);
void tank_unlock_mutex(TANK* tank);

void tank_set_direction(TANK* tank, DIRECTION dir);
void tank_set_player_id(TANK* tank, int id);
void tank_set_player_name(TANK* tank, wchar_t* name);
void tank_set_left(TANK* tank, bool left);
void tank_set_score(TANK* tank, int score);

sfSprite* tank_get_sprite(TANK* tank);
int tank_get_direction(TANK* tank);
int tank_get_player_id(TANK* tank);
wchar_t* tank_get_player_name(TANK* tank);
BULLET* tank_get_bullet(TANK* tank);
bool tank_get_left(TANK* tank);
int tank_get_score(TANK* tank);

#endif //UDSP_TANKS_TANK_H
