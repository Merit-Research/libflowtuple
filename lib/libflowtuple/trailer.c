/*
 *  trailer.c
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

uint64_t flowtuple_trailer_get_packet_count(flowtuple_trailer_t *trailer) {
    ASSERT(trailer != NULL, return 0);
    return trailer->packet_cnt;
}

uint64_t flowtuple_trailer_get_accepted_count(flowtuple_trailer_t *trailer) {
    ASSERT(trailer != NULL, return 0);
    return trailer->accepted_cnt;
}

uint64_t flowtuple_trailer_get_dropped_count(flowtuple_trailer_t *trailer) {
    ASSERT(trailer != NULL, return 0);
    return trailer->dropped_cnt;
}

uint32_t flowtuple_trailer_get_first_packet_time(flowtuple_trailer_t *trailer) {
    ASSERT(trailer != NULL, return 0);
    return trailer->first_packet_time;
}

uint32_t flowtuple_trailer_get_last_packet_time(flowtuple_trailer_t *trailer) {
    ASSERT(trailer != NULL, return 0);
    return trailer->last_packet_time;
}

uint32_t flowtuple_trailer_get_local_final_time(flowtuple_trailer_t *trailer) {
    ASSERT(trailer != NULL, return 0);
    return trailer->local_final_time;
}

uint32_t flowtuple_trailer_get_runtime(flowtuple_trailer_t *trailer) {
    ASSERT(trailer != NULL, return 0);
    return trailer->runtime;
}
