
#ifndef _UTIL_SIGNAL_H_
#define _UTIL_SIGNAL_H_

typedef void jsighandler_t(int);
jsighandler_t* jabber_signal(int signo, jsighandler_t *func);

typedef jsighandler_t util_signal_t;
#define util_signal jabber_signal

#endif