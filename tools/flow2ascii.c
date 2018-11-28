/*
 *  flow2ascii.c
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
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <flowtuple.h>

/* long getopt options */
static const struct option long_opts[] = {
    { "help", no_argument, NULL, 'h' },
    { "octet", optional_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 },
};

/* take integer and convert to array of octets */
void ntoa(uint32_t src, uint8_t *dest) {
    for (int i = 0; i < 4; i++) {
        dest[i] = (uint8_t)(src >> (8 * (3 - i)));
    }
}

/* print header object */
void header_print(flowtuple_header_t *header) {
    const char *traceuri;
    const uint32_t *plugins;
    uint16_t plugin_count;

    printf("# CORSARO_VERSION %d.%d\n",
        flowtuple_header_get_version_major(header), flowtuple_header_get_version_minor(header));
    printf("# CORSARO_INITTIME %d\n", flowtuple_header_get_local_init_time(header));
    printf("# CORSARO_INTERVAL %d\n", flowtuple_header_get_interval_length(header));

    traceuri = flowtuple_header_get_traceuri(header);
    if (traceuri != NULL) {
        printf("# CORSARO_TRACEURI %s\n", traceuri);
    }

    plugin_count = flowtuple_header_get_plugin_count(header);
    plugins = flowtuple_header_get_plugins(header);
    for (int i = 0; i < plugin_count; i++) {
        /* really, there's only one expected plugin */
        if (plugins[i] == FLOWTUPLE_MAGIC_SIXT || plugins[i] == FLOWTUPLE_MAGIC_SIXU) {
            printf("# CORSARO_PLUGIN flowtuple\n");
        }
    }
}

/* print interval object */
void interval_print(flowtuple_interval_t *interval, int is_start) {
    char *tails[] = { "_END", "_START" };
    printf("# CORSARO_INTERVAL%s %d %d\n",
        tails[is_start], flowtuple_interval_get_number(interval),
        flowtuple_interval_get_time(interval));
}

/* print trailer object */
void trailer_print(flowtuple_trailer_t *trailer) {
    uint64_t ac = flowtuple_trailer_get_accepted_count(trailer);
    uint64_t dc = flowtuple_trailer_get_dropped_count(trailer);
    printf("# CORSARO_PACKETCNT %"PRIu64"\n", flowtuple_trailer_get_packet_count(trailer));

    if (ac != UINT64_MAX) {
        printf("# CORSARO_ACCEPTEDCNT %"PRIu64"\n", ac);
    }

    if (dc != UINT64_MAX) {
        printf("# CORSARO_DROPPEDCNT %"PRIu64"\n", dc);
    }

    printf("# CORSARO_FIRSTPKT %d\n", flowtuple_trailer_get_first_packet_time(trailer));
    printf("# CORSARO_LASTPKT %d\n", flowtuple_trailer_get_last_packet_time(trailer));
    printf("# CORSARO_FINALTIME %d\n", flowtuple_trailer_get_local_final_time(trailer));
    printf("# CORSARO_RUNTIME %d\n", flowtuple_trailer_get_runtime(trailer));
}

/* print class object */
void class_print(flowtuple_class_t *ftclass, int is_start) {
    char *starts[] = { "END", "START" };
    char *class_types[] = { "flowtuple_backscatter", "flowtuple_icmpreq", "flowtuple_other" };

    if (is_start) {
        printf("%s %s %d\n", starts[is_start],
               class_types[flowtuple_class_get_class_type(ftclass)],
               flowtuple_class_get_key_count(ftclass));
    } else {
        printf("%s %s\n", starts[is_start], class_types[flowtuple_class_get_class_type(ftclass)]);
    }
}

/* print data object */
void data_print(flowtuple_data_t *data, uint8_t first_octet) {
    uint8_t src_ip[4];
    uint8_t dst_ip[4];
    flowtuple_slash_eight_t se;

    if (flowtuple_data_is_slash_eight(data)) {
        se = flowtuple_data_get_dest_ip_slash_eight(data);
        dst_ip[0] = first_octet;
        dst_ip[1] = se.b;
        dst_ip[2] = se.c;
        dst_ip[3] = se.d;
    } else {
        ntoa(flowtuple_data_get_dest_ip_int(data), dst_ip);
    }

    ntoa(flowtuple_data_get_src_ip(data), src_ip);

    printf("%u.%u.%u.%u|%u.%u.%u.%u|%u|%u|%u|%u|0x%02x|%u,%u\n",
           src_ip[0], src_ip[1], src_ip[2], src_ip[3],
           dst_ip[0], dst_ip[1], dst_ip[2], dst_ip[3],
           flowtuple_data_get_src_port(data),
           flowtuple_data_get_dest_port(data),
           flowtuple_data_get_protocol(data),
           flowtuple_data_get_ttl(data),
           flowtuple_data_get_tcp_flags(data),
           flowtuple_data_get_ip_len(data),
           flowtuple_data_get_packet_count(data));
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
    flowtuple_record_t *r;        /* current record */
    flowtuple_record_type_t type; /* record type */
    flowtuple_errno_t errno;      /* errors */
    void *data;                   /* record data */
    char *filename = NULL;        /* file name */
    int trackers[] = { 1, 1, 0 }; /* is interval start, is class start, first octet */
    int c;                        /* getopt option */
    char *tmp;                    /* getopt tmp string */

    /* we expect arguments, always */
    if (argc < 2) {
        usage(argv[0]);
        return 1;
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
                    return 1;
                }

                if (trackers[2] == 0 && strcmp(tmp, "") != 0) {
                    fprintf(stderr, "option -o requires an integer option\n");
                    usage(argv[0]);
                    return 1;
                }

                if (trackers[2] < 0 || trackers[2] > 255) {
                    fprintf(stderr, "ERROR: octet must be between 0 and 255\n");
                    return 2;
                }
                break;
            case '?':
                if (optopt == 'o') {
                    usage(argv[0]);
                    return 1;
                }
            default:
                usage(argv[0]);
                return 1;
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
    if (errno != FLOWTUPLE_ERR_OK) {
        fprintf(stderr, "ERROR: %s\n", flowtuple_strerr(errno));
        return 3;
    }

    /* loop through records */
    flowtuple_loop(h, -1, process_record, (void*)trackers);

    errno = flowtuple_errno(h);
    if (errno != FLOWTUPLE_ERR_OK) {
        fprintf(stderr, "ERROR: %s\n", flowtuple_strerr(errno));
    }

    flowtuple_release(h);
    return 0;
}
