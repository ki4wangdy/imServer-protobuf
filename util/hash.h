
#ifndef _UTIL_HASH_H_
#define _UTIL_HASH_H_


typedef struct xhn_struct
{
	struct xhn_struct *next;
	struct xhn_struct *prev;
	const char *key;
	int keylen;
	void *val;
} *xhn, _xhn;

typedef struct xht_struct
{
	pool_t p;
	int prime;
	int dirty;
	int count;
	struct xhn_struct *zen;
	struct xhn_struct *free_list; // list of zaped elements to be reused.
	int iter_bucket;
	xhn iter_node;
	int *stat;
} *xht, _xht;

xht xhash_new(int prime);
void xhash_put(xht h, const char *key, void *val);
void xhash_putx(xht h, const char *key, int len, void *val);
void *xhash_get(xht h, const char *key);
void *xhash_getx(xht h, const char *key, int len);
void xhash_zap(xht h, const char *key);
void xhash_zapx(xht h, const char *key, int len);
void xhash_stat(xht h);
void xhash_free(xht h);

typedef void(*xhash_walker)(const char *key, int keylen, void *val, void *arg);
void xhash_walk(xht h, xhash_walker w, void *arg);
int xhash_dirty(xht h);
int xhash_count(xht h);
pool_t xhash_pool(xht h);

/* iteration functions */
int xhash_iter_first(xht h);
int xhash_iter_next(xht h);
void xhash_iter_zap(xht h);
int xhash_iter_get(xht h, const char **key, int *keylen, void **val);

#endif