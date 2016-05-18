
#include <db.h>

void sqlite_message_insert(sqlite_db_t db, char* msgid, int msgstatus, 
		int msgtype, char* msgsender, char* receiver, char* msgstr, int len){

	db = sqlite_db_open("/Cajian/imServer/db/imserver.db");

	time_t t = time(NULL);
	char buf[20] = "";
	snprintf(buf, 20, "%d", (int)t);

	char *sql = "INSERT INTO message (msgid, msgstatus, msgtype, msgsender, msgreceiver, msgtime, msgcontent) VALUES ( ?,?,?,?,?,?,?)";
	int res = sqlite3_prepare(db->db, sql, -1, &(db->stmt), 0);
 	if (res != SQLITE_OK || db->stmt == NULL) {
		fprintf(stderr, "sqlite3_prepare is error\n");
		sqlite_db_close(db);
		return;
	}
	sqlite3_bind_text(db->stmt, 1, msgid, -1, SQLITE_STATIC);
	sqlite3_bind_int(db->stmt, 2, msgstatus);
	sqlite3_bind_int(db->stmt, 3, msgtype);
	sqlite3_bind_text(db->stmt, 4, msgsender, -1, SQLITE_STATIC);
	sqlite3_bind_text(db->stmt, 5, receiver, -1, SQLITE_STATIC);
	sqlite3_bind_text(db->stmt, 6, buf, -1, SQLITE_STATIC);
	sqlite3_bind_text(db->stmt, 7, msgstr, len, SQLITE_STATIC);

	res = sqlite3_step(db->stmt);
 	if (res != SQLITE_DONE) {
		fprintf(stderr, "sqlite3_stemp is error \n");
	}

	sqlite3_finalize(db->stmt);
 	db->stmt = NULL;

	sqlite_db_close(db);

}

void sqlite_message_delete(sqlite_db_t db,char* msgid){
}

void sqlite_message_update_status(sqlite_db_t db, char* msgid, int msgstatus){
	db = sqlite_db_open("../db/imserver.db");
	sqlite3_stmt *stmt;

	char *sql = "UPDATE message SET msgstatus = ? WHERE msgid = ? ";
	int res = sqlite3_prepare(db->db, sql, -1, &stmt, 0);
	if (res != SQLITE_OK || stmt == NULL) {
		fprintf(stderr, "sqlite3_prepare is error, %s \n", sqlite3_errmsg(db->db));
		sqlite_db_close(db);
		return;
	}

	sqlite3_bind_int(stmt, 1, msgstatus);
	sqlite3_bind_text(stmt, 2, msgid, -1, SQLITE_STATIC);

	res = sqlite3_step(stmt);
	if (res != SQLITE_DONE) {
		fprintf(stderr, "sqlite3_step is error, %s \n", sqlite3_errmsg(db->db));
	}
	sqlite3_finalize(db->stmt);
	db->stmt = NULL;	sqlite_db_close(db);
}

