#ifndef ARG_H_
#define ARG_H_

#include "argtable2.h"
#include <stdlib.h>

#include "libpre/config.h"
#include "libpre/log.h"
#include "libpre/macros.h"

struct pre_config pre_args_to_config(int argc, char **argv);
void pre_args_free(void);

#endif
