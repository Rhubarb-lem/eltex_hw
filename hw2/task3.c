#include<stdio.h>
#define N 6
int main(){
    int arr[N][N],  count = N-1;
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            if(j<count){
                arr[j][i] = 0;
            }
            else{
                arr[j][i] = 1;
            }
        }
        count --;
    }
    for(int i = 0; i<N; i++){
        for(int j = 0; j<N; j++){
            printf("%d ",arr[j][i]); 
        }
        printf("\n");
    }
    }