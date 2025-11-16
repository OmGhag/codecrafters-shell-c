#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  while (1)
  {
    //Capture the user's command in the "command" variable
  printf("$ ");

  char command[1024];
   fgets(command, sizeof(command), stdin);
   // Remove trailing newline character if present
   command[strcspn(command, "\n")] = '\0';

   //Print the "<command>": "command not found" message
   printf("%s: command not found\n", command);

  }  
  return 0;
}
