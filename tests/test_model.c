#include "model.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_define_node_create_valid(void) {
  char *term_tokens[] = {"Regular", "Expression"};
  char *def_tokens[] = {"A", "pattern"};

  define_node_t *node = define_node_create(term_tokens, term_tokens + 2,
                                           def_tokens, def_tokens + 2);

  TEST_ASSERT_NOT_NULL(node);
  TEST_ASSERT_EQUAL_STRING("Regular Expression", node->term);
  TEST_ASSERT_EQUAL_STRING("A pattern", node->definition);

  define_node_free(node);
}

void test_define_node_create_null_or_empty(void) {
  char *tokens[] = {"Word"};
  define_node_t *n1 = define_node_create(NULL, tokens + 1, tokens, tokens + 1);
  TEST_ASSERT_NULL(n1);

  define_node_t *n2 = define_node_create(tokens, tokens, tokens, tokens + 1);
  TEST_ASSERT_NULL(n2);
}

void test_directive_list_lifecycle(void) {
  directive_list_t *list = directive_list_create();
  TEST_ASSERT_NOT_NULL(list);
  TEST_ASSERT_EQUAL_UINT(0, list->count);
  TEST_ASSERT_TRUE(list->capacity >= 4);

  char *term[] = {"Concept"};
  char *def[] = {"Definition"};
  define_node_t *node = define_node_create(term, term + 1, def, def + 1);
  TEST_ASSERT_NOT_NULL(node);

  TEST_ASSERT_TRUE(directive_list_append(list, node));
  TEST_ASSERT_EQUAL_UINT(1, list->count);
  TEST_ASSERT_EQUAL_PTR(node, list->items[0]);

  directive_list_free(list);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_define_node_create_valid);
  RUN_TEST(test_define_node_create_null_or_empty);
  RUN_TEST(test_directive_list_lifecycle);
  return UNITY_END();
}
