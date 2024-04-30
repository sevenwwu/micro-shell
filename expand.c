#include "defn.h"
#include <stdio.h>
#include <sys/types.h>

int expand(char* orig, char* new, int newsize)
{
    int origIndex, newIndex = 0;

    while (orig[origIndex] != '\0')
    {
        if (orig[origIndex] == '$')
        {
            char* writeToNew;
            switch (orig[origIndex])
            {
                case '$':
                    pid_t pid = getpid();
                    break;
                case '{':
                    break;
                default:
                    writeToNew = &'$';
                    break;
            }
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

    return 0;
}