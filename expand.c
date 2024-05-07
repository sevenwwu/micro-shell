#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "defn.h"

char* dollarExpand(char* buffer, int bufferSize);
char* envExpand(char* buffer, int bufferSize, char* orig, int* origIndex);
char* hashExpand(char* buffer);
char* argumentExpand(char* buffer, int bufferSize, char* orig, int* origIndex);

int expand(char* orig, char* new, int newsize)
{   
    int origIndex = 0;
    int newIndex = 0;

    while (orig[origIndex] != '\0')
    {
        if (orig[origIndex] == '$')
        {   
            int bufferSize = 32;
            char buffer[bufferSize];
            char* writeToNew;
            origIndex++;
            switch (orig[origIndex])
            {
                case '$':
                    writeToNew = dollarExpand(buffer,bufferSize);
                    break;
                case '{':
                    writeToNew = envExpand(buffer,bufferSize,orig,&origIndex);
                    if (writeToNew == NULL)
                    {
                        return -1;
                    }
                    break;
                case '#':
                    writeToNew = hashExpand(buffer);
                    break;
                case '0': case '1': case '2': case '3': case '4': 
                case '5': case '6': case '7': case '8': case '9':
                    writeToNew = argumentExpand(buffer,bufferSize,orig,&origIndex);
                    break;
                default:
                    buffer[0] = '$';
                    buffer[1] = orig[origIndex];
                    buffer[2] = '\0';
                    writeToNew = buffer;
                    break;
            }
            while (*writeToNew != '\0')
            {
                new[newIndex] = *writeToNew;
                newIndex++;
                writeToNew++;
            }
            origIndex++;
        }
        else
        {
            if (newIndex >= newsize)
            {
                fprintf(stderr, "Expanded String Exceeds Buffer Size\n");
                return 1;
            }
            new[newIndex] = orig[origIndex];
            newIndex++;
            origIndex++;
        }
    }

    if (newIndex >= newsize)
    {
        fprintf(stderr, "Expanded String Exceeds Buffer Size\n");
        return 1;
    }

    new[newIndex] = '\0';

    return 0;
}

char* dollarExpand(char* buffer, int bufferSize)
{
    pid_t pid = getpid();
    snprintf(buffer, bufferSize, "%d", pid);
    return buffer;
}

char* envExpand(char* buffer, int bufferSize, char* orig, int* origIndex)
{
    char *environmentVariable = &orig[*origIndex + 1];
    while (orig[*origIndex] != '}' && orig[*origIndex] != '\0')
    {
        (*origIndex)++;
    }

    if (orig[*origIndex] == '\0')
    {
        fprintf(stderr, "Invalid Variable Syntax\n");
        return NULL;
    }

    orig[*origIndex] = '\0';
    char* returnVal = getenv(environmentVariable);
    if (returnVal == NULL)
    {
        buffer[0] = '\0';
        returnVal = buffer;
    }
    orig[*origIndex] = '}';

    return returnVal;
}

char* hashExpand(char* buffer)
{
    int numberOfRemainingArguments = mainargc - shift - 1;
    if (mainargc == 1)
    {
        numberOfRemainingArguments = mainargc - shift;
    }
    sprintf(buffer, "%d", numberOfRemainingArguments);
                    
    return buffer;
}

char* argumentExpand(char* buffer, int bufferSize, char* orig, int* origIndex)
{
    int currentIndex = *origIndex;
    while (orig[currentIndex] != '\0' && strchr("0123456789", orig[currentIndex]) != NULL)
    {
        currentIndex++;
    }

    char origChar = orig[currentIndex];
    orig[currentIndex] = '\0';

    char *numStr = &(orig[*origIndex]);

    int argumentIndex = atoi(numStr) + 1 + shift;

    if (atoi(numStr) == 0)
    {
        argumentIndex = 1;
    }
    
    if (atoi(numStr) == 0 && mainargc == 1)
    {
        argumentIndex = 0;
    }

    orig[currentIndex] = origChar;

    char* returnVal;

    if (argumentIndex >= mainargc)
    {
        buffer[0] = '\0';
        returnVal = buffer;
    }
    else
    {
        char *arg = mainargv[argumentIndex];

        returnVal = arg;
        *origIndex = currentIndex - 1;
    }

    return returnVal;
}