#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 128
#define MAX_ARGS 64

extern char **environ;

static const char *valid_commands[] = {"echo", "exit", "type"};

typedef struct
{
  char *input;
  uint8_t input_size;
  bool valid_input;
} InputBuffer;

typedef struct
{
  char *args[MAX_ARGS];
  uint8_t arg_count;
} CommandArgs;

InputBuffer Create_Input_Buffer()
{
  InputBuffer input_buffer;

  input_buffer.input = calloc(MAX_BUFFER_SIZE, sizeof(char));
  input_buffer.input_size = 0;
  input_buffer.valid_input = false;

  return input_buffer;
}

void free_input_buffer(InputBuffer *input_buffer)
{
  free(input_buffer->input);
  input_buffer->input = NULL;
}

CommandArgs Create_Command_Args(InputBuffer *input_buffer)
{
  CommandArgs command_args;
  char *token = strtok(input_buffer->input, " ");
  uint8_t index = 0;

  while (token != NULL && index < MAX_ARGS)
  {
    command_args.args[index++] = token;
    token = strtok(NULL, " ");
  }
  command_args.arg_count = index;
  command_args.args[index] = NULL;

  return command_args;
}

bool Is_Command_Valid(const char *command, const char *valid_commands[], size_t valid_command_count)
{
  for (size_t i = 0; i < valid_command_count; i++)
  {
    if (strcmp(command, valid_commands[i]) == 0)
    {
      return true;
    }
  }
  return false;
}

char *search_executable_in_path(const char *arg)
{
  char *path_env = getenv("PATH");
  if (path_env == NULL)
  {
    return NULL;
  }

  char *path_copy = strdup(path_env);
  if (path_copy == NULL)
  {
    return NULL;
  }

  char *token = strtok(path_copy, ":");
  while (token != NULL)
  {
    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", token, arg);
    if (access(full_path, X_OK) == 0)
    {
      free(path_copy);
      return strdup(full_path);
    }
    token = strtok(NULL, ":");
  }
  free(path_copy);
  return NULL;
}

uint8_t capture_input(InputBuffer *input_buffer)
{
  if (fgets(input_buffer->input, MAX_BUFFER_SIZE, stdin) == NULL)
  {
    input_buffer->input_size = 0;
    return 0;
  }

  size_t len = strlen(input_buffer->input);
  while (len > 0 && (input_buffer->input[len - 1] == '\n' || input_buffer->input[len - 1] == '\r'))
  {
    input_buffer->input[--len] = '\0';
  }
  input_buffer->input_size = (uint8_t)len;

  return input_buffer->input_size;
}

void execute_external_command(const char *full_path, char *const argv[])
{
  pid_t pid = fork();
  if (pid < 0)
  {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0)
  {
    execve(full_path, argv, environ);
    perror("execve");
    exit(EXIT_FAILURE);
  }
  else
  {
    waitpid(pid, NULL, 0);
  }
}

bool check_input(InputBuffer *input_buffer)
{
  CommandArgs command_args = Create_Command_Args(input_buffer);
  char *command = command_args.args[0];

  if (command == NULL)
  {
    input_buffer->valid_input = false;
    return false;
  }

  if (strcmp(command, "echo") == 0)
  {
    input_buffer->valid_input = true;
    for (uint8_t i = 1; i < command_args.arg_count; i++)
    {
      printf("%s", command_args.args[i]);
      if (i < command_args.arg_count - 1)
      {
        printf(" ");
      }
    }
    printf("\n");
  }
  else if (strcmp(command, "exit") == 0)
  {
    input_buffer->valid_input = true;
    free_input_buffer(input_buffer);
    exit(EXIT_SUCCESS);
  }
  else if (strcmp(command, "type") == 0)
  {
    input_buffer->valid_input = true;
    if (command_args.arg_count < 2)
    {
      printf("type: missing argument\n");
    }
    else
    {
      char *arg = command_args.args[1];
      bool is_valid = Is_Command_Valid(arg, valid_commands, sizeof(valid_commands) / sizeof(valid_commands[0]));
      if (is_valid)
      {
        printf("%s is a shell builtin\n", arg);
      }
      else
      {
        char *exe_path = search_executable_in_path(arg);
        if (exe_path != NULL)
        {
          printf("%s is %s\n", arg, exe_path);
          free(exe_path);
        }
        else
        {
          printf("%s: not found\n", arg);
        }
      }
    }
  }
  else
  {
    char *exe_path = search_executable_in_path(command);
    if (exe_path != NULL)
    {
      input_buffer->valid_input = true;
      execute_external_command(exe_path, command_args.args);
      free(exe_path);
    }
    else
    {
      input_buffer->valid_input = false;
    }
  }

  return input_buffer->valid_input;
}

void print_input_buffer(InputBuffer *input_buffer)
{
  printf("input: %s\n", input_buffer->input);
  printf("input_size: %d\n", input_buffer->input_size);
  printf("valid_input: %d\n", input_buffer->valid_input);
}

int main(void)
{
  setbuf(stdout, NULL);

  printf("$ ");
  InputBuffer input_buffer = Create_Input_Buffer();

  while (capture_input(&input_buffer))
  {
    check_input(&input_buffer);
    if (!input_buffer.valid_input)
    {
      printf("%s: command not found\n", input_buffer.input);
    }
    free_input_buffer(&input_buffer);
    input_buffer = Create_Input_Buffer();
    printf("$ ");
  }
  return EXIT_SUCCESS;
}