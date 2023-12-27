//
// Created by mikulec on 12/27/23.
//

#ifndef SERVER_SIDE_MOVEMENT_H
#define SERVER_SIDE_MOVEMENT_H

typedef enum movement {
    UP,
    DOWN,
    LEFT,
    RIGHT
} DIRECTION;

typedef struct position {
    float xPosition;
    float  yPosition;
    DIRECTION direction;
} POSITION;

#endif //SERVER_SIDE_MOVEMENT_H
