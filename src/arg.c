#include "arg.h"
#include "libpre/config.h"
#include "libpre/log.h"

struct arg_lit *verb = NULL;
struct arg_lit *help = NULL;
struct arg_lit *version = NULL;

// arg end stores errors
struct arg_end *end = NULL;

#define pre_argtable                                                          \
  { help, version, verb, end, }

struct pre_config pre_args_to_config(int argc, char **argv) {
  help = arg_litn(NULL, "help", 0, 1, "display this help and exit");
  version = arg_litn(NULL, "version", 0, 1, "display version info and exit");
  verb = arg_litn("v", "verbose", 0, PRE_LOG_LEVEL_DBG, "verbose output");
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

  if (verb->count > 0) {
    pre_log_init(verb->count);
  } else {
    pre_log_init(PRE_LOG_LEVEL_ERROR);
  }

  if (nerrors > 0) {
    arg_print_errors(stdout, end, progname);
    printf("Try '%s --help' for more information.\n", progname);
    exitcode = 1;
    goto exit;
  }

  struct pre_config cfg = pre_config_init();

  // map args to cfg data here

  return cfg;
exit:
  pre_args_free();
  exit(exitcode); // NOLINT
}

void pre_args_free(void) {
  void *argtable[] = pre_argtable;
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
}
