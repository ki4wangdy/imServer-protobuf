
#ifndef _UTIL_LOG_H_
#define _UTIL_LOG_H_

#include <util.h>

typedef enum {
	log_STDOUT,
	log_SYSLOG,
	log_FILE
} log_type_t;

typedef struct log_st
{
	log_type_t  type;
	FILE        *file;
} *log_t;

typedef struct log_facility_st
{
	const char  *facility;
	int         number;
} log_facility_t;

log_t    log_new(log_type_t type, const char *ident, const char *facility);
void     log_write(log_t log, int level, const char *msgfmt, ...);
void     log_free(log_t log);

void log_test();

/* debug logging */
#if defined(DEBUG) && 0
int				log_debug_flag;
void            log_debug(char *file, int line, const char *subsys, const char *msgfmt, ...);

# define        log_debug_get_flag()    log_debug_flag
# define        log_debug_set_flag(f)   (log_debug_flag = f ? 1 : 0)
# define        log_debug(...)          if(log_debug_flag) __log_debug(__FILE__,__LINE__,0,__VA_ARGS__)
# define        log_debug_subsys(...)   if(log_debug_flag) __log_debug(__FILE__,__LINE__,__VA_ARGS__)
#else
# define        log_debug_get_flag()    (0)
# define        log_debug_set_flag(f)
# define        log_debug(...)
# define        log_debug_subsys(...)
#endif

typedef log_t util_log_t;

#define util_log_new log_new
#define util_log_write log_write
#define util_log_free log_free

#endif