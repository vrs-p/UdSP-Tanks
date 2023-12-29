//
// Created by vrsp on 28.12.2023.
//

#ifndef UDSP_TANKS_MAP_H
#define UDSP_TANKS_MAP_H

#include <SFML/Graphics.h>
#include "../../common/linked_list/linked_list.h"

typedef struct map {
    LINKED_LIST* listOfWalls;
} MAP;

void map_create(MAP* map);
void map_destroy(MAP* map);
void map_destroy_rectangle_shape(void* shape);

LINKED_LIST* map_get_list_of_walls(MAP* map);

#endif //UDSP_TANKS_MAP_H
