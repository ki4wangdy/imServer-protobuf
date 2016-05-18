
#include <db.h>

void sqlite_login_insert(sqlite_db_t db, const char* uid, const char* token,
	const char* tokentime, const char* tokeninterval){

// 	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");
// 
// 	char *sql = "INSERT INTO login (uid, token, tokentime, intervaltime) VALUES ( ?,?,?,? )";
// 	int res = sqlite3_prepare(db->db, sql, -1, &(db->stmt), 0);
//	if (res != SQLITE_OK || db->stmt == NULL) {
//		fprintf(stderr, "sqlite3_prepare is error\n");
//		return;
//	}
// 
// 	sqlite3_bind_text(db->stmt, 1, uid, -1, SQLITE_STATIC);
//	sqlite3_bind_text(db->stmt, 2, token, -1, SQLITE_STATIC);
// 	sqlite3_bind_text(db->stmt, 3, tokentime, -1, SQLITE_STATIC);
// 	sqlite3_bind_text(db->stmt, 4, tokeninterval, -1, SQLITE_STATIC);
// 
// 	res = sqlite3_step(db->stmt);
//	if (res != SQLITE_DONE) {
//		fprintf(stderr, "sqlite3_stemp is error \n");
//	}

//	sqlite3_finalize(db->stmt);
// 	db->stmt = NULL;
// 
// 	sqlite_db_close(db);
}


void sqlite_login_item(sqlite_db_t db, const char* uid){

//	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");

//	sqlite3_stmt *stmt = db->stmt;

//	char *sql = "DELETE FROM login WHERE uid = ? ";
//	int res = sqlite3_prepare(db->db, sql, -1, &stmt, 0);
//	if (res != SQLITE_OK || stmt == NULL) {
//		fprintf(stderr, "sqlite3_prepare is error, %s \n", sqlite3_errmsg(db->db));
//		return;
//	}

//	sqlite3_bind_text(stmt, 1, uid, -1, SQLITE_STATIC);

//	res = sqlite3_step(stmt);
//	if (res != SQLITE_DONE) {
//		fprintf(stderr, "sqlite3_step is error, %s \n", sqlite3_errmsg(db->db));
//	}
//	sqlite3_finalize(db->stmt);
//	db->stmt = NULL;

//	sqlite_db_close(db);
}

void sqlite_login_update_token_base_uid(sqlite_db_t db, const char* key, const char* value){

//	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");
//	sqlite3_stmt *stmt;

//	char *sql = "UPDATE login SET token = ? WHERE uid = ? ";
//	int res = sqlite3_prepare(db->db, sql, -1, &stmt, 0);
//	if (res != SQLITE_OK || stmt == NULL) {
//		fprintf(stderr, "sqlite3_prepare is error, %s \n", sqlite3_errmsg(db->db));
//		return;
//	}

//	sqlite3_bind_text(stmt, 1, value, -1, SQLITE_STATIC);
//	sqlite3_bind_text(stmt, 2, key, -1, SQLITE_STATIC);

//	res = sqlite3_step(stmt);
//	if (res != SQLITE_DONE) {
//		fprintf(stderr, "sqlite3_step is error, %s \n", sqlite3_errmsg(db->db));
//	}
//	sqlite3_finalize(db->stmt);
//	db->stmt = NULL;
	
//	sqlite_db_close(db);
}

void sqlite_login_update_token_tokentime_base_uid(sqlite_db_t db, const char* uid, 
	const char* token, const char* tokentime){

//	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");

//	sqlite3_stmt *stmt;

//	char *sql = "UPDATE login SET token = ? , tokentime = ? WHERE uid = ? ";
//	int res = sqlite3_prepare(db->db, sql, -1, &stmt, 0);
//	if (res != SQLITE_OK || stmt == NULL) {
//		fprintf(stderr, "sqlite3_prepare is error, %s \n", sqlite3_errmsg(db->db));
//		return;
//	}

//	sqlite3_bind_text(stmt, 1, token, -1, SQLITE_STATIC);
//	sqlite3_bind_text(stmt, 2, tokentime, -1, SQLITE_STATIC);
//	sqlite3_bind_text(stmt, 3, uid, -1, SQLITE_STATIC);

//	res = sqlite3_step(stmt);
//	if (res != SQLITE_DONE) {
//		fprintf(stderr, "sqlite3_step is error, %s \n", sqlite3_errmsg(db->db));
//	}
//	sqlite3_finalize(db->stmt);
//	db->stmt = NULL;

//	sqlite_db_close(db);

}


int sqlite_select_uid_is_exist(sqlite_db_t db, const char* uid){

//	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");

//	sqlite3_stmt *stmt = db->stmt;

//	char *sql = "SELECT COUNT(*) FROM login WHERE uid = ? ";
//	int res = sqlite3_prepare(db->db, sql, -1, &stmt, 0);
//	if (res != SQLITE_OK || stmt == NULL) {
//		fprintf(stderr, "sqlite3_prepare is error, %s \n", sqlite3_errmsg(db->db));
//		return 0;
//	}

//	sqlite3_bind_text(stmt, 1, uid, -1, SQLITE_STATIC);

//	int count = 0;
//	res = sqlite3_step(stmt);
//	if(res == SQLITE_ROW){
//		count = sqlite3_column_int(stmt,0);
//	}
//	sqlite3_finalize(db->stmt);
//	db->stmt = NULL;

//	sqlite_db_close(db);

//	return count > 0 ? 1 : 0;
	return 1;
}

int sqlite_select_uid_token(sqlite_db_t db, const char* uid){

//	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");

//	sqlite3_stmt *stmt = db->stmt;

//	char *sql = "SELECT token FROM login WHERE uid = ? limit 1";
//	int res = sqlite3_prepare(db->db, sql, -1, &stmt, 0);
//	if (res != SQLITE_OK || stmt == NULL) {
//		fprintf(stderr, "sqlite3_prepare is error, %s \n", sqlite3_errmsg(db->db));
//		return 0;
//	}

//	sqlite3_bind_text(stmt, 1, uid, -1, SQLITE_STATIC);

//	res = sqlite3_step(stmt);
//	int isexist = (res == SQLITE_ROW ? 1 : 0);

//	char* token = (char*)sqlite3_column_text(stmt, 0);
//	char buf[30] = "";
//	strcpy(buf, token);
//	fprintf(stderr, "the token is %s\n", buf);

//	sqlite3_finalize(db->stmt);
//	db->stmt = NULL;

//	sqlite_db_close(db);

//	return isexist;
	return 1;
}

char* sqlite_login_get_tokentime(sqlite_db_t db, const char* uid){

//	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");

//	sqlite3_stmt *stmt = db->stmt;

//	char *sql = "SELECT tokentime FROM login WHERE uid = ? limit 1";
//	int res = sqlite3_prepare(db->db, sql, -1, &stmt, 0);
//	if (res != SQLITE_OK || stmt == NULL) {
//		fprintf(stderr, "sqlite3_prepare is error, %s \n", sqlite3_errmsg(db->db));
//		sqlite_db_close(db);
//		return 0;
//	}

//	sqlite3_bind_text(stmt, 1, uid, -1, SQLITE_STATIC);

//	res = sqlite3_step(stmt);
//	int isexist = (res == SQLITE_ROW ? 1 : 0);
//	if (!isexist){
//		sqlite3_finalize(db->stmt);
//		sqlite_db_close(db);
//		return NULL;
//	}

//	char* token = (char*)sqlite3_column_text(stmt, 0);
//	if (!token){
//		sqlite3_finalize(db->stmt);
//		sqlite_db_close(db);
//		return NULL;
//	}

//	int size = strlen(token);
//	char* tokentime = calloc(1, size);
//	memcpy(tokentime, token, size);

//	sqlite3_finalize(db->stmt);
//	db->stmt = NULL;
//	sqlite_db_close(db);
//	return tokentime;
	return "1455777578";
}

void sqlite_test(){
	sqlite_db_t ts = sqlite_db_open("./imserver.db");
	char buf[] = "1000432";
	sqlite_login_insert(ts,buf, "433ll43l43jmm", "34343434343", "34343434");
	sqlite_db_close(ts);
}
