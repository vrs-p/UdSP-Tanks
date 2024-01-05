//
// Created by vrsp on 28.12.2023.
//

#include "map.h"

static void create_wall(sfVector2f size, sfVector2f position, LINKED_LIST *list) {
    sfRectangleShape *tmpShape = sfRectangleShape_create();
    sfRectangleShape_setSize(tmpShape, size);
    sfRectangleShape_setPosition(tmpShape, position);
    ls_push(list, &tmpShape);
}

static sfVector2f generate_vector(int min, int max) {
    float xAxis = (float)(min + rand() / (RAND_MAX / (max - min + 1) + 1));
    float yAxis = (float)(min + rand() / (RAND_MAX / (max - min + 1) + 1));
    return (sfVector2f){xAxis, yAxis};
}

void map_create1(MAP *map) {
    sfVector2f sizes[] = {
            {100.0f, 100.0f},
            {150.0f, 25.0f},
            {10.0f, 80.0f},
            {30.0f, 30.0f},
            {20.0f, 130.0f},
            {45.0f, 10.0f},
            {45.0f, 10.0f},
            {10.0f, 45.0f},
            {10.0f, 45.0f},
            {10.0f, 45.0f},
            {10.0f, 45.0f},
            {45.0f, 10.0f},
            {45.0f, 10.0f}
    };

    sfVector2f positions[] = {
            {100.0f, 100.0f},
            {500.0f, 180.0f},
            {90.0f, 450.0f},
            {600.0f, 600.0f},
            {700.0f, 650.0f},
            {330.0f, 330.0f},
            {425.0f, 330.0f},
            {330.0f, 330.0f},
            {460.0f, 330.0f},
            {330.0f, 425.0f},
            {460.0f, 425.0f},
            {330.0f, 460.0f},
            {425.0f, 460.0f}
    };

    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        create_wall(sizes[i], positions[i], map->listOfWalls);
    }
}

void map_create2(MAP *map) {
    sfVector2f sizes[] = {
            {120.0f, 30.0f},
            {80.0f, 50.0f},
            {50.0f, 100.0f},
            {200.0f, 20.0f},
            {40.0f, 150.0f},
            {70.0f, 70.0f},
            {120.0f, 20.0f}
    };

    sfVector2f positions[] = {
            {50.0f, 50.0f},
            {200.0f, 100.0f},
            {400.0f, 200.0f},
            {150.0f, 350.0f},
            {500.0f, 400.0f},
            {300.0f, 500.0f},
            {100.0f, 600.0f}
    };

    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        create_wall(sizes[i], positions[i], map->listOfWalls);
    }
}

void map_create3(MAP *map) {
    sfVector2f sizes[] = {
            {80.0f, 80.0f},
            {120.0f, 20.0f},
            {60.0f, 150.0f},
            {180.0f, 40.0f},
            {50.0f, 100.0f},
            {100.0f, 30.0f},
            {90.0f, 90.0f}
    };

    sfVector2f positions[] = {
            {50.0f, 50.0f},
            {200.0f, 100.0f},
            {400.0f, 200.0f},
            {150.0f, 350.0f},
            {500.0f, 400.0f},
            {300.0f, 500.0f},
            {100.0f, 600.0f}
    };

    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
        create_wall(sizes[i], positions[i], map->listOfWalls);
    }
}

void map_create_random(MAP* map, unsigned int seed) {
    int numberOfObjects = 5;
    int minSize = 20, maxSize = 200;
    int minPosition = 50, maxPosition = 500;

    for (int i = 0; i < numberOfObjects; ++i) {
        srand(seed++);
        sfVector2f size = generate_vector(minSize, maxSize);
        srand(seed++);
        sfVector2f position = generate_vector(minPosition, maxPosition);
        create_wall(size, position, map->listOfWalls);
    }
}


void map_destroy_rectangle_shape(void *shape) {
    sfRectangleShape* rShape = *(sfRectangleShape**)shape;
    sfRectangleShape_destroy(rShape);
//    free(rShape);
}

void map_destroy(MAP *map) {
    ls_run_function(map->listOfWalls, map_destroy_rectangle_shape);
    ls_destroy(map->listOfWalls);
    map->listOfWalls = NULL;
}

LINKED_LIST *map_get_list_of_walls(MAP *map) {
    return map->listOfWalls;
}

void map_create(MAP *map, TYPE_OF_MAPS mapType, unsigned int seed) {
    map->listOfWalls = malloc(sizeof(LINKED_LIST));
    ls_create(map->listOfWalls, sizeof(sfRectangleShape *));

    switch (mapType) {
        case FIRST:
            map_create1(map);
            break;
        case SECOND:
            map_create2(map);
            break;
        case THIRD:
            map_create3(map);
            break;
        case RANDOM:
            map_create_random(map, seed);
            break;
    }
}
