
#ifndef _DB_H_
#define _DB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sqlite3.h>

typedef struct sqlite_db_st* sqlite_db_t;
struct sqlite_db_st{
	sqlite3* db;
	sqlite3_stmt* stmt;
};

sqlite_db_t sqlite_db_open(char* dbpath);
void sqlite_db_close(sqlite_db_t db);

#include <sqlite_login.h>
#include <sqlite_message.h>
#include <db_cache_leveldb.h>

#endif