
#ifndef _CJ_LOG_H_
#define _CJ_LOG_H_

//log_write(c2s->log, LOG_NOTICE, "log started");

void init_log(c2s_t c2s){
	c2s->log = log_new(c2s->log_type, c2s->log_ident, c2s->log_facility);
}

void restart_log_system(c2s_t c2s){
	if(c2s_logrotate) {
		set_debug_log_from_config(c2s->config);
		log_free(c2s->log);
		c2s->log = log_new(c2s->log_type, c2s->log_ident, c2s->log_facility);
		c2s_logrotate = 0;
	}
}

#endif