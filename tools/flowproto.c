/*
 *  flowproto.c
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
        exit(-1);
    }

    handle = flowtuple_initialize(argv[1], &err);

    flowtuple_loop(handle, -1, process_record, (void*)counts);

    err = err == FLOWTUPLE_ERR_OK ? flowtuple_errno(handle) : err;
    if (err != FLOWTUPLE_ERR_OK) {
        fprintf(stderr, "error: %s\n", flowtuple_strerr(err));
        flowtuple_release(handle);
        exit(err);
    }

    printf("%ld,%ld,%ld,%ld\n", counts[0], counts[1], counts[2], counts[3]);

    flowtuple_release(handle);
    return 0;
}
