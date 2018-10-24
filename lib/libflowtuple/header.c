/*
 *  header.c
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

#include "fttypes.h"
#include "util.h"

uint8_t flowtuple_header_get_version_major(flowtuple_header_t *header) {
    ASSERT(header != NULL, return 0);
    return header->version_major;
}

uint8_t flowtuple_header_get_version_minor(flowtuple_header_t *header) {
    ASSERT(header != NULL, return 0);
    return header->version_minor;
}

uint32_t flowtuple_header_get_local_init_time(flowtuple_header_t *header) {
    ASSERT(header != NULL, return 0);
    return header->local_init_time;
}

uint16_t flowtuple_header_get_interval_length(flowtuple_header_t *header) {
    ASSERT(header != NULL, return 0);
    return header->interval_length;
}

uint16_t flowtuple_header_get_traceuri_len(flowtuple_header_t *header) {
    ASSERT(header != NULL, return 0);
    return header->traceuri_len;
}

const char *flowtuple_header_get_traceuri(flowtuple_header_t *header) {
    ASSERT(header != NULL, return NULL);
    return (const char *)(header->traceuri);
}

uint16_t *flowtuple_header_get_plugin_count(flowtuple_header_t *header) {
    ASSERT(header != NULL, return 0);
    return header->plugin_cnt;
}

uint32_t *flowtuple_header_get_plugins(flowtuple_header_t *header) {
    ASSERT(header != NULL, return NULL);
    return header->plugins;
}
