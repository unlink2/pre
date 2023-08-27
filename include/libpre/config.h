#ifndef CONFIG_H_
#define CONFIG_H_

struct pre_config {};

struct pre_config pre_config_init(void);

int pre_main(struct pre_config *cfg);

#endif
