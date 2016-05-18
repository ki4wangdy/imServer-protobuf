
#ifndef _CJ_CONFIG_H_
#define _CJ_CONFIG_H_

/** pull values out of the config file */
static void _c2s_config_expand(c2s_t c2s){
	const char *str, *ip, *mask;
	char *req_domain, *to_address, *to_port;
	config_elem_t elem;
	int i;
	set_debug_log_from_config(c2s->config);
	c2s->id = config_get_one(c2s->config, "id", 0);
	if(c2s->id == NULL){
		c2s->id = "c2s";
	}

	c2s->log_type = log_STDOUT;
	if(config_get(c2s->config, "log") != NULL) {
		if((str = config_get_attr(c2s->config, "log", 0, "type")) != NULL) {
			if(strcmp(str, "file") == 0)
				c2s->log_type = log_FILE;
			else if(strcmp(str, "syslog") == 0)
				c2s->log_type = log_SYSLOG;
		}
	}

	if(c2s->log_type == log_SYSLOG) {
		c2s->log_facility = config_get_one(c2s->config, "log.facility", 0);
		c2s->log_ident = config_get_one(c2s->config, "log.ident", 0);
		if(c2s->log_ident == NULL)
			c2s->log_ident = "jabberd/c2s";
	} else if(c2s->log_type == log_FILE)
		c2s->log_ident = config_get_one(c2s->config, "log.file", 0);

	c2s->local_ip = config_get_one(c2s->config, "local.ip", 0);
	if(c2s->local_ip == NULL)
		c2s->local_ip = "0.0.0.0";

	c2s->local_port = j_atoi(config_get_one(c2s->config, "local.port", 0), 0);
	c2s->io_max_fds = j_atoi(config_get_one(c2s->config, "io.max_fds", 0), 1024);
	c2s->io_check_interval = j_atoi(config_get_one(c2s->config, "io.check.interval", 0), 0);
	c2s->io_check_idle = j_atoi(config_get_one(c2s->config, "io.check.idle", 0), 0);
	c2s->io_check_keepalive = j_atoi(config_get_one(c2s->config, "io.check.keepalive", 0), 0);

}

int init_config(c2s_t c2s, char *config_file, const char *cli_id){
	// load the xml 
	if(config_load_with_id(c2s->config, config_file, cli_id) != 0){
		fputs("c2s: couldn't load config, aborting\n", stderr);
		config_free(c2s->config);
		free(c2s);
		return -1;
	}
	// expand the c2s.xml into the struct c2s
	_c2s_config_expand(c2s);
	return 0;
}

void reload_configure_file(c2s_t c2s, char* config_file){
	if(c2s_sighup) {
		log_write(c2s->log, LOG_NOTICE, "reloading some configuration items ...");
		if(c2s->config){
			config_free(c2s->config);
		}
		c2s->config = config_new();
		config_load(c2s->config, config_file);
		c2s_sighup = 0;
	}

}

#endif