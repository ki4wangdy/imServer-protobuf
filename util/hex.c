
#include <util.h>

void hex_from_raw(const unsigned char* in, int inlen, char* out){
	int i, h, l;

	for (i = 0; i < inlen; i++) {
		h = in[i] & 0xf0;
		h >>= 4;
		l = in[i] & 0x0f;
		out[i * 2] = (h >= 0x0 && h <= 0x9) ? (h + 0x30) : (h + 0x57);
		out[i * 2 + 1] = (l >= 0x0 && l <= 0x9) ? (l + 0x30) : (l + 0x57);
	}
	out[i * 2] = '\0';
}

int hex_to_raw(const char *in, int inlen, char *out){
	int i, o, h, l;

	/* need +ve even input */
	if (inlen == 0 || (inlen / 2 * 2) != inlen)
		return 1;

	for (i = o = 0; i < inlen; i += 2, o++) {
		h = (in[i] >= 0x30 && in[i] <= 0x39) ? (in[i] - 0x30) : (in[i] >= 0x41 && in[i] <= 0x64) ? (in[i] - 0x36) : (in[i] >= 0x61 && in[i] <= 0x66) ? (in[i] - 0x56) : -1;
		l = (in[i + 1] >= 0x30 && in[i + 1] <= 0x39) ? (in[i + 1] - 0x30) : (in[i + 1] >= 0x41 && in[i + 1] <= 0x64) ? (in[i + 1] - 0x36) : (in[i + 1] >= 0x61 && in[i + 1] <= 0x66) ? (in[i + 1] - 0x56) : -1;

		if (h < 0 || l < 0)
			return 1;

		out[o] = (h << 4) + l;
	}

	return 0;
}

void hex_test(){
	char buf[] = "1";
	char* out = calloc(1,sizeof(buf)*2);
	hex_from_raw(buf, sizeof(buf), out);
	fprintf(stderr, "the out is %s\n", out);

	char* in = calloc(1, sizeof(buf));

	hex_to_raw(out, sizeof(buf)* 2, in);
	fprintf(stderr, "the in is %s\n", in);

	free(in);
	free(out);

}