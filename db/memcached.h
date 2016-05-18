
#ifndef _MEMCACHE_H_
#define _MEMCACHE_H_

#include <sys/time.h>
#include <libmemcached/memcached.h>

memcached_st* cj_memcached_init(char* server, int port);

int cj_memcached_set(memcached_st* memcache_str, char* key, int key_len, char* value, int value_len, time_t t);
int cj_memcached_get(memcached_st* memcache_str, char* key, int key_len, char** value, int* value_len);
int cj_memcached_delete(memcached_st* memcache_str, char* key, int key_len);

void cj_memcached_free(memcached_st* memcache_str);

#endif