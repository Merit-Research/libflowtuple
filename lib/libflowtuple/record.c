/*
 *  record.c
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

#include <string.h>
#include <arpa/inet.h>

#include "flowtuple.h"
#include "fttypes.h"
#include "util.h"
#include "record.h"

void flowtuple_record_free(flowtuple_record_t *record) {
    CHECK(record != NULL, return);

    if (record->type == FLOWTUPLE_RECORD_TYPE_HEADER) {
        FREE(record->record.header.traceuri);
        FREE(record->record.header.plugins);
    }

    FREE(record);
}

flowtuple_record_type_t flowtuple_record_get_type(flowtuple_record_t *record) {
    CHECK(record != NULL, return FLOWTUPLE_RECORD_TYPE_NULL);
    return record->type;
}

flowtuple_header_t *flowtuple_record_get_header(flowtuple_record_t *record) {
    CHECK(record != NULL, return NULL);
    return &(record->record.header);
}

flowtuple_trailer_t *flowtuple_record_get_trailer(flowtuple_record_t *record) {
    CHECK(record != NULL, return NULL);
    return &(record->record.trailer);
}

flowtuple_interval_t *flowtuple_record_get_interval(flowtuple_record_t *record) {
    CHECK(record != NULL, return NULL);
    return &(record->record.interval);
}

flowtuple_class_t *flowtuple_record_get_class(flowtuple_record_t *record) {
    CHECK(record != NULL, return NULL);
    return &(record->record.ftclass);
}

flowtuple_data_t *flowtuple_record_get_data(flowtuple_record_t *record) {
    CHECK(record != NULL, return NULL);
    return &(record->record.data);
}

void _flowtuple_record_read_header(flowtuple_handle_t *handle, flowtuple_record_t *record) {
    /* we may later want to remove usage of the heap in this
     * function as well, but this is the only structure where
     * we don't know the maximum sizes of the buffer */
    uint8_t *buf = NULL;
    flowtuple_header_t header;
    int64_t wand;
    uint16_t trace_uri_len_host;

    CALLOC(buf, 26, sizeof(uint8_t), goto nomem);

    wand = wandio_read(handle->io, buf, 14);
    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
        goto fail;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
        goto fail;
    }

    header.version_major = *(buf + 4);
    header.version_minor = *(buf + 5);
    header.local_init_time = *(uint32_t*)(buf + 6);
    header.interval_length = *(uint16_t*)(buf + 10);

    header.traceuri_len = *(uint16_t*)(buf + 12);
    trace_uri_len_host = ntohs(header.traceuri_len);
    FREE(buf);
    if (trace_uri_len_host != 0) {
        CALLOC(buf, trace_uri_len_host + 1, sizeof(uint8_t), goto nomem);
        CALLOC(header.traceuri, trace_uri_len_host + 1, sizeof(uint8_t), goto nomem);
        wandio_read(handle->io, buf, trace_uri_len_host);
        memcpy(header.traceuri, buf, trace_uri_len_host * sizeof(uint8_t));
        FREE(buf);
    } else {
        header.traceuri = NULL;
    }

    CALLOC(buf, 2, sizeof(uint8_t), return);
    wandio_read(handle->io, buf, 2);
    header.plugin_cnt = ntohs(*(uint16_t*)(buf));
    FREE(buf);

    CALLOC(buf, (size_t)header.plugin_cnt * 4, sizeof(uint8_t), goto nomem);
    CALLOC(header.plugins, header.plugin_cnt, sizeof(uint32_t), goto nomem);

    wandio_read(handle->io, buf, header.plugin_cnt * 4);
    for (size_t i = 0; i < header.plugin_cnt; i++) {
        header.plugins[i] = *(uint32_t*)(buf + (4 * i));
    }

    record->type = FLOWTUPLE_RECORD_TYPE_HEADER;
    record->record.header = header;
    handle->last_record = *record;

    FREE(buf);
    return;

    nomem:
    handle->errno = FLOWTUPLE_ERR_MEM;

    fail:
    FREE(buf);
}

void _flowtuple_record_read_interval(flowtuple_handle_t *handle, flowtuple_record_t *record) {
    uint8_t buf[10];
    flowtuple_interval_t interval;
    int64_t wand;

    wand = wandio_read(handle->io, buf, 10);
    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
    }

    interval.number = *(uint16_t*)(buf + 4);
    interval.time = *(uint32_t*)(buf + 6);

    record->type = FLOWTUPLE_RECORD_TYPE_INTERVAL;
    record->record.interval = interval;
    handle->last_record = *record;
}

void _flowtuple_record_read_trailer(flowtuple_handle_t *handle, flowtuple_record_t *record) {
    uint8_t buf[44];
    flowtuple_trailer_t trailer;
    int64_t wand;

    wand = wandio_read(handle->io, buf, 44);
    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
    }

    trailer.packet_cnt = *(uint64_t*)(buf + 4);
    trailer.accepted_cnt = *(uint64_t*)(buf + 12);
    trailer.dropped_cnt = *(uint64_t*)(buf + 20);
    trailer.first_packet_time = *(uint32_t*)(buf + 28);
    trailer.last_packet_time = *(uint32_t*)(buf + 32);
    trailer.local_final_time = *(uint32_t*)(buf + 36);
    trailer.runtime = *(uint32_t*)(buf + 40);

    record->type = FLOWTUPLE_RECORD_TYPE_TRAILER;
    record->record.trailer = trailer;
    handle->last_record = *record;
}

void _flowtuple_record_read_class(flowtuple_handle_t *handle, flowtuple_record_t *record) {
    flowtuple_class_t ftclass;
    int is_start;
    int64_t wand;
    uint8_t buf[10];

    if (handle->last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA ||
        (handle->last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS &&
         handle->last_record.record.ftclass.is_start)) {
        is_start = 0;
        wand = wandio_read(handle->io, buf, 6);
    } else {
        is_start = 1;
        wand = wandio_read(handle->io, buf, 10);
    }

    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
    }

    ftclass.magic = *(uint32_t*)(buf);
    ftclass.class_type = *(uint16_t*)(buf + 4);

    if (is_start) {
        ftclass.key_count = *(uint32_t*)(buf + 6);
    } else {
        ftclass.key_count = 0;
    }

    ftclass.key_count_host = ntohl(ftclass.key_count);
    ftclass.is_start = is_start;

    record->type = FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS;
    record->record.ftclass = ftclass;
    handle->last_record = *record;
}

void _flowtuple_record_read_data(flowtuple_handle_t *handle, flowtuple_record_t *record) {
    uint8_t buf[21];
    uint32_t magic;
    size_t offset;
    flowtuple_data_t data;
    flowtuple_record_t last_record;

    last_record = handle->last_record;
    if (last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS) {
        data.class_start = last_record.record.ftclass;
        data.number = 1;
    } else if (last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA) {
        data.class_start = last_record.record.data.class_start;
        data.number = last_record.record.data.number + 1;
    } else {
        /* this shouldn't happen */
        handle->errno = FLOWTUPLE_ERR_CORRUPT;
    }

    magic = last_record.record.ftclass.magic;

    if (magic == 0x54584953) {
        /* SIXT */
        wandio_read(handle->io, buf, 20);
    } else if (magic == 0x55584953) {
        /* SIXU */
        wandio_read(handle->io, buf, 21);
    } else {
        /* something's wrong */
        handle->errno = FLOWTUPLE_ERR_WRONG_MAGIC;
    }

    data.src_ip = *(uint32_t*)(buf);

    offset = 4;
    if (magic == 0x54584953) {
        data.has_slash_eight = 1;
        data.dst_ip.y.b = *(buf + offset);
        data.dst_ip.y.c = *(buf + offset + 1);
        data.dst_ip.y.d = *(buf + offset + 2);
        offset += 3;
    } else {
        data.has_slash_eight = 0;
        data.dst_ip.x = *(uint32_t*)(buf + offset);
        offset += 4;
    }

    data.src_port = *(uint16_t*)(buf + offset);
    data.dst_port = *(uint16_t*)(buf + offset + 2);
    data.proto = *(buf + offset + 4);
    data.ttl = *(buf + offset + 5);
    data.tcp_flags = *(buf + offset + 6);
    data.ip_len = *(uint16_t*)(buf + offset + 7);
    data.pkt_cnt = *(uint32_t*)(buf + offset + 9);

    record->type = FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA;
    record->record.data = data;
    handle->last_record = *record;
}
