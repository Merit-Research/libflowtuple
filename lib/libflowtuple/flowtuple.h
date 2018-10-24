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

typedef enum _flowtuple_record_type_t {
    FLOWTUPLE_RECORD_TYPE_NULL,
    FLOWTUPLE_RECORD_TYPE_HEADER,
    FLOWTUPLE_RECORD_TYPE_INTERVAL,
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

const char *flowtuple_handle_get_uri(flowtuple_handle_t *handle);
flowtuple_record_t *flowtuple_handle_get_last_record(flowtuple_handle_t *handle);

flowtuple_record_type_t flowtuple_record_get_type(flowtuple_record_t *record);

flowtuple_header_t *flowtuple_record_get_header(flowtuple_record_t *record);
flowtuple_trailer_t *flowtuple_record_get_trailer(flowtuple_record_t *record);
flowtuple_interval_t *flowtuple_record_get_interval(flowtuple_record_t *record);
flowtuple_class_t *flowtuple_record_get_class(flowtuple_record_t *record);
flowtuple_data_t *flowtuple_record_get_data(flowtuple_record_t *record);

void flowtuple_record_free(flowtuple_record_t *record);

uint8_t flowtuple_header_get_version_major(flowtuple_header_t *header);
uint8_t flowtuple_header_get_version_minor(flowtuple_header_t *header);
uint32_t flowtuple_header_get_local_init_time(flowtuple_header_t *header);
uint16_t flowtuple_header_get_interval_length(flowtuple_header_t *header);
uint16_t flowtuple_header_get_traceuri_len(flowtuple_header_t *header);
const char *flowtuple_header_get_traceuri(flowtuple_header_t *header);
uint16_t *flowtuple_header_get_plugin_count(flowtuple_header_t *header);
uint32_t *flowtuple_header_get_plugins(flowtuple_header_t *header);

uint64_t flowtuple_trailer_get_packet_count(flowtuple_trailer_t *trailer);
uint64_t flowtuple_trailer_get_accepted_count(flowtuple_trailer_t *trailer);
uint64_t flowtuple_trailer_get_dropped_count(flowtuple_trailer_t *trailer);
uint32_t flowtuple_trailer_get_first_packet_time(flowtuple_trailer_t *trailer);
uint32_t flowtuple_trailer_get_last_packet_time(flowtuple_trailer_t *trailer);
uint32_t flowtuple_trailer_get_local_final_time(flowtuple_trailer_t *trailer);
uint32_t flowtuple_trailer_get_runtime(flowtuple_trailer_t *trailer);

uint16_t flowtuple_interval_get_number(flowtuple_interval_t *interval);
uint32_t flowtuple_interval_get_time(flowtuple_interval_t *interval);

uint32_t flowtuple_class_get_magic(flowtuple_class_t *ftclass);
uint16_t flowtuple_class_get_class_type(flowtuple_class_t *ftclass);
uint32_t flowtuple_class_get_key_count(flowtuple_class_t *ftclass);

flowtuple_class_t *flowtuple_data_get_class_start(flowtuple_data_t *data);
uint32_t flowtuple_data_get_number(flowtuple_data_t *data);
uint32_t flowtuple_data_get_src_ip(flowtuple_data_t *data);
uint32_t flowtuple_data_get_dest_ip_int(flowtuple_data_t *data);
flowtuple_slash_eight_t flowtuple_data_get_dest_ip_slash_eight(flowtuple_data_t *data);
uint16_t flowtuple_data_get_src_port(flowtuple_data_t *data);
uint16_t flowtuple_data_get_dest_port(flowtuple_data_t *data);
uint8_t flowtuple_data_get_protocol(flowtuple_data_t *data);
uint8_t flowtuple_data_get_ttl(flowtuple_data_t *data);
uint8_t flowtuple_data_get_tcp_flags(flowtuple_data_t *data);
uint16_t flowtuple_data_get_ip_len(flowtuple_data_t *data);
uint32_t flowtuple_data_get_packet_count(flowtuple_data_t *data);
int flowtuple_data_is_slash_eight(flowtuple_data_t *data);

#ifdef __cplusplus
}
#endif

#endif