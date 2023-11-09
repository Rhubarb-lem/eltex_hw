#include<stdio.h>
#define N 6
int main(){
    int arr[N][N],  count = 1;
    for(int i = 0; i < N; i++){
        for(int j = 0; j<N; j++){
            arr[j][i] = count;
            count ++;
        
        }
    }
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            printf("%d ",arr[j][i]); 
            
        
        }
        printf("\n");
    }
    return 0;
}