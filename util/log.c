
#include <util.h>


#define MAX_LOG_LINE (1024)

#ifdef DEBUG
static int debug_flag;
static FILE *debug_log_target = 0;
#endif

static const char *_log_level[] =
{
	"emergency",
	"alert",
	"critical",
	"error",
	"warning",
	"notice",
	"info",
	"debug"
};

static log_facility_t _log_facilities[] = {
	{ "local0", LOG_LOCAL0 },
	{ "local1", LOG_LOCAL1 },
	{ "local2", LOG_LOCAL2 },
	{ "local3", LOG_LOCAL3 },
	{ "local4", LOG_LOCAL4 },
	{ "local5", LOG_LOCAL5 },
	{ "local6", LOG_LOCAL6 },
	{ "local7", LOG_LOCAL7 },
	{ "log_user", LOG_USER },
	{ NULL, -1 }
};

static int _log_facility(const char *facility) {
	log_facility_t *lp;

	if (facility == NULL) {
		return -1;
	}
	for (lp = _log_facilities; lp->facility; lp++) {
		if (!strcasecmp(lp->facility, facility)) {
			break;
		}
	}
	return lp->number;
}

log_t log_new(log_type_t type, const char *ident, const char *facility)
{
	log_t log;
	int fnum = 0;

	log = (log_t)calloc(1, sizeof(struct log_st));

	log->type = type;

	if (type == log_SYSLOG) {
		fnum = _log_facility(facility);
		if (fnum < 0)
			fnum = LOG_LOCAL7;
		openlog(ident, LOG_PID, fnum);
		return log;
	}

	else if (type == log_STDOUT) {
		log->file = stdout;
		return log;
	}

	log->file = fopen(ident, "a+");
	if (log->file == NULL)
	{
		fprintf(stderr,
			"ERROR: couldn't open logfile: %s\n"
			"       logging will go to stdout instead\n", strerror(errno));
		log->type = log_STDOUT;
		log->file = stdout;
	}

	return log;
}

void log_write(log_t log, int level, const char *msgfmt, ...)
{
	va_list ap;
	char *pos, message[MAX_LOG_LINE + 1];
	int sz, len;
	time_t t;

	if (log && log->type == log_SYSLOG) {
		va_start(ap, msgfmt);
	#ifdef HAVE_VSYSLOG
		vsyslog(level, msgfmt, ap);
	#else
		len = vsnprintf(message, MAX_LOG_LINE, msgfmt, ap);
		if (len > MAX_LOG_LINE)
			message[MAX_LOG_LINE] = '\0';
		else
			message[len] = '\0';
		syslog(level, "%s", message);
	#endif
		va_end(ap);
	
	#ifndef DEBUG
		return;
	#endif
	}

	/* timestamp */
	t = time(NULL);
	pos = ctime(&t);
	sz = strlen(pos);
	/* chop off the \n */
	pos[sz - 1] = ' ';

	/* insert the header */
	len = snprintf(message, MAX_LOG_LINE, "%s[%s] ", pos, _log_level[level]);
	if (len > MAX_LOG_LINE)
		message[MAX_LOG_LINE] = '\0';
	else
		message[len] = '\0';

	/* find the end and attach the rest of the msg */
	for (pos = message; *pos != '\0'; pos++); /*empty statement */
	sz = pos - message;
	va_start(ap, msgfmt);
	vsnprintf(pos, MAX_LOG_LINE - sz, msgfmt, ap);
	va_end(ap);
#ifndef DEBUG
	if (log && log->type != log_SYSLOG) {
#endif
		if (log && log->file) {
			fprintf(log->file, "%s", message);
			fprintf(log->file, "\n");
			fflush(log->file);
		}
#ifndef DEBUG
	}
#endif

#ifdef DEBUG
	if (!debug_log_target) {
		debug_log_target = stderr;
	}
	/* If we are in debug mode we want everything copied to the stdout */
	if ((log == 0) || (get_debug_flag() && log->type != log_STDOUT)) {
		fprintf(debug_log_target, "%s\n", message);
		fflush(debug_log_target);
	}
#endif /*DEBUG*/
}

void log_free(log_t log) {
	if (log->type == log_SYSLOG)
		closelog();
	else if (log->type == log_FILE)
		fclose(log->file);

	free(log);
}

#ifdef DEBUG
#define MAX_DEBUG 880
/** debug logging */
void debug_log(const char *file, int line, const char *msgfmt, ...)
{
	va_list ap;
	char *pos, message[MAX_DEBUG];
	int sz;
	time_t t;

	if (!debug_log_target) {
		debug_log_target = stderr;
	}
	/* timestamp */
	t = time(NULL);
	pos = ctime(&t);
	sz = strlen(pos);
	/* chop off the \n */
	pos[sz - 1] = ' ';

	/* insert the header */
	snprintf(message, MAX_DEBUG, "%s%s:%d ", pos, file, line);

	/* find the end and attach the rest of the msg */
	for (pos = message; *pos != '\0'; pos++); /*empty statement */
	sz = pos - message;
	va_start(ap, msgfmt);
	vsnprintf(pos, MAX_DEBUG - sz, msgfmt, ap);
	va_end(ap);
	fprintf(debug_log_target, "%s", message);
	fprintf(debug_log_target, "\n");
	fflush(debug_log_target);
}

int get_debug_flag(void)
{
	return debug_flag;
}

void set_debug_flag(int v)
{
	debug_flag = v;
}

void set_debug_file(const char *filename)
{
	// Close debug output file but not stderr
	if (debug_log_target != 0 &&
		debug_log_target != stderr)
	{
		fprintf(debug_log_target, "Closing log\n");
		fclose(debug_log_target);

		debug_log_target = stderr;
	}

	// Setup new log target
	if (filename) {
		log_debug(ZONE, "Openning debug log file %s", filename);
		debug_log_target = fopen(filename, "a+");

		if (debug_log_target != NULL) {
			log_debug(ZONE, "Staring debug log");
		}
		else {
			debug_log_target = stderr;
			log_debug(ZONE, "Failed to open debug output file %s. Fallback to stderr", filename);
		}
	}
	else {
		// set stderr
		debug_log_target = stderr;
	}
}

void set_debug_log_from_config(config_t c)
{
	set_debug_file(config_get_one(c, "log.debug", 0));
}


#else /* DEBUG */
void debug_log(const char *file, int line, const char *msgfmt, ...)
{ }

void set_debug_flag(int v)
{ }

#endif

void log_test(){
	util_log_t t = util_log_new(log_STDOUT, "ki", "ki1");
	util_log_write(t, 0, "ab cd de %s", "000000");
	util_log_free(t);
}

