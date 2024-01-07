//
// Created by vrsp on 29.12.2023.
//

#ifndef UDSP_TANKS_SHOW_SCORE_H
#define UDSP_TANKS_SHOW_SCORE_H

#include <string.h>
#include <SFML/Graphics.h>
#include "../../tank/tank.h"
#include "../../../common/linked_list/linked_list.h"

typedef struct showscore {
    bool showScore;

    sfRenderWindow* window;
    sfFont* font;
    sfText* textGame;
    sfText* textEndGame;
    sfText* textYourScore;
    sfText* textOthersScore;
//    char* stringOthersScore;
} SHOWSCORE;

void score_create(SHOWSCORE* shwScore, int playerScore, LINKED_LIST* otherTanks);
void score_destroy(SHOWSCORE* shwScore);

void score_render(SHOWSCORE* shwScore);

#endif //UDSP_TANKS_SHOW_SCORE_H