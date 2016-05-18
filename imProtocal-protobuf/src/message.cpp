
#include "message.h"
#include "message.pb.h"
#include "header.pb.h"

typedef Message* message_t;

void* message_parse_from_data(char* data, int size){
	Message* m = new Message;
	bool is_success = (message_t)m->ParseFromArray(data, size);
	return is_success ? (message_t)m : NULL;
}

void* message_init_no_data(){
	message_t t = new Message;
	return t;
}

void  message_destory(void* message){
	delete (message_t)message;
}

int message_set_id(void* message, char* id, int id_len){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	msg_ptr->set_id(id,id_len);
	return Message_Success;
}

int message_set_from(void* message, char* from, int from_len){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	msg_ptr->set_from(from, from_len);
	return Message_Success;
}

int message_set_to(void* message, char* to, int to_len){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	msg_ptr->set_to(to, to_len);
	return Message_Success;
}

int message_set_body(void* message, char* body, int body_len){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	msg_ptr->set_body(body, body_len);
	return Message_Success;
}

int message_set_type(void* message, int type){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	msg_ptr->set_type(type);
	return Message_Success;
}

int message_add_header(void* message, char* key, int key_len, char* value, int value_len){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	Header* header = msg_ptr->add_header();
	header->set_key(key, key_len);
	header->set_value(value, value_len);
	return Message_Success;
}

int message_serialization_size(void* message, int* len){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	*len = msg_ptr->ByteSize();
	return Message_Success;
}

// 0 for success, -1 for error
int message_to_serialization(void* message, char* str, int len){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	bool is_success = msg_ptr->SerializeToArray(str, len);
	if (is_success){
		return Message_Success;
	}
	return Message_Error;
}

// need to be free
const char* message_get_id(void* message){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return NULL;
	}
	const std::string& id = msg_ptr->id();
	return id.c_str();
}

const char* message_get_from(void* message){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return NULL;
	}
	const std::string& from = msg_ptr->from();
	return from.c_str();
}

const char* message_get_to(void* message){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return NULL;
	}
	const std::string& to = msg_ptr->to();
	return to.c_str();
}

int message_get_type(void* message){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return Message_Error;
	}
	return msg_ptr->type();
}

const char* message_get_header_value(void* header, char* key, int key_len){
	Header* header_ptr = (Header*)header;
	if (!header_ptr){
		return NULL;
	}
	return header_ptr->key().c_str();
}

const char* message_get_body(void* message){
	Message* msg_ptr = (Message*)message;
	if (!msg_ptr){
		return NULL;
	}
	if (msg_ptr->has_body()){
		return msg_ptr->body().c_str();
	}
	return "";
}
