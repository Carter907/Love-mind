#include "model.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

token_e get_tok(char *str) {

  if (strcmp(str, "define") == 0) {
    return DEFINE;
  } else if (strcmp(str, "set") == 0) {
    return SET;
  } else if (strcmp(str, "as") == 0) {
    return AS;
  } else if (strcmp(str, "has") == 0) {
    return HAS;
  }
  return TEXT;
}

static char *read_file(FILE *fp) {
  if (fp == NULL) {
    return NULL;
  }
  if (fseek(fp, 0, SEEK_END) != 0) {
    return NULL;
  }
  long file_size = ftell(fp);
  if (file_size < 0) {
    return NULL;
  }
  rewind(fp);

  char *content = malloc((size_t)file_size + 1);
  if (content == NULL) {
    return NULL;
  }

  size_t read_bytes = fread(content, 1, (size_t)file_size, fp);
  content[read_bytes] = '\0';
  return content;
}

static bool is_directive(token_e tok) { return tok == DEFINE || tok == SET; }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "No input file provided.\n");
    exit(EXIT_FAILURE);
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "Failed to open input file.\n");
    exit(EXIT_FAILURE);
  }

  char *source = read_file(fp);
  fclose(fp);

  if (source == NULL) {
    fprintf(stderr, "Failed to read input file.\n");
    exit(EXIT_FAILURE);
  }

  char **tokens = tokenizer(source);
  if (tokens == NULL) {
    free(source);
    return 0;
  }

  char **p = tokens;
  directive_list_t *def_list = directive_list_create();

  while (*p != NULL) {
    if (get_tok(*p) == DEFINE) {
      bool accept = true;
      p++; // Move past 'define'

      char **term_start = p;

      // The connector 'as' is the delimiting factor distinguishing head from
      // body. Consume tokens in head until the connector 'as' or a
      // directive/EOF.
      while (*p != NULL && get_tok(*p) != AS && !is_directive(get_tok(*p))) {
        p++;
      }

      char **term_end = p;

      // Head must have at least one token
      if (term_start == term_end) {
        accept = false;
      }

      // Check for delimiter connector 'as'
      if (*p == NULL || get_tok(*p) != AS) {
        accept = false;
      } else {
        p++; // Move past connector 'as'
      }

      char **def_start = p;

      // Consume tokens in definition body until the next directive keyword or
      // EOF. Words like 'as' or 'has' inside the body are treated as part of
      // the body.
      while (*p != NULL && !is_directive(get_tok(*p))) {
        p++;
      }

      char **def_end = p;

      // Body must have at least one token
      if (def_start == def_end) {
        accept = false;
      }

      if (accept) {
        define_node_t *node =
            define_node_create(term_start, term_end, def_start, def_end);
        if (node != NULL) {
          directive_list_append(def_list, node);
          printf("Successfully parsed 'define' directive:\n  Term: \"%s\"\n  "
                 "Definition: \"%s\"\n",
                 node->term, node->definition);
        } else {
          fprintf(stderr, "Failed to allocate memory for define_node.\n");
          break;
        }
      } else {
        fprintf(stderr, "Syntax error parsing 'define' directive.\n");
        break;
      }
    } else {
      // Advance past non-DEFINE tokens (or advance past unparsed tokens)
      p++;
    }
  }

  directive_list_free(def_list);
  free_tokens(tokens);
  free(source);

  return 0;
}
