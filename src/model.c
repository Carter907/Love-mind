#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *join_tokens(char **start, char **end) {
  if (start == NULL || end == NULL || start >= end) {
    return NULL;
  }

  size_t total_len = 0;

  for (char **p = start; p < end; p++) {

    total_len += strlen(*p) + 1; // token length + space or null terminator
  }

  char *str = malloc(total_len);

  if (str == NULL) {
    return NULL;
  }

  str[0] = '\0';

  for (char **p = start; p < end; p++) {

    strcat(str, *p);

    if (p + 1 < end) {
      strcat(str, " ");
    }
  }

  return str;
}

define_node_t *define_node_create(char **term_start, char **term_end,
                                  char **def_start, char **def_end) {
  define_node_t *node = malloc(sizeof(define_node_t));
  if (node == NULL) {
    return NULL;
  }

  node->term = join_tokens(term_start, term_end);
  node->definition = join_tokens(def_start, def_end);

  if (node->term == NULL || node->definition == NULL) {
    define_node_free(node);
    return NULL;
  }

  return node;
}

void define_node_free(define_node_t *node) {
  if (node == NULL) {
    return;
  }
  free(node->term);
  free(node->definition);
  free(node);
}

directive_list_t *directive_list_create(void) {
  directive_list_t *list = malloc(sizeof(directive_list_t));
  if (list == NULL) {
    return NULL;
  }
  list->capacity = 4;
  list->count = 0;
  list->items = malloc(sizeof(define_node_t *) * list->capacity);
  if (list->items == NULL) {
    free(list);
    return NULL;
  }
  return list;
}

bool directive_list_append(directive_list_t *list, define_node_t *node) {
  if (list == NULL || node == NULL) {
    return false;
  }
  if (list->count >= list->capacity) {
    size_t new_cap = list->capacity * 2;
    define_node_t **resized =
        realloc(list->items, sizeof(define_node_t *) * new_cap);
    if (resized == NULL) {
      return false;
    }
    list->items = resized;
    list->capacity = new_cap;
  }
  list->items[list->count++] = node;
  return true;
}

void directive_list_free(directive_list_t *list) {
  if (list == NULL) {
    return;
  }
  for (size_t i = 0; i < list->count; i++) {
    define_node_free(list->items[i]);
  }
  free(list->items);
  free(list);
}
