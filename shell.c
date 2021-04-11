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

int main(int argc, char **argv)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;           /* flag to determine when to exit program */

    int z = 0;
    while (z < 1) //should_run)
    {
        printf("osh> ");
        fflush(stdout);

        char input[80];

        fgets(input, 80, stdin);

        char *ptr;
        ptr = strtok(input, " ");

        int index = 0;

        while (ptr != NULL)
        {
            chomp(ptr);
            args[index] = ptr;
            index = index + 1;
            //printf("%s\n", ptr);
            ptr = strtok(NULL, " ");
        }

        //After reading user input, the steps are:
        //(1) fork a child process using fork()
        int child = fork();

        //(2) the child process will invoke execvp()

        if (*args[0] == '&') // any value in third position
        {

            if (child == 0)
            {
                args[2] = NULL;
                execvp(args[0], args); // child process performs the command
                exit(0);
            }
            else
            {
                printf("* running concurently *");
                sleep(1);
                exit(0);
            }
        }
        else
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
                exit(0);
            }
        }

        z = z + 1;
    }

    return 0;
}