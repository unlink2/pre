#ifndef PRE_H__
#define PRE_H__

#include "stdbool.h"

#define PRE_DIR_ENV "PRE_DIR"
#define PRE_DIR_PATH_SEP "/"
#define PRE_DIR_NAME "pre"
#define PRE_REG_IDENT '@'
#define PRE_REG_DEFAULT "@a"

#define PRE_BUF_LEN 1028

enum pre_fmode {
  PRE_READ,
  PRE_WRITE,
  PRE_APPEND
};

struct pre_config {
  bool verbose;
  bool no_echo;
  const char *reg_path;

  const char *reg;
  enum pre_fmode mode;
  
  const char *out;
  const char *in;
};

const char *pre_sfmode(enum pre_fmode mode);
char *pre_join(char *dst, const char *path_sep, const char *suffix);
struct pre_config pre_cfg_defaults(void);
void pre_cfg_free(struct pre_config *cfg);

int pre_main(struct pre_config *cfg);

#endif
