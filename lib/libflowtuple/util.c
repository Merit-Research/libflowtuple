/*
 *  util.c
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
