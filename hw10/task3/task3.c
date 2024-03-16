#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(){
    char exeprog[30];
    int status;
    //canf("29%s", exeprog);
    while(1){
        scanf("%s", exeprog);
        if(!strcmp(exeprog, "exit")){
            break;
        }
        else{
            pid_t pid = fork();
            if(pid == 0){
                execl(exeprog,"", NULL);
                exit(5);
            }
            else{
                wait(&status);
            }
        }
    }
}