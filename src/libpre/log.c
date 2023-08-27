#include "libpre/log.h"

#include <stdint.h>

enum pre_loglevel PRE_LOG_LEVEL;
FILE *PRE_LOG_TARGET;

void pre_log_init(enum pre_loglevel level) {
  PRE_LOG_LEVEL = level;
  PRE_LOG_TARGET = stderr;
}
