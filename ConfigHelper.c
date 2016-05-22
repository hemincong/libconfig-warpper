#include <stdio.h>
#include <libconfig.h>

#include "EDaoSSErrorCode.h"
#include <stringutil.h>
#include <string.h>
#include <stdlib.h>

static const char *config_file_name = "edao-ss-core.cfg";

config_setting_t *getRootSetting(config_t *cfg) {
	config_init(cfg);
	/* 读取文件失败，则初始化一个config */
	if (!config_read_file(cfg, config_file_name)) {
		//fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		//config_destroy(&cfg);
		config_init(cfg);
	}

	return config_root_setting(cfg);
}

config_setting_t *lookUpMemberFromRoot(config_t *cfg, const char *group_name) {
	config_setting_t *root = getRootSetting(cfg);
	if (!root) {
		return NULL;
	}

	return config_setting_get_member(root, group_name);
}

int readStringFromConfigFile(const char *group_name, const char *paramter_name, const char **value) {
	config_t cfg;
	config_init(&cfg);

	config_setting_t *setting = lookUpMemberFromRoot(&cfg, group_name);
	if (!setting) {
		return EDAOSSSDK_CLIENT_ERROR_CONFIG_FILE_FORMAT_ERROR;
	}

	const char *value_ptr = NULL;

	int ret = config_setting_lookup_string(setting, paramter_name, &value_ptr);

	size_t len = strlen(value_ptr) + 1;
	char *cpy_buf = (char *) malloc(len * sizeof(char));
	memset(cpy_buf, '\0', len);

	MyStrCpy(cpy_buf, len, value_ptr);
	*value = cpy_buf;

	config_destroy(&cfg);
	return ret;
}

int readStringFromConfigFile3Level(const char *group_name, const char *group_name2, const char *paramter_name,
								   const char **value) {
	config_t cfg;
	config_init(&cfg);

	config_setting_t *setting = lookUpMemberFromRoot(&cfg, group_name);
	if (!setting) {
		return EDAOSSSDK_CLIENT_ERROR_CONFIG_FILE_FORMAT_ERROR;
	}
	config_setting_t *setting_level2 = config_setting_get_member(setting, group_name2);
	if (!setting_level2) {
		return EDAOSSSDK_CLIENT_ERROR_CONFIG_FILE_FORMAT_ERROR;
	}

	const char *value_ptr = NULL;

	int ret = config_setting_lookup_string(setting_level2, paramter_name, &value_ptr);

	size_t len = strlen(value_ptr) + 1;
	char *cpy_buf = (char *) malloc(len * sizeof(char));
	memset(cpy_buf, '\0', len);

	MyStrCpy(cpy_buf, len, value_ptr);
	*value = cpy_buf;
	config_destroy(&cfg);
	return ret;
}

int writeToConfigFile(config_t *cfg) {
	if (!config_write_file(cfg, config_file_name)) {
		fprintf(stderr, "Error while writing file.\n");
		config_destroy(cfg);
		return EDAOSSSDK_CLIENT_ERROR_IO_ERROR;
	}

	return 0;
}


config_setting_t *getConfigElementFromRoot(config_t *cfg, const char *group_name, const char *paramter_name,
										   int setting_type) {
	config_setting_t *root = getRootSetting(cfg);
	config_setting_t *group = config_setting_get_member(root, group_name);
	if (!group) {
		group = config_setting_add(root, group_name, CONFIG_TYPE_GROUP);
	}

	config_setting_t *setting = config_setting_get_member(group, paramter_name);
	if (setting) {
		/* Notice that there is no simple update and replace */
		config_setting_remove(group, paramter_name);
	}

	return config_setting_add(group, paramter_name, setting_type);
}

config_setting_t *getConfigElementFromRoot3Level(config_t *cfg, const char *group_name1, const char *group_name2,
												 const char *paramter_name, int setting_type) {
	config_setting_t *root = getRootSetting(cfg);

	config_setting_t *group1 = config_setting_get_member(root, group_name1);
	if (!group1) {
		group1 = config_setting_add(root, group_name1, CONFIG_TYPE_GROUP);
	}

	config_setting_t *group2 = config_setting_get_member(group1, group_name2);
	if (!group2) {
		group2 = config_setting_add(group1, group_name2, CONFIG_TYPE_GROUP);
	}

	config_setting_t *setting = config_setting_get_member(group2, paramter_name);
	if (setting) {
		/* Notice that there is no simple update and replace */
		config_setting_remove(group2, paramter_name);
	}
	return config_setting_add(group2, paramter_name, setting_type);
}

/* 两层 */
int setConfigVariableString(const char *group_name, const char *paramter_name, const char *variable) {
	config_t cfg;

	config_setting_t *setting = getConfigElementFromRoot(&cfg, group_name, paramter_name, CONFIG_TYPE_STRING);
	if (!setting) {
		return EDAOSSSDK_CLIENT_ERROR_CONFIG_FILE_FORMAT_ERROR;
	}

	int ret = config_setting_set_string(setting, variable) & writeToConfigFile(&cfg);
	config_destroy(&cfg);
	return ret;
}

/* 三层 */
int setConfigVariableString3Level(const char *group_name, const char *group_name2, const char *paramter_name,
								  const char *variable) {
	config_t cfg;

	config_setting_t *setting = getConfigElementFromRoot3Level(&cfg, group_name, group_name2, paramter_name,
															   CONFIG_TYPE_STRING);
	if (!setting) {
		return EDAOSSSDK_CLIENT_ERROR_CONFIG_FILE_FORMAT_ERROR;
	}

	int ret = config_setting_set_string(setting, variable) & writeToConfigFile(&cfg);

	config_destroy(&cfg);

	return ret;
}

int setConfigVariableInt(const char *group_name, const char *paramter_name, int variable) {
	config_t cfg;

	config_setting_t *setting = getConfigElementFromRoot(&cfg, group_name, paramter_name, CONFIG_TYPE_INT);
	if (!setting) {
		return EDAOSSSDK_CLIENT_ERROR_CONFIG_FILE_FORMAT_ERROR;
	}

	int ret = config_setting_set_int(setting, variable) & writeToConfigFile(&cfg);
	config_destroy(&cfg);
	return ret;
}

int readIntFromConfigFile(const char *group_name, const char *paramter_name, int *value) {
	config_t cfg;

	config_setting_t *setting = lookUpMemberFromRoot(&cfg, group_name);
	if (!setting) {
		return EDAOSSSDK_CLIENT_ERROR_CONFIG_FILE_FORMAT_ERROR;
	}

	int ret = config_setting_lookup_int(setting, paramter_name, value);

	config_destroy(&cfg);
	return ret;
}
