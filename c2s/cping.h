
#ifndef _CJ_PING_H_
#define _CJ_PING_H_

#include <pthread.h>

static time_t temp_time = 0;
static int is_kill = 0 ;

static int len = 0;
static char* ping = NULL;

static pthread_t t = NULL;
static pthread_mutex_t mutex;

static jqueue_t queue;

static int _ping_packet(void* key ,void* value, void* arg);

static void* pthread_run(void* arg){
	c2s_t c2s = (c2s_t)arg;
	while (1){
		// 1.send ping msg
		int s = pthread_mutex_trylock(&mutex);
		if (s == 0){
			ping = (char*)sm_message_ping_packets("1", &len);
			temp_time = time(NULL);
			hash_table_for_each(c2s->sessions, _ping_packet, NULL);
			free(ping);
			pthread_mutex_unlock(&mutex);
		}
		sleep(30);
	}
	pthread_detach(t);
	return NULL;
}

void send_ping_packet(c2s_t c2s){
	pthread_mutex_init(&mutex, NULL);
	queue = jqueue_new();
	if (!t){
		pthread_create(&t, NULL, pthread_run, c2s);
	}
}

void update_kill_sx(){
	// 2.kill the sx
	int s = pthread_mutex_trylock(&mutex);
	if (s == 0){
		sx_t sx = jqueue_pull(queue);
		while (sx != NULL){
			sx_kill(sx);
			sx = jqueue_pull(queue);
		}
		pthread_mutex_unlock(&mutex);
	}
}

static int _ping_packet(void* key ,void* value, void* arg){
	sess_t sess = (sess_t)value;
	if(sess->sx){
		if(temp_time - sess->last_activity > 60 * 2){
			log_write(sess->log, LOG_NOTICE, "during ping, send ping msg to uid:%s", sess->uid);
			sx_raw_write(sess->sx, ping, len);
		}

		if (temp_time - sess->last_activity > 60 * 4){
			log_write(sess->log, LOG_NOTICE, "during ping, kill the uid:%s", sess->uid);
			jqueue_push(queue, sess->sx, 0);
		}

	}
	return 0;
}

#endif