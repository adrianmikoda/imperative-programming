//calculation of simple operations on vectors
#include <stdio.h>

void linspace(double v[],double start, double stop, int n){
    double d = (stop-start)/((double)(n-1));
    double ss = start;
    for(int i=0; i < n; i++){
        v[i] = ss;
        ss+=d;
    }
}

void add(double v1[], double v2[], int n){
    for(int i=0; i < n; i++){
        v1[i]+=v2[i];
    }
}

double dot_product(double v1[], double v2[], int n){
    double prod = 0;
    for(int i=0; i<n; i++){
        prod+=v1[i]*v2[i];
    }
    return prod;
}

void multiply_by_scalar(double v[], int n, double s){
    for(int i=0; i<n; i++){
        v[i]*=s;
    }
}

void range(double v[], int n, double start, double step){
    double ss = start;
    for(int i=0; i<n; i++){
        v[i]=ss;
        ss+=step;
    }
}

void read_vector(double v[], int n){
    for(int i=0; i<n; i++){
        double curr;
        scanf("%lf", &curr);
        v[i] = curr;
    }
}

void print_vector(double v[], int n){
    for(int i=0; i<n; i++){
        printf("%.2lf ", v[i]);
    }
    printf("\n");
}

int main(void){
    int to_do,n;
    scanf("%d", &to_do);
    scanf("%d", &n);
    double start, stop, step, scalar;
    double v1[n], v2[n];

    switch (to_do)
    {
    case 1: //linspace
        scanf("%lf", &start);
        scanf("%lf", &stop);
        linspace(v1, start, stop, n);
        print_vector(v1, n);
        break;
    case 2: //add
        read_vector(v1,n);
        read_vector(v2,n);
        add(v1, v2, n);
        print_vector(v1, n);
        break;
    case 3: //dot_product
        read_vector(v1,n);
        read_vector(v2,n);
        printf("%.2lf\n", dot_product(v1, v2, n));
        break;
    case 4: //multiply_by_scalar
        scanf("%lf", &scalar);
        read_vector(v1, n);
        multiply_by_scalar(v1, n, scalar);
        print_vector(v1, n);
        break;
    case 5: //range
        scanf("%lf", &start);
        scanf("%lf", &step);
        range(v1, n, start, step);
        print_vector(v1, n);
        break;
    default:
        printf("Error: unknown operation %d", to_do);
        break;
    }
    return 0;
}