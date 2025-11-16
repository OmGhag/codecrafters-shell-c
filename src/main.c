#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);  // Disable buffering for immediate output
    
    printf("$ ");
    char inp[MAX_INPUT_SIZE];

    while (fgets(inp, MAX_INPUT_SIZE, stdin) != NULL) {
        // Remove trailing newline safely
        size_t len = strlen(inp);
        if (len > 0 && inp[len - 1] == '\n') {
            inp[len - 1] = '\0';
        }

        // Exit condition
        if (strncmp(inp, "exit", 4) == 0) {
            return 0;   // exits with code 0
        }


        printf("%s: command not found\n", inp);
        printf("$ ");
    }

    return 0;
}
