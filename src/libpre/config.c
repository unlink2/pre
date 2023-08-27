#include "libpre/config.h"
#include <string.h>

struct pre_config pre_config_init(void) {
  struct pre_config c;
  memset(&c, 0, sizeof(c));

  return c;
}

int pre_main(struct pre_config *cfg) {
  return 0;
}
