
#ifndef _DB_LOGIN_H_
#define _DB_LOGIN_H_

void sqlite_login_insert(sqlite_db_t db, const char* uid, const char* token,
	const char* tokentime, const char* tokeninterval);

void sqlite_login_item(sqlite_db_t db, const char* uid);
void sqlite_login_update_token_base_uid(sqlite_db_t db, const char* key, const char* value);

void sqlite_login_update_token_tokentime_base_uid(sqlite_db_t db, const char* uid, const char* token, const char* tokentime);

int sqlite_select_uid_is_exist(sqlite_db_t db, const char* uid);
int sqlite_select_uid_token(sqlite_db_t db, const char* uid);

char* sqlite_login_get_tokentime(sqlite_db_t db, const char* uid);

void sqlite_test();

#endif