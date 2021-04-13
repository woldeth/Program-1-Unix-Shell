#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

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

    while (should_run)
    {
        //PROMPT THE USER
        printf("osh> ");
        fflush(stdout);

        // TAKE INPUT
        char input[MAX_LINE];
        fgets(input, MAX_LINE, stdin);

        if (input[0] != '!' && input[1] != '!')
        {

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
        }
        else if (input[0] == '!' && input[1] == '!' && count > 0)
        {
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
        }
        else if (input[0] == '!' && input[1] == '!' && count == 0)
        {
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

        int pipeFD[2];
        pipe(pipeFD);

         //printf("CHECK DEBUGGER");
        if(cache[0] == '\n'){
            cache[0] = '\0';
           // printf("EXTRA ENTER CAPTURED AFTER & \n");
            continue;
        }

        //After reading user input, the steps are:
        //(1) fork a child process using fork()
        int child = fork();

        //(2) the child process will invoke execvp()

        if ((args[1] == NULL && args[2] == NULL) || (args[1] == NULL && *args[1] != '&')) // NO VALUE IN THIRD POSITION
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
                //printf("* parent waited * \n");
                count++;

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);

                //printf(" Debug \n");
            }
        }else if(*args[1] == '&') {

            if (child == 0)
            {
                //args[1] = NULL;
                //execvp(args[0], args); // child process performs the command
                execlp(args[0], args[0], (char *)NULL);
                exit(0);
            }
            else
            {
                //sleep(1);
                //printf("* running concurently * \n");
                count++;

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);
                
            }

        }
        else if (*args[2] == '&')
        {
            if (child == 0)
            {
                args[2] = NULL;
                execvp(args[0], args); // child process performs the command
                exit(0);
            }
            else
            {
                //sleep(1);
                //printf("* running concurently * \n");
                count++;

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);

            }
        }
        else if (*args[1] == '>')
        {

            if (child == 0)
            {
                int fd = open(args[2], O_CREAT | O_TRUNC | O_WRONLY);
                dup2(fd, STDOUT_FILENO);
                args[1] = NULL;
                args[2] = NULL;
                close(fd);
                execvp(args[0], args);

                exit(0);
            }
            else
            {
                wait(0);
                count++;
                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);
            }
        }
        else if (*args[2] == '>')
        { 

            if (child == 0)
            {
                int fd = open(args[3], O_CREAT | O_TRUNC | O_WRONLY);
                dup2(fd, STDOUT_FILENO);

                args[2] = NULL;
                close(fd);
                execvp(args[0], args);

                exit(0);
            }
            else
            {
                wait(0);
                count++;

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);
            }
        }
        else if (*args[1] == '<')
        {

            if (child == 0)
            {
                int fd = open(args[2], O_RDONLY); // file must be there already
                dup2(fd, STDIN_FILENO);
                args[1] = NULL;
                args[2] = NULL;
                close(fd);
                execvp(args[0], args);

                exit(0);
            }
            else
            {
                wait(0);
                count++;

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);
            }
        }
        else if (*args[1] == '|')
        {
            if (child != 0)
            {
                close(pipeFD[0]);
                close(pipeFD[1]);
            }

            if (child == 0)
            {
                int grandChild = fork();

                if (grandChild == 0)
                {                     // child
                    close(pipeFD[0]); // close the read side
                    dup2(pipeFD[1], STDOUT_FILENO);
                    execlp(args[0], args[0], (char *)NULL);
                }
                else
                { // grand child
                    wait(0);
                    close(pipeFD[1]); // close the write side of the pipe for grand child
                    dup2(pipeFD[0], STDIN_FILENO);
                    execlp(args[2], args[2], (char *)NULL);
                }

                exit(0);
            }
            else
            {
                wait(0);
                count++;

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);
            }
        }
        else if (*args[2] == '|')
        {
            if (child != 0)
            {
                close(pipeFD[0]);
                close(pipeFD[1]);
            }

            if (child == 0)
            {
                int grandChild = fork();

                if (grandChild == 0)
                {                     // child
                    close(pipeFD[0]); // close the read side
                    dup2(pipeFD[1], STDOUT_FILENO);
                    args[2] = NULL;
                    execvp(args[0], args);
                    //execlp(args[0], args[0], (char *) NULL);
                }
                else
                { // grand child
                    wait(0);
                    //char buff[4096];
                    //int n = read(pipeFD[0], buff, 4096);
                    //printf("%s", buff);
                    close(pipeFD[1]); // close the write side of the pipe for grand child
                    dup2(pipeFD[0], STDIN_FILENO);
                    execlp(args[3], args[3], (char *)NULL);
                }

                exit(0);
            }
            else
            {
                wait(0);
                count++;

                // clear the input
                clearInput(input);

                // clear the args;
                initPtr(args);
            }
        }
    }

    return 0;
}