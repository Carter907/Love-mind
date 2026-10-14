#ifndef PARSER_H
#define PARSER_H

#include "model.h"

/**
 * Parses a tokenized stream into a directive list.
 * Returns NULL on critical syntax error or memory failure.
 */
directive_list_t *parse_tokens(char **tokens);

/**
 * High-level helper: tokenizes source text, parses directives,
 * and frees the intermediate token array.
 */
directive_list_t *parse_source(char *source);

#endif // PARSER_H
