
#include <sx.h>

//#define MAX_SIZE 8192
 
/** debug macro helpers */
// void __sx_debug(const char *file, int line, const char *msgfmt, ...) {
// 	va_list ap;
// 	char *pos, message[MAX_SIZE];
// 	int sz;
// 
// 	/* insert the header */
// 	snprintf(message, MAX_SIZE, "sx (%s:%d) ", file, line);
// 
// 	/* find the end and attach the rest of the msg */
// 	for (pos = message; *pos != '\0'; pos++); /*empty statement */
// 	sz = pos - message;
// 	va_start(ap, msgfmt);
// 	vsnprintf(pos, MAX_SIZE - sz, msgfmt, ap);
// 	va_end(ap);
// 	fprintf(stderr, "%s", message);
// 	fprintf(stderr, "\n");
// 	fflush(stderr);
// }