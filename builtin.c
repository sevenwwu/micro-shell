#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "defn.h"

typedef void (*func_ptr)(char**,int);

void exit_builtin(char**,int);
void envset_builtin(char**,int);
void envunset_builtin(char**,int);
void cd(char**,int);

static char* builtInCommandNames[] = {"exit", "envset", "envunset", "cd"};
static func_ptr builtInFunctions[] = {exit_builtin,envset_builtin,envunset_builtin,cd};
static int numOfBuiltIns = 4;

int tryExecuteBuiltin(char** argv, int argc)
{
    if (argc == 0)
    {
        return -1;
    }
    for (int i = 0; i < numOfBuiltIns; i++)
    {
        if (strlen(builtInCommandNames[i]) == strlen(argv[0]) &&
            strcmp(builtInCommandNames[i],argv[0]) == 0)
        {
            builtInFunctions[i](argv,argc);
            return 0;
        }
    }

    return -1;
}

void exit_builtin(char** argv, int argc)
{
    int exit_status = 0;

    if (argc > 1)
    {
        exit_status = atoi(argv[1]);
    }

    exit(exit_status);
}

void envset_builtin(char** argv, int argc)
{
    if (argc < 2)
    {
        return;
    }

    char* envStr = "";
    if (argc > 2)
    {
        envStr = argv[2];
    }
    setenv(argv[1],envStr,1);
}

void envunset_builtin(char** argv, int argc)
{
    if (argc < 2)
    {
        return;
    }

    unsetenv(argv[1]);
}

void cd(char** argv, int argc)
{
    if (argc < 2)
    {
        chdir(getenv("HOME"));
        return;
    }
    char path[256];
    chdir(strcat(strcat(getcwd(path,256),"/"),argv[1]));
}

