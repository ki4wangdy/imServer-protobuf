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

sx_t sx_new(int tag, sx_callback_t cb, void *arg, int memcacheq_fd) {
    sx_t s;
    int i;

    assert((int) (cb != NULL));
    s = (sx_t) calloc(1, sizeof(struct _sx_st));
    s->tag = tag;
    s->cb = cb;
    s->cb_arg = arg;
	s->memcacheq_fd = memcacheq_fd;

    return s;
}

void sx_free(sx_t s) {

	if (s != NULL && s->wbufpending != NULL){
		_sx_buffer_free(s->wbufpending);
	}
    free(s);
}

int __sx_event(const char *file, int line, sx_t s, sx_event_t e, void *data) {
    return (s->cb)(s, e, data, s->cb_arg);
}
