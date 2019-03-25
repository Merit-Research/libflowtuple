/*
 *  error.c
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

#include "flowtuple.h"
#include "fttypes.h"
#include "util.h"

flowtuple_errno_t flowtuple_errno(flowtuple_handle_t *handle) {
    CHECK(handle != NULL, return FLOWTUPLE_ERR_OK);
    return handle->errno;
}

const char *flowtuple_strerr(flowtuple_errno_t err) {
    switch (err) {
        case FLOWTUPLE_ERR_MEM:
            /* out of memory */
            return "out of memory";
        case FLOWTUPLE_ERR_WRONG_MAGIC:
            /* incorrect magic */
            return "incorrect magic encountered";
        case FLOWTUPLE_ERR_CORRUPT:
            /* file appears to be corrupt */
            return "corrupt or invalid file";
        case FLOWTUPLE_ERR_FILE_OPEN:
            /* wandio could not open file */
            return "could not open file";
        case FLOWTUPLE_ERR_FILE_READ:
            /* wandio gave error on read */
            return "could not read file";
        case FLOWTUPLE_ERR_FILE_EOF:
            /* wandio gave EOF when it wasn't expected */
            return "unexpected EOF";
        case FLOWTUPLE_ERR_OK:
            /* nothing's wrong */
            return "";
        default:
            /* an unknown errno was passed */
            return "unexpected error";
    }
}
