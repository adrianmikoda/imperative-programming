//approximation of the area in between the x axis and the sine function graph using the Monte Carlo method

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
double a,b;
int n;

struct point{
    double x, y;
};
typedef struct point point;

void input(void)
{
    printf("start of the interval a[pi]: ");
    scanf("%lf", &a);

    printf("end of the interval b[pi]: ");
    scanf("%lf", &b);

    printf("accuracy: ");
    scanf("%d", &n);
}

point gen_random_point(void)
{
    point rand_point;

    double rand_point_x = a+(b-a)*((double)rand()/RAND_MAX);
    double rand_point_y = 2*((double)rand()/RAND_MAX)-1;

    rand_point.x = rand_point_x*M_PI;
    rand_point.y = rand_point_y;

    return rand_point;
}

double monte_carlo(void)
{
    double area = (b-a)*2*M_PI;
    int points_hit = 0;
    for(int i = 0; i < n; i++){
        point rand_point = gen_random_point();
        double sin_val = sin(rand_point.x);

        //checking if the point is between the x axis and the sine function graph
        if(sin_val <= 0 && rand_point.y >= sin_val && rand_point.y <= 0){
            points_hit++;
        }
        if(sin_val >= 0 && rand_point.y <= sin_val && rand_point.y >= 0)
        {
            points_hit++;
        }
    }
    double x = (double)points_hit/n;
    return(x*area);
}

int main(void)
{
    input();
    if(b < a){
        printf("Error: b must be greater or equal to a");
    }
    else{
        srand(time(NULL)); //use time in seconds to set seed
        printf("%lf", monte_carlo());
    }
    return 0;
}