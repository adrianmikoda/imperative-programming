#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define BUFFER_SIZE 1024
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef int  (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement *next;
    void *data;
} ListElement;

typedef struct {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    DataFp modify_data;
} List;

void *safe_malloc(const size_t size){
    void *ptr = malloc(size);
    if (ptr) return(ptr);
    printf("malloc error");
    exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
    void *ptr = safe_malloc(strlen(string) + 1);
    strcpy(ptr, string);
    return ptr;
}

// Generic functions for any data type
int is_empty(List* p_list){
    return (p_list->head == NULL || p_list->tail == NULL);
}

void init_list(List* p_list, const ConstDataFp dump_data, const DataFp free_data,
        const CompareDataFp compare_data, const DataFp modify_data) {
        p_list->head = NULL;
        p_list->tail = NULL;
        p_list->dump_data = dump_data;
        p_list->free_data = free_data;
        p_list->compare_data = compare_data;
        p_list->modify_data = modify_data;
}

// Print elements from the list
void dump_list(const List* p_list) {
    if (p_list == NULL) return;
    if (p_list->dump_data == NULL) return;

    ListElement *current_element = p_list->head;
    while (current_element != NULL){
        p_list->dump_data(current_element->data);
        current_element = current_element->next;
    }
}

// Print elements of the list with matching data
void dump_list_if(const List* p_list, const void *data_to_match) {
    if (p_list == NULL) return;
    if (p_list->compare_data == NULL && p_list->dump_data == NULL) return;

    ListElement *current_element = p_list->head;
    while (current_element != NULL){
        if (p_list->compare_data(current_element->data, data_to_match)){
            p_list->dump_data(current_element->data);
        }
        current_element = current_element->next;
    }
}

// Free all elements of the list
void free_list(List* p_list) {
    if (p_list == NULL) return;
    if (p_list->free_data == NULL) return;

    ListElement *current_element = p_list->head;
    while (current_element != NULL){
        ListElement *next = current_element->next;
        p_list->free_data(current_element->data);
        free(current_element);
        current_element = next;
    }
}

// Push element at the beginning of the list
void push_front(List* p_list, void *data) {
    if (p_list == NULL) return;

    ListElement *new_element = (ListElement *)safe_malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = p_list->head;

    if(is_empty(p_list)){
        p_list->tail = new_element;
    }
    p_list->head = new_element;
}

// Push element at the end of the list
void push_back(List* p_list, void *data) {
    if (p_list == NULL) return;

    ListElement *new_element = (ListElement *)safe_malloc(sizeof(ListElement));
    new_element->data = data;
    new_element->next = NULL;

    if(is_empty(p_list)){
        p_list->tail = new_element;
        p_list->head = new_element;
    }
    else{
        p_list->tail->next = new_element;
        p_list->tail = new_element;
    }
}

// Remove the first element
void pop_front(List* p_list) {
    if (p_list == NULL) return;
    if (is_empty(p_list)) return;
    if (p_list->free_data == NULL) return;

    ListElement *element_to_delete = p_list->head;
    p_list->head = p_list->head->next;
    if (element_to_delete == p_list->tail){
        p_list->tail = NULL;
        p_list->head = NULL;
    }
    p_list->free_data(element_to_delete->data);
    free(element_to_delete);
}

// Reverse the list
void reverse(List* p_list) {
    if (is_empty(p_list)) return;

    ListElement *current_element = p_list->head;
    ListElement *next_element = current_element->next;
    ListElement *next_next_element;

    current_element->next = NULL;
    p_list->tail = p_list->head;
    while (current_element != NULL && next_element != NULL){
        next_next_element = next_element->next;
        next_element->next = current_element;
        current_element = next_element;
        next_element = next_next_element;
    }
    p_list->head = current_element;
}

// Find an element in a sorted list after which to insert a given element
ListElement* find_insertion_point(const List* p_list, const void *p_data) {
    int order = -1;
    if (p_list->compare_data(p_list->head->data, p_list->tail->data) == 1){
        order = 1;
    }

    ListElement *new_element = (ListElement *)safe_malloc(sizeof(ListElement));
    ListElement *current_element = p_list->head;
    ListElement *previous_element = NULL;
    while(current_element != NULL && p_list->compare_data(current_element->data, p_data) == order){
        previous_element = current_element;
        current_element = current_element->next;
    }
    return previous_element;
}

// Insert element after 'previous_element'
void push_after(List *p_list, ListElement *previous_element, void *p_data){
    if (p_list == NULL) return;

    ListElement *new_element = (ListElement *)safe_malloc(sizeof(ListElement));
    new_element->data = p_data;
    new_element->next = previous_element->next;
    previous_element->next = new_element;

    if (p_list->tail == previous_element){
        p_list->tail = new_element;
    }
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
    if (p_list == NULL) return;
    if (p_list->compare_data == NULL) return;

    if(is_empty(p_list)){
        push_front(p_list, p_data);
    }
    else{
        ListElement *previous_element = find_insertion_point(p_list, p_data);
        if (previous_element == NULL){
            if(p_list->compare_data(p_list->head->data, p_data)){
                push_front(p_list, p_data);
            }
            else if(p_list->modify_data != NULL){
                p_list->modify_data(p_list->head->data);
            }
        }
        else{
            int is_previous_diff = p_list->compare_data(previous_element->data, p_data);
            int is_next_diff_or_doesnt_exist = 1;
            if (previous_element->next != NULL){
                is_next_diff_or_doesnt_exist = p_list->compare_data(previous_element->next->data, p_data);
            }
            if(is_previous_diff && is_next_diff_or_doesnt_exist){
                push_after(p_list, previous_element, p_data);
            }
            else if(p_list->modify_data != NULL){
                if(!is_previous_diff){
                    p_list->modify_data(previous_element->data);
                }
                else if(previous_element->next != NULL && !is_next_diff_or_doesnt_exist){
                    p_list->modify_data(previous_element->next->data);
                }
            }
        }
    }
}

// int element
void dump_int(const void* d) {
    if (d == NULL) return;

    int *d_int = (int*)(d);
    printf("%d ", *d_int);
}

void free_int(void* d) {
    free(d);
}

int cmp_int(const void* a, const void* b) {
    int* a_int = (int*)(a);
    int* b_int = (int*)(b);
    if (*a_int < *b_int){
        return -1;
    }
    if (*a_int == *b_int){
        return 0;
    }
    return 1;
}

int* create_data_int(const int v) {
    int *new_data_int = (int *)safe_malloc(sizeof(int));
    *new_data_int = v;
    return new_data_int;
}

// word element
typedef struct DataWord {
    char* word;
    int counter;
} DataWord;

void dump_word(const void* d) {
    if (d == NULL) return;
    
    DataWord *data_word = (DataWord *)d;

    printf("%s ", data_word->word);
}

void dump_word_to_lowercase(const void* d){
    if (d == NULL) return;

    DataWord *word_data = (DataWord *)d;
    char *word = word_data->word;
    char *word_to_lowercase = (char *)safe_strdup(word);
    char *word_to_lowercase_p = word_to_lowercase;

    while (*word_to_lowercase_p){
        *word_to_lowercase_p = tolower(*word_to_lowercase_p);
        word_to_lowercase_p++;
    }
    printf("%s ", word_to_lowercase);

    free(word_to_lowercase);
}

void free_word(void* d) {
    if (d == NULL) return;

    DataWord *data_word = (DataWord *)d;

    free(data_word->word);
    free(data_word);
}

int cmp_word_alphabet(const void* a, const void* b) {
    DataWord *a_data_word = (DataWord *)a;
    DataWord *b_data_word = (DataWord *)b;
    char *a_word = a_data_word->word;
    char *b_word = b_data_word->word;

    while(*a_word != '\0' && *b_word != '\0'){
        int a_char = tolower(*a_word);
        int b_char = tolower(*b_word);
        if(a_char < b_char){
            return -1;
        }
        else if(a_char > b_char){
            return 1;
        }
        a_word++;
        b_word++;
    }
    if (*a_word == '\0' && *b_word == '\0'){
        return 0;
    }
    if(*a_word == '\0'){
        return -1;
    }
    return 1;
}

int are_counters_equal(const void* a, const void* b) {
    DataWord *a_data_word = (DataWord *)a;
    DataWord *b_data_word = (DataWord *)b;
    if (a_data_word->counter == b_data_word->counter){
        return 1;
    }
    return 0;
}

void modify_word(void* d) {
    if (d == NULL) return;

    DataWord *p_data_word = (DataWord *)d;
    p_data_word->counter = p_data_word->counter+1;
}

void* create_data_word(const char* string, const int counter) {
    DataWord *new_data_word = (DataWord *)safe_malloc(sizeof(DataWord));
    new_data_word->counter = counter;
    new_data_word->word = (char *)safe_strdup(string);
    return new_data_word;
}

// Read text, parse it to words, and insert those words to the list.
// The order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL, the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List* p_list, FILE* stream) {
    if (p_list == NULL || stream == NULL) return;

    const char *delimits = " \t\n\r.,?!:;-";
    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, BUFFER_SIZE, stream) != NULL){
        char *string = strtok(buffer, delimits);
        while (string != NULL){
            if (p_list->compare_data == NULL){
                push_back(p_list, create_data_word(string, 1));
            }
            else{
                insert_in_order(p_list, create_data_word(string, 1));
            }
            string = strtok(NULL, delimits);
        }
    }
}

// Handle the integer list
void handle_list_int(List* p_list, const int n) {
    if (p_list == NULL) return;

    char op;
    int v;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        switch (op) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int command, n;
    List list;

    scanf ("%d", &command);
    switch (command) {
        case 1: // Handle an integer list, then print
            scanf("%d",&n);
            init_list(&list, dump_int, free_int, cmp_int, NULL);
            handle_list_int(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // Read words from the stdin stream, insert into a list, and print the list
            init_list(&list, dump_word, free_word, NULL, NULL);
            stream_to_list(&list, stdin);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // Read words from the stdin stream, insert into a list alphabetically, print words encountered n times from the list
            scanf("%d",&n);
            init_list(&list, dump_word_to_lowercase, free_word, cmp_word_alphabet, modify_word);
            stream_to_list(&list, stdin);
            list.compare_data = are_counters_equal;
            const DataWord data = {NULL, n};
            dump_list_if(&list, &data);
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", command);
            break;
    }
    return 0;
}