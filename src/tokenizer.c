#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>

char **tokenizer(char *source) {
  if (source == NULL) {
    return NULL;
  }

  size_t capacity = 16;
  size_t count = 0;

  char **tokens = malloc(sizeof(char *) * capacity);
  if (tokens == NULL) {
    return NULL;
  }

  char *tk = strtok(source, " \t\r\n");

  while (tk != NULL) {

    if (count + 1 >= capacity) {

      capacity *= 2;

      char **resized = realloc(tokens, sizeof(char *) * capacity);

      if (resized == NULL) {
        free(tokens);
        return NULL;
      }

      tokens = resized;
    }

    tokens[count++] = tk;

    tk = strtok(NULL, " \t\r\n");
  }

  tokens[count] = NULL;
  return tokens;
}

void free_tokens(char **tokens) { free(tokens); }
