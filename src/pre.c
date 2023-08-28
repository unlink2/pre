#include "pre.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

char *pre_join(char *dst, const char *path_sep, const char *suffix) {
  if (!dst || !suffix || !path_sep) {
    return NULL;
  }

  size_t len = strlen(dst) + strlen(suffix) + strlen(path_sep) + 1;
  char *new_dst = realloc(dst, len);
  strcat(new_dst, path_sep);
  strcat(new_dst, suffix);
  return new_dst;
}

const char *pre_reg_path(void) {
  const char *path = getenv(PRE_DIR_ENV);
  if (path) {
    return strdup(path);
  }

  path = getenv("XDG_DATA_HOME");
  if (path) {
    return pre_join(strdup(path), PRE_DIR_PATH_SEP, PRE_DIR_NAME);
  }

  const char *home = getenv("HOME");
  if (home) {
    return pre_join(pre_join(strdup(home), PRE_DIR_PATH_SEP, ".local"),
                    PRE_DIR_PATH_SEP, "share");
  }

  // TODO: corss-platform default fallback
  return pre_join(strdup(getenv("TMPDIR")), PRE_DIR_PATH_SEP, PRE_DIR_NAME);
}

struct pre_config pre_cfg_defaults(void) {
  struct pre_config cfg;
  memset(&cfg, 0, sizeof(cfg));

  const char *path = pre_reg_path();
  if (!path) {
    fprintf(stderr, "Unable to find valid path for registers!\n");
    exit(EXIT_FAILURE);
  }
  cfg.reg_path = path;

  return cfg;
}

void pre_cfg_free(struct pre_config *cfg) { free((void *)cfg->reg_path); }

int pre_mkdir(const char *path) {
  struct stat path_stat;

  if (stat(path, &path_stat) == -1) {
    perror("stat");
    return -1;
  }

  if (!S_ISDIR(path_stat.st_mode)) {
    fprintf(stderr, "'%s' exists, but is not a directory!\n", path);
    return -1;
  }

  if (mkdir(path, 0777) == -1) {
    perror("mkdir");
    return -1;
  }

  return 0;
}

int pre_main(struct pre_config *cfg) {
  return 0;
}
