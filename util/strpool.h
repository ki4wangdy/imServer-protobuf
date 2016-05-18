
#ifndef _UTIL_STR_POOL_H_
#define _UTIL_STR_POOL_H_

struct spool_node
{
	const char *c;
	struct spool_node *next;
};

typedef struct spool_struct
{
	pool_t p;
	int len;
	struct spool_node *last;
	struct spool_node *first;
} *spool;

spool spool_new(pool_t p); /* create a string pool */
void spooler(spool s, ...); /* append all the char * args to the pool, terminate args with s again */
const char *spool_print(spool s); /* return a big string */
void spool_add(spool s, const char *str); /* add a single string to the pool */
void spool_escape(spool s, const char *raw, int len); /* add and xml escape a single string to the pool */
const char *spools(pool_t p, ...); /* wrap all the spooler stuff in one function, the happy fun ball! */

typedef spool util_spool_t;

#define util_spool_new		spool_new
#define util_spooler		spooler
#define util_spool_print	spool_print
#define util_spool_add		spool_add
#define util_spool_escape	spool_escape
#define util_spools			spools

#endif