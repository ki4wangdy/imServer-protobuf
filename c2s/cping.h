
#ifndef _CJ_PING_H_
#define _CJ_PING_H_

static time_t start_ping_time = 0;
static time_t temp_time = 0;

static char ping[100];
static int len = 0;

static int is_set = 0;
static int is_kill = 0 ;

static int _ping_packet(void* key ,void* value, void* arg);

void send_ping_packet(c2s_t c2s){
// 	char* p = NULL;
// 	time_t temp_time = time(NULL);
//
//	if(!is_set){
// 		p = (char*)sm_message_ping_packet(&len);//		snprintf(ping,100,"%s",p);//		log_write(c2s->log,LOG_INFO,"ping_packet:%s",p);//		is_set = 1;
// 		free(p);
// 	}
// 
//	if(temp_time - start_ping_time > 60 * 2.5){
// 		hash_table_for_each(c2s->sessions,_ping_packet,c2s);
// 		start_ping_time = temp_time;
// 	}

}

void init_ping(c2s_t c2s){
	start_ping_time = time(NULL);
}

static int _ping_packet(void* key ,void* value, void* arg){
	sess_t sess = (sess_t)arg;
	// 1. check the time
	if(sess->sx){
		if(temp_time - sess->last_activity > 60 * 3){
			sx_kill(sess->sx);
			is_kill = 1;
		}
	}
	// 2. send the ping packet
	if(!is_kill){
		if(sess->sx){
			sx_raw_write(sess->sx,ping,len);
		}
	}
	is_kill = 0;
	return 0;
}

#endif