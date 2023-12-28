//
// Created by vrsp on 28.12.2023.
//

#include "bullet.h"

void bullet_create(BULLET* bullet) {
    bullet->speed = 15;
    bullet->diameter = 4;
    bullet->icon = sfRectangleShape_create();
    sfVector2f vec = {bullet->diameter, bullet->diameter};
    sfRectangleShape_setSize(bullet->icon, vec);
    bullet->fired = false;
    bullet->firedAndSent = false;
}

void bullet_destroy(BULLET* bullet) {
    sfRectangleShape_destroy(bullet->icon);
}

void bullet_render(BULLET* bullet, sfRenderWindow* window, LINKED_LIST* listOfWalls) {
    if (bullet_check_borders(listOfWalls) && bullet->fired) {
        bullet_move(bullet);
        sfRenderWindow_drawRectangleShape(window, bullet->icon, NULL);
    } else {
        bullet->fired = false;
        bullet->firedAndSent = false;
    }
}

void bullet_shot(BULLET* bullet, float xPosition, float yPosition, DIRECTION dir) {
    switch (dir) {
        case UP:
        case DOWN:
            sfVector2f vec = {xPosition - bullet->diameter / 2, yPosition};
            sfRectangleShape_setPosition(bullet->icon, vec);
            break;
        case LEFT:
        case RIGHT:
            sfVector2f vec = {xPosition, yPosition - bullet->diameter / 2};
            sfRectangleShape_setPosition(bullet->icon, vec);
            break;
    }

    bullet->fired = true;
    bullet->direction = direction;
}

void bullet_set_fired(BULLET* bullet, bool fired) {
    bullet->fired = fired;
}

void bullet_set_was_fired_and_sent(BULLET* bullet) {
    bullet->firedAndSent = true;
}

void bullet_reset_was_fired_and_sent(BULLET* bullet) {
    bullet->firedAndSent = false;
}

sfVector2f bullet_get_position(BULLET* bullet) {
    return sfRectangleShape_getPosition(bullet->icon);
}

sfVector2f bullet_get_size(BULLET* bullet) {
    return sfRectangleShape_getSize(bullet->icon);
}

bool bullet_was_fired(BULLET* bullet) {
    return bullet->fired;
}

bool bullet_was_fired_and_sent(BULLET* bullet) {
    return bullet->firedAndSent;
}

bool bullet_check_borders(BULLET* bullet, LINKED_LIST* listOfWalls) {
    bool canContinue = true;
    sfVector2f vec = sfRectangleShape_getPosition(bullet->icon);
    float xPosition = vec.x;
    float yPosition = vec.y;

    LINKED_LIST_ITERATOR iterator;
    ls_iterator_init(iterator, listOfWalls);
    while (ls_iterator_has_next(iterator)) {
        sfRectangleShape wall = (sfRectangleShape*)ls_iterator_move_next(iterator);
        if (sfFloatRect_intersects(sfRectangleShape_getGlobalBounds(wall), sfRectangleShape_getGlobalBounds(bullet->icon), NULL)) {
            canContinue = false;
        }
    }

    if (xPosition < 0 || xPosition > 800 || yPosition < 0 || yPosition > 800) {
        canContinue = false;
    }

    return canContinue;
}

void bullet_move(BULLET* bullet) {
    if (bullet->direction == UP){
        sfVector2f vec = {0.0f, -bullet->speed};
        sfRectangleShape_move(bullet->icon, vec);
    } else if (bullet->direction_ == DOWN) {
        sfVector2f vec = {0.0f, bullet->speed};
        sfRectangleShape_move(bullet->icon, vec);
    } else if (bullet->direction_ == LEFT) {
        sfVector2f vec = {-bullet->speed, 0.0f};
        sfRectangleShape_move(bullet->icon, vec);
    } else {
        sfVector2f vec = {bullet->speed, 0.0f};
        sfRectangleShape_move(bullet->icon, vec);
    }
}