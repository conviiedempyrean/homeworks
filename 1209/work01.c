#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double add(double n1,double n2)
{
    double n_rt=0.0;
    n_rt=n1+n2;
    return n_rt;
}
double subtract(double n1,double n2)
{
    double n_rt=0.0;
    n_rt=n1-n2;
    return n_rt;
}
double multiply(double n1,double n2)
{
    double n_rt=0.0;
    n_rt=n1*n2;
    return n_rt;
}
double divide(double n1,double n2)
{
    double n_rt=0.0;
    if(n2==0.0)
    {
        printf("errer number\n");
        return 0;
    }
    n_rt=n1/n2;
    return n_rt;
}

int main()
{
double (*ops[4])(double, double) = {add, subtract, multiply, divide};
double n1=0.0,n2=0.0;
int choise=0;

printf("input your first number:\n");
scanf("%lf",&n1);
printf("input your second number:\n");
scanf("%lf",&n2);
printf("Select operation (0=add, 1=subtract, 2=multiply, 3=divide)\n");
scanf("%d",&choise);
printf("result: %lf\n",(ops[choise])(n1,n2));

return 0;
}