#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 5

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

Game *init_game(Game *game) {
    game->player = WHITE;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Pawn *pawn = (Pawn *)malloc(sizeof(Pawn));
            game->board[i][j] = pawn;
        }
    }
    return game;
}

void init_pawns(Game *game) {
    srand(time(NULL));
    int n = 0, s = rand()%5;
    for (int i = 0; i < 2; i++) {
        Pawn *pawn = game->board[0][2+i];
        pawn->color = WHITE;
        if (n == s) pawn->type = SPY;
        else pawn->type = SCOUT;
        n++;
    }
    for (int i = 0; i < 2; i++) {
        Pawn *pawn = game->board[1+i][BOARD_SIZE-1];
        pawn->color = WHITE;
        if (n == s) pawn->type = SPY;
        else pawn->type = SCOUT;
        n++;
    }
    Pawn *pawn = game->board[1][3];
    pawn->color = WHITE;
    if (n == s) pawn->type = SPY;
    else pawn->type = SCOUT;
    n = 0;
    s = rand()%5;
    for (int i = 0; i < 2; i++) {
        Pawn *pawn = game->board[BOARD_SIZE-1][1+i];
        pawn->color = BLACK;
        if (n == s) pawn->type = SPY;
        else pawn->type = SCOUT;
        n++;
    }
    for (int i = 0; i < 2; i++) {
        Pawn *pawn = game->board[2+i][0];
        pawn->color = BLACK;
        if (n == s) pawn->type = SPY;
        else pawn->type = SCOUT;
        n++;
    }
    pawn = game->board[3][1];
    pawn->color = BLACK;
    if (n == s) pawn->type = SPY;
    else pawn->type = SCOUT;
}

void move_pawn(Game *game, Movement *movement) {
    Pawn *pawn = game->board[movement->start.x][movement->start.y];
    game->board[movement->end.x][movement->end.y] = pawn;
    Pawn *new_pawn = (Pawn *)malloc(sizeof(Pawn));
    game->board[movement->start.x][movement->start.y] = new_pawn;
}

void print_board(Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("|");
            Pawn *pawn = game->board[i][j];
            if (pawn->color == WHITE) printf("W");
            else if (pawn->color == BLACK) printf("B");
            else if ((i == 0 && j == 4) || (i == 4 && j == 0)) printf("X");
            else printf(" ");
        }
        printf("|\n");
    }
    printf("\n");
}

void free_board(Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            free(game->board[i][j]);
        }
    }
}

int is_valid_move(Game *game, Movement *movement) {
    int x = movement->start.x;
    int y = movement->start.y;
    int x_ = movement->end.x;
    int y_ = movement->end.y;
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || x_ < 0 || x_ >= BOARD_SIZE || y_ < 0 || y_ >= BOARD_SIZE) return 0;
    Pawn *start = game->board[x][y];
    Pawn *end = game->board[x_][y_];
    if (start->color != game->player) return 0;
    else if (end->color == BLACK || end->color == WHITE) return 0;
    else return 1;
}

Movement *get_movement() {
    Movement *movement = (Movement *)malloc(sizeof(Movement));
    printf("Enter the starting position (x1, y1) --> (x2, y2): ");
    scanf("(%d, %d) --> (%d, %d)", &movement->start.x, &movement->start.y, &movement->end.x, &movement->end.y);
    return movement;
}

void play(Game *game) {
    int movement_valid = 0;
    while (!movement_valid) {
        Movement *movement = get_movement();
        movement_valid = is_valid_move(game, movement);
        if (!movement_valid) printf("Invalid move.\n");
        else move_pawn(game, movement);
    }
}

int main(char argc, char *argv[]) {
    Game *game = (Game *)malloc(sizeof(Game));
    init_game(game);
    init_pawns(game);
    print_board(game);

    int playing = 1;
    while (playing) {
        play(game);
        print_board(game);
        if (game->player == WHITE) game->player = BLACK;
        else game->player = WHITE;
    }

    system("pause");

    free_board(game);
    free(game);
    return 0;
}