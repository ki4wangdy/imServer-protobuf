
#ifndef _UTIL_HEX_H_
#define _UTIL_HEX_H_

void hex_from_raw(const unsigned char* in, int inlen, char* out);
int hex_to_raw(const char *in, int inlen, char *out);

void hex_test();

#define util_hex_from_raw hex_from_raw
#define util_hex_to_raw hex_to_raw

#endif