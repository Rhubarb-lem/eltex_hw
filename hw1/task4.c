#include <stdio.h>

int main()
{
    int num, bit;
    printf("Enter positive integer number:\n");
    scanf("%d", &num);
    printf("\n");
    if (num < 0)
    {
        printf("Wrong number!\n");
        return 0;
    }
    printf("Enter value of bit:\n");
    scanf("%d", &bit);
    printf("\n");
    if (bit != 0 && bit != 1)
    {
        printf("Wrong value of bit!\n");
        return 0;
    }
    if (bit == 1)
    {
        num |= 1 << 2;
    }
    else
    {
        num &= ~(1 << 2);
    }
    printf("Changed number:%d\n", num);

    return 0;
}