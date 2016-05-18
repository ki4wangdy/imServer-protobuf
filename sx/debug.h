
#ifndef _SX_DEBUG_H_
#define _SX_DEBUG_H_

/** debugging macros */
#define ZONE __FILE__,__LINE__

/** helper functions for macros when we're debugging */
//void __sx_debug(const char *file, int line, const char *msgfmt, ...);

#ifdef SX_DEBUG
	/** print debug output */
	#define _sx_debug if(get_debug_flag()) log_debug
	/** state changes with output */
	#define _sx_state(s,st) do { _sx_debug(ZONE, "%d state change from %d to %d", s->tag, s->state, st); s->state = st; } while(0)
#else
	/* clean and efficient versions */
	#define _sx_debug if(0) log_debug
	#define _sx_state(s,st) s->state = st
#endif

#endif