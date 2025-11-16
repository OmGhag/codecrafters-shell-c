#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 64
int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);  // Disable buffering for immediate output
    
    printf("$ ");
    char inp[MAX_INPUT_SIZE];
    char* args[MAX_ARG_SIZE];

    while (fgets(inp, MAX_INPUT_SIZE, stdin) != NULL) {
        // Remove trailing newline safely
        size_t len = strlen(inp);
        if (len > 0 && inp[len - 1] == '\n') {
            inp[len - 1] = '\0';
        }

        char* command = strtok(inp, " ");
        if (strcmp(command, "echo") == 0){
          printf("%s\n", inp + 5);
          printf("$ ");
        }else if (strncmp(inp, "exit", 4) == 0) {
            return 0;   // exits with code 0
        }else{
          printf("%s: command not found\n", inp);
          printf("$ ");
        }
    }

    return 0;
}
