
#include "db_cache_leveldb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// g++ -I. -I/test/thirdlib/leveldb-1.18/include -fPIC db_cache_leveldb.cpp /test/thirdlib/leveldb-1.18/libleveldb.a -shared -o libdbs.so
// gcc -I. -I/test/thirdlib/leveldb-1.18/include main.c -o main ./libdbs.so  -lpthread

int main(){
	db_leveldb_t ts = db_leveldb_init("/test");

	db_leveldb_put(ts,"a","a1");
	char* b = db_leveldb_get(ts,"a");
	fprintf(stderr,"the value is %s\n",b);
	free(b);

	db_leveldb_put(ts,"b","b1");
	db_leveldb_remove(ts,"b");

	b = db_leveldb_get(ts,"b");
	if(b){
		fprintf(stderr,"again, the value is %s\n",b);
		free(b);
	}

	db_leveldb_close(ts);
	return 0;
}