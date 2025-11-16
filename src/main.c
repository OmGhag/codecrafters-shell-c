#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 128

typedef struct {
  char *input;
  uint8_t input_size;
  bool valid_input;
} InputBuffer;

InputBuffer Create_Input_Buffer() {
  InputBuffer input_buffer;

  input_buffer.input = calloc(MAX_BUFFER_SIZE, sizeof(char));
  input_buffer.input_size = 0;
  input_buffer.valid_input = false;

  return input_buffer;
}

uint8_t capture_input(InputBuffer *input_buffer) {
  fgets(input_buffer->input, MAX_BUFFER_SIZE, stdin);
  input_buffer->input_size = strlen(input_buffer->input) - 1;
  input_buffer->input[input_buffer->input_size] = '\0';

  return input_buffer->input_size;
}

bool check_input(InputBuffer *input_buffer) {
  char *command = strtok(input_buffer->input, " ");
  if (strcmp(command, "echo") == 0) {
    input_buffer->valid_input = true;
    printf("%s\n", input_buffer->input + strlen(command) + 1);
  } else if (strcmp(command, "exit") == 0) {
    input_buffer->valid_input = true;
    exit(EXIT_SUCCESS);
  } else {
    input_buffer->valid_input = false;
  }

  return input_buffer->valid_input;
}

void print_input_buffer(InputBuffer *input_buffer) {
  printf("input: %s\n", input_buffer->input);
  printf("input_size: %d\n", input_buffer->input_size);
  printf("valid_input: %d\n", input_buffer->valid_input);
}

int main(void) {
  setbuf(stdout, NULL);

  printf("$ ");
  InputBuffer input_buffer = Create_Input_Buffer();

  while (capture_input(&input_buffer)) {
    check_input(&input_buffer);
    if (input_buffer.valid_input) {
      ;
    } else {
      printf("%s: command not found\n", input_buffer.input);
    }

    printf("$ ");
  }
  return EXIT_SUCCESS;
}