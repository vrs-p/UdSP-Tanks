//
// Created by vrsp on 29.12.2023.
//

#include "show_score.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

void score_initialize_window(SHOWSCORE* shwScore) {
    sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT};
    shwScore->window = sfRenderWindow_create(mode, "UdSP-Tanks", sfClose, NULL);
    sfRenderWindow_setFramerateLimit(shwScore->window, 60);
    sfRenderWindow_setActive(shwScore->window, sfTrue);
}

void score_create(SHOWSCORE* shwScore, int playerScore, LINKED_LIST* otherTanks) {
    shwScore->font = sfFont_createFromFile("../font/consola.ttf");

    sfText_setFont(shwScore->textGame, shwScore->font);
    sfText_setCharacterSize(shwScore->textGame, 32);
    sfText_setFillColor(shwScore->textGame, sfWhite);
    sfText_setString(shwScore->textGame, "UdSP-Tanks");
    sfVector2f vec = {(SCREEN_WIDTH - sfText_getLocalBounds(shwScore->textGame).width) / 2, SCREEN_HEIGHT / 2 -
            sfText_getLocalBounds(shwScore->textGame).height * 11};
    sfText_setPosition(shwScore->textGame, vec);

    sfText_setFont(shwScore->textEndGame, shwScore->font);
    sfText_setCharacterSize(shwScore->textEndGame, 24);
    sfText_setFillColor(shwScore->textEndGame, sfWhite);
    sfText_setString(shwScore->textEndGame, "Game Over");
    vec = (sfVector2f){(SCREEN_WIDTH - sfText_getLocalBounds(shwScore->textEndGame).width) / 2, SCREEN_HEIGHT / 2 -
                      sfText_getLocalBounds(shwScore->textGame).height * 9};
    sfText_setPosition(shwScore->textEndGame, vec);

    sfText_setFont(shwScore->textYourScore, shwScore->font);
    sfText_setCharacterSize(shwScore->textYourScore, 32);
    sfText_setFillColor(shwScore->textYourScore, sfWhite);
    char* str = "Your score is: ";
    char* str2 = "";
    sprintf(str2, "%d", playerScore);
    strcat(str, str2);
//    strcat(str, playerScore); TODO: fix this !!
    sfText_setString(shwScore->textYourScore, str);
    vec = (sfVector2f){(SCREEN_WIDTH - sfText_getLocalBounds(shwScore->textYourScore).width) / 2, SCREEN_HEIGHT / 2};
    sfText_setPosition(shwScore->textYourScore, vec);

    sfText_setFont(shwScore->textOthersScore, shwScore->font);
    sfText_setCharacterSize(shwScore->textOthersScore, 32);
    sfText_setFillColor(shwScore->textOthersScore, sfWhite);
    char strOthersScore[50];
    LINKED_LIST_ITERATOR* iterator;
    ls_iterator_init(iterator, otherTanks);
    while (ls_iterator_has_next(iterator)) {
        TANK* tank = ls_iterator_move_next(iterator);
        strcat(strOthersScore, "Score of player ");
        strcat(strOthersScore, tank_get_player_name(tank));
        strcat(strOthersScore, " is: ");
        char* strScore = "";
        sprintf(strScore, "%d", tank_get_score(tank));
        strcat(strOthersScore, strScore);
        strcat(strOthersScore, "\n");
    }
    ls_iterator_destroy(iterator);

    sfText_setString(shwScore->textOthersScore, strOthersScore);
    vec = (sfVector2f){(SCREEN_WIDTH - sfText_getLocalBounds(shwScore->textOthersScore).width) / 2, SCREEN_HEIGHT / 2 +
            sfText_getLocalBounds(shwScore->textYourScore).height * 2};
    sfText_setPosition(shwScore->textOthersScore, vec);

    shwScore->showScore = true;
}

void score_destroy(SHOWSCORE* shwScore) {
    sfRenderWindow_destroy(shwScore->window);
    shwScore->window = NULL;
    sfFont_destroy(shwScore->font);
    shwScore->font = NULL;
    sfText_destroy(shwScore->textGame);
    shwScore->textGame = NULL;
    sfText_destroy(shwScore->textEndGame);
    shwScore->textEndGame = NULL;
    sfText_destroy(shwScore->textYourScore);
    shwScore->textYourScore = NULL;
    sfText_destroy(shwScore->textOthersScore);
    shwScore->textOthersScore = NULL;
}

void score_render(SHOWSCORE* shwScore) {
    score_initialize_window(shwScore);
    sfRenderStates states = sfRenderStates_default();

    sfRenderWindow_clear(shwScore->window, sfBlack);
    sfRenderWindow_drawText(shwScore->window, shwScore->textGame, &states);
    sfRenderWindow_drawText(shwScore->window, shwScore->textEndGame, &states);
    sfRenderWindow_drawText(shwScore->window, shwScore->textYourScore, &states);
    sfRenderWindow_drawText(shwScore->window, shwScore->textOthersScore, &states);
    sfRenderWindow_display(shwScore->window);

    while (shwScore->showScore) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(shwScore->window, &event)) {
            if (event.type == sfEvtClosed) {
                shwScore->showScore = false;
            } else if (event.type == sfEvtKeyPressed) {
                if (event.key.code == sfKeyQ || event.key.code == sfKeyEscape) {
                    shwScore->showScore = false;
                }
            }
        }
    }

    sfRenderWindow_setActive(shwScore->window, sfFalse);
    sfRenderWindow_close(shwScore->window);
}

#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT