
#include "message.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test1(){
	void* msg_t = message_init_no_data();
		
	message_set_id(msg_t, "w3fsdf", 6);
	message_set_from(msg_t, "ki", 2);
	message_set_to(msg_t, "ki_3", 4);
	message_set_type(msg_t, Message_Login_Type);

	message_add_header(msg_t, "header_key", 10, "value", 5);
	message_add_header(msg_t, "header_key1", 11, "value1", 6);

	message_set_body(msg_t, "abc", 3);

	int len = 0;
	int s = message_serialization_size(msg_t,&len);
	if (!s){
		fprintf(stderr, "the size:%d\n", len);
	}

	char* rs = (char*)malloc(sizeof(char)*len);
	message_to_serialization(msg_t, rs, len);
	message_destory(msg_t);

	void* msg_ts = message_parse_from_data(rs, sizeof(char)*len);
	if (!msg_ts){
		return ;
	}
	free(rs);

	char* id = (char*)message_get_id(msg_ts);
	fprintf(stderr, "the id:%s the size:%d\n", id,strlen(id));

	const char* from = message_get_from(msg_ts);
	fprintf(stderr, "the from:%s\n", from);

	const char* to = message_get_to(msg_ts);
	fprintf(stderr, "the to:%s\n", to);

	int type = message_get_type(msg_ts);
	fprintf(stderr, "the id:%d\n", type);

	const char* body = message_get_body(msg_ts);
	if (body){
		fprintf(stderr, "the body:%s\n", body);
	}
	message_destory(msg_ts);
	

}

int main(){
	int i = 0;
	//for (; i < 10000 * 100; i++){
		test1();
	//}
}