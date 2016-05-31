/*
 * jabberd - Jabber Open Source Server
 * Copyright (c) 2002 Jeremie Miller, Thomas Muldowney,
 *                    Ryan Eatmon, Robert Norris
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA02111-1307USA
 */

#include "sx.h"

/** handler for read data */
void _sx_process_read(sx_t s, sx_buf_t buf) {
    char *errstring;
    int i;
    int ns, elem;

    /* count bytes read */
    s->rbytes += buf->len;

	/** process the buffer content */
	_sx_debug(ZONE, "the buf content is %s", buf->data);
	_sx_event(s, event_PACKET, (void *)buf);

    /* done with the buffer */
    _sx_buffer_free(buf);

}

/** we can read */
int sx_can_read(sx_t s) {
    sx_buf_t in, out;
    int read, ret;

    assert((int) (s != NULL));

    /* do we care? */
	if (!s->want_read && (s->state >= state_CLOSED)
		|| s->state < state_STREAM){
		/* no more thanks */
		return 0;
	}

    /* new buffer */
    in = _sx_buffer_new(NULL, 1024, NULL, NULL);

    /* get them to read stuff */
    read = _sx_event(s, event_READ, (void *) in);

    /* bail if something went wrong */
    if(read < 0) {
        _sx_buffer_free(in);
        s->want_read = 0;
        s->want_write = 0;
        return 0;
    }

    if(read == 0) {
        /* nothing to read
         * should never happen because we did get a read event,
         * thus there is something to read, or error handled
         * via (read < 0) block before (errors return -1) */
        _sx_debug(ZONE, "decoded 0 bytes read data - this should not happen");
        _sx_buffer_free(in);

    } else {
        /* into the parser with you */
        _sx_process_read(s, in);
    }

    if(s->state == state_CLOSED)
        return 0;

	if(s->want_write){
		_sx_event(s, event_WANT_WRITE, NULL);
	}
    return s->want_read;
}

/** we can write */
static int _sx_get_pending_write(sx_t s) {
    sx_buf_t in = NULL;
    int ret;

    assert(s != NULL);

    if (s->wbufpending != NULL) {
		/* there's already a pending buffer ready to write */
		return 0;
    }

    /* get the first buffer off the queue */
	char* data = NULL;
	int len = 0;
	char* serve1 = "server1";
	int result = memcacheq_get(s->memcacheq_fd,serve1,&data,&len);
	if(result == 1){
		char* temp_data = b64_decode(data);
		free(data);
		int temp_len = strlen(temp_data);
		in = _sx_buffer_new(temp_data,temp_len,NULL,NULL);
		free(temp_data);
	}

	s->want_write = (in == NULL) ? 0 : 1;
	s->wbufpending = (in != NULL ? in : NULL);	

    return 0;
}

int sx_can_write(sx_t s) {
    sx_buf_t out;
    int ret, written;

    assert((int) (s != NULL));

    /* do we care? */
	if (!s->want_write && (s->state >= state_CLOSED)
		|| s->state < state_STREAM){
		/* no more thanks */
		return 0;
	}

    ret = _sx_get_pending_write(s);
    if (ret < 0) {
        /* fatal error */
        _sx_debug(ZONE, "fatal error after attempt to write on fd %d", s->tag);
        /* permanent error so inform the app it can kill us */
        sx_kill(s);
        return 0;
    }

    /* if there's nothing to write, then we're done */
    if(s->wbufpending == NULL) {
        if(s->want_read) _sx_event(s, event_WANT_READ, NULL);
        return s->want_write;
    }

    out = s->wbufpending;
    s->wbufpending = NULL;

    /* get the callback to do the write */
    written = _sx_event(s, event_WRITE, (void *) out);

    if(written < 0) {
        /* bail if something went wrong */
        _sx_buffer_free(out);
        s->want_read = 0;
        s->want_write = 0;
        return 0;
    } else if(written < out->len) {
        /* if not fully written, this buffer is still pending */
        out->len -= written;
        out->data += written;
        s->wbufpending = out;
        s->want_write ++;
    } else {
        /* notify */
        if(out->notify != NULL)
            (out->notify)(s, out->notify_arg);

        /* done with this */
        _sx_buffer_free(out);
    }

	if(s->state == state_CLOSED){
		return 0;
	}

    return s->want_write;
}

/** send raw data out */
int _sx_raw_write(sx_t s, char *buf, int len) {

    /* siltently drop it if we're closing or closed */
    if(s->state >= state_CLOSED) {
        log_debug(ZONE, "stream closed, dropping outgoing raw data");
        return 1;
    }

    /* ready to go */
	char* b64_buf = b64_encode(buf,len);
	int b64_len = strlen(b64_buf); 

	char* server1 = "server1";
	int result = memcacheq_set(s->memcacheq_fd,server1,b64_buf,b64_len);
	assert(result > 0);

	free(b64_buf);

    /* things to write */
    s->want_write = 1;

    return 0;
}

/** app version */
void sx_raw_write(sx_t s, char *buf, int len) {

    assert((int) (s != NULL));
    assert((int) (buf != NULL));
    assert(len);

	if(_sx_raw_write(s, buf, len) == 1){
		return;
	}
    _sx_event(s, event_WANT_WRITE, NULL);
}

void sx_close(sx_t s) {
    assert((int) (s != NULL));
	if (s->state >= state_CLOSED){
		return;
	}
	_sx_state(s, state_CLOSED);
	_sx_event(s, event_CLOSED, NULL);
}

void sx_kill(sx_t s) {
	sx_close(s);
}
