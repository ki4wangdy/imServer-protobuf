
#ifndef _DB_DB_CACHE_LEVELDB_H_
#define _DB_DB_CACHE_LEVELDB_H_

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct db_leveldb_st* db_leveldb_t;

	db_leveldb_t db_leveldb_init(char* dbpath);

	void db_leveldb_put(db_leveldb_t db,char* key, char* value);
	char* db_leveldb_get(db_leveldb_t db,char* key);

	void db_leveldb_remove(db_leveldb_t db,char*);
	void db_leveldb_close(db_leveldb_t db);

#ifdef __cplusplus
};
#endif

#endif