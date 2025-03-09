//calculation of the value of the cosine function using the Taylor series expansion

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

double a,b,dx=1.0;
int n;

void input(void)
{
    printf("start of the interval a[pi]: ");
    scanf("%f", &a);
    printf("end of the interval b[pi]: ");
    scanf("%f", &b);
    printf("step size Dx[pi]: ");
    scanf("%f", &dx);
    printf("Taylor series accuracy (1-20): ");
    scanf("%d", &n);
}

double cos_taylor_series(double x)
{
    double ans = 1;
    double mult = -1;
    long long factorial = 2;
    double x_power = x*x;

    for(int i =2; i<=n; i+=2){
        ans += mult*x_power/factorial;
        factorial *= (i+1)*(i+2);
        x_power *= x*x;
        mult *= -1;
    }
    return ans;
}

void calc(void)
{
    printf("\n%-10s %-20s %-20s\n", "x", "cos(x) (expected)", "cos(x) (Taylor series)");
    for(float i = a; i <= b; i+=dx){
        printf("%-10f %-20f %-20f\n", i, cos(i*M_PI), cos_taylor_series(i*M_PI));
    }
}

int main(void)
{
    input();
    if(b < a) {
        printf("Error: b must be greater or equal to a");
    }
    else {
        calc();
    }
    return 0;

}

