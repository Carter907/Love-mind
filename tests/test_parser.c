#include "parser.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_parse_source_single_define(void) {
  char source[] = "define Apple as A sweet fruit";
  directive_list_t *list = parse_source(source);

  TEST_ASSERT_NOT_NULL(list);
  TEST_ASSERT_EQUAL_UINT(1, list->count);
  TEST_ASSERT_EQUAL_STRING("Apple", list->items[0]->term);
  TEST_ASSERT_EQUAL_STRING("A sweet fruit", list->items[0]->definition);

  directive_list_free(list);
}

void test_parse_source_multiple_defines(void) {
  char source[] = "define Term One as Definition one\n"
                  "define Term Two as Definition two";
  directive_list_t *list = parse_source(source);

  TEST_ASSERT_NOT_NULL(list);
  TEST_ASSERT_EQUAL_UINT(2, list->count);
  TEST_ASSERT_EQUAL_STRING("Term One", list->items[0]->term);
  TEST_ASSERT_EQUAL_STRING("Definition one", list->items[0]->definition);
  TEST_ASSERT_EQUAL_STRING("Term Two", list->items[1]->term);
  TEST_ASSERT_EQUAL_STRING("Definition two", list->items[1]->definition);

  directive_list_free(list);
}

void test_parse_source_missing_as(void) {
  char source[] = "define Invalid Concept without keyword";
  directive_list_t *list = parse_source(source);

  TEST_ASSERT_NULL(list);
}

void test_parse_source_empty_term(void) {
  char source[] = "define as Empty term";
  directive_list_t *list = parse_source(source);

  TEST_ASSERT_NULL(list);
}

void test_parse_source_empty_definition(void) {
  char source[] = "define Lone Term as";
  directive_list_t *list = parse_source(source);

  TEST_ASSERT_NULL(list);
}

void test_parse_source_null_input(void) {
  directive_list_t *list = parse_source(NULL);
  TEST_ASSERT_NULL(list);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_parse_source_single_define);
  RUN_TEST(test_parse_source_multiple_defines);
  RUN_TEST(test_parse_source_missing_as);
  RUN_TEST(test_parse_source_empty_term);
  RUN_TEST(test_parse_source_empty_definition);
  RUN_TEST(test_parse_source_null_input);
  return UNITY_END();
}
