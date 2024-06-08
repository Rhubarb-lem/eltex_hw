#include"split_args.h"
void split_args(char *command, char *args[])
{
    char *token = strtok(command, " \t\n");
    int i = 0;
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}