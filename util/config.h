
#ifndef _UTIL_CONFIG_H_
#define _UTIL_CONFIG_H_

/* config files */
typedef struct config_elem_st   *config_elem_t;
typedef struct config_st        *config_t;

/** holder for the config hash and nad */
struct config_st
{
	xht                 hash;
	nad_t               nad;
};

/** a single element */
struct config_elem_st
{
	const char          **values;
	int                 nvalues;
	const char          ***attrs;
};

config_t         config_new(void);
int              config_load(config_t c, const char *file);
int              config_load_with_id(config_t c, const char *file, const char *id);
config_elem_t    config_get(config_t c, const char *key);
const char      *config_get_one(config_t c, const char *key, int num);
const char      *config_get_one_default(config_t c, const char *key, int num, const char *default_value);
int              config_count(config_t c, const char *key);
char             *config_get_attr(config_t c, const char *key, int num, const char *attr);
char             *config_expand(config_t c, const char *value); //! Replaces $(some.value) with config_get_one(c, "some.value", 0)
void             config_free(config_t);

typedef config_t util_config_t;
typedef config_elem_t util_config_elem_t;

#define util_config_new				config_new
#define util_config_load			config_load
#define util_config_load_with_id	config_load_with_id

#define util_config_get				config_get
#define util_config_get_one			config_get_one
#define util_config_get_default		config_get_one_default

#define util_config_count			config_count
#define util_config_get_attr		config_get_attr
#define util_config_expand			config_expand
#define util_config_free			config_free

#endif