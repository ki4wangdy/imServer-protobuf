
#ifndef _UTIL_BASE64_H_
#define _UTIL_BASE64_H_

/* convenience, result string must be free()'d by caller */
char *b64_encode(char *buf, int len);
char *b64_decode(char *buf);

void util_base64_test();

#define util_base64_encode b64_encode
#define util_base64_decode b64_decode

#endif