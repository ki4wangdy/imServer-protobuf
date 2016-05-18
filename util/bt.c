
#include <util.h>
#include <execinfo.h>

#define maxBackTraceFrames 20

void printStackTrace() {
	void *b[maxBackTraceFrames];
	char buf[1024] = "";
	char* temp = buf;

	int size = backtrace(b, maxBackTraceFrames);
	char** strings;
	strings = backtrace_symbols(b, size);
	int i = 0;
	for (; i < size; ++i) {
		int size = strlen(strings[i]);
		memcpy(buf,strings[i],size);
		temp += size;
		memcpy(temp,"\n",sizeof("\n")-1);
	}  
	free(strings);

	fprintf(stderr,"the bt %s\n",buf);

}

