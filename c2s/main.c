
#include <c2s.h>
#include <csignal.h>
#include <cjconfig.h>
#include <cpid.h>
#include <cdb.h>
#include <clog.h>
#include <cping.h>

void init_c2s(c2s_t c2s){
	c2s->sessions = make_string_hash_table(HASH_MAX_COUNT);
	c2s->mio = mio_new(c2s->io_max_fds);
	if(c2s->mio == NULL) {
		log_write(c2s->log, LOG_ERR, "failed to create MIO, aborting");
		exit(1);
	}
}

void destory_c2s(c2s_t c2s){
	mio_free(c2s->mio);
	destory_db(c2s);

	hash_table_destroy(c2s->sessions);

	log_free(c2s->log);
	config_free(c2s->config);

	free(c2s);
}

int main(int argc, char** argv){

	c2s_t c2s;
	union xhashv xhv;
	char *config_file;

	int optchar;
	int mio_timeout;
	sess_t sess;
	char* ping;

	time_t check_time = 0;
	const char *cli_id = 0;

	umask((mode_t) 0027);
	srand(time(NULL));


	// start to calloc c2s
	c2s = (c2s_t) calloc(1, sizeof(struct c2s_st));
	c2s->config = config_new();
	config_file = "./c2s.xml";

	while((optchar = getopt(argc, argv, "Dbc:hi:?")) >= 0)
	{
		switch(optchar){
		case 'c':
			config_file = optarg;
			break;
		case 'D':
#ifdef DEBUG
			set_debug_flag(1);
#else
			printf("WARN: Debugging not enabled.  Ignoring -D.\n");
#endif
			break;
		case 'b':
			c2s_daemon();
			break;
		case 'i':
			cli_id = optarg;
			break;
		case 'h': case '?': default:
			fputs(
				"c2s - jabberd client-to-server connector \n"
				"Usage: c2s <options>\n"
				"Options are:\n"
				"   -c <config>     config file to use [default: ./c2s.xml]\n"
				"   -i id           Override <id> config element\n"
#ifdef DEBUG
				"   -D              Show debug output\n"
#endif
				,
				stdout);
			config_free(c2s->config);
			free(c2s);
			return 1;
		}
	}

	// init the signal system
	init_signal();

	// load the configure file from the c2s.xml
	if(init_config(c2s,config_file,cli_id) < 0){
		return 2;
	}

	// store the pid into the file
	init_pid_file(c2s);

	// init the database configure
	init_db(c2s);

	// init the log system
	init_log(c2s);

	// init the c2s system
	init_c2s(c2s);

	c2s->fd = mio_listen(c2s->mio, c2s->local_port, c2s->local_ip, c2s_client_mio_callback, (void *) c2s);
	if(c2s->fd == NULL) {
		log_write(c2s->log, LOG_ERR, "[%s, port=%d] unable to listen (%s)", c2s->local_ip, c2s->local_port, MIO_STRERROR(MIO_ERROR));
		exit(1);
	}

	send_ping_packet(c2s);

	mio_timeout = 10;
	while(!c2s_shutdown) {
		mio_run(c2s->mio, mio_timeout);

		/** 1.restart the log system */
		restart_log_system(c2s);

		/** 2.restart to load the configure xml */
		reload_configure_file(c2s,config_file);

		/** 3.release the dead sx, during the ping condition*/ 
		update_kill_sx(c2s);
	}

	if (c2s->fd != NULL){
		close(c2s->fd->fd);
		free(c2s->fd);
	}

	destory_c2s(c2s);
	return 0;
}

