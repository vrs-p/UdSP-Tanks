//
// Created by vrsp on 28.12.2023.
//

#include "map.h"

void map_create(MAP* map) {
    map->listOfWalls = malloc(sizeof(LINKED_LIST));
    ls_create(map->listOfWalls, sizeof(sfRectangleShape*));

    sfRectangleShape* tmpShape = sfRectangleShape_create();
    sfVector2f vec = {100.0, 100.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {100.0, 100.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {150.0, 25.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {500.0, 180.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {10.0, 80.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {90.0, 450.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {30.0, 30.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {600.0, 600.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {20.0, 130.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {700.0, 650.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {45.0, 10.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {330.0, 330.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {45.0, 10.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {425.0, 330.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {10.0, 45.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {330.0, 330.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {10.0, 45.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {460.0, 330.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {10.0, 45.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {330.0, 425.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {10.0, 45.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {460.0, 425.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {45.0, 10.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {330.0, 460.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);

    tmpShape = sfRectangleShape_create();
    sfVector2f vec = {45.0, 10.0};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = {425.0, 460.0};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, tmpShape);
}

void map_destroy(MAP* map) {
    ls_run_function(map->listOfWalls, sfRectangleShape_destroy);
    ls_destroy(map->listOfWalls);
    map->listOfWalls = NULL;
}

LINKED_LIST* map_get_list_of_walls(MAP* map) {
    return map->listOfWalls;
}