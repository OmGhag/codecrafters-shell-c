#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024
int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  // Capture the user's command in the "command" variable
    printf("$ ");
  char inp[MAX_INPUT_SIZE];
  while ( (fgets(inp, MAX_INPUT_SIZE, stdin) != NULL)) {
    // check if input is given
    if (fgets(inp, MAX_INPUT_SIZE, stdin) == "exit" ){
      break;
    }
    inp[strlen(inp) - 1] = '\0';
    printf("%s: command not found\n", inp);
    printf("$ ");
  }
  return 0;
}
