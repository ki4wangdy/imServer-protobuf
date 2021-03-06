
#include <util.h>

static void sha1_hashblock(sha1_state_t *ctx);

void sha1_init(sha1_state_t *ctx) {
	int i;

	ctx->lenW = 0;
	ctx->sizeHi = ctx->sizeLo = 0;

	/* Initialize H with the magic constants (see FIPS180 for constants)
	*/
	ctx->H[0] = 0x67452301L;
	ctx->H[1] = 0xefcdab89L;
	ctx->H[2] = 0x98badcfeL;
	ctx->H[3] = 0x10325476L;
	ctx->H[4] = 0xc3d2e1f0L;

	for (i = 0; i < 80; i++)
		ctx->W[i] = 0;
}


void sha1_append(sha1_state_t *ctx, const unsigned char *dataIn, int len) {
	int i;

	/* Read the data into W and process blocks as they get full
	*/
	for (i = 0; i < len; i++) {
		ctx->W[ctx->lenW / 4] <<= 8;
		ctx->W[ctx->lenW / 4] |= (uint32_t)dataIn[i];
		if ((++ctx->lenW) % 64 == 0) {
			sha1_hashblock(ctx);
			ctx->lenW = 0;
		}
		ctx->sizeLo += 8;
		ctx->sizeHi += (ctx->sizeLo < 8);
	}
}


void sha1_finish(sha1_state_t *ctx, unsigned char hashout[20]) {
	unsigned char pad0x80 = 0x80;
	unsigned char pad0x00 = 0x00;
	unsigned char padlen[8];
	int i;

	/* Pad with a binary 1 (e.g. 0x80), then zeroes, then length
	*/
	padlen[0] = (unsigned char)((ctx->sizeHi >> 24) & 255);
	padlen[1] = (unsigned char)((ctx->sizeHi >> 16) & 255);
	padlen[2] = (unsigned char)((ctx->sizeHi >> 8) & 255);
	padlen[3] = (unsigned char)((ctx->sizeHi >> 0) & 255);
	padlen[4] = (unsigned char)((ctx->sizeLo >> 24) & 255);
	padlen[5] = (unsigned char)((ctx->sizeLo >> 16) & 255);
	padlen[6] = (unsigned char)((ctx->sizeLo >> 8) & 255);
	padlen[7] = (unsigned char)((ctx->sizeLo >> 0) & 255);
	sha1_append(ctx, &pad0x80, 1);
	while (ctx->lenW != 56)
		sha1_append(ctx, &pad0x00, 1);
	sha1_append(ctx, padlen, 8);

	/* Output hash
	*/
	for (i = 0; i < 20; i++) {
		hashout[i] = (unsigned char)(ctx->H[i / 4] >> 24);
		ctx->H[i / 4] <<= 8;
	}

	/*
	*  Re-initialize the context (also zeroizes contents)
	*/
	sha1_init(ctx);
}


void sha1_hash(const unsigned char *dataIn, int len, unsigned char hashout[20]) {
	sha1_state_t ctx;

	sha1_init(&ctx);
	sha1_append(&ctx, dataIn, len);
	sha1_finish(&ctx, hashout);
}


#define SHA_ROTL(X,n) ((((X) << (n)) | ((X) >> (32-(n)))) & 0xffffffffL)

static void sha1_hashblock(sha1_state_t *ctx) {
	int t;
	uint32_t A, B, C, D, E, TEMP;

	for (t = 16; t <= 79; t++)
		ctx->W[t] =
		SHA_ROTL(ctx->W[t - 3] ^ ctx->W[t - 8] ^ ctx->W[t - 14] ^ ctx->W[t - 16], 1);

	A = ctx->H[0];
	B = ctx->H[1];
	C = ctx->H[2];
	D = ctx->H[3];
	E = ctx->H[4];

	for (t = 0; t <= 19; t++) {
		TEMP = (SHA_ROTL(A, 5) + (((C^D)&B) ^ D) + E + ctx->W[t] + 0x5a827999L) & 0xffffffffL;
		E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
	}
	for (t = 20; t <= 39; t++) {
		TEMP = (SHA_ROTL(A, 5) + (B^C^D) + E + ctx->W[t] + 0x6ed9eba1L) & 0xffffffffL;
		E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
	}
	for (t = 40; t <= 59; t++) {
		TEMP = (SHA_ROTL(A, 5) + ((B&C) | (D&(B | C))) + E + ctx->W[t] + 0x8f1bbcdcL) & 0xffffffffL;
		E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
	}
	for (t = 60; t <= 79; t++) {
		TEMP = (SHA_ROTL(A, 5) + (B^C^D) + E + ctx->W[t] + 0xca62c1d6L) & 0xffffffffL;
		E = D; D = C; C = SHA_ROTL(B, 30); B = A; A = TEMP;
	}

	ctx->H[0] += A;
	ctx->H[1] += B;
	ctx->H[2] += C;
	ctx->H[3] += D;
	ctx->H[4] += E;
}

void sha_test(){
	char buf[] = "sdf32r2k3k42k34k3";
	char bufs[20] = "";
	util_sha1_hash(buf, sizeof(buf)-1, bufs);

	fprintf(stderr, "the %s to %s\n", buf, bufs);
}
