#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXTS 20

void place_letters(char* text, const char* original_text, const char letter){
    while (*original_text != '\0'){
        if( *original_text == letter)
            *text = letter;
        original_text++;
        text++;
    }
}
int guess_a_letter(char* text, const char* original_text) {
    char letter_guessed = 'A'+ rand() % 26;
    char* letter_in_original_text = strchr(original_text, letter_guessed);
    if (letter_in_original_text != NULL){
        char* letter_in_text = strchr(text, letter_guessed);
        if (letter_in_text == NULL){
            place_letters(text, original_text, letter_guessed);
            return 1;
        }
    }
    return 0;

}

char* play(const char* original_text, const int number_of_players, const int turns, int* p_player) {
    int n = strlen(original_text);
    char* text = malloc((n+1)*sizeof(char));
    for (int i = 0; i < n; i++){
        if(original_text[i] != ' '){
            text[i] = '_';
        }
        else{
            text[i] = ' ';
        }
    }
    text[n] = '\0';
    *p_player = -1;
    int flg = 0, current_player = 0;
    int current_turn = 1;
    while (strchr(text, '_') != NULL && current_turn <= turns){
        flg = guess_a_letter(text, original_text);
        *p_player = current_player;
        if (flg == 0){
            current_player = (current_player+1) % number_of_players;
            current_turn++;
        }
    }
    return text;
    
}

int main() {

    const char* texts[MAX_TEXTS] = {
        "PAN TADEUSZ",
        "HENRYK SIENKIEWICZ",
        "MORZE KASPIJSKIE",
        "POGODA DLA BOGACZY",
        "CZERWONE GITARY",
        "KAZANIE PIOTRA SKARGI",
        "QUO VADIS",
        "ADAM MICKIEWICZ",
        "ALBERT EINSTEIN",
        "DENNIS RITCHIE",
        "FIZYKA KWANTOWA",
        "PROGRAMOWANIE IMPERATYWNE",
        "ALGORYTMY I STRUKTURY DANYCH",
        "BRIAN KERNIGHAN",
        "CZERWONY KAPTUREK",
        "MARIA KONOPNICKA",
        "WILLIAM SHAKESPEARE",
        "ZBIGNIEW ZAPASIEWICZ",
        "MAGDALENA SAMOZWANIEC",
        "JEZIORO ONTARIO"
    };

    int number_of_players, turns, player;
    unsigned seed;

    scanf("%d %u %d", &number_of_players, &seed, &turns);
    srand(seed);
    const int number = rand() % MAX_TEXTS;
    char* text = play(texts[number], number_of_players, turns, &player);
    printf("%s\n", text);
    printf("%d\n", player);
    free(text);

    return 0;
}