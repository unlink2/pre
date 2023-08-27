#include <libpre/macros.h>
#include <libpre/error.h>
#include <stdlib.h>
#include <string.h>

_Thread_local enum pre_error PRE_ERR;

void pre_err_set(enum pre_error err) { PRE_ERR = err; }

enum pre_error pre_err(void) {
  enum pre_error latest = PRE_ERR;
  return latest;
}
