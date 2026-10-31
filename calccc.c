#include <stdio.h>
  int main(){
    double n1;
    double n2;
    char operation;

    printf("Entre the Question : ");
    if(scanf("%lf %c %lf",&n1 ,&operation ,&n2) !=3){
        printf("Dude 🤣You are the problem!");
        return 1;
    }
    switch(operation){
        case '+' :
        printf("Sum : %.1lf",n1 + n2);
        break;

        case '-' : 
        printf("Difference : %.1lf",n1-n2);
        break;

        case '*' :
        printf("Product : %.1lf",n1 * n2);
        break;

        case '/' :

        if (n2==0){
            printf("I donnot deal with infinites🫥");
            return 1;
        }
        printf("Quotient : %.1lf",n1/n2);
        break;
        default :
        printf("You are so bad at computations,Mind searching for ubuntu on IG 🥹");
        return 1;
    }
    
    return 0;
  }