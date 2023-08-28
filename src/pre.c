#include "pre.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>

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
    return pre_join(pre_join(pre_join(strdup(home), PRE_DIR_PATH_SEP, ".local"),
                             PRE_DIR_PATH_SEP, "share"),
                    PRE_DIR_PATH_SEP, PRE_DIR_NAME);
  }

  const char *posix_tmp = getenv("TMPDIR");
  if (posix_tmp) {
    return pre_join(strdup(posix_tmp), PRE_DIR_PATH_SEP, PRE_DIR_NAME);
  }
  // TODO: corss-platform default fallback
  return pre_join(strdup("/tmp"), PRE_DIR_PATH_SEP, PRE_DIR_NAME);
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
  cfg.reg = PRE_REG_DEFAULT;

  return cfg;
}

void pre_cfg_free(struct pre_config *cfg) { free((void *)cfg->reg_path); }

int pre_mkdir(const char *path) {
  struct stat path_stat;

  if (stat(path, &path_stat) == -1 && errno != ENOENT) {
    fprintf(stderr, "%s: ", path);
    perror("stat");
    return -1;
  }

  if (errno == ENOENT && mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s: ", path);
    perror("mkdir");
    return -1;
  }

  return 0;
}

FILE *pre_reg_fopen(const struct pre_config *cfg, const char *reg,
                    const char *mode) {
  size_t len = strnlen(reg, PATH_MAX);

  if (len == 2 && reg[0] == PRE_REG_IDENT && isalnum(reg[1])) {
    char *path = pre_join(strdup(cfg->reg_path), PRE_DIR_PATH_SEP, reg);
    if (cfg->verbose) {
      fprintf(stderr, "Register path: '%s'\n", path);
    }
    FILE *f = fopen(path, mode);
    free(path);
    return f;
  }

  const char *path = reg;
  if (cfg->verbose) {
    fprintf(stderr, "File path: '%s'\n", path);
  }
  return fopen(path, mode);
}

int pre_main(struct pre_config *cfg) {
  if (cfg->verbose) {
    fprintf(stderr, "Register path: '%s'\n", cfg->reg_path);
  }
  if (pre_mkdir(cfg->reg_path) == -1) {
    return EXIT_FAILURE;
  }

  FILE *f = pre_reg_fopen(cfg, cfg->reg, "we");
  if (!f) {
    perror(cfg->reg);
    return EXIT_FAILURE;
  }

  fclose(f);
  return 0;
}
