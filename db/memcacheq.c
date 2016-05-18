
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "memcacheq.h"

int memcacheq_init(char* server, int port){
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *host;
	int portnumber;
	
	host = gethostbyname(server);
	portnumber = port;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
		return 0;
	}
	
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portnumber);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	
	if (connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1){
		fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
		return 0;
	 }
	return sockfd;
}

// if the return value is true, the set handle is success
int memcacheq_set(int fd, char* topic, char* value, int value_len){
	int nbytes = 0;
	if (fd == 0){
		return -1;
	}

	char buf[1024*10] = "";
	snprintf(buf, 1024*10, "set %s 0 0 %d\r\n%s\r\n", topic, value_len, value);

	int len = strlen(buf);
	int s = write(fd, buf, len);
	if (s <= 0){
		return -1;
	}
	assert(s == len);

	if ((nbytes = read(fd, buf, 100)) == -1){
		fprintf(stderr, "Read Error:%s\n", strerror(errno));
		return -1;
	}
	buf[nbytes] = '\0';

	if (strstr(buf,"STORED") != 0){
		return 1;
	}
	return -1;
}

// if the return value is false, then the topic queue has no message
int memcacheq_get(int fd, char* topic, char** value, int* len){
	int nbytes = 0;
	if (fd == 0){
		return -1;
	}

	char buf[1024*10] = "";
	snprintf(buf, 1024*10, "get %s\r\n", topic);

	int temp_len = strlen(buf);
	int s = write(fd, buf, temp_len);
	if (s <= 0){
		return -1;
	}
	assert(s == temp_len);

	if ((nbytes = read(fd, buf, 1024*10)) == -1){
		fprintf(stderr, "Read Error:%s\n", strerror(errno));
		return -1;
	}
	buf[nbytes] = '\0';

	char r[] = "END\r\n";
	if (memcmp(buf,r,sizeof(r)-1) == 0){
		return 0;
	}

	char prefix[20] = "";
	snprintf(prefix, 20, "VALUE %s 0 ", topic);
	int prefix_size = strlen(prefix);
	if (nbytes <= prefix_size){
		return -1;
	}

	int i = prefix_size + 1;
	int j = 1;
	for (; i < nbytes; i++,j++){
		if (buf[i] == '\r'){
			break;
		}
	}

	char dital_string[10] = "";
	const char* temp = &buf[prefix_size];
	memcpy(dital_string, temp, j);
	int size = atoi(dital_string);

	assert(size > 0);
	*len = size;

	const char* temps = &buf[prefix_size + j + sizeof("\r\n") - 1];
	char* result = calloc(1, size);
	memcpy(result, temps, size);
	*value = result;

	return 1;

}

int memcacheq_close(int fd){
	if (fd <= 0){
		return 0;
	}
	close(fd);
}

