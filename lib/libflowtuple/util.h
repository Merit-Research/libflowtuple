/*
 *  util.h
 *
 *  Copyright (c) 2018 Merit Network, Inc.
 *  Copyright (c) 2012 The Regents of the University of California.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTIL_H
#define UTIL_H

#include <inttypes.h>
#include <stdlib.h>

#include "flowtuple.h"

#define MALLOC(p, s, action) do { p = malloc(s); if (p == NULL) { action; } } while(0)
#define CALLOC(p, l, s, action) do { p = calloc(l, s); if (p == NULL) { action; } } while(0)
#define FREE(p) do { if (p != NULL) { free(p); p = NULL; } } while(0)

#define CHECK(cond, action) do { if (!(cond)) { action; } } while(0)

int _flowtuple_check_magic(flowtuple_handle_t *handle);
uint64_t _flowtuple_bytes_to_int(const uint8_t *bytes, size_t len);

#endif
