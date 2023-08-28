#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include "pre.h"

void test_pre_join(void **state) {
  const char *res = pre_join("prefix", "/", "suffix");
  assert_string_equal("prefix/suffix", res);
  free((void*)res);
}

int main(int arc, char **argv) {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_pre_join),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
