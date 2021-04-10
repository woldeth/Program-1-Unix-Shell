#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length command */

int main(int argc, char** argv)
{
  char *args[MAX_LINE/2 + 1]; /* command line arguments */
  int should_run = 1; /* flag to determine when to exit program */

  int i = 0;

  printf("osh> ");
  printf(argv[0]);
  printf(" ");
  printf(argv[1]);
  printf(" ");
  printf(argv[2]);
  printf("\n");


  while(argv[i] != NULL){
      args[i] = argv[i + 1];
      i = i + 1;
  }

  printf("osh> ");
  printf(args[0]);
  printf(" ");
  printf(args[1]);
  printf(" ");
  printf(args[2]);

  execvp(args[0], args);
  
//   printf("osh> ");
//   printf(args[0]);
//   printf(" ");
//   printf(args[1]);
  
  //execvp(args[0], );


//   while (should_run) {
//     printf("osh>");
//     fflush(stdout);

//     /**
//     * After reading user input, the steps are:
//     * (1) fork a child process using fork()
//     * (2) the child process will invoke execvp()
//     * (3) parent will invoke wait() unless command included &
//     */
//   }
  return 0;
}