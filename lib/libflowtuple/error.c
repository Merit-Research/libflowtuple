/*
 *  error.c
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
