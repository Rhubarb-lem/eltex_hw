#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include"split_commands.h"
#include"split_args.h"

#define MAX_ARGS 100



int main()
{
    char input[256];
    char *commands[10];
    int num_commands;

    while (1)
    {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        split_commands(input, commands, &num_commands);

        int pipe_fd[2];
        pid_t pid;
        int fd_in = 0;

        for (int i = 0; i < num_commands; i++)
        {
            pipe(pipe_fd);
            pid = fork();

            if (pid == 0)
            {

                dup2(fd_in, 0);

                if (i < num_commands - 1)
                {
                    dup2(pipe_fd[1], 1);
                }

                close(pipe_fd[0]);

                char *args[MAX_ARGS];
                split_args(commands[i], args);

                execvp(args[0], args);
                perror("Execution error");
                exit(1);
            }
            else
            {
                wait(NULL);
                close(pipe_fd[1]);
                fd_in = pipe_fd[0];
            }
        }
    }

    return 0;
}
