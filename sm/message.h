
#ifndef _SM_MESSAGE_H_
#define _SM_MESSAGE_H_

#define Msg_Status_Init			1
#define Msg_Status_Received		2

void sm_message_insert_message(sqlite_db_t db, void* msg, char* msgcontent, int len);
void sm_message_ack(sqlite_db_t db, char* id);

char* sm_message_ping_packet(int* len);
char* sm_message_ack_packet(char* id, int* len);

char* sm_message_send_user(char* id, char* to, char* from, char* body, int* len);

#endif