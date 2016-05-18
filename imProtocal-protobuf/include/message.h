
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "message_const.h"

#ifdef __cplusplus
extern "C" {
#endif

void* message_parse_from_data(char* data, int size);
void* message_init_no_data();
void  message_destory(void*);

// 0 for success, -1 for error
int message_set_id(void* message, char* id, int id_len);
int message_set_from(void* message, char* from, int from_len);
int message_set_to(void* message, char* to, int to_len);
int message_set_body(void* message, char* body, int body_len);
int message_set_type(void* message, int type);

int message_add_header(void* message, char* key, int key_len, char* value, int value_len);

// 0 for success, -1 for error
int message_serialization_size(void* message, int* len);
int message_to_serialization(void* message, char* str, int len);

// don't allow free
const char* message_get_id(void* message);
const char* message_get_from(void* message);
const char* message_get_to(void* message);
int   message_get_type(void* message);

const char* message_get_header_value(void* header, char* key, int key_len);
const char* message_get_body(void* message);

#ifdef __cplusplus
}
#endif

#endif