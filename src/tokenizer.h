#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>

typedef enum TOKEN { TEXT, DEFINE, AS, HAS, SET, COMMA, NEW_LINE } token_e;

/**
 * Splits a whitespace-delimited source string into an array of tokens.
 * Modifies the source string in-place (via strtok).
 * Returns a NULL-terminated array of char pointers.
 */
char **tokenizer(char *source);

/**
 * Frees the dynamically allocated token pointer array.
 */
void free_tokens(char **tokens);

#endif // TOKENIZER_H
