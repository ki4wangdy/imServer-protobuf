
#ifndef _CJ_DB_H_
#define _CJ_DB_H_

int memcacheq_init(char* server, int port);

void init_db(c2s_t c2s){
	char* dbname = (char*)config_get_one(c2s->config, "authreg.sqlite.dbname",0);
	//c2s->db = sqlite_db_open(dbname);
	c2s->dbcache = db_leveldb_init("../db");

	c2s->memcacheq_fd = memcacheq_init("121.42.10.208",11212);
	assert(c2s->memcacheq_fd > 0);

}

void destory_db(c2s_t c2s){
	//sqlite_db_close(c2s->db);
	db_leveldb_close(c2s->dbcache);
	memcacheq_close(c2s->memcacheq_fd);
}

#endif