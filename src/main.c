
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 128
#define MAX_ARGS 64
#define MAX_ARG_SIZE 16

static const char *valid_commands[] = {"echo", "exit","type"};
typedef struct {
  char *input;
  uint8_t input_size;
  bool valid_input;
} InputBuffer;

typedef struct {
  char *args[MAX_ARGS];
  uint8_t arg_count;
} CommandArgs;

InputBuffer Create_Input_Buffer() {
  InputBuffer input_buffer;

  input_buffer.input = calloc(MAX_BUFFER_SIZE, sizeof(char));
  input_buffer.input_size = 0;
  input_buffer.valid_input = false;

  return input_buffer;
}


void free_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->input);
  input_buffer->input = NULL;
}

CommandArgs Create_Command_Args(InputBuffer *input_buffer) {
  CommandArgs command_args;
  char *token = strtok(input_buffer->input, " ");
  uint8_t index = 0;

  while (token != NULL && index < MAX_ARGS) {
    command_args.args[index++] = token;
    token = strtok(NULL, " ");
  }
  command_args.arg_count = index;

  return command_args;
}

bool Is_Command_Valid(const char *command, const char *valid_commands[], size_t valid_command_count) {
  for (size_t i = 0; i < valid_command_count; i++) {
    if (strcmp(command, valid_commands[i]) == 0) {
      return true;
    }    
  }
  return false;
}

char *search_executable_in_path(const char *arg) {
  char *path_env = getenv("PATH");
  if (path_env == NULL) {
    return NULL;
  }

  char *path_copy = strdup(path_env);
  char *token = strtok(path_copy, ":");
  while (token != NULL) {
    char full_path[MAX_BUFFER_SIZE];
    snprintf(full_path, sizeof(full_path), "%s/%s", token, arg);
    if (access(full_path, X_OK) == 0) {
      free(path_copy);
      return strdup(full_path);
    }
    token = strtok(NULL, ":");
  }
  free(path_copy);
  return NULL;
}

uint8_t capture_input(InputBuffer *input_buffer) {
  fgets(input_buffer->input, MAX_BUFFER_SIZE, stdin);
  input_buffer->input_size = strlen(input_buffer->input) - 1;
  input_buffer->input[input_buffer->input_size] = '\0';

  return input_buffer->input_size;
}

bool check_input(InputBuffer *input_buffer) {
  CommandArgs command_args = Create_Command_Args(input_buffer);
  char *command = command_args.args[0];
  if (strcmp(command, "echo") == 0) {
    input_buffer->valid_input = true;
    for (size_t i = 1; i < command_args.arg_count; i++) {
      printf("%s", command_args.args[i]);
      if (i < command_args.arg_count - 1) {
        printf(" ");
      }
    }
    printf("\n");
  } else if (strcmp(command, "exit") == 0) {
    input_buffer->valid_input = true;
    free_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  } else if(strcmp(command, "type") == 0) {
    input_buffer->valid_input = true;
    char *arg;
    if (command_args.arg_count < 2) {
      printf("type: missing argument\n");
      return false;
    }else {
    arg = command_args.args[1];
    }
    bool is_valid = Is_Command_Valid(arg, valid_commands, sizeof(valid_commands) / sizeof(valid_commands[0]));
    if (is_valid) {
      printf("%s is a shell builtin\n", arg);
    } else {
      char *exe_path = search_executable_in_path(arg);
      if (exe_path != NULL) {
        printf("%s is %s\n", arg, exe_path);
        free(exe_path);
      }else{
        printf("%s: not found\n", arg);
        input_buffer->valid_input = true;
      }
    
  }
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
    free_input_buffer(&input_buffer);
    input_buffer = Create_Input_Buffer();
    printf("$ ");
  }
  return EXIT_SUCCESS;
}