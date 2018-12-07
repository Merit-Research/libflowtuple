/*
 *  flowproto.c
 *
 *  Copyright (c) 2018 Merit Network, Inc.
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

#include <stdio.h>
#include <stdlib.h>

#include <flowtuple.h>

void process_record(flowtuple_record_t *record, void *args) {
    long *counts = (long*)args;
    flowtuple_data_t *data;

    if (flowtuple_record_get_type(record) == FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA) {
        data = flowtuple_record_get_data(record);

        switch (flowtuple_data_get_protocol(data)) {
            case 1:
                counts[0] += flowtuple_data_get_packet_count(data);
                break;
            case 6:
                counts[1] += flowtuple_data_get_packet_count(data);
                break;
            case 17:
                counts[2] += flowtuple_data_get_packet_count(data);
                break;
            default:
                counts[3] += flowtuple_data_get_packet_count(data);
        }
    }
}

int main(int argc, char *argv[]) {
    long counts[] = { 0, 0, 0, 0 };
    flowtuple_errno_t err;
    flowtuple_handle_t *handle;

    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(1);
    }

    handle = flowtuple_initialize(argv[1], &err);

    flowtuple_loop(handle, -1, process_record, (void*)counts);

    err = flowtuple_errno(handle);
    if (err != FLOWTUPLE_ERR_OK) {
        fprintf(stderr, "error: %s\n", flowtuple_strerr(err));
        exit(err);
    }

    printf("%ld,%ld,%ld,%ld\n", counts[0], counts[1], counts[2], counts[3]);

    flowtuple_release(handle);
    return 0;
}