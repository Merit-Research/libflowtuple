/*
 *  record.c
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

#include <string.h>

#include "flowtuple.h"
#include "fttypes.h"
#include "util.h"
#include "record.h"

void flowtuple_record_free(flowtuple_record_t *record) {
    ASSERT(record != NULL, return);

    if (record->type == FLOWTUPLE_RECORD_TYPE_HEADER) {
        FREE(record->record.header.traceuri);
        FREE(record->record.header.plugins);
    }

    FREE(record);
}

flowtuple_record_type_t flowtuple_record_get_type(flowtuple_record_t *record) {
    ASSERT(record != NULL, return FLOWTUPLE_RECORD_TYPE_NULL);
    return record->type;
}

flowtuple_header_t *flowtuple_record_get_header(flowtuple_record_t *record) {
    ASSERT(record != NULL, return NULL);
    return &(record->record.header);
}

flowtuple_trailer_t *flowtuple_record_get_trailer(flowtuple_record_t *record) {
    ASSERT(record != NULL, return NULL);
    return &(record->record.trailer);
}

flowtuple_interval_t *flowtuple_record_get_interval(flowtuple_record_t *record) {
    ASSERT(record != NULL, return NULL);
    return &(record->record.interval);
}

flowtuple_class_t *flowtuple_record_get_class(flowtuple_record_t *record) {
    ASSERT(record != NULL, return NULL);
    return &(record->record.ftclass);
}

flowtuple_data_t *flowtuple_record_get_data(flowtuple_record_t *record) {
    ASSERT(record != NULL, return NULL);
    return &(record->record.data);
}

flowtuple_record_t *_flowtuple_record_read_header(flowtuple_handle_t *handle) {
    uint8_t *buf = NULL;
    flowtuple_header_t header;
    flowtuple_record_t *ret;
    int64_t wand;

    CALLOC(ret, 1, sizeof(flowtuple_header_t), goto nomem);
    CALLOC(buf, 26, sizeof(uint8_t), goto nomem);

    wand = wandio_read(handle->io, buf, 14);
    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
        goto fail;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
        goto fail;
    }

    header.version_major = (uint8_t)_flowtuple_bytes_to_int(buf + 4, 1);
    header.version_minor = (uint8_t)_flowtuple_bytes_to_int(buf + 5, 1);
    header.local_init_time = (uint32_t)_flowtuple_bytes_to_int(buf + 6, 4);
    header.interval_length = (uint16_t)_flowtuple_bytes_to_int(buf + 10, 2);

    header.traceuri_len = (uint16_t)_flowtuple_bytes_to_int(buf + 12, 2);
    FREE(buf);
    if (header.traceuri_len != 0) {
        CALLOC(buf, header.traceuri_len + 1, sizeof(uint8_t), goto nomem);
        CALLOC(header.traceuri, header.traceuri_len + 1, sizeof(uint8_t), goto nomem);
        wandio_read(handle->io, buf, header.traceuri_len);
        memcpy(header.traceuri, buf, header.traceuri_len * sizeof(uint8_t));
        FREE(buf);
    } else {
        header.traceuri = NULL;
    }

    CALLOC(buf, 2, sizeof(uint8_t), return NULL);
    wandio_read(handle->io, buf, 2);
    header.plugin_cnt = (uint16_t)_flowtuple_bytes_to_int(buf, 2);
    FREE(buf);

    CALLOC(buf, (size_t)header.plugin_cnt * 4, sizeof(uint8_t), goto nomem);
    CALLOC(header.plugins, header.plugin_cnt, sizeof(uint32_t), goto nomem);

    wandio_read(handle->io, buf, header.plugin_cnt * 4);
    for (size_t i = 0; i < header.plugin_cnt; i++) {
        header.plugins[i] = (uint32_t)_flowtuple_bytes_to_int(buf + (4 * i), 4);
    }

    ret->type = FLOWTUPLE_RECORD_TYPE_HEADER;
    ret->record.header = header;
    handle->last_record = *ret;

    FREE(buf);
    return ret;

    nomem:
    handle->errno = FLOWTUPLE_ERR_MEM;

    fail:
    FREE(buf);
    flowtuple_record_free(ret);
    return NULL;
}

flowtuple_record_t *_flowtuple_record_read_interval(flowtuple_handle_t *handle) {
    uint8_t *buf = NULL;
    flowtuple_interval_t interval;
    flowtuple_record_t *ret;
    int64_t wand;

    CALLOC(ret, 1, sizeof(flowtuple_record_t), goto nomem);
    CALLOC(buf, 10, sizeof(uint8_t), goto nomem);

    wand = wandio_read(handle->io, buf, 10);
    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
        goto fail;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
        goto fail;
    }

    interval.number = (uint16_t)_flowtuple_bytes_to_int(buf + 4, 2);
    interval.time = (uint32_t)_flowtuple_bytes_to_int(buf + 6, 4);

    ret->type = FLOWTUPLE_RECORD_TYPE_INTERVAL;
    ret->record.interval = interval;
    handle->last_record = *ret;

    FREE(buf);
    return ret;

    nomem:
    handle->errno = FLOWTUPLE_ERR_MEM;

    fail:
    FREE(buf);
    flowtuple_record_free(ret);
    return NULL;
}

flowtuple_record_t *_flowtuple_record_read_trailer(flowtuple_handle_t *handle) {
    uint8_t *buf = NULL;
    flowtuple_trailer_t trailer;
    flowtuple_record_t *ret;
    int64_t wand;

    CALLOC(ret, 1, sizeof(flowtuple_record_t), goto nomem);
    CALLOC(buf, 44, sizeof(uint8_t), goto nomem);

    wand = wandio_read(handle->io, buf, 44);
    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
        goto fail;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
        goto fail;
    }

    trailer.packet_cnt = _flowtuple_bytes_to_int(buf + 4, 8);
    trailer.accepted_cnt = _flowtuple_bytes_to_int(buf + 12, 8);
    trailer.dropped_cnt = _flowtuple_bytes_to_int(buf + 20, 8);
    trailer.first_packet_time = (uint32_t)_flowtuple_bytes_to_int(buf + 28, 4);
    trailer.last_packet_time = (uint32_t)_flowtuple_bytes_to_int(buf + 32, 4);
    trailer.local_final_time = (uint32_t)_flowtuple_bytes_to_int(buf + 36, 4);
    trailer.runtime = (uint32_t)_flowtuple_bytes_to_int(buf + 40, 4);

    ret->type = FLOWTUPLE_RECORD_TYPE_TRAILER;
    ret->record.trailer = trailer;
    handle->last_record = *ret;

    FREE(buf);
    return ret;

    nomem:
    handle->errno = FLOWTUPLE_ERR_MEM;

    fail:
    FREE(buf);
    flowtuple_record_free(ret);
    return NULL;
}

flowtuple_record_t *_flowtuple_record_read_class(flowtuple_handle_t *handle) {
    uint8_t *buf = NULL;
    flowtuple_class_t ftclass;
    flowtuple_record_t *ret;
    int is_start;
    int64_t wand;

    CALLOC(ret, 1, sizeof(flowtuple_record_t), goto nomem);

    if (handle->last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA ||
        (handle->last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS &&
         handle->last_record.record.ftclass.is_start)) {
        is_start = 0;
        CALLOC(buf, 6, sizeof(uint8_t), goto nomem);
        wand = wandio_read(handle->io, buf, 6);
    } else {
        is_start = 1;
        CALLOC(buf, 10, sizeof(uint8_t), goto nomem);
        wand = wandio_read(handle->io, buf, 10);
    }

    if (wand == 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_EOF;
        goto fail;
    } else if (wand < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
        goto fail;
    }

    ftclass.magic = (uint32_t)_flowtuple_bytes_to_int(buf, 4);
    ftclass.class_type = (uint16_t)_flowtuple_bytes_to_int(buf + 4, 2);

    if (is_start) {
        ftclass.key_count = (uint32_t) _flowtuple_bytes_to_int(buf + 6, 4);
    } else {
        ftclass.key_count = 0;
    }

    ftclass.is_start = is_start;

    ret->type = FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS;
    ret->record.ftclass = ftclass;
    handle->last_record = *ret;

    FREE(buf);
    return ret;

    nomem:
    handle->errno = FLOWTUPLE_ERR_MEM;

    fail:
    FREE(buf);
    flowtuple_record_free(ret);
    return NULL;
}

flowtuple_record_t *_flowtuple_record_read_data(flowtuple_handle_t *handle) {
    ASSERT(handle != NULL, return NULL);

    uint8_t *buf = NULL;
    uint32_t magic;
    size_t offset;
    flowtuple_data_t data;
    flowtuple_record_t *ret;
    flowtuple_record_t last_record;

    CALLOC(ret, 1, sizeof(flowtuple_record_t), goto nomem);

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
        goto fail;
    }

    magic = last_record.record.ftclass.magic;

    if (magic == 0x53495854) {
        /* SIXT */
        CALLOC(buf, 20, sizeof(uint8_t), goto nomem);
        wandio_read(handle->io, buf, 20);
    } else if (magic == 0x53495855) {
        /* SIXU */
        CALLOC(buf, 21, sizeof(uint8_t), goto nomem);
        wandio_read(handle->io, buf, 21);
    } else {
        /* something's wrong */
        handle->errno = FLOWTUPLE_ERR_WRONG_MAGIC;
        goto fail;
    }

    data.src_ip = (uint32_t)_flowtuple_bytes_to_int(buf, 4);

    offset = 4;
    if (magic == 0x53495854) {
        data.has_slash_eight = 1;
        data.dst_ip.y.b = (uint8_t)_flowtuple_bytes_to_int(buf + offset, 1);
        data.dst_ip.y.c = (uint8_t)_flowtuple_bytes_to_int(buf + offset + 1, 1);
        data.dst_ip.y.d = (uint8_t)_flowtuple_bytes_to_int(buf + offset + 2, 1);
        offset += 3;
    } else {
        data.has_slash_eight = 0;
        data.dst_ip.x = (uint32_t)_flowtuple_bytes_to_int(buf + offset, 4);
        offset += 4;
    }

    data.src_port = (uint16_t)_flowtuple_bytes_to_int(buf + offset, 2);
    data.dst_port = (uint16_t)_flowtuple_bytes_to_int(buf + offset + 2, 2);
    data.proto = (uint8_t)_flowtuple_bytes_to_int(buf + offset + 4, 1);
    data.ttl = (uint8_t)_flowtuple_bytes_to_int(buf + offset + 5, 1);
    data.tcp_flags = (uint8_t)_flowtuple_bytes_to_int(buf + offset + 6, 1);
    data.ip_len = (uint16_t)_flowtuple_bytes_to_int(buf + offset + 7, 2);
    data.pkt_cnt = (uint32_t)_flowtuple_bytes_to_int(buf + offset + 9, 4);

    ret->type = FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA;
    ret->record.data = data;
    handle->last_record = *ret;

    FREE(buf);
    return ret;

    nomem:
    handle->errno = FLOWTUPLE_ERR_MEM;

    fail:
    FREE(buf);
    flowtuple_record_free(ret);
    return NULL;
}