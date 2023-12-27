//
// Created by mikulec on 12/27/23.
//

#ifndef SERVER_SIDE_PLAYER_H
#define SERVER_SIDE_PLAYER_H

#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "../base/network.h"
#include "../base/movement.h"

typedef struct player {
    int id;
    int score;
    int killedBy;
    bool left;
    bool fired;
    bool killed;
    bool scoreWasSent;
    char* playerName;
    CONNECTION* connection;
    POSITION* position;
    POSITION* initialPosition;

    pthread_mutex_t mutex;
} PLAYER;

void player_create(PLAYER* player, int id, char* playerName, float xPosition, float yPosition, DIRECTION direction, unsigned short port, sfIpAddress ipAddress);
void player_destroy(PLAYER* player);
void player_destroy_void(void* player);

POSITION* player_get_position(PLAYER* player);
CONNECTION* player_get_connection(PLAYER* player);

bool player_fired_get(PLAYER* player);
bool player_killed_get(PLAYER* player);
bool player_left_get(PLAYER* player);
bool player_sent_score_get(PLAYER* player);

int player_id_get(PLAYER* player);
int player_killed_by_get(PLAYER* player);
int player_score_get(PLAYER* player);

void player_update_position(PLAYER* player, float xPosition, float yPosition, DIRECTION direction);
void player_set_initial_position(PLAYER* player, float xPosition, float yPosition, DIRECTION direction);
void player_set_fired(PLAYER* player, bool fired);
void player_reset_position(PLAYER* player);
void player_killed(PLAYER* player);
void player_killed_by(PLAYER* player, int pId);
void player_left(PLAYER* player, bool left);
void player_increase_score(PLAYER* player);
void player_score_sent(PLAYER* player, bool sent);

char* player_name(PLAYER* player);

void player_lock_mutex(PLAYER* player);
void player_unlock_mutex(PLAYER* player);

#endif //SERVER_SIDE_PLAYER_H
