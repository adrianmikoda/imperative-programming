#include <stdio.h>
#include <stdlib.h>
#define USE_POINTERS 1

#if USE_POINTERS == 0
	void reset(int array[], int n){
		for (int i = 0; i < n; i++) {
			array[i] = 0;
		}
	}

	void print(int array[], int first, int last){
		for (int i = first; i <= last; i++) {
			printf("%d ", array[i]);
		}
		printf("\n");
	}

	void set(int array[], int i, int x){
		array[i] = x;
	}

	int sum(int array[], int first, int last){
		int sum = 0;
		for (int i = first; i <= last; i++){
			sum += array[i];
		}
		return sum;
	}

	int diff_check(int array[], int n, int x){
		int counter = 0;
		for (int i = 0; i < n; i++) {
			if (array[i] != x) {
				counter++;
			}
		}
		return counter;
	}

	void swap(int array[], int i, int j){
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}

	int partition(int array[], int first, int last){
		int pivot = (first+last)/2;
		int i = first-1, j = last+1;

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

	void quicksort(int array[], int first, int last){
		while (first < last) {
			int pivot = partition(array, first, last);
			if (pivot-first < last-pivot) {
				quicksort(array, first, pivot-1);
				first = pivot+1;
			}
			else {
				quicksort(array, pivot+1, last);
				last = pivot-1;
			}
		}
	}

#elif USE_POINTERS == 1
	void reset(int *array_p, int n){
		int *array_f = array_p;
		for (array_p; array_p-array_f < n; array_p++) {
			*array_p = 0;
		}
	}

	void print(int *array_p, int first, int last){
		array_p += first;
		int *array_f = array_p;
		for (array_p; array_p-array_f <= last; array_p++) {
			printf("%d ", *array_p);
		}
		printf("\n");
	}

	void set(int *array_p, int i, int x){
		array_p += i;
		*array_p = x;
	}

	int sum(int *array_p, int first, int last){
		int sum = 0;
		int *array_f = array_p;
		array_p += first;
		for (array_p; array_p-array_f <= last; array_p++) {
			sum += *array_p;
		}
		return sum;
	}

	int diff_check(int *array_p, int n, int x){
		int counter = 0;
		int *array_f = array_p;
		for (array_p; array_p-array_f < n; array_p++) {
			if (*array_p != x) {
				counter++;
			}
		}
		return counter;
	}

	void swap(int *array_p, int i, int j){
		int *pointer1 = array_p+i;
		int *pointer2 = array_p+j;
		int temp = *pointer1;
		*pointer1 = *pointer2;
		*pointer2 = temp;
	}

	int partition(int *array_p, int first, int last){
		int *pivot = array_p+(first+last)/2;
		int *i = array_p+first-1, *j = array_p+last+1;

		while (1) {
			do {
				i++;
			} while (*i< *pivot);

			do {
				j--;
			} while (*j > *pivot);

			if (i >= j) {
				return j-array_p;
			}
			swap(array_p, i-array_p, j-array_p);
		}
	}

	void quicksort(int array[], int first, int last){
		while (first < last) {
			int pivot = partition(array, first, last);
			if (pivot-first < last-pivot) {
				quicksort(array, first, pivot-1);
				first = pivot+1;
			}
			else {
				quicksort(array, pivot+1, last);
				last = pivot-1;
			}
		}
	}

#endif
int main(void){
	int array[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	int n = sizeof(array)/sizeof(array[0]);
	int to_do, x, i,j;

	do {
		printf("choose operation:\n"
			"1 - reset\n"
			"2 - print\n"
			"3 - set\n"
			"4 - sum\n"
			"5 - diff_check\n"
			"6 - swap\n"
			"7 - sort\n"
			"0 - exit\n\n");
		scanf("%d", &to_do);
		switch (to_do) {
			case 1:
				reset(array, n);
				break;
			case 2:
				printf("enter start and end indexes ([start] [end]):\n");
				scanf("%d %d", &i, &j);
				print(array, i, j);
				break;
			case 3:
				printf("enter index and value ([index] [value]):\n");
				scanf("%d %d", &i, &x);
				set(array, i, x);
				break;
			case 4:
				printf("enter start and end indexes ([start] [end]):\n");
				scanf("%d %d", &i, &j);
				printf("%d", sum(array, i, j));
				break;
			case 5:
				printf("enter value ([value]): ");
				scanf("%d", &x);
				printf("%d\n", diff_check(array, n, x));
				break;
			case 6:
				printf("enter start and end indexes ([start] [end]): ");
				scanf("%d %d", &i, &j);
				swap(array, i, j);
				break;
			case 7:
				quicksort(array, 0, n-1);
				break;
			default:
				printf("error: unknown operation %d\n", to_do);
				break;
		}
		printf("press ENTER to continue...\n");
		char ch;
		ch = getchar();
		while ((ch = getchar()) != '\n');
		system("cls");
	} while(to_do != 0);
}