#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

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

  directive_list_t *def_list = parse_source(source);

  if (def_list != NULL) {
    for (size_t i = 0; i < def_list->count; i++) {
      define_node_t *node = def_list->items[i];
      printf("Successfully parsed 'define' directive:\n  Term: \"%s\"\n  "
             "Definition: \"%s\"\n",
             node->term, node->definition);
    }
  }

  directive_list_free(def_list);
  free(source);

  return 0;
}
