
#include <sm.h>

#include <message.h>
#include <message_const.h>

void sm_message_insert_message(sqlite_db_t db, void* msg, char* msgcontent, int len){

	char* id = (char*)message_get_id(msg);
	char* from = (char*)message_get_from(msg);
	char* to = (char*)message_get_to(msg);
	char* body = (char*)message_get_body(msg);
	int t = message_get_type(msg);

	// 1.insert the database
	sqlite_message_insert(db, id, Msg_Status_Init, t, from,to,msgcontent,len);
	
}

void sm_message_ack(sqlite_db_t db, char* id){
	sqlite_message_update_status(db,id,Msg_Status_Received);
}

char* sm_message_ping_packet(int* len){
	void* msg = (void*)message_init_no_data();
	message_set_body(msg, "1", 1);
	message_set_type(Message_Ping_Type);

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);

	message_destory(msg);
	return m;
}

char* sm_message_ping_packets(char* id, int* len){
	void* msg = (void*)message_init_no_data();
	message_set_body(msg, id, strlen(id));
	message_set_type(msg, Message_Ping_Type);

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);

	message_destory(msg);
	return m;
}

char* sm_message_ack_packet(char* id, int* len){
	void* msg = (void*)message_init_no_data();
	message_set_body(msg, id, strlen(id));
	message_set_type(msg,Message_Ack_Type);

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);

	message_destory(msg);
	return m;
}

char* sm_message_send_user(char* id, char* to, char* from, char* body, int* len){
	void* msg = (void*)message_init_no_data();
	message_set_id(msg, id, strlen(id));
	message_set_from(msg, from, strlen(from));
	message_set_to(msg, to, strlen(to));
	message_set_body(msg, body, strlen(body));
	message_set_type(msg, Message_Message_Type);

	int size = 0;
	message_serialization_size(msg, &size);
	*len = size;

	char* m = malloc(sizeof(char)*size);
	message_to_serialization(msg, m, sizeof(char)*size);

	message_destory(msg);
	return m;

}
