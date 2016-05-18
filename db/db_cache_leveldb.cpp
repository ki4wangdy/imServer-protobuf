
#include <db_cache_leveldb.h>
#include <leveldb/db.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct db_leveldb_st{
	leveldb::DB* db;
};

db_leveldb_t db_leveldb_init(char* dbpath){
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, dbpath, &db);
	assert(status.ok());
	
	db_leveldb_t ds = (db_leveldb_t)calloc(1,sizeof(db_leveldb_st));
	ds->db = db;
	return ds;

}

void db_leveldb_put(db_leveldb_t db,char* key, char* value){
	assert(db);
	leveldb::Status status = db->db->Put(leveldb::WriteOptions(), key, value);
	if(!status.ok()){
		fprintf(stderr,"put is error!");
	}
}

char* db_leveldb_get(db_leveldb_t db,char* key){
	assert(db);
	std::string value;
	leveldb::Status s = db->db->Get(leveldb::ReadOptions(), key, &value);
	if(!s.ok()){
		fprintf(stderr,"get is error!");
		return NULL;
	}

	const char* data = value.c_str();
	int size = value.size();
	char* r = (char*)calloc(1,size+1);
	memcpy(r,data,size);
	return r;
}

void db_leveldb_remove(db_leveldb_t db,char* key){
	assert(db);
	leveldb::Status s = db->db->Delete(leveldb::WriteOptions(), key);
	if(!s.ok()){
		fprintf(stderr,"remove is error");
	}
}

void db_leveldb_close(db_leveldb_t db){
	assert(db);
	delete db->db;
	free(db);
}
