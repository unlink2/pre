#ifndef ERROR_H_
#define ERROR_H_

#include "libpre/macros.h"
#include <stddef.h>
#include <errno.h>
#include <stdio.h>

/**
 * Error is a error type struct that provides
 * an errno-style interface.
 */

// Possible error types.
// Some may or may not require special error info
enum pre_error {
  PRE_OK = 0,
  // errno is stored in err_detail
  PRE_ERRNO
};

#define pre_errno()                                                           \
  { pre_err_set(PRE_ERRNO); }

#define pre_err_fset(err, ...)                                                \
  {                                                                            \
    pre_err_set((err));                                                       \
    pre_error(__VA_ARGS__);                                                   \
  }

// sets err to a value
// to indicate a failure in a call
void pre_err_set(enum pre_error err);

void pre_err_print(FILE *f);

// return error code
enum pre_error pre_err(void);

#endif
