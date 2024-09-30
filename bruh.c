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
        if (n == s) {
            pawn->type = SPY;
        } else {
            pawn->type = SCOUT;
        }
        n++;
    }
    for (int i = 0; i < 2; i++) {
        Pawn *pawn = game->board[1+i][BOARD_SIZE-1];
        pawn->color = WHITE;
        if (n == s) {
            pawn->type = SPY;
        } else {
            pawn->type = SCOUT;
        }
        n++;
    }
    Pawn *pawn = game->board[1][3];
    pawn->color = WHITE;
    if (n == s) {
        pawn->type = SPY;
    } else {
        pawn->type = SCOUT;
    }
    n = 0;
    s = rand()%5;
    for (int i = 0; i < 2; i++) {
        Pawn *pawn = game->board[BOARD_SIZE-1][1+i];
        pawn->color = BLACK;
        if (n == s) {
            pawn->type = SPY;
        } else {
            pawn->type = SCOUT;
        }
        n++;
    }
    for (int i = 0; i < 2; i++) {
        Pawn *pawn = game->board[2+i][0];
        pawn->color = BLACK;
        if (n == s) {
            pawn->type = SPY;
        } else {
            pawn->type = SCOUT;
        }
        n++;
    }
    pawn = game->board[3][1];
    pawn->color = BLACK;
    if (n == s) {
        pawn->type = SPY;
    } else {
        pawn->type = SCOUT;
    }
}

void print_board(Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("|");
            if ((i == 0 && j == 4) || (i == 4 && j == 0)) {
                printf("X");
            } else {
                Pawn *pawn = game->board[i][j];
                if (pawn->color == WHITE) {
                    printf("W");
                } else if (pawn->color == BLACK) {
                    printf("B");
                } else {
                    printf(" ");
                }
            }
        }
        printf("|\n");
    }
}


int main(char argc, char *argv[]) {
    Game *game = (Game*)malloc(sizeof(Game));
    init_game(game);
    init_pawns(game);
    print_board(game);
    system("pause");
    return 0;
}