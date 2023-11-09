#include <stdio.h>

int main()
{
    int num, sum = 0;
    printf("Enter positive integer number:\n");
    scanf("%d", &num);
    if (num < 0)
    {
        printf("Wrong number!\n");
        return 0;
    }
    for(int i = sizeof(int)*8-1; i>=0; i--){
        if((num>>i)&1){
            sum++;
        }
    }
    printf("Number of 1 in binary representation of number: %d\n", sum);
    return 0;
}