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


/* Constants */ 

#define LINELEN 1024

/* Prototypes */

void processline (char *line);
char** arg_parse(char* line, int* argcptr);

/* Shell main */

int main (void)
{
    // char line[] = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '\0'};
    // int num = 0;
    // char** args = arg_parse(line,&num);

    // for (int i = 0; i < num; i++)
    // {
    //     printf("%s\n",*args);
    //     args++;
    // }
    char   buffer [LINELEN];
    int    len;

    while (1) {

        /* prompt and get line */
	fprintf (stderr, "%% ");
	if (fgets (buffer, LINELEN, stdin) != buffer)
	  break;

        /* Get rid of \n at end of buffer. */
	len = strlen(buffer);
	if (buffer[len-1] == '\n')
	    buffer[len-1] = 0;

	/* Run it ... */
	processline (buffer);

    }

    if (!feof(stdin))
        perror ("read");

    return 0;		/* Also known as exit (0); */
}


void processline (char *line)
{
    pid_t  cpid;
    int    status;
    
    int argc;
    char** argv = arg_parse(line,&argc);

    if (argc == 0)
    {
        return;
    }

    /* Start a new process to do the job. */
    cpid = fork();
    if (cpid < 0) {
      /* Fork wasn't successful */
      perror ("fork");
      return;
    }
    
    /* Check for who we are! */
    if (cpid == 0) {
      /* We are the child! */
      execvp(argv[0], argv);
      /* execlp reurned, wasn't successful */
      perror ("exec");
      fclose(stdin);  // avoid a linux stdio bug
      exit (127);
    }
    
    /* Have the parent wait for child to complete */
    if (wait (&status) < 0) {
      /* Wait wasn't successful */
      perror ("wait");
    }

    free(argv);
}


char** arg_parse(char* line, int* argcptr)
{
    *argcptr = 0;
    char* firstPass = line;
    while (1)
    {
        while (*firstPass == ' ')
        {
            firstPass++;
        }
        if (*firstPass == '\0')
        {
            break;
        }

        while (*firstPass != ' ' && *firstPass != '\0')
        {
            firstPass++;
        }
        (*argcptr)++;
        if (*firstPass == '\0')
        {
            break;
        }
        *firstPass = '\0';
        firstPass++;
    }

    char** args = malloc(*argcptr+1);

    args[*argcptr] = NULL;

    char** currentArg = args;

    int argCount = 0;
    while (argCount < *argcptr)
    {
        while (*line == ' ')
        {
            line++;
        }

        *currentArg = line;
        (currentArg)++;
        while (*line != '\0')
        {
            line++;
        }

        line++;
        argCount++;
    }

    return args;
}
    


