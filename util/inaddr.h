
#ifndef _UTIL_IN_ADDR_H_
#define _UTIL_IN_ADDR_H_

int         j_inet_pton(const char *src, struct sockaddr_storage *dst);
const char *j_inet_ntop(struct sockaddr_storage* src, char* dst, size_t size);
int         j_inet_getport(struct sockaddr_storage *sa);
int	     j_inet_setport(struct sockaddr_storage *sa, in_port_t port);
socklen_t   j_inet_addrlen(struct sockaddr_storage *sa);

#define util_inet_pton j_inet_pton
#define util_inet_ntop j_inet_ntop

#define util_inet_getport	j_inet_getport
#define util_inet_setport	j_inet_setport

#define util_inet_addrlen	j_inet_addrlen

#endif