#ifndef MODEL_H
#define MODEL_H

#include <stddef.h>

typedef struct {
  char *term;
  char *definition;
} define_node_t;

typedef struct {
  char *term;
  char **elements;
} set_node_t;

typedef struct {
  define_node_t **items;
  size_t count;
  size_t capacity;
} directive_list_t;

define_node_t *define_node_create(char **term_start, char **term_end,
                                  char **def_start, char **def_end);

void define_node_free(define_node_t *node);

directive_list_t *directive_list_create(void);

bool directive_list_append(directive_list_t *list, define_node_t *node);

void directive_list_free(directive_list_t *list);

#endif
