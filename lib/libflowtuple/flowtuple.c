/*
 *  flowtuple.c
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

flowtuple_record_t *flowtuple_get_next(flowtuple_handle_t *handle) {
    ASSERT(handle != NULL, return NULL);

    int type;
    uint8_t buf[5];

    check:
    type = _flowtuple_check_magic(handle);
    switch (type) {
        case 1:
            if (wandio_read(handle->io, buf, 4) < 0) {
                handle->errno = FLOWTUPLE_ERR_FILE_READ;
                return NULL;
            }
            goto check;
        case 2:
            return _flowtuple_record_read_interval(handle);
        case 3:
            return _flowtuple_record_read_header(handle);
        case 4:
            return _flowtuple_record_read_trailer(handle);
        case 5:
        case 6:
            return _flowtuple_record_read_class(handle);
        case 7:
        case 0:
            return _flowtuple_record_read_data(handle);
        default:
            return NULL;
    }
}

const char *flowtuple_handle_get_uri(flowtuple_handle_t *handle) {
    ASSERT(handle != NULL, return NULL);
    return handle->uri;
}

flowtuple_record_t *flowtuple_handle_get_last_record(flowtuple_handle_t *handle) {
    ASSERT(handle != NULL, return NULL);
    flowtuple_record_t *ret;
    MALLOC(ret, sizeof(flowtuple_record_t), return NULL);
    memcpy(ret, &(handle->last_record), sizeof(flowtuple_record_t));
    return ret;
}