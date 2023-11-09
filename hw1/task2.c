#include <stdio.h>

int main()
{
    int num;
    char bit;
    printf("Enter negative integer number:\n");
    scanf("%d", &num);
    if (num >= 0)
    {
        printf("Wrong number!\n");
        return 0;
    }
    printf("Binary representation of number:\n");
    for(int i = sizeof(int)*8-1; i>=0; i--){
        printf("%d", (num>>i)&1);
    }
    printf("\n");

    return 0;
}