#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int* A) {
    return A[row * cols + col];
}

void set(int cols, int row, int col, int* A, int value) {
    A[row * cols + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int* A, int* B, int* AB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            int sum = 0;
            for (int k = 0; k < colsA; k++) {
                sum += get(colsA, i, k, A) * get(colsB, k, j, B);
            }
            set(colsB, i, j, AB, sum);
        }
    }
}

void read_mat(int rows, int cols, int* t) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &t[i * cols + j]);
        }
    }
}

void print_mat(int rows, int cols, int* t) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", get(cols, i, j, t));
        }
        printf("\n");
    }
}

int read_int_lines_cont(int* ptr_array[]) {
    int *continuous_array_pointer = ptr_array[0];
    int line = 0;
    char buffer[BUF_SIZE];
    char *pointer;
    int num;

    while(fgets(buffer, BUF_SIZE, stdin) != NULL && line < TAB_SIZE){
        line++;
        ptr_array[line] = continuous_array_pointer;

        pointer = buffer;
        while(pointer != NULL){
            num = (int)strtol(pointer, NULL, 10);
            pointer = strchr(pointer + 1, ' ');
            *continuous_array_pointer = num;
            continuous_array_pointer++;
        }
    }

    ptr_array[line+1] = continuous_array_pointer;
    return line;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int *pointer = ptr_array[n];
    int *endptr = ptr_array[n+1];
    while(pointer < endptr){
        printf("%d ", *pointer);
        pointer++;
    }
}

int read_char_lines(char *array[]){
    char buffer[BUF_SIZE];
    int pointer = 0;
    int count = 0;
    while(fgets(buffer, BUF_SIZE, stdin) != NULL && count < TAB_SIZE){
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        array[count] = (char*)malloc((len + 1) * sizeof(char));

        strcpy(array[count], buffer);
        count++;
    }
    return count;
}

void write_char_line(char *array[], int n) {
    printf("%s", array[n]);
}

void delete_lines(char *array[]) {
    for (int i = 0; array[i] != NULL && i < TAB_SIZE; i++) {
        free(array[i]);
    }
}

int read_int(void) {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    const int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, rowsA, colsA, rowsB, colsB;
    char* char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int* ptr_array[TAB_SIZE+1];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int();
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}