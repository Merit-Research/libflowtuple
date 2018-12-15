/*
 *  util.c
 *
 *  Copyright (c) 2018 Merit Network, Inc.
 *  Copyright (c) 2012 The Regents of the University of California.
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

#include <wandio.h>

#include "util.h"
#include "fttypes.h"

int _flowtuple_check_magic(flowtuple_handle_t *handle) {
    char *buf;
    int ret;
    int64_t peek;
    CALLOC(buf, 5, sizeof(char), return -1);

    if ((peek = wandio_peek(handle->io, buf, 4)) == 0) {
        ret = -1; /* EOF */
    } else if (peek < 0) {
        handle->errno = FLOWTUPLE_ERR_FILE_READ;
        ret = -1;
    } else if (strcmp(buf, "EDGR") == 0) {
        ret = 1;
    } else if (strcmp(buf, "INTR") == 0) {
        ret = 2;
    } else if (strcmp(buf, "HEAD") == 0) {
        ret = 3;
    } else if (strcmp(buf, "FOOT") == 0) {
        ret = 4;
    } else if (strcmp(buf, "SIXT") == 0) {
        ret = 5;
    } else if (strcmp(buf, "SIXU") == 0) {
        ret = 6;
    } else if (strcmp(buf, "DATA") == 0) {
        ret = 7;
    } else {
        ret = 0;
    }

    FREE(buf);
    return ret;
}

uint64_t _flowtuple_bytes_to_int(const uint8_t *bytes, size_t len) {
    uint64_t ret = 0;

    for (size_t i = 0; i < len; i++) {
        ret |= (uint64_t)(bytes[i] << (8 * (len - 1 - i)));
    }

    return ret;
}
