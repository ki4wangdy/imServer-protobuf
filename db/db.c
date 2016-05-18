
#include <db.h>

sqlite_db_t sqlite_db_open(char* dbpath){
	sqlite3 *db = NULL;
	int ret = sqlite3_open(dbpath, &db);
	if (ret != SQLITE_OK){ 		fprintf(stderr, "sqlite_db_open error to open the db"); 		return NULL; 	}

	sqlite_db_t ds = calloc(1, sizeof(struct sqlite_db_st));
	ds->db = db;
	ds->stmt = NULL;

}

void sqlite_db_close(sqlite_db_t db){
	assert(db);
	assert(db->db);
	sqlite3_finalize(db->stmt);
	sqlite3_close(db->db);
	free(db);
}
