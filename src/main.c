#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "screen.h"
#include "game.h"

int main(void)
{
    Game* game = game_alloc(MAX_SNAKE_LENGTH);
    if (game == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for game\n");
        return EXIT_FAILURE;
    }

    screen_start();
    game_init(game);

    while (!game->quit) {
        game_render(game);
        usleep(GAME_SPEED);
        game_input(game);
        game_logic(game);
    }

    screen_end();
    game_free(game);
    printf("Score: %ld\n", game->snake_length);

    return EXIT_SUCCESS;
}
