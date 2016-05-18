
#ifndef _UTIL_MD5_H_
#define _UTIL_MD5_H_

typedef uint8_t  md5_byte_t; /* 8-bit byte */
typedef uint32_t md5_word_t; /* 32-bit word */

/* Define the state of the MD5 Algorithm. */
typedef struct md5_state_s {
	md5_word_t count[2];	/* message length in bits, lsw first */
	md5_word_t abcd[4];		/* digest buffer */
	md5_byte_t buf[64];		/* accumulate block */
} md5_state_t;

#ifdef __cplusplus
extern "C"
{
#endif

/* Initialize the algorithm. */
void md5_init(md5_state_t *pms);

/* Append a string to the message. */
void md5_append(md5_state_t *pms, const md5_byte_t *data, int nbytes);

/* Finish the message and return the digest. */
void md5_finish(md5_state_t *pms, md5_byte_t digest[16]);

/* Test the Md5 function*/
void md5_test();

#define util_md5_init md5_init
#define util_md5_append md5_append
#define util_md5_finish md5_finish

#ifdef __cplusplus
}  /* end extern "C" */
#endif

#endif