/* CS 352 -- Micro Shell!  
 *
 *   Sept 21, 2000,  Phil Nelson
 *   Modified April 8, 2001 
 *   Modified January 6, 2003
 *   Modified January 8, 2017
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "defn.h"


/* Constants */ 

#define LINELEN 1024

/* Prototypes */

void processline (char *line);
char** arg_parse(char* line, int* argcptr);

/* Shell main */

int main(void)
{   
    char buffer[LINELEN];
    int len;

    while (1)
    {

        /* prompt and get line */
        fprintf(stderr, "%% ");
        if (fgets(buffer, LINELEN, stdin) != buffer)
            break;

        /* Get rid of \n at end of buffer. */
        len = strlen(buffer);
        if (buffer[len - 1] == '\n')
            buffer[len - 1] = 0;

        /* Run it ... */
        processline(buffer);
    }

    if (!feof(stdin))
        perror("read");

    return 0; /* Also known as exit (0); */
}

void processline(char *line)
{
    pid_t cpid;
    int status;

    int argc;

    char expandedLine[LINELEN];

    if (expand(line,expandedLine,LINELEN) != 0)
    {
        return;
    }

    char **argv = arg_parse(expandedLine, &argc);

    if (argc == 0)
    {
        return;
    }

    

    if (tryExecuteBuiltin(argv,argc) != 0)
    {
        /* Start a new process to do the job. */
        cpid = fork();
        if (cpid < 0)
        {
            /* Fork wasn't successful */
            perror("fork");
            return;
        }

        /* Check for who we are! */
        if (cpid == 0)
        {
            /* We are the child! */
            execvp(argv[0], argv);
            /* execlp reurned, wasn't successful */
            perror("exec");
            fclose(stdin); // avoid a linux stdio bug
            exit(127);
        }

        /* Have the parent wait for child to complete */
        if (wait(&status) < 0)
        {
            /* Wait wasn't successful */
            perror("wait");
        }
    }

    free(argv);
}

char** arg_parse(char* line, int* argcptr)
{
    *argcptr = 0;
    int i = 0;
    int onWhiteSpace = 1;
    int inQuotes = 0;
    while (line[i] != '\0')
    {
        if (line[i] == ' ' && !inQuotes)
        {   
            if (!onWhiteSpace)
            {
                onWhiteSpace = 1;
                line[i] = '\0';
            }
            i++;
            continue;
        }
        
        if (onWhiteSpace)
        {
            (*argcptr)++;
            onWhiteSpace = 0;
        }

        if (line[i] == '"')
        {
            if (inQuotes)
            {
                inQuotes = 0;
            }
            else
            {
                inQuotes = 1;
            }
        }

        i++;
    }

    if (inQuotes)
    {
        fprintf(stderr, "Quote never closed\n");
        return NULL;
    }

    char** args = malloc(*argcptr+1);
    args[*argcptr] = '\0';

    int argIndex = 0;
    int lineIndex = 0;

    int argCount = 0;
    while (argCount < *argcptr)
    {
        while (line[lineIndex] == ' ')
        {
            lineIndex++;
        }

        args[argIndex] = &(line[lineIndex]);
        argIndex++;
        while (line[lineIndex] != '\0')
        {
            if (line[lineIndex] == '"')
            {
                int shift = lineIndex;
                while (line[shift] != '\0')
                {
                    line[shift] = line[shift+1];
                    shift++;
                }
                line[shift] = ' ';
                lineIndex--;
            }
            lineIndex++;
        }

        lineIndex++;
        argCount++;
    }

    return args;
}
    


