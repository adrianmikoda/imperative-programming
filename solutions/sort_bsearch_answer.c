#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME_MAX  20   // max. length of name + 1
#define FOOD_MAX  30   // max. number of goods
#define RECORD_MAX 64  // max. line length
typedef int (*CompareFp)(const void *, const void *);

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char name[NAME_MAX];
    float price;
    int amount;
    Date exp_date;
} Article;

int read_int() {
    char buff[RECORD_MAX];
    int value;
    fgets(buff, RECORD_MAX, stdin);
    sscanf(buff, "%d", &value);

    return value;
}

time_t create_time(const Date d){
    struct tm date = {0};
    date.tm_year = d.year - 1900;
    date.tm_mon = d.month - 1;
    date.tm_mday = d.day;

    return mktime(&date);
}

Date unpack_time(const time_t time){
    Date d;
    struct tm date = *localtime(&time);
    d.year = (int)date.tm_year + 1900;
    d.month = (int)date.tm_mon + 1;
    d.day = (int)date.tm_mday;

    return d;
}

int cmp_name(const void *a, const void *b){
    Article *a_article = (Article *)a;
    Article *b_article = (Article *)b;

    return strcmp(a_article->name, b_article->name);
}

int cmp_date(const void *a, const void *b) {
    Article *a_article = (Article *)a;
    Article *b_article = (Article *)b;

    time_t a_time = create_time(a_article->exp_date);
    time_t b_time = create_time(b_article->exp_date);

    if (a_time < b_time) return -1;
    if (a_time > b_time) return 1;
    return 0;
}

int cmp(const void *a, const void *b){
    Article *a_article = (Article *)a;
    Article *b_article = (Article *)b;

    int strcmp_answer = strcmp(a_article->name, b_article->name);

    if (strcmp_answer != 0) return strcmp_answer < 0 ? -1: 1;
    if (a_article->price < b_article->price) return -1;
    if (a_article->price > b_article->price) return 1;
    return cmp_date(a, b);
}

void* bsearch2 (const void *key, const void *base, const size_t n_items,
        const size_t size, const CompareFp compare, void **insertion_p) {
        if (n_items == 0) {
            *insertion_p = (void *)base;
            return NULL;
        }

        size_t a = 0, b = n_items-1, mid;
        const char *arr = (const char*)base;

        while (a <= b){
            mid = a + (b-a)/2;
            int cmp_result = compare((void*)(arr + mid * size), key);
            if (cmp_result < 0){
                a = mid+1;
            }
            else if(cmp_result == 0){
                *insertion_p = (void *)(arr + mid * size);
                return (void*)(arr + mid * size);
            }
            else{
                if (mid == 0) break;
                b = mid-1;
            }
        }
        *insertion_p = (void *)(arr + a * size);
        return NULL;
}

void add_record(Article *tab, int *article_count, const CompareFp compare, const Article *new_article) {
    if(compare == NULL){
        tab[*article_count] = *new_article;
        (*article_count)++;
    }
    else{
        void *insertion_p;
        void *bsearch_result = bsearch2(new_article, tab, *article_count, sizeof(Article), compare, &insertion_p);
        if (bsearch_result != NULL){
            Article *bsearch_result_food = (Article *)bsearch_result;
            bsearch_result_food->amount += new_article->amount;
        }
        else{
            Article *insertion_point_food = (Article *)insertion_p;
            
            int insert_index = (insertion_point_food - tab);
            for (int i = *article_count; i > insert_index; --i) {
                tab[i] = tab[i-1];
            }
            tab[insert_index] = *new_article;
            (*article_count)++;
        }
    }
}

int read_goods(Article *tab, FILE *stream, const int sorted) {
    char buffer[RECORD_MAX];
    int line_count = read_int(), i = 0, article_count = 0;
    
    while (i < line_count && fgets(buffer, RECORD_MAX, stream)){
        Article new_article;
        Date new_date;

        char *name = strtok(buffer, " ");
        strcpy(new_article.name, name);
        new_article.price = atof(strtok(NULL, " "));
        new_article.amount = atoi(strtok(NULL, " "));

        char *date_string = strtok(NULL, " ");
        new_date.day = atoi(strtok(date_string, "."));
        new_date.month = atoi(strtok(NULL, "."));
        new_date.year = atoi(strtok(NULL, "."));
        new_article.exp_date = new_date;
        
        add_record(tab, &article_count, sorted ? cmp : NULL, &new_article);
        i++;
    }
    return article_count;
}

void print_art(const void *tab, const int n, const char* art_name){
    const Article *tab_article = (Article *)tab;
    Article art;
    strcpy(art.name, art_name);

    void *bsearch_answer = bsearch(art_name, tab, n, sizeof(Article), cmp_name);
    if (bsearch_answer == NULL) return;

    Article *bsearch_answer_article = (Article *)bsearch_answer;
    int index = bsearch_answer_article-tab_article;
    Article article;

    while (index > 0){
        if (cmp_name(tab_article[index-1].name, art.name) == 0) index--;
        else break;
    }
    for (int i = index; i < n; ++i){
        if (cmp_name(tab_article[i].name, art.name) == 0){
            article = tab_article[i];
            printf("%.2f %d %02d.%02d.%04d\n", article.price, article.amount, article.exp_date.day, article.exp_date.month, article.exp_date.year);
        }
        else break;
    }
}

float calculate_value(Article *tab_article, const size_t n, const Date curr_date, int days) {
    qsort(tab_article, n, sizeof(Article), cmp_date);
    float value = 0;

    Article art;
    time_t future_time=create_time(curr_date) + days*24*60*60;
    Date future_date = unpack_time(future_time);
    art.exp_date = future_date;

    void *bsearch_answer = bsearch(&art, tab_article, n, sizeof(Article), cmp_date);
    if (bsearch_answer == NULL) return 0;

    Article *bsearch_answer_article = (Article *)bsearch_answer;
    int index = bsearch_answer_article-tab_article;
    Article article;

    for (int i = index-1; i >= 0; --i){
        article = tab_article[i];
        time_t article_time = create_time(article.exp_date);
        if(article_time == future_time) value += article.price*article.amount;
        else break;
    }
    for (int i = index; i < n; ++i){
        article = tab_article[i];
        time_t article_time = create_time(article.exp_date);
        if(article_time == future_time) value += article.price*article.amount;
        else break;
    }
    return value;
}

int main(void) {
    int n;
    Article food_tab[FOOD_MAX];
    char buff[RECORD_MAX];
    const int to_do = read_int();

    switch (to_do) {
        case 1:  //bsearch
            n = read_goods(food_tab, stdin, 1);
            scanf("%s", buff);
            print_art(food_tab, n, buff);
            break;
        case 2: //qsort
            n = read_goods(food_tab, stdin, 0);
            Date curr_date;
            int days;
            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
            scanf("%d", &days);
            printf("%.2f\n", calculate_value(food_tab, (size_t)n, curr_date, days));
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}