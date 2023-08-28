#include "pre.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

const char *pre_join(const char *prefix, const char *path_sep,
                     const char *suffix) {
  if (!prefix || !suffix || !path_sep) {
    return NULL;
  }

  size_t len = strlen(prefix) + strlen(suffix) + strlen(path_sep) + 1;
  char *dst = malloc(len);
  memset(dst, 0, len);

  strcat(dst, prefix);
  strcat(dst, path_sep);
  strcat(dst, suffix);
  return dst;
}

const char *pre_reg_path(void) {
  const char *path = getenv(PRE_DIR_ENV);
  if (path) {
    return strdup(path);
  }

  path = getenv("XDG_DATA_HOME");
  if (path) {
    return pre_join(path, PRE_DIR_PATH_SEP, PRE_DIR_NAME);
  }

  // TODO: corss-platform default fallback
  return pre_join(getenv("TMPDIR"), PRE_DIR_PATH_SEP, PRE_DIR_NAME);
}

struct pre_config pre_cfg_defaults(void) {
  struct pre_config cfg;
  memset(&cfg, 0, sizeof(cfg));

  const char *path = pre_reg_path();
  if (!path) {
    fprintf(stderr, "Unable to find valid path for registers!\n");
    exit(PRE_EXIT_PATH_NOT_FOUND);
  }

  return cfg;
}

void pre_cfg_free(struct pre_config *cfg) { free((void *)cfg->reg_path); }

int pre_mkdir(const char *path) {
  struct stat path_stat;
  if (stat(path, &path_stat) != 0) {
    return 0;
  }

  if (!S_ISDIR(path_stat.st_mode)) {
  }

}

int pre_main(struct pre_config *cfg) { return 0; }
