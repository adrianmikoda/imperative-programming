#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_ID_LEN 64
#define MAX_IDS 1024
#define MAX_LINE 128
#define DEBUGGING 1

char identifiers[MAX_IDS][MAX_ID_LEN];
const char *keywords[] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int escape_char_counter(char line[], int current_pos){
    int counter = 0;
    while (current_pos >= 0 && line[current_pos] == '\\'){
        counter++;
        current_pos--;
    }
    return counter;
}

int check_for_line_comment(char line[], int current_pos){
    if (current_pos > 0 && line[current_pos-1] == '/' && line[current_pos] == '/'){
        return 1;
    }
    return 0;
}

int check_for_block_opening(char line[], int current_pos){
    if (current_pos > 0 && line[current_pos-1] == '/' && line[current_pos] == '*'){
        return 1;
    }
    return 0;
}

int check_for_block_ending(char line[], int current_pos){
    if (current_pos > 0 && line[current_pos-1] == '*' && line[current_pos] == '/'){
        return 1;
    }
    return 0;
}

void find_comments_in_line(char line[], int *inside_quotation_marks, int *opened_block_comments, int * line_comment_counter, int * block_comment_counter){
    int current_pos = 0;

    while (line[current_pos] != '\0' && current_pos < MAX_LINE){
        if (line[current_pos] == '"' && (escape_char_counter(line, current_pos-1))%2 == 0){
            *inside_quotation_marks = ((*inside_quotation_marks)+1)%2;
        }
        else if (*inside_quotation_marks == 0){
            if(check_for_block_opening(line, current_pos) && *opened_block_comments == 0){
                (*opened_block_comments)++;
                (*block_comment_counter)++;
            }
            if(check_for_line_comment(line, current_pos) && *opened_block_comments == 0){
                (*line_comment_counter)++;
                return;
            }
            if(check_for_block_ending(line, current_pos)){
                (*opened_block_comments)--;
            }
        }
        current_pos++;
    }
}

void find_comments(int *line_comment_counter, int *block_comment_counter){
    int inside_quotation_marks = 0;
    int inside_block_comment = 0;
    char line[MAX_LINE];
    while (feof(stdin) == 0){
        fgets(line, MAX_LINE, stdin);
        find_comments_in_line(line, &inside_quotation_marks, &inside_block_comment, line_comment_counter, block_comment_counter);
    }
}

int allowed_char(char c, int word_length){
    if ((c == '_') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return 1;
    }
    if(word_length != 0 && c >= '0' && c <= '9'){
        return 1;
    }
    return 0;
}

int in_keywords(char *word){
    int num_keywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(word, keywords[i]) == 0){
            return 1;
        }
    }
    return 0;
}

void handle_repetition(char *word, int *ident_number){
    for (int i = 0; i < *ident_number; i++){
        if (strcmp(word, identifiers[i]) == 0){
            return;
        }
    }
    strcpy(identifiers[*ident_number], word);
    (*ident_number)++;
}

void handle_word(char line[], int start, int length, int *ident_number){
    char word[length+1];
    for (int i = 0; i < length; i++){
        word[i] = line[start+i];
    }
    word[length] = '\0';
    if (!in_keywords(word)){
        handle_repetition(word, ident_number);
    }
}

void find_idents_in_line(char line[], int *inside_quotation_marks, int *inside_apostrophe, int *opened_block_comments, int *ident_number){
    int current_pos = 0;
    int word_starting_pos = 0;
    int word_length = 0;
    while (line[current_pos] != '\0' && current_pos < MAX_LINE){
        if (allowed_char(line[current_pos], word_length)){
            if (word_length == 0){
                word_starting_pos = current_pos;
            }
            word_length++;
        }
        else{
            if (word_length > 0 && *inside_quotation_marks == 0 && *inside_apostrophe == 0 && *opened_block_comments == 0){
                handle_word(line, word_starting_pos, word_length, ident_number);
            }

            if (line[current_pos] == '"' && (escape_char_counter(line, current_pos-1))%2 == 0){
                *inside_quotation_marks = ((*inside_quotation_marks)+1)%2;
            }
            else if (*inside_quotation_marks == 0 && line[current_pos] == '\'' && (escape_char_counter(line, current_pos-1))%2 == 0){
                *inside_apostrophe = ((*inside_apostrophe)+1)%2;
            }
            else if (*inside_quotation_marks == 0 && *inside_apostrophe == 0){
                if(check_for_block_opening(line, current_pos) && *opened_block_comments == 0){
                    (*opened_block_comments)++;
                }
                if(check_for_line_comment(line, current_pos) && *opened_block_comments == 0){
                    return;
                }
                if(check_for_block_ending(line, current_pos)){
                    (*opened_block_comments)--;
                }
            }
            word_length = 0;
        }
        current_pos++;
    }
}

int find_idents(){
    int ident_number = 0;
    int inside_quotation_marks = 0;
    int inside_apostrophe = 0;
    int inside_block_comment = 0;
    char line[MAX_LINE];
    while (feof(stdin) == 0){
        line[0] = '\0';
        fgets(line, MAX_LINE, stdin);
        find_idents_in_line(line, &inside_quotation_marks, &inside_apostrophe, &inside_block_comment, &ident_number);
    }
    return ident_number;
}

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int line_comment_counter = 0, block_comment_counter = 0;
    const int to_do = read_int();
    switch (to_do) {
        case 1:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        case 2:
            printf("%d\n", find_idents());
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}