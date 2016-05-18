
#include <sx.h>

/** utility: make a new buffer
if len>0 but data is NULL, the buffer will contain that many bytes
of garbage, to be overwritten by caller. otherwise, data pointed to
by 'data' will be copied into buf */
sx_buf_t _sx_buffer_new(const char *data, int len, _sx_notify_t notify, void *notify_arg) {
	sx_buf_t buf;

	buf = (sx_buf_t)calloc(1, sizeof(struct _sx_buf_st));

	if (len <= 0) {
		buf->data = buf->heap = NULL;
		buf->len = 0;
	}
	else {
		buf->data = buf->heap = (char *)malloc(sizeof(char)* len);
		if (data != NULL)
			memcpy(buf->data, data, len);
		else
			memset(buf->data, 0, len);  /* catch uninitialized use */
		buf->len = len;
	}

	buf->notify = notify;
	buf->notify_arg = notify_arg;

	return buf;
}

/** utility: kill a buffer */
void _sx_buffer_free(sx_buf_t buf) {
	if (buf->heap != NULL)
		free(buf->heap);

	free(buf);
}

/** utility: clear out a buffer, but don't deallocate it */
void _sx_buffer_clear(sx_buf_t buf) {
	if (buf->heap != NULL) {
		free(buf->heap);
		buf->heap = NULL;
	}
	buf->data = NULL;
	buf->len = 0;
}

/** utility: ensure a certain amount of allocated space adjacent to buf->data */
void _sx_buffer_alloc_margin(sx_buf_t buf, int before, int after)
{
	char *new_heap;

	assert(before >= 0);
	assert(after >= 0);

	/* If there wasn't any data in the buf, we can just allocate space for the margins */
	if (buf->data == NULL || buf->len == 0) {
		if (buf->heap != NULL)
			buf->heap = realloc(buf->heap, before + after);
		else
			buf->heap = malloc(before + after);
		buf->data = buf->heap + before;
		return;
	}

	if (buf->heap != NULL) {
		int old_leader = buf->data - buf->heap;
		/* Hmmm, maybe we can just call realloc() ? */
		if (old_leader >= before && old_leader <= (before * 4)) {
			buf->heap = realloc(buf->heap, before + buf->len + after);
			buf->data = buf->heap + old_leader;
			return;
		}
	}

	/* Most general case --- allocate a new buffer, copy stuff over, free the old one. */
	new_heap = malloc(before + buf->len + after);
	memcpy(new_heap + before, buf->data, buf->len);
	if (buf->heap != NULL)
		free(buf->heap);
	buf->heap = new_heap;
	buf->data = new_heap + before;
}

/** utility: reset a sx_buf_t's contents. If newheap is non-NULL it is assumed to be 'data's malloc block and ownership of the block is taken by the buffer. If newheap is NULL then the data is copied. */
void _sx_buffer_set(sx_buf_t buf, char* newdata, int newlength, char* newheap)
{
	if (newheap == NULL) {
		buf->len = 0;
		_sx_buffer_alloc_margin(buf, 0, newlength);
		if (newlength > 0)
			memcpy(buf->data, newdata, newlength);
		buf->len = newlength;
		return;
	}

	_sx_buffer_clear(buf);
	buf->data = newdata;
	buf->len = newlength;
	buf->heap = newheap;
}

void _sx_buffer_test(){
	sx_buf_t t = _sx_buffer_new(NULL,10,NULL,NULL);

	char buf[] = "abc";
	int size = strlen(buf);

	_sx_buffer_alloc_margin(t,t->len,size+1);
	memcpy(t,buf,size);

	_sx_buffer_free(t);
}
