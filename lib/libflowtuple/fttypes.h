/*
 *  fttypes.h
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

#ifndef FTTYPES_H
#define FTTYPES_H

#include <inttypes.h>

#include <wandio.h>

#include "flowtuple.h"

/* Slash eight structure. */
typedef struct _flowtuple_slash_eight_t {
    uint8_t b;
    uint8_t c;
    uint8_t d;
} flowtuple_slash_eight_t;

struct _flowtuple_header_t {
    uint8_t version_major;
    uint8_t version_minor;
    uint32_t local_init_time;
    uint16_t interval_length;
    uint16_t traceuri_len;
    uint8_t *traceuri;
    uint16_t plugin_cnt;
    uint32_t *plugins;
};

struct _flowtuple_trailer_t {
    uint64_t packet_cnt;
    uint64_t accepted_cnt;
    uint64_t dropped_cnt;
    uint32_t first_packet_time;
    uint32_t last_packet_time;
    uint32_t local_final_time;
    uint32_t runtime;
};

struct _flowtuple_interval_t {
    uint16_t number;
    uint32_t time;
};

struct _flowtuple_class_t {
    uint32_t magic; /* this magic is important */
    uint16_t class_type;
    uint32_t key_count;

    /* this is so we don't have to call
     * ntohl on every creation of a
     * data struct, once is good */
    uint32_t key_count_host;

    int is_start;
};

struct _flowtuple_data_t {
    struct _flowtuple_class_t class_start;
    uint32_t number;

    uint32_t src_ip;

    int has_slash_eight;
    union {
        uint32_t x;
        flowtuple_slash_eight_t y;
    } dst_ip;

    uint16_t src_port;
    uint16_t dst_port;
    uint8_t proto;
    uint8_t ttl;
    uint8_t tcp_flags;
    uint16_t ip_len;
    uint32_t pkt_cnt;
};

struct _flowtuple_record_t {
    flowtuple_record_type_t type;
    union {
        flowtuple_header_t header;
        flowtuple_trailer_t trailer;
        flowtuple_interval_t interval;
        flowtuple_class_t ftclass;
        flowtuple_data_t data;
    } record;
};

struct _flowtuple_handle_t {
    char *uri;
    io_t *io;
    flowtuple_record_t last_record;
    flowtuple_errno_t errno;
};

#endif
