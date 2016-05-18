
#include "memcached.h"

memcached_st* cj_memcached_init(char* server, int port){

	memcached_return rc;
	memcached_server_st *servers;
	memcached_st* memc = memcached_create(NULL);

	servers = memcached_server_list_append(NULL, server, port, &rc);
	rc = memcached_server_push(memc, servers);
	if (rc != MEMCACHED_SUCCESS){
		memcached_server_list_free(servers);
		memcached_free(memc);
		return NULL;
	}
	memcached_server_list_free(servers);
	return memc;
}

int cj_memcached_set(memcached_st* memcache_str, char* key, int key_len, char* value, int value_len, time_t t){
	memcached_return rc;
	rc = memcached_set(memcache_str, key, key_len, value, value_len, t, 0);
	return rc == MEMCACHED_SUCCESS ? 1 : -1;
}

int cj_memcached_get(memcached_st* memcache_str, char* key, int key_len, char** value, int* value_len){
	memcached_return rc;
	*value = memcached_get(memcache_str, key, key_len, value_len, 0, &rc);
	return rc == MEMCACHED_SUCCESS ? 1 : -1;
}

int cj_memcached_delete(memcached_st* memcache_str, char* key, int key_len){
	memcached_return rc;
	rc = memcached_delete(memcache_str, key, key_len, 0);
	return rc == MEMCACHED_SUCCESS ? 1 : -1;
}

void cj_memcached_free(memcached_st* memcache_str){
	if (memcache_str){
		memcached_free(memcache_str);
	}
}