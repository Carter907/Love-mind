#include "parser.h"
#include "tokenizer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static token_e get_tok(char *str) {
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

static bool is_directive(token_e tok) { return tok == DEFINE || tok == SET; }

static define_node_t *parse_define(char ***p_ptr) {
  char **p = *p_ptr;
  p++; // Advance past 'define'

  char **term_start = p;

  // Consume concept tokens up to 'as', next directive, or EOF
  while (*p != NULL && get_tok(*p) != AS && !is_directive(get_tok(*p))) {
    p++;
  }

  char **term_end = p;

  // Head must have at least one token
  if (term_start == term_end) {
    fprintf(stderr, "Syntax error: missing term in 'define' directive.\n");
    *p_ptr = p;
    return NULL;
  }

  // Expect connector 'as'
  if (*p == NULL || get_tok(*p) != AS) {
    fprintf(stderr, "Syntax error: expected 'as' in 'define' directive.\n");
    *p_ptr = p;
    return NULL;
  }
  p++; // Move past 'as'

  char **def_start = p;

  // Consume definition tokens until next directive or EOF
  while (*p != NULL && !is_directive(get_tok(*p))) {
    p++;
  }

  char **def_end = p;

  // Body must have at least one token
  if (def_start == def_end) {
    fprintf(stderr,
            "Syntax error: missing definition in 'define' directive.\n");
    *p_ptr = p;
    return NULL;
  }

  define_node_t *node =
      define_node_create(term_start, term_end, def_start, def_end);

  *p_ptr = p;
  return node;
}

directive_list_t *parse_tokens(char **tokens) {
  if (tokens == NULL) {
    return NULL;
  }

  directive_list_t *list = directive_list_create();
  if (list == NULL) {
    return NULL;
  }

  char **p = tokens;

  while (*p != NULL) {
    token_e tok = get_tok(*p);

    if (tok == DEFINE) {
      define_node_t *node = parse_define(&p);
      if (node == NULL) {
        directive_list_free(list);
        return NULL;
      }
      if (!directive_list_append(list, node)) {
        define_node_free(node);
        directive_list_free(list);
        return NULL;
      }
    } else {
      // Advance past unrecognized or unparsed tokens
      p++;
    }
  }

  return list;
}

directive_list_t *parse_source(char *source) {
  if (source == NULL) {
    return NULL;
  }

  char **tokens = tokenizer(source);
  if (tokens == NULL) {
    return NULL;
  }

  directive_list_t *list = parse_tokens(tokens);
  free_tokens(tokens);

  return list;
}
