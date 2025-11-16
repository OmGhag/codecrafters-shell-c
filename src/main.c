#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  // Flush after every printf
  setbuf(stdout, NULL);
  // Capture the user's command in the "command" variable
    printf("$ ");
  const int max_size = 1024;
  char inp[max_size];
  while (fgets(inp, max_size, stdin) != NULL) {

    // check if input is given
    inp[strlen(inp) - 1] = '\0';
    printf("%s: command not found\n", inp);
    printf("$ ");
  }
  return 0;
}
