
#ifndef _DB_MESSAGE_H_
#define _DB_MESSAGE_H_

void sqlite_message_insert(sqlite_db_t db, char* msgid, int msgstatus, 
						   int msgtype, char* msgsender, char* receiver,
						   char* msgstr, int len);

void sqlite_message_delete(sqlite_db_t db, char* msgid);
void sqlite_message_update_status(sqlite_db_t db, char* msgid, int msgstatus);


#endif