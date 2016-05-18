
#ifndef _UTIL_SHA_H_
#define _UTIL_SHA_H_

typedef struct sha1_state_s {
	uint32_t H[5];
	uint32_t W[80];
	int lenW;
	uint32_t sizeHi, sizeLo;
} sha1_state_t;

void sha1_init(sha1_state_t *ctx);
void sha1_append(sha1_state_t *ctx, const unsigned char *dataIn, int len);
void sha1_finish(sha1_state_t *ctx, unsigned char hashout[20]);
void sha1_hash(const unsigned char *dataIn, int len, unsigned char hashout[20]);

void sha_test();

#define util_sha1_init   sha1_init
#define util_sha1_append sha1_append
#define util_sha1_finish sha1_finish
#define util_sha1_hash   sha1_hash


#endif