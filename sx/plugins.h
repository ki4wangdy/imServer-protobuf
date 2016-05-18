
#ifndef INCL_SX_PLUGINS_H
#define INCL_SX_PLUGINS_H

struct _sx_plugin_st {

	sx_env_t                env;

	int                     magic;              /* unique id so that plugins can find each other */
	int                     index;
	void                    *private;

	void(*new)(sx_t s, sx_plugin_t p);                          /* pre-run init */
	void(*free)(sx_t s, sx_plugin_t p);                         /* conn being freed */

	void(*client)(sx_t s, sx_plugin_t p);                       /* client init */
	void(*server)(sx_t s, sx_plugin_t p);                       /* server init */

	/* return -2 == failed (permanent), -1 == failed (temporary), 0 == handled, 1 == pass */
	int(*wio)(sx_t s, sx_plugin_t p, sx_buf_t buf);            /* before being written */
	int(*rio)(sx_t s, sx_plugin_t p, sx_buf_t buf);            /* after being read */

	/* return 0 == handled, 1 == pass */
//	int(*wnad)(sx_t s, sx_plugin_t p, nad_t nad, int elem);    /* before being written */
//	int(*rnad)(sx_t s, sx_plugin_t p, nad_t nad);              /* after being read */

	void(*header)(sx_t s, sx_plugin_t p, sx_buf_t buf);         /* before header req/res write */
	void(*stream)(sx_t s, sx_plugin_t p);                       /* after-stream init */

//	void(*features)(sx_t s, sx_plugin_t p, nad_t nad);          /* offer features */

	/* return 0 == handled, 1 == pass */
	int(*process)(sx_t s, sx_plugin_t p, nad_t nad);           /* process completed nads */
	void(*unload)(sx_plugin_t p);                               /* plugin unloading */

};

/** load a plugin into the environment */
sx_plugin_t sx_env_plugin(sx_env_t env, sx_plugin_init_t init, ...);

/** an environment */
struct _sx_env_st {
	sx_plugin_t             *plugins;
	int                     nplugins;
};

/* make/break an environment */
sx_env_t sx_env_new(void);

void sx_env_free(sx_env_t env);

#endif
