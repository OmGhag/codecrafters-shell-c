#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  //Capture the user's command in the "command" variable
  char command[1024];
   fgets(command, sizeof(command), stdin);
   // Remove trailing newline character if present
   command[strcspn(command, "\n")] = '\0';

   //Print the "<command>": "command not found" message
   printf("command not found: %s\n", command);

  return 0;
}
