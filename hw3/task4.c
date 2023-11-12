#include<stdio.h>

int main(){
    char str1[20], str2[5];
    char *ptr = NULL;
    int flag = 0;
    printf("Enter first string(length 20) \n");
    scanf("%s", str1);
    printf("Enter second string(length 5)\n");
    scanf("%s", str2);
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 5; j++){
            if(str1[i+j] != str2[j]){
                flag = 0;
                break;
            }
            flag = 1;
        }
        if(flag){
            ptr = str1+i;
        }
        
    }
    printf("%p\n", ptr);
}