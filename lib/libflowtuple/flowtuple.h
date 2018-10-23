/*
 *  flowtuple.h
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

#ifndef FLOWTUPLE_H
#define FLOWTUPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdio.h>

typedef enum _flowtuple_record_type_t {
    FLOWTUPLE_RECORD_TYPE_NULL,
    FLOWTUPLE_RECORD_TYPE_HEADER,
    FLOWTUPLE_RECORD_TYPE_TRAILER,
    FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS,
    FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA,
} flowtuple_record_type_t;

typedef struct _flowtuple_handle_t flowtuple_handle_t;
typedef struct _flowtuple_header_t flowtuple_header_t;
typedef struct _flowtuple_trailer_t flowtuple_trailer_t;
typedef struct _flowtuple_interval_t flowtuple_interval_t;
typedef struct _flowtuple_class_t flowtuple_class_t;
typedef struct _flowtuple_data_t flowtuple_data_t;
typedef struct _flowtuple_record_t flowtuple_record_t;

typedef struct _flowtuple_slash_eight_t {
    uint8_t b;
    uint8_t c;
    uint8_t d;
} flowtuple_slash_eight_t;

flowtuple_handle_t *flowtuple_initialize(const char *filename);
void flowtuple_release(flowtuple_handle_t *handle);

flowtuple_record_t *flowtuple_get_next(flowtuple_handle_t *handle);
flowtuple_record_type_t flowtuple_record_get_type(flowtuple_record_t *record);


#ifdef __cplusplus
}
#endif

#endif