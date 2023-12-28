//
// Created by vrsp on 28.12.2023.
//

#ifndef UDSP_TANKS_BULLET_H
#define UDSP_TANKS_BULLET_H

#include <SFML/Graphics.h>
#include "../../common/linked_list/linked_list.h"

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} DIRECTION;

typedef struct bullet {
    float speed;
    int diameter;
    sfRectangleShape* icon;
    bool fired;
    bool firedAndSent
    DIRECTION direction;
} BULLET;

void bullet_create(BULLET* bullet);
void bullet_destroy(BULLET* bullet);

void bullet_render(BULLET* bullet, sfRenderWindow* window, LINKED_LIST* listOfWalls);
void bullet_shot(BULLET* bullet, float xPosition, float yPosition, DIRECTION dir);
void bullet_set_fired(BULLET* bullet, bool fired);
void bullet_set_was_fired_and_sent(BULLET* bullet);
void bullet_reset_was_fired_and_sent(BULLET* bullet);
sfVector2f bullet_get_position(BULLET* bullet);
sfVector2f bullet_get_size(BULLET* bullet);
bool bullet_was_fired(BULLET* bullet);
bool bullet_was_fired_and_sent(BULLET* bullet);
bool bullet_check_borders(BULLET* bullet, LINKED_LIST* listOfWalls);

#endif //UDSP_TANKS_BULLET_H
