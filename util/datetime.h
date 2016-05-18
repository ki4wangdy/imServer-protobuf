
#ifndef _UTIL_DATETIME_H_
#define _UTIL_DATETIME_H_

typedef enum {
	dt_DATE = 1,
	dt_TIME = 2,
	dt_DATETIME = 3,
	dt_LEGACY = 4
} datetime_t;

time_t  datetime_in(char *date);
void datetime_out(time_t t, datetime_t type, char *date, int datelen);

void datetime_test();

#define util_datetime_in datetime_in
#define util_datetime_out datetime_out

#endif