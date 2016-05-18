
#ifndef _SX_CONST_H_
#define _SX_CONST_H_

/** things that can happen */
typedef enum {
	event_WANT_READ,        /* we want read actions */
	event_WANT_WRITE,       /* we want write actions */
	event_READ,             /* read some stuff for me */
	event_WRITE,            /* write this to the fd */
	event_PACKET,           /* got a packet */
	event_CLOSED,           /* its over */
	event_ERROR             /* something's wrong */
} sx_event_t;

/** connection states */
typedef enum {
	state_NONE,             /* pre-init */
	state_STREAM,           /* stream established */
	state_CLOSED            /* closed (same as NONE, but can't be used any more) */
} _sx_state_t;

/** connection types */
typedef enum {
	type_NONE,
	type_CLIENT,            /* we initiated the connection */
	type_SERVER             /* they initiated */
} _sx_type_t;

#endif