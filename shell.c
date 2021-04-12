#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 /* The maximum length command */

// gets rid of \n at the end of the function
void chomp(char *s)
{
    while (*s && *s != '\n' && *s != '\r')
        s++;

    *s = 0;
}

int main()
{
    char *args[MAX_LINE]; /* command line arguments */

    int should_run = 1; /* flag to determine when to exit program */

    int count = 0;

    while (should_run)
    {

        //PROMPT THE USER
        printf("osh> ");
        fflush(stdout);

        // TAKE INPUT
        char input[80];
        fgets(input, 80, stdin);
        char *ptr;
        ptr = strtok(input, " ");

        // FORMAT INPUT TO CLEAR STRINGS
        int index = 0;
        while (ptr != NULL)
        {
            chomp(ptr);
            args[index] = ptr;
            index = index + 1;
            ptr = strtok(NULL, " ");
        }

        // DETERMINE IF EXIT COMMAND OR REPEAT COMMAND 
        const char *e = "exit";
        const char *repeat = "!!";
        if (strcmp(args[0], e) == 0)
        {
            should_run = 0;
            continue;
        }
        else if (strcmp(args[0], repeat) == 0)
        {
            printf("repeat the process \n");
            if (count == 0)
            {
                printf("ERROR -> You can not repeat command on first try");
                should_run = 0;
                continue;
            }
            else
            {
                printf("PEFORM LAST COMMAND \n");
                should_run = 0;
                continue;
            }
        }

        //After reading user input, the steps are:
        //(1) fork a child process using fork()
        int child = fork();

        //(2) the child process will invoke execvp()

        if (args[2] == NULL) // NO VALUE IN THIRD POSITION
        {
            if (child == 0)
            {
                execvp(args[0], args); // child process performs the command
                exit(0);
            }
            else
            {
                //(3) parent will invoke wait() unless command included &
                wait(0); // parent waits
                printf("* parent waited * \n");
                count++;
            }
        }
        // else if(*args[2] == '&')
        // {
        //     if (child == 0)
        //     {
        //         args[2] = NULL;
        //         execvp(args[0], args); // child process performs the command
        //         exit(0);
        //     }
        //     else
        //     {

        //         sleep(1);
        //         printf("* running concurently * \n");
        //         count++;

        //     }
        // }

        //printf("debug");
    }

    return 0;
}