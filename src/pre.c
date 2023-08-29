#include "pre.h"
#include <asm-generic/errno-base.h>
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

bool pre_isdir(const char *path) {
  struct stat pstat;
  if (stat(path, &pstat) == -1) {
    return false;
  }

  return S_ISDIR(pstat.st_mode);
}

const char *pre_reg_path(void) {
  const char *path = getenv(PRE_DIR_ENV);
  if (path && pre_isdir(path)) {
    return strdup(path);
  }

  path = getenv("XDG_DATA_HOME");
  if (path && pre_isdir(path)) {
    return pre_join(strdup(path), PRE_DIR_PATH_SEP, PRE_DIR_NAME);
  }

  const char *home = getenv("HOME");
  if (home && pre_isdir(home)) {
    return pre_join(pre_join(pre_join(strdup(home), PRE_DIR_PATH_SEP, ".local"),
                             PRE_DIR_PATH_SEP, "share"),
                    PRE_DIR_PATH_SEP, PRE_DIR_NAME);
  }

  const char *posix_tmp = getenv("TMPDIR");
  if (posix_tmp && pre_isdir(posix_tmp)) {
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
  if (mkdir(path, 0777) == -1 && errno != EEXIST) {
    fprintf(stderr, "%s: ", path);
    perror("mkdir");
    return -1;
  }

  if (!pre_isdir(path)) {
    fprintf(stderr, "%s: is not a directory\n", path);
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
      fprintf(stderr, "Register path: '%s',%s\n", path, mode);
    }
    FILE *f = fopen(path, mode);
    free(path);
    return f;
  }

  const char *path = reg;
  if (cfg->verbose) {
    fprintf(stderr, "File path: '%s',%s\n", path, mode);
  }
  return fopen(path, mode);
}

const char *pre_sfmode(enum pre_fmode mode) {
  switch (mode) {
  case PRE_READ:
    return "re";
  case PRE_WRITE:
    return "we";
  case PRE_APPEND:
    return "a";
  }

  return "re";
}

int pre_read(struct pre_config *cfg, FILE *f, FILE *out) {
  char buf[PRE_BUF_LEN];
  memset(buf, 0, PRE_BUF_LEN);

  size_t read = 0;
  while ((read = fread(buf, 1, PRE_BUF_LEN, f)) > 0) {
    if (fwrite(buf, 1, read, out) == -1) {
      perror("");
      return EXIT_FAILURE;
    }
  }

  if (read == -1) {
    perror("");
    return EXIT_FAILURE;
  }

  return 0;
}

int pre_write(struct pre_config *cfg, FILE *f, FILE *in, FILE *out) {
  char buf[PRE_BUF_LEN];
  memset(buf, 0, PRE_BUF_LEN);

  size_t read = 0;
  while ((read = fread(buf, 1, PRE_BUF_LEN, in)) > 0) {
    if (!cfg->no_echo && fwrite(buf, 1, read, out) == -1) {
      perror("");
      return EXIT_FAILURE;
    }
    if (fwrite(buf, 1, read, f) == -1) {
      perror("");
      return EXIT_FAILURE;
    }
  }

  return 0;
}

int pre_main(struct pre_config *cfg) {
  if (cfg->verbose) {
    fprintf(stderr, "Register path: '%s'\n", cfg->reg_path);
  }
  if (pre_mkdir(cfg->reg_path) == -1) {
    return EXIT_FAILURE;
  }

  FILE *f = pre_reg_fopen(cfg, cfg->reg, pre_sfmode(cfg->mode));
  if (!f) {
    perror(cfg->reg);
    return EXIT_FAILURE;
  }

  FILE *out = stdout;
  if (cfg->out) {
    if (cfg->verbose) {
      fprintf(stderr, "Output file: '%s'\n", cfg->out);
    }

    out = fopen(cfg->out, "we");
    if (!out) {
      perror(cfg->out);
      return EXIT_FAILURE;
    }
  }

  FILE *in = stdin;
  if (cfg->in) {
    if (cfg->verbose) {
      fprintf(stderr, "Input file: '%s'\n", cfg->in);
    }

    in = fopen(cfg->in, "re");
    if (!in) {
      perror(cfg->in);
      return EXIT_FAILURE;
    }
  }

  int exit_code = 0;
  switch (cfg->mode) {
  case PRE_READ:
    exit_code = pre_read(cfg, f, out);
    break;
  case PRE_WRITE:
  case PRE_APPEND:
    exit_code = pre_write(cfg, f, in, out);
    break;
  }

  fclose(f);
  return exit_code;
}
