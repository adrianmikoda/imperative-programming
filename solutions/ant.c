#include <stdio.h>
#include <stdlib.h>
#define N 20

int directions[4] = {'N', 'W', 'S', 'E'};
int moves[4][2] = {{-1, 0}, {0, -1}, {1,0}, {0,1}};

typedef struct Ant {
    int line, column;
    int direction;
} Ant;

int rnd(const int min, const int max) {
    return (rand() % (max - min + 1)) + min;
}

int real_mod(int a, int b) {
    return ((a % b) + b) % b;
}

void print_board(int **board, const int n){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

void print_ant(const Ant ant) {
    printf("%d %d %c", ant.line, ant.column, directions[ant.direction]);
}

void init_board(int ***board, const int n){
    *board = (int**)malloc(n*sizeof(int *));
    for (int i =0; i < n; i++) {
        (*board)[i] = (int*)malloc(n*sizeof(int));
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            (*board)[i][j] = rnd(1,8);
        }
    }
}

void free_board(int **board, const int n){
    for (int i = 0; i < n; i++){
        free(board[i]);
    }
    free(board);
}

void ant_move(int ***board, const int n, Ant* ant){
    int operation = (*board)[ant->line][ant->column];
    int move[2];
    switch (operation){
        case 1:
            move[0] = moves[(ant->direction + 0)%4][0];
            move[1] = moves[(ant->direction + 0)%4][1];
            (*board)[ant->line][ant->column] = 7;
            ant->line += move[0];
            ant->column += move[1];
            break;
        case 2:
            ant->direction = real_mod(ant->direction-1, 4);
            (*board)[ant->line][ant->column] = 4;
            break;
        case 3:
            ant->direction = real_mod(ant->direction+1, 4);
            (*board)[ant->line][ant->column] = 2;
            break;
        case 4:
            move[0] = moves[(ant->direction + 3)%4][0];
            move[1] = moves[(ant->direction + 3)%4][1];
            (*board)[ant->line][ant->column] = 6;
            ant->line += move[0];
            ant->column += move[1];
            break;
        case 5:
            move[0] = moves[(ant->direction + 1)%4][0];
            move[1] = moves[(ant->direction + 1)%4][1];
            (*board)[ant->line][ant->column] = 3;
            ant->line += move[0];
            ant->column += move[1];
            break;
        case 6:
            move[0] = moves[(ant->direction + 2)%4][0];
            move[1] = moves[(ant->direction + 2)%4][1];
            (*board)[ant->line][ant->column] = 5;
            ant->line += move[0];
            ant->column += move[1];
            break;
        case 7:
            ant->direction = real_mod(ant->direction+2, 4);
            (*board)[ant->line][ant->column] = 8;
            break;
        case 8:
            (*board)[ant->line][ant->column] = 1;
            break;
    }
    ant->line = real_mod(ant->line,  n);
    ant->column = real_mod(ant->column, n);
}

void ant_simulation(int **board, const int n, Ant *ant, const int steps) {
    ant->line = 0;
    ant->column = 0;
    ant->direction = 3;

    for (int move_nr = 0; move_nr < steps; move_nr++){
        ant_move(&board, n, ant);
    }
}

int main(void){
    int **board;
    int n, steps;
    unsigned seed;
    Ant ant;

    scanf("%d %d %d", &n, &seed, &steps);
    srand(seed);
    init_board(&board, n);
    ant_simulation(board, n, &ant, steps);

    print_board(board, n);
    print_ant(ant);

    free_board(board, n);

    return 0;
}