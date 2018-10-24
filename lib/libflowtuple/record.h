/*
 *  record.h
 *
 *  Copyright (c) 2018 Merit Network, Inc.
 *  Copyright (c) 2018 The Regents of the University of California.
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

#ifndef RECORD_H
#define RECORD_H

#include "flowtuple.h"
#include "fttypes.h"

flowtuple_record_t *_flowtuple_read_interval(flowtuple_handle_t *handle);
flowtuple_record_t *_flowtuple_read_header(flowtuple_handle_t *handle);
flowtuple_record_t *_flowtuple_read_trailer(flowtuple_handle_t *handle);
flowtuple_record_t *_flowtuple_read_flowtuple_class(flowtuple_handle_t *handle);
flowtuple_record_t *_flowtuple_read_flowtuple_data(flowtuple_handle_t *handle);

#endif
