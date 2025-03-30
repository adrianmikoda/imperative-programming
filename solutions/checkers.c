#include <stdio.h>
#include <stdlib.h>
#define N 20

int cnt[2];
char filler = '_';

int rnd(const int min, const int max) {
    return (rand() % (max - min)) + min;
}

void print(char board[][N], const int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%c ", board[i][j]);
        }
    printf("\n");
    }
}

void start(char board[][N], const int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = filler;
        }
    }
    int line = 0;
    for (line = 0; line < 4; line++) {
        for (int col = line%2; col < n; col+=2) {
            board[line][col] = '1';
        }
    }
    line += n-8;
    for (line; line < n; line++) {
        for (int col = line%2; col < n; col+=2) {
            board[line][col] = '2';
        }
    }
}

int move(char board[][N], const int i, const int j, const int n) {
    int made_move = 0;
    if (board[i][j] == '1' && i+1 < n) {
        if (j+1 < n && board[i+1][j+1] == filler) {
            board[i][j] = filler;
            board[i+1][j+1] = '1';
            made_move = 1;
        }
        else if (j-1 >= 0 && board[i+1][j-1] == filler) {
            board[i][j] = filler;
            board[i+1][j-1] = '1';
            made_move = 1;
        }
    }
    if (board[i][j] == '2' && i-1 >= 0) {
        if (j+1 < n && board[i-1][j+1] == filler) {
        board[i][j] = filler;
        board[i-1][j+1] = '2';
        made_move = 1;
        }
        else if (j-1 >= 0 && board[i-1][j-1] == filler) {
        board[i][j] = filler;
        board[i-1][j-1] = '2';
        made_move = 1;
        }
    }
    return made_move;
}

int capture(char board[][N], const int i, const int j, const int n) {
    int captured = 0;
    if (board[i][j] == '1' && i+2 < n) {
        if (j+2 < n && board[i+1][j+1] == '2' && board[i+2][j+2] == filler) {
            board[i][j] = filler;
            board[i+1][j+1] = filler;
            board[i+2][j+2] = '1';
            cnt[1] -= 1;
            captured = 1;
        }
        else if (j-2 >= 0 && board[i+1][j-1] == '2' && board[i+2][j-2] == filler) {
            board[i][j] = filler;
            board[i+1][j-1] = filler;
            board[i+2][j-2] = '1';
            cnt[1] -= 1;
            captured = 1;
        }
    }
    if(board[i][j] == '2' && i-2 >= 0){
        if(j+2 < n && board[i-1][j+1] == '1' && board[i-2][j+2] == filler){
            board[i][j] = filler;
            board[i-1][j+1] = filler;
            board[i-2][j+2] = '2';
            cnt[0] -= 1;
            captured = 1;
        }
        else if(j-2 >= 0 && board[i-1][j-1] == '1' && board[i-2][j-2] == filler){
            board[i][j] = filler;
            board[i-1][j-1] = filler;
            board[i-2][j-2] = '2';
            cnt[0] -= 1;
            captured = 1;
        }
    }
    return captured;
}


int main(void) {
    char board[N][N];
    int n, steps;
    unsigned seed;
    scanf("%d %u %d", &n, &seed, &steps);
    srand(seed);
    cnt[0] = cnt[1] = 2 * n;

    start(board, n);
    for (int i = 0; i < steps; i++) {
        int ix, iy;
        const char turn = (i % 2 == 0) ? '1' : '2';
        do {
            ix = rnd(0, n);
            iy = rnd(0, n);
        } while (board[ix][iy] != turn);
        if (!capture(board, ix, iy, n)) {
            move(board, ix, iy, n);
        }
    }
    print(board, n);
    printf("%d %d\n", cnt[0], cnt[1]);

    return 0;
}