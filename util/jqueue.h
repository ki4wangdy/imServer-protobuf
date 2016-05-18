
#ifndef _UTIL_JQUEUE_H_
#define _UTIL_JQUEUE_H_

typedef struct _jqueue_node_st  *_jqueue_node_t;
struct _jqueue_node_st {
	void            *data;

	int             priority;

	_jqueue_node_t  next;
	_jqueue_node_t  prev;
};

typedef struct _jqueue_st {
	pool_t          p;
	_jqueue_node_t  cache;

	_jqueue_node_t  front;
	_jqueue_node_t  back;

	int             size;
	char            *key;
	time_t          init_time;
} *jqueue_t;

jqueue_t    jqueue_new(void);
void        jqueue_free(jqueue_t q);
void        jqueue_push(jqueue_t q, void *data, int pri);
void        *jqueue_pull(jqueue_t q);
int         jqueue_size(jqueue_t q);
time_t      jqueue_age(jqueue_t q);

void util_queue_test();

typedef jqueue_t util_queue_t;

#define util_queue_new		jqueue_new
#define util_queue_free		jqueue_free

#define util_queue_push		jqueue_push
#define util_queue_pull		jqueue_pull

#define util_queue_size		jqueue_size
#define util_queue_age		jqueue_age

#endif