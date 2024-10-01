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
    free(game->board[movement->end.y][movement->end.x]);
    Pawn *pawn = game->board[movement->start.y][movement->start.x];
    game->board[movement->end.y][movement->end.x] = pawn;
    Pawn *new_pawn = (Pawn *)malloc(sizeof(Pawn));
    game->board[movement->start.y][movement->start.x] = new_pawn;
}

void print_board(Game *game) {
    printf("\n");
    printf("  0 1 2 3 4\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("|");
            Pawn *pawn = game->board[i][j];
            // if (pawn->color == WHITE) printf("W");
            // else if (pawn->color == BLACK) printf("B");
            if (pawn->type == SPY) printf("S");
            else if (pawn->type == SCOUT) printf("C");
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
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || x_ < 0 || x_ >= BOARD_SIZE || y_ < 0 || y_ >= BOARD_SIZE) return 1; //out of bounds or not a movement
    Pawn *start = game->board[y][x];
    Pawn *end = game->board[y_][x_];
    if ((start->color == BLACK && (x_ == 0 && y_ == 4)) || (start->color == WHITE && (x_ == 4 && y_ == 0))) return 2; //cannot move to self's base
    else if (start->color != BLACK && start->color != WHITE) return 3; //no pawn at this position
    else if (start->color != game->player) return 4; //cannot move opponent's pawn
    else if (end->color == BLACK || end->color == WHITE) return 5; //cannot move to a taken spot
    // pawn can only move like a queen in chess
    if (x != x_ && y != y_) {
        if (abs(x-x_) != abs(y-y_)) return 1; //not a valid movement
    }
    return 0; //valid movement
}

Movement *get_movement() { //get the movement from the player
    Movement *movement = (Movement *)malloc(sizeof(Movement));
    printf("Enter movement (x1, y1) --> (x2, y2):\n");
    fflush(stdin);
    scanf("(%d, %d) --> (%d, %d)", &movement->start.x, &movement->start.y, &movement->end.x, &movement->end.y);
    printf("\n");
    return movement;
}

int check_adjacent(Game *game, int x, int y) { //check if there's a pawn adjacent to the given position
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x+j >= 0 && x+j < BOARD_SIZE && y+i >= 0 && y+i < BOARD_SIZE) {
                Pawn *pawn = game->board[y+i][x+j];
                if (pawn->color == game->player) return 1;
            }
        }
    }
    return 0;
}

int question_pawn(Game *game) {
    int x, y;
    int valid_question = 0;
    Pawn *pawn;

    while (!valid_question) { //get the position of the pawn to question
        printf("Which pawn do you want to question ? (x, y)\n");
        fflush(stdin);
        scanf("(%d, %d)", &x, &y);
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) printf("Out of bounds.\n");
        pawn = game->board[y][x];
        if (pawn->color == game->player) printf("Cannot question your own pawn.\n");
        else if (pawn->color != BLACK && pawn->color != WHITE) printf("There is no pawn at this position.\n");
        else valid_question = 1;
    }

    if (check_adjacent(game, x, y)) { //if there's no pawn adjacent to the questioned pawn, the player can't question it
        int x_, y_;
        int valid_interrogator = 0;
        Pawn *interrogator;
        while (!valid_interrogator) { //get the position of the interrogator
            printf("Which of your pawns is the interrogator ? (x, y)\n");
            fflush(stdin);
            scanf("(%d, %d)", &x_, &y_);
            interrogator = game->board[y_][x_];
            if (interrogator->color != game->player) printf("Cannot question a pawn that is not yours.\n");
            else if (interrogator->color != BLACK && interrogator->color != WHITE) printf("There is no pawn at this position.\n");
            else valid_interrogator = 1;
        }

        if (pawn->type == SPY) { //if the questioned pawn is a spy
            printf("You found the spy !\n");
            if (interrogator->color == WHITE) printf("White wins !\n");
            else printf("Black wins !\n");
            return 1; //end of game
        } else { //if the questioned pawn is a scout
            printf("You did not find the spy and your pawn also got removed.\n");
            if (interrogator->type == SPY) { //if the interrogator is a spy
                printf("The interrogator was a spy.\n");
                if (interrogator->color == WHITE) printf("Black wins !\n");
                else printf("White wins !\n");
                return 1; //end of game
            }
            game->board[y_][x_] = (Pawn *)malloc(sizeof(Pawn));
            return 2; //player failed to find the spy
        }

    } else {
        printf("Cannot question a pawn that is not adjacent to one of your pawns.\n");
        return 0; //player can try again
    }
}

int check_win(Game *game) {
    if (game->board[0][4]->color == WHITE) return 1;
    else if (game->board[4][0]->color == BLACK) return 2;
}

int main(char argc, char *argv[]) {
    Game *game = (Game *)malloc(sizeof(Game));
    init_game(game);
    init_pawns(game);

    int playing = 1;
    while (playing) {
        print_board(game);
        int movement_valid = 0;
        if (game->player == WHITE) printf("White's turn.\n");
        else printf("Black's turn.\n");

        printf("Do you want to:\n");
        printf("    1. Move a pawn\n");
        printf("    2. Question a pawn\n");
        fflush(stdin);
        int option;
        scanf("%d", &option);

        if (option == 1) { //move a pawn
            movement_valid = 0;
            Movement *movement = get_movement(); // gets the movement from the player
            movement_valid = is_valid_move(game, movement);
            if (movement_valid != 0) {
                printf("Invalid movement. ");
                if (movement_valid == 1) printf("Try again.\n");
                else if (movement_valid == 2) printf("Cannot move to self's base.\n");
                else if (movement_valid == 3) printf("No pawn at this position.\n");
                else if (movement_valid == 4) printf("Cannot move opponent's pawn.\n");
                else if (movement_valid == 5) printf("Cannot move to a taken spot.\n");
            } else {
                move_pawn(game, movement);
                if (game->player == WHITE) game->player = BLACK;
                else game->player = WHITE;
                if (check_win(game) == 1) {
                    printf("White wins !\n");
                    playing = 0;
                } else if (check_win(game) == 2) {
                    printf("Black wins !\n");
                    playing = 0;
                }
            }
            free(movement);

        } else if (option == 2) { //question a pawn
            int found = 0;
            while (found == 0) found = question_pawn(game);
            if (found == 1) playing = 0;
            else if (found == 2) {
                if (game->player == WHITE) game->player = BLACK;
                else game->player = WHITE;
            }
        }
    }

    free_board(game);
    free(game);

    system("pause");
    return 0;
}