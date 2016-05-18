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

	s->expat = XML_ParserCreateNS(NULL, '|');
	XML_SetReturnNSTriplet(s->expat, 1);
	XML_SetUserData(s->expat, (void *) s);
	/* Prevent the "billion laughs" attack against expat by disabling
	* internal entity expansion.  With 2.x, forcibly stop the parser
	* if an entity is declared - this is safer and a more obvious
	* failure mode.  With older versions, simply prevent expenansion
	* of such entities. */
#ifdef HAVE_XML_STOPPARSER
	XML_SetEntityDeclHandler(s->expat, (void *) _sx_entity_declaration);
#else
	XML_SetDefaultHandler(s->expat, NULL);
#endif

#ifdef HAVE_XML_SETHASHSALT
	XML_SetHashSalt(s->expat, rand());
#endif

    _sx_debug(ZONE, "allocated new sx for %d", tag);

    return s;
}

void sx_free(sx_t s) {
    sx_buf_t buf;
    nad_t nad;
    int i;
    _sx_chain_t scan, next;

    if (s == NULL)
        return;

    _sx_debug(ZONE, "freeing sx for %d", s->tag);

    if(s->req_to != NULL) free((void*)s->req_to);
    if(s->req_from != NULL) free((void*)s->req_from);
    if(s->req_version != NULL) free((void*)s->req_version);

    if(s->res_to != NULL) free((void*)s->res_to);
    if(s->res_from != NULL) free((void*)s->res_from);
    if(s->res_version != NULL) free((void*)s->res_version);

    if(s->id != NULL) free((void*)s->id);
	if(s->nad != NULL) nad_free(s->nad);

	XML_ParserFree(s->expat);

    if (s->wbufpending != NULL)
        _sx_buffer_free(s->wbufpending);

    if(s->auth_method != NULL) free((void*)s->auth_method);
    if(s->auth_id != NULL) free((void*)s->auth_id);

    free(s);
}

int __sx_event(const char *file, int line, sx_t s, sx_event_t e, void *data) {
    return (s->cb)(s, e, data, s->cb_arg);
}
