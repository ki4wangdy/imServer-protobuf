
#ifndef _SX_BUFFER_H_
#define _SX_BUFFER_H_

typedef struct _sx_buf_st {
	char           *data;     /* pointer to buffer's data */
	unsigned int   len;       /* length of buffer's data */
	char           *heap;     /* beginning of malloc() block containing data, if non-NULL */

	/* function to call when this buffer gets written */
	_sx_notify_t            notify;
	void                    *notify_arg;
}* sx_buf_t;

/* buffer utilities */
sx_buf_t _sx_buffer_new(const char *data, int len, _sx_notify_t notify, void *notify_arg);
void _sx_buffer_free(sx_buf_t buf);
void _sx_buffer_clear(sx_buf_t buf);
void _sx_buffer_alloc_margin(sx_buf_t buf, int before, int after);
void _sx_buffer_set(sx_buf_t buf, char *newdata, int newlength, char *newheap);

void _sx_buffer_test();

#endif