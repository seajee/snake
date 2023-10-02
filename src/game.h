#ifndef _GAME_H_
#define _GAME_H_

#include <stddef.h>

#include "vec.h"

#define CHAR_SNAKE_HEAD '@'
#define CHAR_SNAKE_BODY '0'
#define CHAR_FOOD '$'
#define CHAR_BORDER '#'

#define GAME_SPEED 128 * 1000
#define MAX_SNAKE_LENGTH 4096

typedef struct
{
    Vec2* snake;
    Vec2 snake_dir;
    size_t snake_length;
    Vec2 food;

    Vec2 window_size;
    bool quit;
} Game;

Game* game_alloc(int32_t max_snake_size);
void game_free(Game* game);
void game_init(Game* game);
void game_render(Game* game);
void game_input(Game* game);
void game_logic(Game* game);

#endif // _GAME_H_
