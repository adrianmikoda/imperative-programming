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
} Food;

void print_product(const Food product){
        printf("%s %f %d %d.%d.%d\n", product.name, product.price, product.amount, product.exp_date.day, product.exp_date.month, product.exp_date.year);
}

void print_tab(const Food *tab, int np){
    printf("\n");
    for (int i =0; i < np; i++){
        print_product(tab[i]);
    }
}

int read_int() {
    char buff[RECORD_MAX];
    int value;
    fgets(buff, RECORD_MAX, stdin);
    sscanf(buff, "%d", &value);
    return value;
}

int cmp_date(const void *a, const void *b) {
    Food *a_food = (Food *)a;
    Food *b_food = (Food *)b;

    Date a_date = a_food->exp_date;
    Date b_date = b_food->exp_date;

    struct tm date1 = {0};
    struct tm date2 = {0};

    date1.tm_year = a_date.year - 1900;
    date1.tm_mon = a_date.month - 1;
    date1.tm_mday = a_date.day;

    date2.tm_year = b_date.year - 1900;
    date2.tm_mon = b_date.month - 1;
    date2.tm_mday = b_date.day;
    time_t t1 = mktime(&date1);
    time_t t2 = mktime(&date2);

    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

int cmp(const void *a, const void *b){
    Food *product_a = (Food *)a;
    Food *product_b = (Food *)b;

    int strcmp_answer = strcmp(product_a->name, product_b->name);
    if (strcmp_answer != 0) return strcmp_answer < 0 ? -1: 1;
    if (product_a->price < product_b->price) return -1;
    if (product_a->price > product_b->price) return 1;
    return cmp_date(a, b);
}

void* bsearch2 (const void *key, const void *base, const size_t n_items,
        const size_t size, const CompareFp compare, void **insertion_point) {
        if (n_items == 0) {
            *insertion_point = (void *)base;
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
            else if(cmp_result == 0) return (void*)(arr + mid * size);
            else{
                if (mid == 0) break;
                b = mid-1;
            }
        }
        *insertion_point = (void *)(arr + a * size);
        return NULL;
}

void add_record(Food *tab, int *product_count, const CompareFp compare, const Food *new_product) {
    if(compare == NULL){
        tab[*product_count] = *new_product;
        (*product_count)++;
    }
    else{
        void *insertion_point;
        void *bsearch_result = bsearch2(new_product, tab, *product_count, sizeof(Food), compare, &insertion_point);
        if (bsearch_result != NULL){
            Food *bsearch_result_food = (Food *)bsearch_result;
            bsearch_result_food->amount += new_product->amount;
        }
        else{
            Food to_insert = *new_product, temp;
            Food *insertion_point_food = (Food *)insertion_point;

            int insert_index = (insertion_point_food - tab);
            for (int i = *product_count; i > insert_index; i--) {
                tab[i] = tab[i - 1];
            }
            tab[insert_index] = *new_product;
            (*product_count)++;
        }
    }
}

int read_goods(Food *tab, FILE *stream, const int sorted) {
    char buffer[RECORD_MAX];
    int line_count = read_int(), i = 0, product_count = 0;

    while (i < line_count && fgets(buffer, RECORD_MAX, stream)){
        char *name = strtok(buffer, " ");
        Food new_product;
        strcpy(new_product.name, name);

        new_product.price = atof(strtok(NULL, " "));
        new_product.amount = atoi(strtok(NULL, " "));

        Date new_date;
        char *date_string = strtok(NULL, " ");
        new_date.day = atoi(strtok(date_string, "."));
        new_date.month = atoi(strtok(NULL, "."));
        new_date.year = atoi(strtok(NULL, "."));
        new_product.exp_date = new_date;
        
        if(sorted) add_record(tab, &product_count, cmp, &new_product);
        else add_record(tab, &product_count, NULL, &new_product);

        i++;
    }
    return product_count;
}

void print_art(const void *tab, const int n, const char* art_name){
    const Food *tab_food = (Food *)tab;
    Food article;
    for (int i = 0; i < n; i++){
        article = tab_food[i];
        if(strcmp(article.name, art_name) == 0){
            printf("%.2f %d %02d.%02d.%04d\n", article.price, article.amount, article.exp_date.day, article.exp_date.month, article.exp_date.year);
        }
    }
}

float calculate_value(Food *tab_food, const size_t n, const Date curr_date, const int days) {
    qsort(tab_food, n, sizeof(Food), cmp_date);
    struct tm date = {0};
    date.tm_year = curr_date.year - 1900;
    date.tm_mon = curr_date.month - 1;
    date.tm_mday = curr_date.day;
    time_t current_time = mktime(&date);

    struct tm art_date = {0};
    Food *article_food;
    float value = 0;

    for (int i = 0; i < n; i++){
        article_food = tab_food + i;
        art_date.tm_year = article_food->exp_date.year - 1900;
        art_date.tm_mon = article_food->exp_date.month - 1;
        art_date.tm_mday = article_food->exp_date.day;
        time_t article_time = mktime(&art_date);

        if (article_time > current_time+days*24*3600) break;
        if (article_time == current_time+days*24*3600) value += article_food->amount*article_food->price;
    }
    return value;	
}

int main(void) {
    int n;
    Food food_tab[FOOD_MAX];
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