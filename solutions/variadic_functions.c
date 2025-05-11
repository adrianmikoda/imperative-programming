#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define MAX_LINE 256
#define MAX_NUMBER 64
#define MAX_TEXTS 64

void swap(double *array, int i, int j){
    double temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int partition(double *array, int start, int end){
    double pivot = array[(start + end) / 2];
    int i = start-1, j = end+1;

    while (1) {
        do {
            i++;
        } while (array[i] < pivot);

        do {
            j--;
        } while (array[j] > pivot);

        if (i >= j) {
            return j;
        }
        swap(array, i, j);
    }
}

void quicksort(double *array, int start, int end){
    while (start < end) {
        int pivot = partition(array, start, end);
        if (pivot-start+1 < end-pivot) {
            quicksort(array, start, pivot);
            start = pivot+1;
        }
        else {
            quicksort(array, pivot+1, end);
            end = pivot;
        }
    }
}

double sum_var(int number, ...){
    va_list args;
    va_start(args, number);
    double sum = 0;

    for (int i = 0; i < number; i++){
        sum += va_arg(args, double);
    }
    
    va_end(args);
    return sum;
}

double average_var(int number, ...){
    va_list args;
    va_start(args, number);
    double sum = 0;

    for (int i = 0; i < number; i++){
        sum += va_arg(args, double);
    }
    
    va_end(args);
    return sum/number;
}

double median_var(int number, ...){
    double numbers[number];
    va_list args;
    va_start(args, number);
    for (int i = 0; i < number; i++){
        numbers[i] = va_arg(args, double);
    }

    va_end(args);
    quicksort(numbers, 0, number-1);
    
    if (number%2 == 0){
        return (numbers[number/2] + numbers[number/2 - 1])/2;
    }
    else{
        return numbers[number/2];
    }
}

double min_var(int number, ...){
    va_list args;
    va_start(args, number);
    double min_num = va_arg(args, double);

    for (int i = 1; i < number; i++){
        double num = va_arg(args, double);
        if (num < min_num){
            min_num = num;
        }
    }

    va_end(args);
    return min_num;
}

double max_var(int number, ...){
    va_list args;
    va_start(args, number);
    double max_num = va_arg(args, double);
    for (int i = 1; i < number; i++){
        double num = va_arg(args, double);
        if (num > max_num) {
            max_num = num;
        }
    }

    va_end(args);
    return max_num;
}

double sum(const int number, const double *values){
    if (number == 0) return 0;

    double sum = 0;
    for (int i = 0; i < number; i++){
        sum += values[i];
    }
    return sum;
}

double average(const int number, const double *values){
    if (number == 0) return 0;
    return sum(number, values)/number;
}

double median(const int number, double *values){
    if (number == 0) return 0;

    quicksort(values, 0, number-1);
    if (number%2 == 0){
        return (values[number/2] + values[number/2 - 1])/2;
    }
    else{
        return values[number/2];
    }
}

double min(const int number, const double *values){
    if (number == 0) return 0;

    double min_val = values[0];
    for (int i = 1; i < number; i++){
        if (values[i] < min_val){
            min_val = values[i];
        }
    }
    return min_val;
}

double max(const int number, const double *values){
    if (number == 0) return 0;

    double max_val = values[0];
    for (int i = 1; i < number; i++){
        if (values[i] > max_val){
            max_val = values[i];
        }
    }
    return max_val;
}

int read_from_line(char* c_buf, double *values, char **texts, int *text_counter) {
    int value_counter = 0;
    int length = strlen(c_buf);
    char *start = c_buf;
    char *end = c_buf;
    while (start != NULL && end != NULL){
        double num = strtod(start, &end);
        
        if(start != end){
            *(values+value_counter) = num;
            value_counter += 1;
        }
        else{
            end = strchr(end+1, ' ');
            if(*start == ' ' || (*start >= '0' && *start <= '9')){
                start += 1;
            }
            int size;
            if(end != NULL){
                size = end-start;
            }
            else{
                size = strlen(start);
            }
            if (size > 0) {
                char text_buf[size+1];
                strncpy(text_buf, start, size);
                text_buf[size] = '\0';
                texts[*text_counter] = (char*) malloc((size+1) * sizeof(char));
                strcpy(texts[*text_counter], text_buf);
                (*text_counter)++;
            }
        }
        start = end;
    }
    return value_counter;
}

int read_int(void) {
    char c_buf[MAX_LINE];
    fgets(c_buf, MAX_LINE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

void free_texts(char **texts, int text_counter){
    for (int i = 0; i < text_counter; i++){
        free(texts[i]);
    }
}
int main(void) {
    int number;
    double v1, v2, v3, v4, v5;

    char c_buf[MAX_LINE];
    double values[MAX_NUMBER];
    char *texts[MAX_TEXTS];
    int text_counter;

    const int to_do = read_int();

    switch (to_do) {
        case 1:
            number = 3;
            scanf("%lf%lf%lf", &v1, &v2, &v3);
            printf("%.2f %.2f %.2f %.2f %.2f\n",
                    sum_var(number, v1, v2, v3),
                    average_var(number, v1, v2, v3),
                    median_var(number, v1, v2, v3),
                    min_var(number, v1, v2, v3),
                    max_var(number, v1, v2, v3));

            number = 5;
            scanf("%lf%lf%lf%lf%lf", &v1, &v2, &v3, &v4, &v5);
            printf("%.2f %.2f %.2f %.2f %.2f\n",
                    sum_var(number, v1, v2, v3, v4, v5),
                    average_var(number, v1, v2, v3, v4, v5),
                    median_var(number, v1, v2, v3, v4, v5),
                    min_var(number, v1, v2, v3, v4, v5),
                    max_var(number, v1, v2, v3, v4, v5));
            break;
        case 2:
            fgets(c_buf, MAX_LINE, stdin);
            text_counter = 0;
            number = read_from_line(c_buf, values, texts, &text_counter);
            printf("%.2f %.2f %.2f %.2f %.2f\n",
                    sum(number, values),
                    average(number, values),
                    median(number, values),
                    min(number, values),
                    max(number, values));
            for (int k = 0; k < text_counter; k++) {
                printf("%s\n", texts[k]);
            }
            free_texts(texts, text_counter);
            break;
        default:
            printf("Nothing to do for n = %d\n", to_do);
            break;
    }

    return EXIT_SUCCESS;
}