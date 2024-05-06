#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "defn.h"


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
                    pid_t pid = getpid();
                    snprintf(buffer, sizeof(buffer), "%d", pid);
                    writeToNew = buffer;
                    break;
                case '{':
                    char* environmentVariable = &orig[origIndex+1];
                    while (orig[origIndex] != '}' && orig[origIndex] != '\0')
                    {
                        origIndex++;
                    }

                    if (orig[origIndex] == '\0')
                    {
                        fprintf(stderr, "Invalid Variable Syntax\n");
                        return 1;
                    }

                    orig[origIndex] = '\0';
                    writeToNew = getenv(environmentVariable);
                    if (writeToNew == NULL)
                    {
                        buffer[0] = '\0';
                        writeToNew = buffer;
                    }
                    orig[origIndex] = '}';
                    break;
                case '#':
                    sprintf(buffer, "%d", mainargc - shift - 1);
                    
                    writeToNew = buffer;
                    break;
                default:
                    int currentIndex = origIndex;
                    while (orig[currentIndex] != '\0' && strchr("0123456789",orig[currentIndex]) != NULL)
                    {
                        currentIndex++;
                    }

                    if (currentIndex != origIndex)
                    {
                        char numStr[currentIndex - origIndex];
                        strncpy(numStr,&orig[origIndex],currentIndex - origIndex);
                        
                        int argumentIndex = atoi(numStr) + 1 + shift;

                        if (atoi(numStr) == 0)
                        {
                            argumentIndex = 1;
                        }

                        if (argumentIndex >= mainargc)
                        {
                            buffer[0] = '\0';
                            writeToNew = buffer;
                            break;
                        }
                        char* arg = mainargv[argumentIndex];

                        writeToNew = arg;
                        origIndex = currentIndex-1;
                    }
                    else
                    {
                        buffer[0] = '$';
                        buffer[1] = orig[origIndex];
                        buffer[2] = '\0';
                        writeToNew = buffer;
                    }

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

