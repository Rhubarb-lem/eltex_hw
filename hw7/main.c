#include<stdio.h>
#include"add.h"
#include"sub.h"
#include"mul.h"
#include"div.h"

void print_menu()
{
    printf("Select an option\n\n");
    printf("1) Addition\n");
    printf("2) Subtraction\n");
    printf("3) Multiplication\n");
    printf("4) Division\n");
    printf("5) Exit\n");
}


int main(){
    int option, a, b;
    while (1)
    {
        print_menu();
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Enter first parametr:\n");
            scanf("%d", &a);
            printf("Enter second parametr\n");
            scanf("%d", &b);
            add(a, b);
            break;
        case 2:
            printf("Enter first parametr:\n");
            scanf("%d", &a);
            printf("Enter second parametr\n");
            scanf("%d", &b);
            sub(a, b);
            break;
        case 3:
            printf("Enter first parametr:\n");
            scanf("%d", &a);
            printf("Enter second parametr\n");
            scanf("%d", &b);
            mul(a, b);
            break;
        case 4:
            printf("Enter first parametr:\n");
            scanf("%d", &a);
            printf("Enter second parametr\n");
            scanf("%d", &b);
            div(a, b);
            break;
        case 5:
        printf("End of programm\n");
                return 0;
                break;

            default:
                printf("Unknow option!\n");
                break;
        }
    }
}

