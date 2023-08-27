#include <stdio.h>
#include <stdlib.h>
#include "libpre/config.h"
#include "libpre/log.h"
#include "arg.h"

int main(int argc, char **argv) {
  struct pre_config cfg = pre_args_to_config(argc, argv);
  int res = pre_main(&cfg);
  pre_args_free();
  return res;
}
