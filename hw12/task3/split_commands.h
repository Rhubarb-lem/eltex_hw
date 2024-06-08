#pragma once
#include<stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
void split_commands(char *input, char *commands[], int *num_commands);