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

void copy(char* org[], char* copy[]){

    for(int i = 0; org[i] != '\0'; i++){
        copy[i] = org[i];
    }
}

void clear(char* org[]){
     for(int i = 0; org[i] != '\0'; i++){
         org[i] = '\0';
     }
}

int main(int argc, char **argv)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    char *cache[MAX_LINE / 2 + 1]; // cache for repeat

    int should_run = 1;           /* flag to determine when to exit program */

    int count = 0;

    int z = 0;
    while (should_run)
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
        
        
        const char *e = "exit";
        const char *repeat = "!!";

        if(strcmp(args[0], e) == 0){
            should_run = 0;
            exit(0);
        } else if(strcmp(args[0], repeat) == 0){
            printf("repeat the process \n");
            if(count == 0){
                printf("ERROR -> You can not repeat command on first try");
                exit(0);
            }else{
                copy(cache, args);
            }
        }

        //After reading user input, the steps are:
        //(1) fork a child process using fork()
        int child = fork();

        //(2) the child process will invoke execvp()

        if (args[2] == NULL) // any value in third position
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
                copy(args, cache);
                clear(args);
    
            }
        }
        else if(*args[2] == '&')
        {
            if (child == 0)
            {
                args[2] = NULL;
                execvp(args[0], args); // child process performs the command
                exit(0);
            }
            else
            {

                sleep(1);
                printf("* running concurently * \n");
                count++;
                copy(args, cache);
                clear(args);

            }
        }
    }

    return 0;
}