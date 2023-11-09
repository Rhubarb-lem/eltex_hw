#include <stdio.h>

int main()
{
    int num;
    char bit;
    printf("Enter positive integer number:\n");
    scanf("%d\n", &num);
    printf("Enter value of bit:\n");
    scanf("%c\n", &bit);
    if (num < 0)
    {
        printf("Wrong number!\n");
        return 0;
    }
    printf("Changed number:");
    if(bit){
        printf("%d\n", num|(1<<3));
    }
    else{
        printf("%d\n", num&~(1<<3));
    }
    

    return 0;
}