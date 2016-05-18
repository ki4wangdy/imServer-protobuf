
#include <c2s.h>

int msg_check(sess_t sess,sx_buf_t buf,int* target_type,void** msg);

int c2s_client_sx_callback(sx_t s, sx_event_t e, void *data, void *arg) {
	sess_t sess = (sess_t) arg;
	sx_buf_t buf = (sx_buf_t) data;
	int rlen, len, attr, ns, sns, n;
	nad_t nad;

	int data_len = 0;
	char* r = NULL;
	char* id = NULL;
	char* to = NULL;
	char* from = NULL;
	char* body = NULL;
	char* temp = NULL;
	char* dbtemp = NULL;
	int size = 0;
	sess_t temp_sess = NULL;
	sess_msg_item_t item = NULL;
	int type = 0;
	void* msg = NULL;
	sx_buf_t temp_buf = NULL;
	time_t pri = 0;
	int temp_size = 0;

	switch(e) {
		case event_WANT_READ:
			mio_read(sess->c2s->mio, sess->fd);
			break;

		case event_WANT_WRITE:
			mio_write(sess->c2s->mio, sess->fd);
			break;

		case event_READ:
			/* no limit, just read as much as we can */
			rlen = buf->len;
			/* do the read */
			len = recv(sess->fd->fd, buf->data, rlen, 0);
			if(len < 0) {
				if(MIO_WOULDBLOCK) {
					buf->len = 0;
					return 0;
				}
				log_write(sess->log, LOG_NOTICE, "event_READ: uid:%s the fd:%d the ip:%s port:%d ,read the len less zero, because of %s", 
					sess->uid,sess->fd->fd,sess->ip,sess->port,strerror(errno));
				sx_kill(sess->sx);
				return -1;
			} else if(len == 0) {
				log_write(sess->log, LOG_NOTICE, "event_READ: uid:%s the fd:%d the ip:%s port:%d,read the len is zero, because of %s", 
					sess->uid,sess->fd->fd,sess->ip,sess->port,strerror(errno));
				/* they went away */
				sx_kill(sess->sx);
				return -1;
			}
			log_write(sess->log, LOG_NOTICE, "event_READ: read the content's len:%d",len);
			buf->len = len;
			return len;

		case event_WRITE:
			len = send(sess->fd->fd, buf->data, buf->len, 0);
			if(len >= 0) {
				log_write(sess->log, LOG_NOTICE, "event_WRITE: write the content's len:%d",len);
				return len;
			}
			if (MIO_WOULDBLOCK){
				log_write(sess->log, LOG_NOTICE, "event_WRITE: uid:%s,the fd:%d the ip:%s, port:%d because of MIO_WOULDBLOCK",
					sess->uid,sess->fd->fd, sess->ip,sess->port);
				return 0;
			}
			log_write(sess->log, LOG_NOTICE, "event_WRITE: uid:%s, the sx will be killed, the fd:%d, the ip:%s, port:%d, because of %s", 
				sess->uid, sess->fd->fd, sess->ip, sess->port,strerror(errno));
			sx_kill(sess->sx);
			return -1;

		case event_ERROR:
			log_write(sess->log, LOG_NOTICE, "event_ERROR: the uid:%s, the fd:%d is error",
				sess->uid,sess->fd->fd);
			break;

		case event_PACKET:
			if(msg_check(sess,buf,&type,&msg) < 0){
				log_write(sess->log, LOG_NOTICE, "event_PACKET: the msg_check is error");
				return 0;
			}

			switch (type){
				case Message_Login_Type:

					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's login type start");

					from = (char*)message_get_from(msg);
					// 0. check whether or not the (ip:session) is exist
					sess_t t = (sess_t)hash_table_get(sess->c2s->sessions,from);
					if(t){
						log_write(sess->log, LOG_INFO, "event_PACKET: kill the stored session, the sess's uid:%s ,ip:%s port:%d",
							t->uid,t->ip,t->port);					
						sx_kill(t->sx);
					}

					// 1. store the ip:session
					size = strlen(from);
					memcpy(sess->uid,from,size);
					hash_table_put(sess->c2s->sessions, sess->uid, (void *) sess);

					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's login type put the table");

					// 2. login successfully
					r = (char*)sm_login_auth(sess->c2s->db,buf->data,(int)buf->len,&data_len);
					sx_raw_write(sess->sx, r, data_len);
					free(r);

					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's login type end");

					break;

				case Message_Ack_Type:
					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's ack type module start");
					// 1.update the database status
					id = (char*)message_get_body(msg);
					sm_message_ack(sess->c2s->db, id);
					// 2.remove the dbcache message's id
					db_leveldb_remove(sess->c2s->dbcache, id);
					break;
				case Message_Pang_Type:
					break;
				case Message_Friend_Add_Type:
				case Message_Friend_Agree_Type:
				case Message_Friend_Reply_Type:
				case Message_Message_Type:

					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's content type module start");

					//1.first find the session of "to id"
					to = (char*)message_get_to(msg);
					if (!to){
						log_write(sess->log, LOG_INFO, "event_PACKET: the message's to is null, the uid:%s, ip:%s port:%d",
							sess->uid, sess->ip, sess->port);
						message_destory(msg);
						sx_close(sess->sx);
						return 0;
					}
					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's content type to success");

					//2.put this message into database
					sm_message_insert_message(sess->c2s->db, msg, buf->data, buf->len);
					id = (char*)message_get_id(msg);
					dbtemp = calloc(1, buf->len + 1);
					memcpy(dbtemp, buf->data, buf->len);
					db_leveldb_put(sess->c2s->dbcache, id, dbtemp);
					free(dbtemp);
					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's content type put cache success");

					//3.route this message to "to" id
					temp_sess = (sess_t)hash_table_get(sess->c2s->sessions, to);
					if (temp_sess){
						from = (char*)message_get_from(msg);
						body = (char*)message_get_body(msg);
						char* m = (char*)sm_message_send_user(id, to, from, body, &temp_size);
						sx_raw_write(temp_sess->sx, m, temp_size*sizeof(char));
						free(m);
					}
					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's content type cache is exist");

					//4.put this message's id to resend msg system and send "ack" to from 
 					len = 0;
 					temp = (char*)sm_message_ack_packet(id, &len);
					sx_raw_write(sess->sx, temp, len);
					free(temp);

					log_write(sess->log, LOG_NOTICE, "event_PACKET: the message's content type module end");

					break;

				default:
					sx_close(sess->sx);
					break;
				}
				message_destory(msg);
				return 0;

			case event_CLOSED:
				mio_close(sess->c2s->mio, sess->fd);
				sess->fd = NULL;
				return 0;
	}

	return 0;
}

int c2s_client_mio_callback(mio_t m, mio_action_t a, mio_fd_t fd, void *data, void *arg) {
	sess_t sess = (sess_t) arg;
	c2s_t c2s = (c2s_t) arg;
	int nbytes,i,port;
	char* temp = NULL;
	struct sockaddr_storage sa;
	socklen_t namelen = sizeof(sa);

	switch(a) {
		case action_READ:
			/* they did something */
			sess->last_activity = time(NULL);
			ioctl(fd->fd, FIONREAD, &nbytes);
			if(nbytes == 0) {
				log_write(sess->log,LOG_INFO,"action_READ: uid:%s, the fd:%d will be killed because of the nbytes is zero, the ip:%s port:%d", 
					sess->uid, fd->fd,sess->ip, sess->port);
				sx_kill(sess->sx);
				return 0;
			}
			return sx_can_read(sess->sx);

		case action_WRITE:
			return sx_can_write(sess->sx);

		case action_CLOSE:
			log_write(sess->log, LOG_NOTICE, "action_CLOSE: uid:%s, the fd:%d will close, the ip:%s port:%d",
				sess->uid, fd->fd,sess->ip, sess->port);
			sess_t t = hash_table_get(sess->c2s->sessions,sess->uid);
			if(t){
				hash_table_remove(sess->c2s->sessions,t->uid);
				sx_free(t->sx);
				pool_free(t->memory_provider);
				free(t->ip);
				free(t);
			}
			break;

		case action_ACCEPT:

			getpeername(fd->fd, (struct sockaddr *) &sa, &namelen);
			port = j_inet_getport(&sa);

			// get the session from dead_session
			sess = (sess_t)calloc(1, sizeof(struct sess_st));
			sess->memory_provider = pool_new();

			sess->c2s = c2s;
			sess->fd = fd;
			sess->log = c2s->log;

			sess->ip = strdup((char *) data);
			sess->port = port;

			/* they did something */
			sess->last_activity = time(NULL);

			sess->sx = sx_new(fd->fd, c2s_client_sx_callback, (void *) sess, c2s->memcacheq_fd);
			mio_app(m, fd, c2s_client_mio_callback, (void *) sess);

			/* give IP to SX */
			sess->sx->ip = sess->ip;
			sess->sx->port = sess->port;

			sess->sx->type = type_SERVER;
			sess->sx->want_read = 1;

			_sx_state(sess->sx, state_STREAM);
			_sx_event(sess->sx, event_WANT_READ, NULL);

			log_write(sess->log, LOG_NOTICE, "action_ACCEPT: the ip:%s port:%d, the fd:%d",sess->ip,sess->port,fd->fd);

			break;
	}

	return 0;
}

int msg_check(sess_t sess,sx_buf_t buf,int* target_type,void** msgs){

	void* msg = message_parse_from_data(buf->data, buf->len);
	if (!msg){
		log_write(sess->log, LOG_NOTICE, "[sx,packet]:sx msg is null!");
		sx_close(sess->sx);
		return -1;
	}

	int type = message_get_type(msg);
	*target_type = type;

	*msgs = msg;
	return 0;
}

