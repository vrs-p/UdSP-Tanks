//
// Created by mikulec on 12/27/23.
//

#include <malloc.h>
#include "player.h"


void player_create(PLAYER *player, int id, char *playerName, float xPosition, float yPosition, DIRECTION direction,
                   unsigned short port, sfIpAddress ipAddress) {
    player->id = id;
    player->playerName = malloc(sizeof(char) * strlen(playerName));
    strcpy(player->playerName, playerName);

    player->connection = malloc(sizeof(CONNECTION));
    player->position = malloc(sizeof(POSITION));

    player->position->xPosition = xPosition;
    player->position->yPosition = yPosition;
    player->position->direction = direction;

    player->connection->ipAddress = ipAddress;
    player->connection->port = port;

    player->fired = false;
    player->left = false;
    player->scoreWasSent = false;
    player->score = 0;

    pthread_mutex_init(&player->mutex, NULL);
}

void player_destroy(PLAYER *player) {
    free(player->position);
    player->position = NULL;

    free(player->connection);
    player->position = NULL;

    free(player->initialPosition);
    player->initialPosition = NULL;

    free(player->playerName);
    player->playerName = NULL;

}

POSITION *player_get_position(PLAYER *player) {
    return player->position;
}

CONNECTION *player_get_connection(PLAYER *player) {
    return player->connection;
}

bool player_fired_get(PLAYER *player) {
    return player->fired;
}

bool player_killed_get(PLAYER *player) {
    return player->killed;
}

bool player_left_get(PLAYER *player) {
    return player->left;
}

bool player_sent_score_get(PLAYER *player) {
    return player->scoreWasSent;
}

int player_id_get(PLAYER *player) {
    return player->id;
}

int player_killed_by_get(PLAYER *player) {
    return player->killedBy;
}

int player_score_get(PLAYER *player) {
    return player->score;
}

void player_update_position(PLAYER *player, float xPosition, float yPosition, DIRECTION direction) {
    player->position->xPosition = xPosition;
    player->position->yPosition = yPosition;
    player->position->direction = direction;
}

void player_set_initial_position(PLAYER *player, float xPosition, float yPosition, DIRECTION direction) {
    player->initialPosition = malloc(sizeof(POSITION));
    player->initialPosition->xPosition = xPosition;
    player->initialPosition->yPosition = yPosition;
    player->initialPosition->direction = direction;
}

void player_set_fired(PLAYER *player, bool fired) {
    player->fired = fired;
}

void player_reset_position(PLAYER *player) {
    player->position->xPosition = player->initialPosition->xPosition;
    player->position->yPosition = player->initialPosition->yPosition;
    player->position->direction = player->initialPosition->direction;
    player->killed = false;
}

void player_killed(PLAYER *player) {
    player->killed = true;
}

void player_killed_by(PLAYER *player, int pId) {
    player->killedBy = pId;
}

void player_left(PLAYER *player, bool left) {
    player->left = left;
}

void player_increase_score(PLAYER *player) {
    player->score++;
}

void player_score_sent(PLAYER *player, bool sent) {
    player->scoreWasSent = sent;
}

char *player_name(PLAYER *player) {
    return player->playerName;
}

void player_lock_mutex(PLAYER *player) {
    pthread_mutex_lock(&player->mutex);
}

void player_unlock_mutex(PLAYER *player) {
    pthread_mutex_unlock(&player->mutex);
}

void player_destroy_void(void *player) {
    player_destroy((PLAYER*)player);
}
