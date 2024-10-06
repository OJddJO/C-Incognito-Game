#ifndef INCO_H
#define INCO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define BOARD_SIZE 5
#define NB_PAWNS (BOARD_SIZE-1)*(BOARD_SIZE-2)/2 - 1
#define WIN_SIZE 500

typedef enum _color {WHITE, BLACK} Color;
typedef enum _type {SPY, SCOUT} Type;

typedef struct _pawn {
    Color color;
    Type type;
} Pawn;

typedef struct _game {
    Pawn * board[BOARD_SIZE][BOARD_SIZE];
    Color player;
} Game;

typedef struct {
    int x, y;
} Case, Direction;

typedef struct _movement {
    Case start, end;
} Movement;

Game *init_game(Game *game);
int ediv(int a, int b);
void init_pawns(Game *game);
void move_pawn(Game *game, Movement *movement, bool save, char *save_file);
void free_board(Game *game);
int is_valid_move(Game *game, Movement *movement);
bool pawn_adjacent(Game *game, int x, int y);
int question_pawn(Game *game, int x, int y, int x_, int y_);
int check_win(Game *game, bool save, char *save_file);

void init_save(char *save_file, Game *game);
void save_move(char *path, Movement *movement);
void save_question(char *save_file, int x, int y, int x_, int y_);
void save_win(char *save_file, int win);
void eval_question(Game *game, int x, int y, int x_, int y_, bool save, char *save_file);
void replace_spies(Game *game, int x, int y, int x_, int y_);
bool read_save(FILE *load_file, Game *game, bool save, char *save_file);

void cmd_game(bool save, char *save_file, bool load, FILE *load_file);
void print_board(Game *game);
Movement *cmd_get_movement(Game *game);
int cmd_question_pawn(Game *game, bool save, char *save_file);

void graphical_game(bool render_image, bool save, char *save_file, bool load, FILE *load_file);
// void init_window(SDL_Window *window, SDL_Renderer *renderer);
void draw_board(SDL_Renderer *renderer, TTF_Font *font, bool render_image, Game *game);
Case *handle_click(Game *game, int x, int y);
void highlight_pawn(SDL_Renderer *renderer, Case *selected);
int eval_intention(Game *game, Case *pos);

#endif // BRUH_H