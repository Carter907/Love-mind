#include "tokenizer.h"
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {}
void tearDown(void) {}

void test_tokenizer_single_token(void) {
  char input[] = "define";
  char **tokens = tokenizer(input);

  TEST_ASSERT_NOT_NULL(tokens);
  TEST_ASSERT_EQUAL_STRING("define", tokens[0]);
  TEST_ASSERT_NULL(tokens[1]);

  free_tokens(tokens);
}

void test_tokenizer_directive_sentence(void) {
  char input[] = "define Concept as Definition";
  char **tokens = tokenizer(input);

  TEST_ASSERT_NOT_NULL(tokens);
  TEST_ASSERT_EQUAL_STRING("define", tokens[0]);
  TEST_ASSERT_EQUAL_STRING("Concept", tokens[1]);
  TEST_ASSERT_EQUAL_STRING("as", tokens[2]);
  TEST_ASSERT_EQUAL_STRING("Definition", tokens[3]);
  TEST_ASSERT_NULL(tokens[4]);

  free_tokens(tokens);
}

void test_tokenizer_whitespace_and_newlines(void) {
  char input[] = "set   Entity \t has \n - item1\n - item2";
  char **tokens = tokenizer(input);

  TEST_ASSERT_NOT_NULL(tokens);
  TEST_ASSERT_EQUAL_STRING("set", tokens[0]);
  TEST_ASSERT_EQUAL_STRING("Entity", tokens[1]);
  TEST_ASSERT_EQUAL_STRING("has", tokens[2]);
  TEST_ASSERT_EQUAL_STRING("-", tokens[3]);
  TEST_ASSERT_EQUAL_STRING("item1", tokens[4]);
  TEST_ASSERT_EQUAL_STRING("-", tokens[5]);
  TEST_ASSERT_EQUAL_STRING("item2", tokens[6]);
  TEST_ASSERT_NULL(tokens[7]);

  free_tokens(tokens);
}

void test_tokenizer_empty_string(void) {
  char input[] = "";
  char **tokens = tokenizer(input);

  TEST_ASSERT_NOT_NULL(tokens);
  TEST_ASSERT_NULL(tokens[0]);

  free_tokens(tokens);
}

void test_tokenizer_null_input(void) {
  char **tokens = tokenizer(NULL);
  TEST_ASSERT_NULL(tokens);
}

void test_tokenizer_realloc_growth(void) {
  // Test input with more than 16 tokens to verify dynamic array reallocation
  char input[256] = "t01 t02 t03 t04 t05 t06 t07 t08 t09 t10 t11 t12 t13 t14 t15 t16 t17 t18 t19 t20";
  char **tokens = tokenizer(input);

  TEST_ASSERT_NOT_NULL(tokens);
  TEST_ASSERT_EQUAL_STRING("t01", tokens[0]);
  TEST_ASSERT_EQUAL_STRING("t16", tokens[15]);
  TEST_ASSERT_EQUAL_STRING("t20", tokens[19]);
  TEST_ASSERT_NULL(tokens[20]);

  free_tokens(tokens);
}

void test_tokenizer_preserves_commas(void) {
  char input[] = "define Apple as A sweet, red fruit";
  char **tokens = tokenizer(input);

  TEST_ASSERT_NOT_NULL(tokens);
  TEST_ASSERT_EQUAL_STRING("define", tokens[0]);
  TEST_ASSERT_EQUAL_STRING("Apple", tokens[1]);
  TEST_ASSERT_EQUAL_STRING("as", tokens[2]);
  TEST_ASSERT_EQUAL_STRING("A", tokens[3]);
  TEST_ASSERT_EQUAL_STRING("sweet,", tokens[4]);
  TEST_ASSERT_EQUAL_STRING("red", tokens[5]);
  TEST_ASSERT_EQUAL_STRING("fruit", tokens[6]);
  TEST_ASSERT_NULL(tokens[7]);

  free_tokens(tokens);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_tokenizer_single_token);
  RUN_TEST(test_tokenizer_directive_sentence);
  RUN_TEST(test_tokenizer_whitespace_and_newlines);
  RUN_TEST(test_tokenizer_empty_string);
  RUN_TEST(test_tokenizer_null_input);
  RUN_TEST(test_tokenizer_realloc_growth);
  RUN_TEST(test_tokenizer_preserves_commas);
  return UNITY_END();
}
