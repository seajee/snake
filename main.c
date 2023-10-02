#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>

#define VEC2(X, Y) (Vec2) { .x = (X), .y = (Y) }

#define VEC2_DOWN  VEC2( 0,  1)
#define VEC2_UP    VEC2( 0, -1)
#define VEC2_LEFT  VEC2(-1,  0)
#define VEC2_RIGTH VEC2( 1,  0)

#define CHAR_SNAKE_HEAD '@'
#define CHAR_SNAKE_BODY '0'
#define CHAR_FOOD '$'
#define CHAR_BORDER '#'

#define GAME_SPEED 128 * 1000
#define MAX_SNAKE_LENGTH 4096

typedef struct
{
    int32_t x;
    int32_t y;
} Vec2;

typedef struct
{
    Vec2* snake;
    Vec2 snake_dir;
    size_t snake_length;
    Vec2 food;

    Vec2 window_size;
    bool quit;
} Game;

Vec2 vec2_add(Vec2 a, Vec2 b);
bool vec2_equals(Vec2 a, Vec2 b);

Game* game_alloc(int32_t max_snake_size);
void game_free(Game* game);
void game_init(Game* game);
void game_render(Game* game);
void game_input(Game* game);
void game_logic(Game* game);

int32_t rand_range(int32_t min, int32_t max);

Vec2 vec2_add(Vec2 a, Vec2 b)
{
    return VEC2(a.x + b.x, a.y + b.y);
}

bool vec2_equals(Vec2 a, Vec2 b)
{
    return (a.x == b.x && a.y == b.y);
}

Game* game_alloc(int32_t max_snake_size)
{
    Game* game = (Game*) malloc(sizeof(Game));
    if (game == NULL) {
        return NULL;
    }

    game->snake = malloc(sizeof(*game->snake) * max_snake_size);
    if (game->snake == NULL) {
        return NULL;
    }

    return game;
}

void game_free(Game* game)
{
    free(game->snake);
    free(game);
}

void game_init(Game* game)
{
    getmaxyx(stdscr, game->window_size.y, game->window_size.x);

    game->snake[0] = VEC2(game->window_size.x / 2, game->window_size.y / 2);
    game->snake_dir = VEC2(1, 0);
    game->snake_length = 1;
    game->food = VEC2(
            rand_range(1, game->window_size.x - 2),
            rand_range(1, game->window_size.y - 2));
    game->quit = false;
}

void game_render(Game* game)
{
    clear();

    Vec2* snake = game->snake;
    Vec2 window_size = game->window_size;

    // Border
    for (int32_t x = 0; x < window_size.x; ++x) {
        move(0, x);
        addch(CHAR_BORDER);
        move(window_size.y - 1, x);
        addch(CHAR_BORDER);
    }
    for (int32_t y = 1; y < window_size.y - 1; ++y) {
        move(y, 0);
        addch(CHAR_BORDER);
        move(y, window_size.x - 1);
        addch(CHAR_BORDER);
    }

    // Snake body
    for (size_t i = 1; i < game->snake_length; ++i) {
        move(snake[i].y, snake[i].x);
        addch(CHAR_SNAKE_BODY);
    }

    // Snake head
    move(snake[0].y, snake[0].x);
    addch(CHAR_SNAKE_HEAD);

    // Food
    move(game->food.y, game->food.x);
    addch(CHAR_FOOD);

    refresh();
}

void game_input(Game* game)
{
    int32_t input = getch();

    Vec2* dir = &game->snake_dir;

    switch (input) {
        case KEY_DOWN:
            if (!vec2_equals(*dir, VEC2_UP)) {
                *dir = VEC2_DOWN;
            }
            break;

        case KEY_UP:
            if (!vec2_equals(*dir, VEC2_DOWN)) {
                *dir = VEC2_UP;
            }
            break;

        case KEY_LEFT:
            if (!vec2_equals(*dir, VEC2_RIGTH)) {
                *dir = VEC2_LEFT;
            }
            break;

        case KEY_RIGHT:
            if (!vec2_equals(*dir, VEC2_LEFT)) {
                *dir = VEC2_RIGTH;
            }
            break;

        default:
            break;
    }
}

void game_logic(Game* game)
{
    Vec2* snake = game->snake;

    // Handle window resize
    getmaxyx(stdscr, game->window_size.y, game->window_size.x);
    Vec2 window_size = game->window_size;

    // Next position
    Vec2 next = vec2_add(snake[0], game->snake_dir);
    if (next.x >= window_size.x - 1) {
        next.x = 1;
    }
    if (next.x <= 0) {
        next.x = window_size.x - 2;
    }
    if (next.y >= window_size.y - 1) {
        next.y = 1;
    }
    if (next.y <= 0) {
        next.y = window_size.y - 2;
    }

    // Food
    move(next.y, next.x);
    if (inch() == CHAR_FOOD) {
        ++game->snake_length;

        bool food_inside_snake = false;

        do {
            game->food = VEC2(
                    rand_range(1, window_size.x - 2),
                    rand_range(1, window_size.y - 2));

            for (size_t i = 0; i < game->snake_length; ++i) {
                if (vec2_equals(game->food, game->snake[i])) {
                    food_inside_snake = true;
                    break;
                }
            }
        } while (food_inside_snake);
    }

    // Movement
    Vec2 prev = snake[0];
    snake[0] = next;
    for (size_t i = 1; i < game->snake_length; ++i) {
        Vec2 curr = snake[i];
        snake[i] = prev;
        prev = curr;
    }

    // Collision
    for (size_t i = 4; i < game->snake_length; ++i) {
        if (vec2_equals(snake[0], snake[i])) {
            game->quit = true;
            return;
        }
    }
}

int32_t rand_range(int32_t min, int32_t max)
{
    return (rand() % (max - min)) + min;
}

int main(void)
{
    srand(time(NULL));

    Game* game = game_alloc(MAX_SNAKE_LENGTH);
    if (game == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for game\n");
        return EXIT_FAILURE;
    }

    initscr();
    noecho();
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);

    game_init(game);

    while (!game->quit) {
        game_render(game);
        usleep(GAME_SPEED);
        game_input(game);
        game_logic(game);
    }

    endwin();
    game_free(game);
    printf("Score: %ld\n", game->snake_length);

    return EXIT_SUCCESS;
}
