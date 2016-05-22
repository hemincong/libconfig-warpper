#ifndef _CONFIG_HELPER_H_
#define _CONFIG_HELPER_H_

#ifdef __cplusplus
extern C{
#endif

#include "libconfig.h"

config_setting_t *getRootSetting(config_t *cfg);

int writeToConfigFile(config_t *cfg);

config_setting_t *getConfigElementFromRoot(config_t *cfg, const char *group_name, const char *paramter_name,
										   int setting_type);

int readStringFromConfigFile(const char *group_name, const char *paramter_name, const char **value);

int readStringFromConfigFile3Level(const char *group_name, const char *group_name2, const char *paramter_name,
								   const char **value);

int setConfigVariableString(const char *group_name, const char *paramter_name, const char *variable);

int setConfigVariableString3Level(const char *group_name, const char *group_name2, const char *paramter_name,
								  const char *variable);

int setConfigVariableInt(const char *group_name, const char *paramter_name, int variable);

int readIntFromConfigFile(const char *group_name, const char *paramter_name, int *value);

#ifdef __cplusplus
}
#endif

#endif // _CONFIG_HELPER_H_
