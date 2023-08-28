#ifndef PRE_H__
#define PRE_H__

#include "stdbool.h"

#define PRE_DIR_ENV "PRE_DIR"
#define PRE_DIR_PATH_SEP "/"
#define PRE_DIR_NAME "pre"


struct pre_config {
  bool verbose;
  const char *reg_path;
};

char *pre_join(char *dst, const char *path_sep, const char *suffix);
struct pre_config pre_cfg_defaults(void);
void pre_cfg_free(struct pre_config *cfg);

int pre_main(struct pre_config *cfg);

#endif
