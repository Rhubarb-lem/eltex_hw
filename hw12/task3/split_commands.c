#include"split_commands.h"
void split_commands(char *input, char *commands[], int *num_commands)
{
    char *token = strtok(input, "|");
    *num_commands = 0;
    while (token != NULL)
    {
        commands[(*num_commands)++] = token;
        token = strtok(NULL, "|");
    }
}