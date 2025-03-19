#include <stdio.h>
#include <stdbool.h>

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };


//LIFO stack
#define STACK_SIZE 10
int stack[STACK_SIZE];
int top = 0; //pointer to the next free location in the stack

int stack_push(int x) {
	if (top >= STACK_SIZE) return OVERFLOW;
	stack[top] = x;
	top++;
	return OK;
}

int stack_pop(void) {
	if (top <= 0) return UNDERFLOW;
	int rtrn = stack[top-1];
	stack[top-1] = 0;
	top--;
	return rtrn;
}

int stack_state(void) {
	return top;
}


//FIFO queue with shifts
#define QUEUE_SIZE 10
int queue[QUEUE_SIZE], curr_nr = 1;
int in = 0; //number of clients wainting in the queue/pointer to the next free location in the queue

int queue_push(int in_nr) { // in_nr clients try to enter the queue
	bool flag = false;
	for (int i = 0; i < in_nr; i++) {
		if (in < QUEUE_SIZE) {
			queue[in] = curr_nr;
			in++;
		}
		else flag = true;
		curr_nr++;
	}
	if(flag) return OVERFLOW;
	return OK;
}

int queue_pop(int out_nr) { // out_nr clients leave the queue
	if (out_nr > QUEUE_SIZE) return UNDERFLOW;
	for (int i = out_nr; i < QUEUE_SIZE; i++) { //moving remaining numebrs to the front
		queue[i-out_nr] = queue[i];
	}
	/*for (int i = QUEUE_SIZE - out_nr; i < QUEUE_SIZE; i++) { //cleaning
		queue[i] = 0;
	}*/
	in-=out_nr;
	return (in >= 0) ? in : 0;
}

int queue_state(void) {
	return in;
}

void queue_print(void) {
	for (int i = 0; i < in; i++) {
		printf("%d ", queue[i]);
	}
	printf("\n");
}


//FIFO queue with cyclic buffer
#define CBUFF_SIZE 10
int cbuff[10];
int len = 0, out = 0;

int cbuff_push(int cli_nr) { // client with number cli_nr enters the queue
	if (len >= CBUFF_SIZE) return OVERFLOW;
	cbuff[(out+len)%CBUFF_SIZE] = cli_nr;
	len++;
	return OK;
}

int cbuff_pop(void) { // longest waiting client leaves the queue
	if(len <= 0) return UNDERFLOW;
	
	int rtrn = cbuff[out];
	len--;
	out = (out+1)%CBUFF_SIZE;
	return rtrn;
}

int cbuff_state(void) {
	return len;
}

void cbuff_print(void) {
	for (int i = 0; i < len; i++){
		printf("%d ", cbuff[(out+i)%CBUFF_SIZE]);
	}
	printf("\n");
}



int main(void){
	int to_do, n, answer;
	scanf("%d", &to_do);

	switch(to_do){
		case 1: // stack
			do{
				scanf("%d", &n);
				if (n > 0){
					if((answer = stack_push(n)) < 0) printf("%d", answer);
				} else if (n < 0){
					printf("%d ", stack_pop());
				} else printf("\n%d\n", stack_state()); 
			} while(n != 0);
			break;
		case 2: // queue
			do{
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = queue_push(n)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
				} else {
					printf("\n%d\n", queue_state());
					queue_print();
				}
			} while (n != 0);
			break;
		case 3: // queue with cyclic buffer
			int client_no = 0;
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					printf("%d ", cbuff_pop());
				} else {
					printf("\n%d\n", cbuff_state());
					cbuff_print();
				}
			} while(n != 0);
				break;
		default:
			printf("Error: unknown operation %d", to_do);
			break;
	}
	return 0;
}