#include<stdio.h>
#define N 6
int main(){
    int arr[N], buf;
    printf("Original array:\n");
    for(int i = 0; i<N; i++){
        arr[i] = i+1;
        printf("%d ", arr[i]);
    }
    printf("\n");
    for(int i = 0, j = N - 1; i < j; i++, j--){
        buf = arr[i];
        arr[i] = arr[j];
        arr[j] = buf;
    }
    printf("Reversed array:\n");
    for(int i = 0; i<N; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}