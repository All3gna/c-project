//                                   simple calculator

#include <stdio.h>
   int main()
   
   {
    double n1;
    double n2;
    char operation;

    printf("                                    Simple calculator--free version\n");

    printf("\nEntre your math : ");
    if(scanf("%lf %c %lf",&n1 ,&operation,&n2) !=3)
    {
        printf("Refine your expression");
        return 1;
    }

    switch(operation){

        case '+' :
         printf(" Sum : %.1lf", n1+n2);
         break;

        case '-' :
         printf("Difference : %.1lf", n1-n2);
         break;

        case '*' : 
         printf("Product : %.1lf", n1*n2);
         break;

        case '/' :
        if(n2==0){
            printf("math error");
            return 1;
        }
        printf("Quotient : %.1lf", n1/n2);
        break;


        default :
        printf("Syntax error");
        return 1;

    }
    printf("\n          Build with precision by ubuntu ");
    return 0;
   }