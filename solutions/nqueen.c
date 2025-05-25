#include<stdio.h>
#include<stdlib.h>
#define N 20

int solution_is_found = 0;
int diag_diff[2*N];  //col-row
int diag_sum[2*N];   //col+row
int col[N];

int is_placement_correct(int *queen_position, const int n, const int current_row, const int current_col){
    if (col[current_col]){
        return 0;
    }
    int diff_pos = current_col-current_row+N-1;
    int sum_pos = current_col+current_row;
    if (diag_diff[diff_pos]){
        return 0;
    }
    if (diag_sum[sum_pos]){
        return 0;
    }
    return 1;
}

void print_board(const int* queen_position, const int n) {
    for (int i = 0; i < n; i++){
        printf("%d ", queen_position[i]);
    }
    printf("\n");
}

void set_markers(int* queen_position, const int current_row, const int current_col){
    int diff_pos = current_col-current_row+N-1;
    int sum_pos = current_col+current_row;

    diag_diff[diff_pos] = 1;
    diag_sum[sum_pos] = 1;
    queen_position[current_row] = current_col;
    col[current_col] = 1;
}

void reset_markers(int *queen_position, const int current_row, const int current_col){
    int diff_pos = current_col-current_row+N-1;
    int sum_pos = current_col+current_row;

    diag_diff[diff_pos] = 0;
    diag_sum[sum_pos] = 0;
    queen_position[current_row] = -1;
    col[current_col] = 0;
}

int place_queens(int* queen_position, const int n, const int k, const int ndx) {
    if (solution_is_found){
        return 1;
    }
    if (ndx == n){
        if(k-1 == 0){
            print_board(queen_position, n);
            solution_is_found = 1;
        }
        return 1;
    }

    int answer_count = 0;
    int current_row = ndx;
    for (int current_col = 0; current_col < n; current_col++){
        if(is_placement_correct(queen_position, n, current_row, current_col)){
            set_markers(queen_position, current_row, current_col);
            answer_count += place_queens(queen_position, n, k-answer_count, current_row+1);
            reset_markers(queen_position, current_row, current_col);
        }
    }
    return answer_count;
}

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int queen_position[N];
    int solution_count = 0;
    for (int i = 0; i < N; i++){
        queen_position[i] = -1;
    }
    int answer = place_queens(queen_position, n, k, 0);
    if (answer < k) {
        printf("-1\n");
    }
    return 0;
}