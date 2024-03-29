/*
 *  flowtuple.c
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

#include <stdlib.h>
#include <string.h>

#include <wandio.h>

#include "flowtuple.h"
#include "fttypes.h"
#include "util.h"
#include "record.h"

flowtuple_handle_t *flowtuple_initialize(const char *filename, flowtuple_errno_t *err) {
    flowtuple_errno_t local_err = FLOWTUPLE_ERR_OK;
    flowtuple_handle_t *handle = NULL;

    if (filename == NULL) {
        local_err = FLOWTUPLE_ERR_FILE_OPEN;
        goto fail;
    }

    CALLOC(handle, 1, sizeof(flowtuple_handle_t), goto nomem);

    CALLOC(handle->uri, strlen(filename) + 1, sizeof(char), goto nomem);
    strcpy(handle->uri, filename);

    handle->io = wandio_create(filename);
    if (handle->io == NULL) {
        local_err = FLOWTUPLE_ERR_FILE_OPEN;
        goto fail;
    }

    *err = local_err;
    handle->errno = local_err;
    return handle;

    nomem:
    local_err = FLOWTUPLE_ERR_MEM;

    fail:
    *err = local_err;
    flowtuple_release(handle);
    return NULL;
}

void flowtuple_release(flowtuple_handle_t *handle) {
    if (handle == NULL) {
        return;
    }

    if (handle->io != NULL) {
        wandio_destroy(handle->io);
    }

    if (handle->uri != NULL) {
        FREE(handle->uri);
    }

    FREE(handle);
}

static int _flowtuple_get_next(flowtuple_handle_t *handle, flowtuple_record_t **record) {
    int type;
    uint8_t buf[5];

    if (*record == NULL) {
        CALLOC(*record, 1, sizeof(flowtuple_record_t), return -1);
    }

    check:
    type = _flowtuple_check_magic(handle);
    switch (type) {
        case 1:
            if (wandio_read(handle->io, buf, 4) < 0) {
                handle->errno = FLOWTUPLE_ERR_FILE_READ;
                *record = NULL;
            }
            goto check;
        case 2:
            _flowtuple_record_read_interval(handle, *record);
            break;
        case 3:
            _flowtuple_record_read_header(handle, *record);
            break;
        case 4:
            _flowtuple_record_read_trailer(handle, *record);
            break;
        case 5:
        case 6:
            if ((handle->last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA &&
                    handle->last_record.record.data.number < handle->last_record.record.data.class_start.key_count_host) ||
                (handle->last_record.type == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS &&
                    handle->last_record.record.ftclass.key_count_host > 0)) {
                /* fixes issue #2
                 * makes sure that we read a data record if a class
                 * is unexpected
                 */
                _flowtuple_record_read_data(handle, *record);
                break;
            }
            _flowtuple_record_read_class(handle, *record);
            break;
        case 7:
        case 0:
            _flowtuple_record_read_data(handle, *record);
            break;
        default:
            flowtuple_record_free(*record);
            *record = NULL;
            break;
    }

    if (handle->errno != FLOWTUPLE_ERR_OK) {
        return -1;
    }
    return 0;
}

flowtuple_record_t *flowtuple_get_next(flowtuple_handle_t *handle) {
    CHECK(handle != NULL, return NULL);
    flowtuple_record_t *record = NULL;
    int result = _flowtuple_get_next(handle, &record);

    if (result < 0) {
        return NULL;
    }
    return record;
}

long flowtuple_loop(flowtuple_handle_t *handle, long cnt, flowtuple_handler callback, void *args) {
    CHECK(handle != NULL, return 0);
    flowtuple_record_t *record_ptr = NULL;
    long ret = 0;
    int res;

    while (cnt < 0 || ret < cnt) {
        res = _flowtuple_get_next(handle, &record_ptr);

        if (res < 0 || record_ptr == NULL) {
            break;
        }

        callback(record_ptr, args);
        ret++;
    }

    flowtuple_record_free(record_ptr);
    return ret;
}

const char *flowtuple_handle_get_uri(flowtuple_handle_t *handle) {
    CHECK(handle != NULL, return NULL);
    return handle->uri;
}

flowtuple_record_t *flowtuple_handle_get_last_record(flowtuple_handle_t *handle) {
    CHECK(handle != NULL, return NULL);
    flowtuple_record_t *ret;
    MALLOC(ret, sizeof(flowtuple_record_t), return NULL);
    memcpy(ret, &(handle->last_record), sizeof(flowtuple_record_t));
    return ret;
}
