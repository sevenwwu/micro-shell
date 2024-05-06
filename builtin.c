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
void cd_builtin(char**,int);
void shift_builtin(char**,int);
void unshift_builtin(char**,int);

static char* builtInCommandNames[] = {"exit", "envset", "envunset", "cd","shift","unshift"};
static func_ptr builtInFunctions[] = {exit_builtin,envset_builtin,envunset_builtin,cd_builtin,shift_builtin,unshift_builtin};
static int numOfBuiltIns = 6;

int tryExecuteBuiltin(char** argv, int argc)
{
    if (argc == 0)
    {
        return -1;
    }
    for (int i = 0; i < numOfBuiltIns; i++)
    {
        if (strcmp(builtInCommandNames[i],argv[0]) == 0)
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

    char* envValue = "";
    if (argc > 2)
    {
        envValue = argv[2];
    }


    if (setenv(argv[1], envValue, 1) != 0)
    {
        perror("failed to set environment variable");
    }
}

void envunset_builtin(char** argv, int argc)
{
    if (argc < 2)
    {
        return;
    }

    unsetenv(argv[1]);
}

void cd_builtin(char** argv, int argc)
{
    if (argc < 2)
    {
        chdir(getenv("HOME"));
        return;
    }

    if (argc == 2)
    {
        if (chdir(argv[1]))
        {
            perror("failed to change directory");
        }
    }
    else
    {
        perror("invalid directory");
    }
}


void shift_builtin(char** argv, int argc)
{
    int shiftNum = 1;
    if (argc >= 2)
    {
        shiftNum = atoi(argv[1]);
    }

    shift += shiftNum;
}

void unshift_builtin(char** argv, int argc)
{
    int shiftNum = shift;
    if (argc >= 2)
    {
        shiftNum = atoi(argv[1]);
    }

    shift -= shiftNum;
}


