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

void clearInput(char input[])
{
    int index = 0;

    while (index < MAX_LINE)
    {
        input[index] = '\0';
        index++;
    }
}

void initPtr(char *input[])
{
    int init = 0;

    while (init < MAX_LINE / 2 + 1)
    {
        input[init] = NULL;
        init++;
    }
}

int main()
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    char cache[MAX_LINE];

    initPtr(args);

    int should_run = 1; /* flag to determine when to exit program */

    int count = 0;

    while (should_run) {

        //PROMPT THE USER
        printf("osh> ");
        fflush(stdout);

        // TAKE INPUT
        char input[MAX_LINE];
        fgets(input, MAX_LINE, stdin);

        if (input[0] != '!' && input[1] != '!') {

            memcpy(cache, input, 80);

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

        } else if(input[0] == '!' && input[1] == '!' && count > 0){
            memcpy(input, cache, 80);

             // FORMAT INPUT TO CLEAR STRINGS
            char *ptr;
            ptr = strtok(input, " ");
            int index = 0;
            while (ptr != NULL)
            {
                chomp(ptr);
                args[index] = ptr;
                index = index + 1;
                ptr = strtok(NULL, " ");
            }

        } else if(input[0] == '!' && input[1] == '!' && count == 0){
            printf("ERROR -> YOU CAN NOT REPEAT COMMAND ON FIRST TRY");
            should_run = 0;
            exit(0);
        }

        // DETERMINE IF EXIT COMMAND OR REPEAT COMMAND
        const char *e = "exit";
        //const char *repeat = "!!";

        if (strcmp(args[0], e) == 0)
        {
            should_run = 0;
            exit(0);
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

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);

                printf(" Debug \n");
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

        // printf(" ");
    }

    return 0;
}