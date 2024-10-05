#include "inco.h"

/*Main function*/
int main(int argc, char *argv[]) {
    bool graphical = false, save = false, load = false;
    char *save_file;
    FILE *load_file;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            graphical = false;
        }
        if (strcmp(argv[i], "-g") == 0) {
            graphical = true;
        }
        if (strcmp(argv[i], "-s") == 0) {
            printf("Saving game to '%s'\n", argv[i+1]);
            save = true;
            save_file = argv[i+1];
            FILE *file = fopen(save_file, "w");
            if (file == NULL) {
                printf("Error: could not open file '%s'\n", argv[i+1]);
                return 1;
            }
            i++;
        }
        if (strcmp(argv[i], "-c") == 0) {
            printf("Loading game from '%s'\n", argv[i+1]);
            load = true;
            load_file = fopen(argv[i+1], "r");
            if (load_file == NULL) {
                printf("Error: could not open file '%s'\n", argv[i+1]);
                return 1;
            }
            i++;
        }
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [-a | -g] [-s save_file] [-c load_file]\n", argv[0]);
            printf("Options:\n");
            printf("  -h, --help: Display this help message\n");
            printf("  -a: ASCII mode (default)\n");
            printf("  -g: Graphical mode\n");
            printf("  -s save_file: Save the game to save_file\n");
            printf("  -c load_file: Load the game from load_file\n");
            return 0;
        }
    }

    if (graphical) {
        printf("Graphical mode not implemented yet.\n");
    } else {
        cmd_game(save, save_file, load, load_file);
    }

    return 0;
}

/**
 * \brief Inits the game board
 * \param game: the game struct to initialize
 * \return initialized game struct
 */
Game *init_game(Game *game) {
    game->player = WHITE;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Pawn *pawn = (Pawn *)malloc(sizeof(Pawn));
            game->board[i][j] = NULL;
        }
    }
    return game;
}

/**
 * \brief Euclidean division of two integers
 * \param a: the dividend
 * \param b: the divisor
 * \return the result of the division
 */
int ediv(int a, int b) {
    return (a-(a%b))/b;
}

/**
 * \brief Initializes the pawns on the board
 * \param game: the game to initialize
 */
void init_pawns(Game *game) {
    srand(time(NULL));
    int r = BOARD_SIZE-3; //number of pawns in a row
    int c = 0; //column
    int n = 0; //number of pawns placed
    int s = rand()%NB_PAWNS; //spy position
    for (int i = 0; i < BOARD_SIZE-2; i++) {
        for (int j = 0; j < r; j++) {
            Pawn *pawn = (Pawn *)malloc(sizeof(Pawn));
            pawn->color = WHITE;
            if (i == s) pawn->type = SPY;
            else pawn->type = SCOUT;
            game->board[c][2+c+j] = pawn;
            if (++n == NB_PAWNS) break;
        }
        c++;
        if (c > 1) r = BOARD_SIZE-3 - (c-1);
        else r = BOARD_SIZE-3;
    }
    // print_board(game); //debug
    r = BOARD_SIZE-3;
    c = 0;
    n = 0;
    s = rand()%NB_PAWNS;
    for (int i = 0; i < BOARD_SIZE-2; i++) {
        for (int j = 0; j < r; j++) {
            Pawn *pawn = (Pawn *)malloc(sizeof(Pawn));
            pawn->color = BLACK;
            if (i == s) pawn->type = SPY;
            else pawn->type = SCOUT;
            game->board[BOARD_SIZE-1-c][BOARD_SIZE-3-c-j] = pawn;
            if (++n == NB_PAWNS) break;
        }
        c++;
        if (c > 1) r = BOARD_SIZE-3 - (c-1);
        else r = BOARD_SIZE-3;
    }
    // print_board(game); //debug
}

/**
 * \brief Moves a pawn on the board
 * \param game: the game where the movement is done
 * \param movement: the movement to do
 * \param save: whether to save the movement
 * \param save_file: the file where to save the movement
 */
void move_pawn(Game *game, Movement *movement, bool save, char *save_file) {
    Pawn *pawn = game->board[movement->start.y][movement->start.x];
    game->board[movement->end.y][movement->end.x] = pawn;
    game->board[movement->start.y][movement->start.x] = NULL;
    if (save) save_move(save_file, movement);
}

/**
 * \brief Prints the board
 * \param game: the game to print
 */
void print_board(Game *game) {
    printf("\n ");
    for (int i = 0; i < BOARD_SIZE; i++) { //print column numbers
        printf(" %d", i);
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d", i); //print row number
        for (int j = 0; j < BOARD_SIZE; j++) { //print board
            printf("|");
            if (game->board[i][j] == NULL) {
                if (i == 0 && j == BOARD_SIZE-1) printf("\033[30;107mX\033[0m"); 
                else if (i == BOARD_SIZE-1 && j == 0) printf("\033[100mX\033[0m");
                else printf(" ");
            }
            else {
                Pawn *pawn = game->board[i][j];
                if (pawn->color == WHITE) printf("\033[30;107mW\033[0m");
                else if (pawn->color == BLACK) printf("\033[100mB\033[0m");
                // if (pawn->type == SPY) printf("S");
                // else if (pawn->type == SCOUT) printf("C");
            }
        }
        printf("|\n");
    }
    printf("\n");
}

/**
 * \brief Frees the game board
 * \param game: the game to free
 */
void free_board(Game *game) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] != NULL) free(game->board[i][j]);
        }
    }
    free(game);
}

/**
 * \brief Checks if a movement is valid
 * \param game: the game where the movement is done
 * \param movement: the movement to check
 * \return 0 if the movement is valid,
 *         1 if the movement is invalid,
 *         2 if the movement is to the player's base,
 *         3 if there is no pawn at the start position,
 *         4 if the pawn is not the player's,
 *         5 if the end position is taken
 */
int is_valid_move(Game *game, Movement *movement) {
    int x = movement->start.x;
    int y = movement->start.y;
    int x_ = movement->end.x;
    int y_ = movement->end.y;
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE || x_ < 0 || x_ >= BOARD_SIZE || y_ < 0 || y_ >= BOARD_SIZE) return 1; //out of bounds or not a movement
    Pawn *start = game->board[y][x];
    Pawn *end = game->board[y_][x_];
    if (start == NULL) return 3; //no pawn at this position
    else if ((start->color == BLACK && (x_ == 0 && y_ == 4)) || (start->color == WHITE && (x_ == 4 && y_ == 0))) return 2; //cannot move to self's base
    else if (start->color != game->player) return 4; //cannot move opponent's pawn
    else if (end != NULL) return 5; //cannot move to a taken spot
    // pawn can only move like a queen in chess
    if (x != x_ && y != y_) {
        if (abs(x-x_) != abs(y-y_)) return 1; //not a valid movement
    }
    return 0; //valid movement
}

/**
 * \brief Gets a movement from the player
 * \param game: the game where the movement is done
 * \return the movement
 */
Movement *get_movement(Game *game) { //get the movement from the player
    int movement_valid = 1;
    Movement *movement = (Movement *)malloc(sizeof(Movement));
    while (movement_valid != 0) {
        printf("Enter movement (x1, y1) --> (x2, y2):\n");
        fflush(stdin);
        // scanf("(%d, %d) --> (%d, %d)", &movement->start.x, &movement->start.y, &movement->end.x, &movement->end.y);
        char tmp[100];
        fgets(tmp, 100, stdin);
        sscanf(tmp, "(%d, %d) --> (%d, %d)", &movement->start.x, &movement->start.y, &movement->end.x, &movement->end.y);
        printf("\n");
        movement_valid = is_valid_move(game, movement);
        if (movement_valid != 0) {
            printf("Invalid movement. ");
            if (movement_valid == 1) printf("Try again.\n");
            else if (movement_valid == 2) printf("Cannot move to self's base.\n");
            else if (movement_valid == 3) printf("No pawn at this position.\n");
            else if (movement_valid == 4) printf("Cannot move opponent's pawn.\n");
            else if (movement_valid == 5) printf("Cannot move to a taken spot.\n");
        }
    }
    return movement;
}

/**
 * \brief Checks if there is a pawn adjacent to the given position
 * \param game: the game where the pawn is checked
 * \param x: the x position of the pawn
 * \param y: the y position of the pawn
 * \return true if there is a pawn adjacent to the given position,
 *         false otherwise
 */
bool check_adjacent(Game *game, int x, int y) { //check if there's a pawn adjacent to the given position
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x+j >= 0 && x+j < BOARD_SIZE && y+i >= 0 && y+i < BOARD_SIZE) {
                Pawn *pawn = game->board[y+i][x+j];
                if (pawn != NULL) if (pawn->color == game->player) return true;
            }
        }
    }
    return false;
}

/**
 * \brief Questions a pawn
 * \param game: the game where the question is done
 * \param save: whether to save the question
 * \param save_file: the file where to save the question
 * \return 0 if the player can try again,
 *         1 if the player won,
 *         2 if the player failed to find the spy
 */
int question_pawn(Game *game, bool save, char *save_file) { //question a pawn
    int x, y;
    int valid_question = 0;
    Pawn *pawn;

    while (!valid_question) { //get the position of the pawn to question
        printf("Which pawn do you want to question ? (x, y)\n");
        fflush(stdin);
        // scanf("(%d, %d)", &x, &y);
        char tmp[100];
        fgets(tmp, 100, stdin);
        sscanf(tmp, "(%d, %d)", &x, &y);
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) printf("Out of bounds.\n");
        pawn = game->board[y][x];
        if (pawn == NULL) printf("There is no pawn at this position.\n");
        else if (pawn->color == game->player) printf("Cannot question your own pawn.\n");
        else valid_question = 1;
    }

    if (check_adjacent(game, x, y)) { //if there's no pawn adjacent to the questioned pawn, the player can't question it
        int x_, y_;
        int valid_interrogator = 0;
        Pawn *interrogator;
        while (!valid_interrogator) { //get the position of the interrogator
            printf("Which of your pawns is the interrogator ? (x, y)\n");
            fflush(stdin);
            // scanf("(%d, %d)", &x_, &y_);
            char tmp[100];
            fgets(tmp, 100, stdin);
            sscanf(tmp, "(%d, %d)", &x_, &y_);
            interrogator = game->board[y_][x_];
            if (interrogator == NULL) printf("There is no pawn at this position.\n");
            else if (interrogator->color != game->player) printf("Cannot question a pawn that is not yours.\n");
            else if (x-x_ > 1 || x-x_ < -1 || y-y_ > 1 || y-y_ < -1) printf("The interrogator must be adjacent to the questioned pawn.\n");
            else valid_interrogator = 1;
        }

        if (save) save_question(save_file, x, y, x_, y_);

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
            free(game->board[y_][x_]);
            game->board[y_][x_] = NULL;
            return 2; //player failed to find the spy
        }

    } else {
        printf("Cannot question a pawn that is not adjacent to one of your pawns.\n");
        return 0; //player can try again
    }
}

/********************************************************
 * The following functions are used for load/save files *
 ********************************************************/

/**
 * \brief Checks if a player won by reaching the opponent's base
 * \param game: the game where the win is checked
 * \param save: whether to save the win
 * \param save_file: the file where to save the win
 * \return 0 if no player won,
 *         1 if black won,
 *         2 if white won
 */
int *check_win(Game *game, bool save, char *save_file) { //check if a player won by reaching the opponent's base
    int *win = (int *)malloc(sizeof(int)); 
    Pawn *black_base = game->board[4][0];
    Pawn *white_base = game->board[0][4];
    if (white_base != NULL) {if (white_base->color == BLACK) *win = 1;}
    else if (black_base != NULL) {if (black_base->color == WHITE) *win = 2;}
    else *win = 0;
    if (save) save_win(save_file, win);
    return win;
}

/**
 * \brief Saves a movement to a file
 * \param path: the file where the movement needs to be saved
 * \param movement: the movement to save
 */
void save_move(char *path, Movement *movement) {
    FILE *fptr = fopen(path, "a");
    char x = movement->start.x + 'a';
    char y = movement->start.y + '1';
    char x_ = movement->end.x + 'a';
    char y_ = movement->end.y + '1';
    fprintf(fptr, "D %c%c->%c%c\n", x, y, x_, y_);
    fclose(fptr);
}

/**
 * \brief Saves a question to a file
 * \param save_file: the file where the question needs to be saved
 * \param x: the x position of the questioned pawn
 * \param y: the y position of the questioned pawn
 * \param x_: the x position of the interrogator
 * \param y_: the y position of the interrogator
 */
void save_question(char *save_file, int x, int y, int x_, int y_) {
    FILE *fptr = fopen(save_file, "a");
    char qx, qy, ix, iy;
    qx = x + 'a';
    qy = y + '1';
    ix = x_ + 'a';
    iy = y_ + '1';
    fprintf(fptr, "I %c%c->%c%c\n", ix, iy, qx, qy);
    fclose(fptr);
}

/**
 * \brief Saves a win to a file
 * \param save_file: the file where the win needs to be saved
 * \param win: the winner
 */
void save_win(char *save_file, int *win) {
    FILE *fptr = fopen(save_file, "a");
    if (*win == 1) fprintf(fptr, "B\n");
    else if (*win == 2) fprintf(fptr, "W\n");
    fclose(fptr);
}

/**
 * \brief Evaluates a question (for load save file purposes)
 * \param game: the game where the question is evaluated
 * \param x: the x position of the questioned pawn
 * \param y: the y position of the questioned pawn
 * \param x_: the x position of the interrogator
 * \param y_: the y position of the interrogator
 * \param save: whether to save the question
 * \param save_file: the file where to save the question
 */
void eval_question(Game *game, int x, int y, int x_, int y_, bool save, char *save_file) {
    Pawn *pawn = game->board[y][x];
    Pawn *interrogator = game->board[y_][x_];
    if (pawn->type == SPY) {
        if (interrogator->color == WHITE) printf("White wins !\n");
        else printf("Black wins !\n");
    } else {
        if (interrogator->type == SPY) {
            if (interrogator->color == WHITE) printf("Black wins !\n");
            else printf("White wins !\n");
        } else {
            free(game->board[y_][x_]);
            game->board[y_][x_] = NULL;
        }
    }
    if (save) save_question(save_file, x, y, x_, y_);
}

/**
 * \brief Relocates the spies on the board
 * \param game: the game where the spies need to be relocated
 * \param x: the x position of the white spy
 * \param y: the y position of the white spy
 * \param x_: the x position of the black spy
 * \param y_: the y position of the black spy
 */
void replace_spies(Game *game, int x, int y, int x_, int y_) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (game->board[i][j] != NULL) {
                if (game->board[i][j]->type == SPY) game->board[i][j]->type = SCOUT;
            }
        }
    }
    game->board[y][x]->type = SPY;
    game->board[y_][x_]->type = SPY;
}

/**
 * \brief Reads a save file
 * \param fptr: the file where the save file needs to be read
 * \param game: the game to update
 * \param save: whether to save the initial state
 * \param save_file: the file where to save the initial state
 * \return true if the game is over,
 *         false otherwise
 */
bool read_save(FILE *fptr, Game *game, bool save, char *save_file) {
    char line[30]; //readed line
    char x, y, x_, y_;
    fgets(line, 30, fptr);
    sscanf(line, "B %c%c", &x, &y);
    int wx = x - 'a';
    int wy = y - '1';
    fgets(line, 30, fptr);
    sscanf(line, "N %c%c", &x, &y);
    int bx = x - 'a';
    int by = y - '1';
    replace_spies(game, wx, wy, bx, by);

    if (save) init_save(save_file, game); //save the initial state
    fgets(line, 30, fptr); //skip the next line

    while (fgets(line, 30, fptr) != NULL) {
        print_board(game);
        if (line[0] == 'B' || line[0] == 'W') { //win
            int win;
            if (line[0] == 'B') {
                win = 1;
                printf("Black wins !\n");
            }
            else {
                win = 2;
                printf("White wins !\n");
            }
            if (save) save_win(save_file, &win);
            return true;
        } else {
            if (game->player == WHITE) printf("White's turn. "); //print player's turn
            else printf("Black's turn. ");

            if (line[0] == 'D') { //movement
                sscanf(line, "D %c%c->%c%c", &x, &y, &x_, &y_);
                int sx = x - 'a'; //movement start
                int sy = y - '1';
                int ex = x_ - 'a'; //movement end
                int ey = y_ - '1';
                Movement *movement = (Movement *)malloc(sizeof(Movement));
                movement->start.x = sx;
                movement->start.y = sy;
                movement->end.x = ex;
                movement->end.y = ey;
                move_pawn(game, movement, save, save_file);
                printf("Moved pawn from (%d, %d) to (%d, %d)\n", sx, sy, ex, ey);
                free(movement);
            }
            else if (line[0] == 'I') { //question
                sscanf(line, "I %c%c->%c%c", &x, &y, &x_, &y_);
                int ix = x - 'a'; //questioned pawn
                int iy = y - '1';
                int qx = x_ - 'a'; //interrogator
                int qy = y_ - '1';
                eval_question(game, qx, qy, ix, iy, save, save_file);
                printf("Questioned pawn at (%d, %d) with pawn at (%d, %d)\n", qx, qy, ix, iy);
            }
        }
        if (game->player == WHITE) game->player = BLACK;
        else game->player = WHITE;
    }
    fclose(fptr);
    return false;
}

/**
 * \brief Initializes the save file
 * \param save_file: the file where the game needs to be saved
 * \param game: the game to save
 */
void init_save(char *save_file, Game *game) {
    FILE *fptr = fopen(save_file, "w");
    char wx, wy, bx, by;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Pawn *pawn = game->board[i][j];
            if (pawn == NULL) continue;
            if (pawn->color == WHITE && pawn->type == SPY) {
                wx = j + 'a';
                wy = i + '1';
            } else if (pawn->color == BLACK && pawn->type == SPY) {
                bx = j + 'a';
                by = i + '1';
            }
        }
    }
    fprintf(fptr, "B %c%c\nN %c%c\nB\n", wx, wy, bx, by);
    fclose(fptr);
}

/********************************************************
 * Main game functions***********************************
 ********************************************************/

/**
 * \brief Starts a game in command line mode
 * \param save: whether to save the game
 * \param save_file: the file where to save the game
 * \param load: whether to load a game
 * \param load_file: the file where to load the game
 */
void cmd_game(bool save, char *save_file, bool load, FILE *load_file) {
    Game *game = (Game *)malloc(sizeof(Game));
    init_game(game);
    init_pawns(game);

    if (load) {
        bool end_game = read_save(load_file, game, save, save_file);
        if (end_game) {
            free_board(game);
            return;
        }
    }
    else if (save) init_save(save_file, game);

    int playing = 1;
    while (playing) {
        print_board(game);
        if (game->player == WHITE) printf("White's turn. ");
        else printf("Black's turn. ");

        printf("Do you want to:\n");
        printf("    1. Move a pawn\n");
        printf("    2. Question a pawn\n");
        fflush(stdin);
        int option;
        // scanf("%d", &option);
        char tmp[100];
        fgets(tmp, 100, stdin);
        sscanf(tmp, "%d", &option);

        if (option == 1) { //move a pawn
            Movement *movement = get_movement(game); // gets the movement from the player
            move_pawn(game, movement, save, save_file);
            if (game->player == WHITE) game->player = BLACK;
            else game->player = WHITE;
            int *win = check_win(game, save, save_file);
            if (*win == 1) {
                printf("Black wins !\n");
                playing = 0;
            } else if (*win == 2) {
                printf("White wins !\n");
                playing = 0;
            }
            free(win);
            free(movement);
        } 
        else if (option == 2) { //question a pawn
            int found = 0;
            while (found == 0) found = question_pawn(game, save, save_file);
            if (found == 1) playing = 0;
            else if (found == 2) {
                if (game->player == WHITE) game->player = BLACK;
                else game->player = WHITE;
            }
        } 
        else printf("Invalid option.\n");
    }

    free_board(game);
}