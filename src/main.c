#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pre.h"
#include "argtable2.h"

struct arg_lit *verb = NULL;
struct arg_lit *help = NULL;
struct arg_lit *version = NULL;

struct arg_lit *read = NULL;
struct arg_lit *write = NULL;
struct arg_lit *append = NULL;

struct arg_file *out = NULL;
struct arg_file *in = NULL;

struct arg_file *reg = NULL;

// arg end stores errors
struct arg_end *end = NULL;

#define pre_argtable                                                           \
  { help, version, verb, read, write, append, out, in, reg, end, }

void pre_args_free(void) {
  void *argtable[] = pre_argtable;
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}

void pre_args_parse(int argc, char **argv) {
  help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
  version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
  verb = arg_litn("v", "verbose", 0, 1, "verbose output");

  read = arg_lit0("r", "read", "Read from register");
  write = arg_lit0("w", "write", "Read to register");
  append = arg_lit0("a", "append", "Append to register");

  out = arg_file0("o", "out", "PATH", "Output file");
  in = arg_file0("i", "in", "PATH", "Input file");

  reg = arg_file0(NULL, NULL, "@[A-Za-z0-9]|PATH", "Register");

  end = arg_end(20);

  void *argtable[] = pre_argtable;

  // output params
  char progname[] = "pre";
  char short_desc[] = "";

  // version info
  int version_major = 0;
  int version_minor = 0;
  int version_patch = 1;

  int nerrors = arg_parse(argc, argv, argtable);
  int exitcode = 0;

  if (help->count > 0) {
    printf("Usage: %s", progname);
    arg_print_syntax(stdout, argtable, "\n");
    printf("%s\n\n", short_desc);
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    goto exit;
  }

  if (version->count) {
    printf("%s version %d.%d.%d\n", progname, version_major, version_minor,
           version_patch);
    goto exit;
  }

  if (nerrors > 0) {
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }
  return;
exit:
  pre_args_free();
  exit(exitcode); // NOLINT
}

int main(int argc, char **argv) {
  pre_args_parse(argc, argv);

  // map args to cfg here
  struct pre_config cfg = pre_cfg_defaults();

  if (in->count) {
    cfg.in = in->filename[0];
  }
  if (out->count) {
    cfg.out = out->filename[0];
  }
  if (read->count) {
    cfg.mode = PRE_READ;
  }
  if (write->count) {
    cfg.mode = PRE_WRITE;
  }
  if (append->count) {
    cfg.mode = PRE_APPEND;
  }
  if (reg->count) {
    cfg.reg = reg->filename[0]; 
  }

  cfg.verbose = verb->count > 0;

  int res = pre_main(&cfg);

  pre_cfg_free(&cfg);
  pre_args_free();
  return res;
}
