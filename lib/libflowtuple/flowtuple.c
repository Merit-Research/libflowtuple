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

flowtuple_handle_t *flowtuple_initialize(const char *filename) {
    flowtuple_handle_t *handle = NULL;

    CALLOC(handle, 1, sizeof(flowtuple_handle_t), return NULL);

    strcpy(handle->uri, filename);

    handle->io = wandio_create(filename);
    if (handle->io == NULL) {
        flowtuple_release(handle);
        return NULL;
    }

    return handle;
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
