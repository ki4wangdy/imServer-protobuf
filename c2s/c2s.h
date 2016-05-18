
#ifndef _C2S_C2S_H_
#define _C2S_C2S_H_

#include "mio/mio.h"
#include "sx/sx.h"
#include "util/util.h"
#include <mio/mio.h>
#include <db/db.h>
#include <constdef.h>
#include <db/memcached.h>
#include <db/memcacheq.h>

#include <signal.h>
#include <sys/stat.h>

#include <message.h>
#include <message_const.h>

typedef struct c2s_st*		c2s_t;
typedef struct sess_st*		sess_t;

struct sess_st {

	c2s_t               c2s;
	sx_t                sx;

	/* the ip of the client **/
	char				*ip;
	int                 port;

	/* the uid **/
	char				uid[12];

	/* the server's fd **/
	mio_fd_t            fd;

	/* the server's time */
	time_t              last_activity;

	/* the server's log */
	log_t				log;

	/* memory cache */
	pool_t				memory_provider;

};

typedef struct hash_table* chash_t;

struct c2s_st {

	/** our id (hostname) with the router */
	const char          *id;

	/** mio context */
	mio_t               mio;

	/** the server fd */
    mio_fd_t            fd;

	/** config */
	config_t            config;

	/** logging */
	log_t               log;
	log_type_t          log_type;
	const char          *log_facility;
	const char          *log_ident;

	/** time checks */
	int                 io_check_interval;
	int                 io_check_idle;
	int                 io_check_keepalive;
	time_t              next_check;

	/** max file descriptors */
	int                 io_max_fds;

	/** ip to listen on and the port */
	const char          *local_ip;
	int                 local_port;

	/* the server's database **/
	sqlite_db_t			db;

	/* the db cache **/
	db_leveldb_t        dbcache;

	/** sessions (uid : session)*/
	chash_t             sessions;

	/** memcacheq */
	int					memcacheq_fd;

};

/* union for xhash_iter_get to comply with strict-alias rules for gcc3 */
union xhashv{
	void **val;
	const char **char_val;
	sess_t *sess_val;
};

// Data for stream redirect errors
typedef struct stream_redirect_st{
	const char *to_address;
	const char *to_port;
} *stream_redirect_t;

typedef struct sess_msg_item_st{
	int count;
	char* msgid;
	int time;
}*sess_msg_item_t;

int c2s_client_mio_callback(mio_t m, mio_action_t a, mio_fd_t fd, void *data, void *arg);
int c2s_client_sx_callback(sx_t s, sx_event_t e, void *data, void *arg);
int c2s_daemon();

#endif