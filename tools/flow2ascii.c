/*
 *  flow2ascii.c
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
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <endian.h>
#include <arpa/inet.h>

#include <flowtuple.h>

/* long getopt options */
static const struct option long_opts[] = {
    { "help", no_argument, NULL, 'h' },
    { "octet", optional_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 },
};

/* print header object */
void header_print(flowtuple_header_t *header) {
    const char *traceuri;
    const uint32_t *plugins;
    uint16_t plugin_count;

    printf("# CORSARO_VERSION %d.%d\n",
        flowtuple_header_get_version_major(header), flowtuple_header_get_version_minor(header));
    printf("# CORSARO_INITTIME %d\n", ntohl(flowtuple_header_get_local_init_time(header)));
    printf("# CORSARO_INTERVAL %d\n", ntohs(flowtuple_header_get_interval_length(header)));

    traceuri = flowtuple_header_get_traceuri(header);
    if (traceuri != NULL) {
        printf("# CORSARO_TRACEURI %s\n", traceuri);
    }

    plugin_count = ntohs(flowtuple_header_get_plugin_count(header));
    plugins = flowtuple_header_get_plugins(header);
    for (int i = 0; i < plugin_count; i++) {
        /* really, there's only one expected plugin */
        /* not true, ...... */
        if (plugins[i] == FLOWTUPLE_MAGIC_SIXT || plugins[i] == FLOWTUPLE_MAGIC_SIXU) {
            printf("# CORSARO_PLUGIN flowtuple\n");
        } else if (plugins[i] == 0x414E4F4E) {
            printf("# CORSARO_PLUGIN anon\n");
        }
    }
}

/* print interval object */
void interval_print(flowtuple_interval_t *interval, int is_start) {
    char *tails[] = { "_END", "_START" };
    printf("# CORSARO_INTERVAL%s %d %d\n",
        tails[is_start], ntohs(flowtuple_interval_get_number(interval)),
        ntohl(flowtuple_interval_get_time(interval)));
}

/* print trailer object */
void trailer_print(flowtuple_trailer_t *trailer) {
    uint64_t ac = be64toh(flowtuple_trailer_get_accepted_count(trailer));
    uint64_t dc = be64toh(flowtuple_trailer_get_dropped_count(trailer));
    printf("# CORSARO_PACKETCNT %"PRIu64"\n", be64toh(flowtuple_trailer_get_packet_count(trailer)));

    if (ac != UINT64_MAX) {
        printf("# CORSARO_ACCEPTEDCNT %"PRIu64"\n", ac);
    }

    if (dc != UINT64_MAX) {
        printf("# CORSARO_DROPPEDCNT %"PRIu64"\n", dc);
    }

    printf("# CORSARO_FIRSTPKT %d\n", ntohl(flowtuple_trailer_get_first_packet_time(trailer)));
    printf("# CORSARO_LASTPKT %d\n", ntohl(flowtuple_trailer_get_last_packet_time(trailer)));
    printf("# CORSARO_FINALTIME %d\n", ntohl(flowtuple_trailer_get_local_final_time(trailer)));
    printf("# CORSARO_RUNTIME %d\n", ntohl(flowtuple_trailer_get_runtime(trailer)));
}

/* print class object */
void class_print(flowtuple_class_t *ftclass, int is_start) {
    char *starts[] = { "END", "START" };
    char *class_types[] = { "flowtuple_backscatter", "flowtuple_icmpreq", "flowtuple_other" };

    if (is_start) {
        printf("%s %s %d\n", starts[is_start],
               class_types[flowtuple_class_get_class_type(ftclass)],
               ntohl(flowtuple_class_get_key_count(ftclass)));
    } else {
        printf("%s %s\n", starts[is_start], class_types[flowtuple_class_get_class_type(ftclass)]);
    }
}

/* print data object */
void data_print(flowtuple_data_t *data, uint8_t first_octet) {
    uint8_t src_ip[4];
    uint8_t dst_ip[4];

    *(uint32_t*)(dst_ip) = flowtuple_data_get_dest_ip(data);
    if (flowtuple_data_is_slash_eight(data)) {
        dst_ip[0] = first_octet;
    }

    *(uint32_t*)(src_ip) = flowtuple_data_get_src_ip(data);

    printf("%u.%u.%u.%u|%u.%u.%u.%u|%u|%u|%u|%u|0x%02x|%u,%u\n",
           src_ip[0], src_ip[1], src_ip[2], src_ip[3],
           dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3],
           ntohs(flowtuple_data_get_src_port(data)),
           ntohs(flowtuple_data_get_dest_port(data)),
           flowtuple_data_get_protocol(data),
           flowtuple_data_get_ttl(data),
           flowtuple_data_get_tcp_flags(data),
           ntohs(flowtuple_data_get_ip_len(data)),
           ntohl(flowtuple_data_get_packet_count(data)));
}

void process_record(flowtuple_record_t *record, void *args) {
    int *iargs = (int*)args;
    flowtuple_record_type_t type = flowtuple_record_get_type(record);
    void *data;

    switch (type) {
        case FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS:
            data = (void*)flowtuple_record_get_class(record);
            class_print((flowtuple_class_t*)data, iargs[1]);
            iargs[1] = !iargs[1];
            break;
        case FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA:
            data = (void*)flowtuple_record_get_data(record);
            data_print((flowtuple_data_t*)data, (uint8_t)iargs[2]);
            break;
        case FLOWTUPLE_RECORD_TYPE_HEADER:
            data = (void*)flowtuple_record_get_header(record);
            header_print((flowtuple_header_t*)data);
            break;
        case FLOWTUPLE_RECORD_TYPE_TRAILER:
            data = (void*)flowtuple_record_get_trailer(record);
            trailer_print((flowtuple_trailer_t*)data);
            break;
        case FLOWTUPLE_RECORD_TYPE_INTERVAL:
            data = (void*)flowtuple_record_get_interval(record);
            interval_print((flowtuple_interval_t*)data, iargs[0]);
            iargs[0] = !iargs[0];
            break;
        case FLOWTUPLE_RECORD_TYPE_NULL:
        default:
            break;
    }
}

/* print usage */
void usage(const char *program_name) {
    printf("Usage: %s [-o octet] inputfile\n", program_name);
}

int main(int argc, char **argv) {
    flowtuple_handle_t *h;        /* handle */
    flowtuple_errno_t errno;      /* errors */
    char *filename = NULL;        /* file name */
    int trackers[] = { 1, 1, 0 }; /* is interval start, is class start, first octet */
    int c;                        /* getopt option */
    char *tmp;                    /* getopt tmp string */

    /* we expect arguments, always */
    if (argc < 2) {
        usage(argv[0]);
        return -1;
    }

    while ((c = getopt_long(argc, argv, ":ho:", long_opts, NULL)) != -1) {
        switch (c) {
            case 'h':
                /* help */
                usage(argv[0]);
                return 0;
            case 'o':
                /* octet */
                if (optarg != NULL) {
                    trackers[2] = (int)strtol(optarg, &tmp, 10);
                } else {
                    fprintf(stderr, "option --octet requires an integer option\n");
                    usage(argv[0]);
                    return -1;
                }

                if (trackers[2] == 0 && strcmp(tmp, "") != 0) {
                    fprintf(stderr, "option -o requires an integer option\n");
                    usage(argv[0]);
                    return -1;
                }

                if (trackers[2] < 0 || trackers[2] > 255) {
                    fprintf(stderr, "ERROR: octet must be between 0 and 255\n");
                    return -1;
                }
                break;
            case '?':
                if (optopt == 'o') {
                    usage(argv[0]);
                    return -1;
                }
            default:
                usage(argv[0]);
                return -1;
        }
    }

    /* get non-getopt options */
    for (int index = optind; index < argc; index++) {
        if (filename != NULL) {
            break;
        }
        filename = argv[optind];
        optind++;
    }

    /* initialize flowtuple handle */
    h = flowtuple_initialize(filename, &errno);

    /* loop through records */
    flowtuple_loop(h, -1, process_record, (void*)trackers);

    errno = errno == FLOWTUPLE_ERR_OK ? flowtuple_errno(h) : errno;
    if (errno != FLOWTUPLE_ERR_OK) {
        fprintf(stderr, "ERROR: %s\n", flowtuple_strerr(errno));
    }

    flowtuple_release(h);
    return errno;
}
