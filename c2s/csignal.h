
#ifndef _CJ_SIGNAL_H_
#define _CJ_SIGNAL_H_

static sig_atomic_t c2s_shutdown = 0;
static sig_atomic_t c2s_logrotate = 0;
static sig_atomic_t c2s_sighup = 0;

static void _c2s_signal(int signum){
	c2s_shutdown = 1;
}

static void _c2s_signal_hup(int signum){
	c2s_logrotate = 1;
	c2s_sighup = 1;
}

static void _c2s_signal_usr1(int signum){
	set_debug_flag(0);
}

static void _c2s_signal_usr2(int signum){
	set_debug_flag(1);
}

void init_signal(){
	jabber_signal(SIGINT, _c2s_signal);
	jabber_signal(SIGTERM, _c2s_signal);
	jabber_signal(SIGHUP, _c2s_signal_hup);
	jabber_signal(SIGPIPE, SIG_IGN);
	jabber_signal(SIGUSR1, _c2s_signal_usr1);
	jabber_signal(SIGUSR2, _c2s_signal_usr2);
}

#endif