
#include <sx/sx.h>

int main(){
	sx_buf_t ts = _sx_buffer_new(NULL,10,NULL,NULL);
	sx_buf_free(ts);
	return 0;
}