#ifndef LOG_H_
#define LOG_H_

#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

enum pre_loglevel {
  PRE_LOG_LEVEL_OFF,
  PRE_LOG_LEVEL_ERROR,
  PRE_LOG_LEVEL_WARN,
  PRE_LOG_LEVEL_INFO,
  PRE_LOG_LEVEL_DBG,
};

void pre_log_init(enum pre_loglevel level);

extern enum pre_loglevel PRE_LOG_LEVEL;
extern FILE *PRE_LOG_TARGET;

#define pre_dbg_assert(a) assert((a))
#define pre_log_fprintf(f, level, ...)                                        \
  if ((level) <= PRE_LOG_LEVEL) {                                             \
    /* fprintf((f), "[%d] ", level); */                                        \
    fprintf((f), __VA_ARGS__);                                                 \
  }

#define pre_dbg(...) pre_log_fprintf(stderr, PRE_LOG_LEVEL_DBG, __VA_ARGS__)
#define pre_warn(...)                                                         \
  pre_log_fprintf(stderr, PRE_LOG_LEVEL_WARN, __VA_ARGS__)
#define pre_info(...)                                                         \
  pre_log_fprintf(stderr, PRE_LOG_LEVEL_INFO, __VA_ARGS__)
#define pre_error(...)                                                        \
  pre_log_fprintf(stderr, PRE_LOG_LEVEL_ERROR, __VA_ARGS__)

#define pre_panic(...)                                                        \
  {                                                                            \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(-1);                                                                  \
  }
#define pre_todo(...)                                                         \
  { pre_panic(__VA_ARGS__); }

#endif
