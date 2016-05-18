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

#ifndef INCL_SX_H
#define INCL_SX_H

#include <expat.h>
#include <util/util.h>
#include <const.h>

typedef struct _sx_st*			sx_t;
typedef struct _sx_env_st*		sx_env_t;
typedef struct _sx_plugin_st*	sx_plugin_t;
typedef struct _sx_chain_st*	_sx_chain_t;

typedef void(*_sx_notify_t)(sx_t s, void *arg);
typedef int(*sx_callback_t)(sx_t s, sx_event_t e, void *data, void *arg);
typedef int(*sx_plugin_init_t)(sx_env_t env, sx_plugin_t p, va_list args);

#include <buffer.h>
#include <sx_def.h>
#include <error.h>

#include <plugins.h>
#include <debug.h>

#include <db/memcacheq.h>

#endif
