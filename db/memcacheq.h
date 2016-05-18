
#ifndef _C2S_MEMCACHEQ_H_
#define _C2S_MEMCACHEQ_H_

// init the memcacheq 
int memcacheq_init(char* server, int port);

// 1  for success
// -1 for error
int memcacheq_set(int fd, char* topic, char* value, int value_len);

// 1  for has message
// 0  for nothing
// -1 for error 
int memcacheq_get(int fd, char* topic, char** value, int* len);

// close the connect
int memcacheq_close(int fd);

#endif