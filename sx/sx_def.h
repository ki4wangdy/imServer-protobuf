
#ifndef _SX_IMP_H_
#define _SX_IMP_H_

/** holds the state for a single stream */
struct _sx_st {

	/* tag, for logging */
	int                      tag;

	/* IP address of the connection */
	/* pointing to sess.ip and owned by sess structure */
	const char              *ip;

	/* TCP port of the connection */
	/* pointing to sess.port and owned by sess structure */
	int                     port;

	/* callback */
	sx_callback_t            cb;
	void                    *cb_arg;

	/* type */
	_sx_type_t               type;

	/* internal queues */
	sx_buf_t                 wbufpending;        /* buffer passed through wio but not written yet */

	/* do we want to read or write? */
	int                      want_read, want_write;

	/* bytes read from socket */
	int                      rbytes;

	/* read bytes maximum */
	int                      rbytesmax;

	/* current state */
	_sx_state_t              state;

	/* the memcacheq fd */
	int						memcacheq_fd;
};

/* make/break */
sx_t sx_new(int tag, sx_callback_t cb, void *arg, int memcacheq_fd);
void sx_free(sx_t s);

/* get things ready */
void sx_client_init(sx_t s, unsigned int flags, const char *ns,
	const char *to, const char *from, const char *version);
void sx_server_init(sx_t s, unsigned int flags);

/* activity on socket, do stuff! (returns 1 if more read/write actions wanted, 0 otherwise) */
int sx_can_read(sx_t s);
int sx_can_write(sx_t s);

/** processor for incoming wire data */
void _sx_process_read(sx_t s, sx_buf_t buf);

/** main nad processor */
void _sx_nad_process(sx_t s, nad_t nad);

/** sending a nad (internal) */
int _sx_nad_write(sx_t s, nad_t nad, int elem);

/* send errors and close stuff */
void _sx_close(sx_t s);

/* send errors and close stuff */
void sx_error(sx_t s, int err, const char *text);
void sx_error_extended(sx_t s, int err, const char *content);

void sx_close(sx_t s);
void sx_kill(sx_t s);

/** sending raw data */
void sx_raw_write(sx_t s, char *buf, int len);

/** authenticate the stream and move to the auth'd state */
void sx_auth(sx_t s, const char *auth_method, const char *auth_id);

/** helper and internal macro for firing the callback */
int __sx_event(const char *file, int line, sx_t s, sx_event_t e, void *data);

#define _sx_event(s,e,data) __sx_event(ZONE, s, e, data)

#endif