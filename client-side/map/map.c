//
// Created by vrsp on 28.12.2023.
//

#include "map.h"

void map_create(MAP *map) {
    map->listOfWalls = malloc(sizeof(LINKED_LIST));
    ls_create(map->listOfWalls, sizeof(sfRectangleShape *));

    sfRectangleShape *tmpShape = sfRectangleShape_create();
    sfVector2f vec = {100.0f, 100.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){100.0f, 100.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){150.0f, 25.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){500.0f, 180.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){10.0f, 80.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){90.0f, 450.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){30.0f, 30.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){600.0f, 600.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){20.0f, 130.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){700.0f, 650.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){45.0f, 10.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){330.0f, 330.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){45.0f, 10.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){425.0f, 330.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){10.0f, 45.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){330.0f, 330.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){10.0f, 45.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){460.0f, 330.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){10.0f, 45.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){330.0f, 425.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){10.0f, 45.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){460.0f, 425.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){45.0f, 10.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){330.0f, 460.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);

    tmpShape = sfRectangleShape_create();
    vec = (sfVector2f){45.0f, 10.0f};
    sfRectangleShape_setSize(tmpShape, vec);
    vec = (sfVector2f){425.0f, 460.0f};
    sfRectangleShape_setPosition(tmpShape, vec);
    ls_push(map->listOfWalls, &tmpShape);
}

void map_destroy_rectangle_shape(void *shape) {
    sfRectangleShape_destroy(shape);
}

void map_destroy(MAP *map) {
    ls_run_function(map->listOfWalls, map_destroy_rectangle_shape);
    ls_destroy(map->listOfWalls);
    map->listOfWalls = NULL;
}

LINKED_LIST *map_get_list_of_walls(MAP *map) {
    return map->listOfWalls;
}


