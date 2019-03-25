/*
 *  trailer.c
 *
 *  Copyright (c) 2018 Merit Network, Inc.
 *  Copyright (c) 2012 The Regents of the University of California.
 *
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "fttypes.h"
#include "util.h"

uint64_t flowtuple_trailer_get_packet_count(flowtuple_trailer_t *trailer) {
    CHECK(trailer != NULL, return 0);
    return trailer->packet_cnt;
}

uint64_t flowtuple_trailer_get_accepted_count(flowtuple_trailer_t *trailer) {
    CHECK(trailer != NULL, return 0);
    return trailer->accepted_cnt;
}

uint64_t flowtuple_trailer_get_dropped_count(flowtuple_trailer_t *trailer) {
    CHECK(trailer != NULL, return 0);
    return trailer->dropped_cnt;
}

uint32_t flowtuple_trailer_get_first_packet_time(flowtuple_trailer_t *trailer) {
    CHECK(trailer != NULL, return 0);
    return trailer->first_packet_time;
}

uint32_t flowtuple_trailer_get_last_packet_time(flowtuple_trailer_t *trailer) {
    CHECK(trailer != NULL, return 0);
    return trailer->last_packet_time;
}

uint32_t flowtuple_trailer_get_local_final_time(flowtuple_trailer_t *trailer) {
    CHECK(trailer != NULL, return 0);
    return trailer->local_final_time;
}

uint32_t flowtuple_trailer_get_runtime(flowtuple_trailer_t *trailer) {
    CHECK(trailer != NULL, return 0);
    return trailer->runtime;
}
