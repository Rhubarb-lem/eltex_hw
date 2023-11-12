#include<stdio.h>

int main(){
    int num;
    char bit;
    char *ptr;
    ptr = &num;

    printf("Enter  integer number:\n");
    scanf("%d", &num);
    printf("\n");
    printf("Enter value of bit:\n");
    scanf("%c\n", &bit);
    printf("\n");

    ptr += 2;
    *ptr = bit;
    ptr -= 2;
    for(int i = 0; i<4; i++){
        printf("%d\n", *ptr);
        ptr++;
    }
    // ptr += 2;
    // *ptr = bit;
    // printf("%d\n", num);
    return 0;

}