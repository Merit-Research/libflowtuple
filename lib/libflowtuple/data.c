/*
 *  data.c
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

#include "fttypes.h"
#include "util.h"

flowtuple_class_t *flowtuple_data_get_class_start(flowtuple_data_t *data) {
    CHECK(data != NULL, return NULL);
    return &(data->class_start);
}

uint32_t flowtuple_data_get_number(flowtuple_data_t *data) {
    CHECK(data != NULL, return -1);
    return data->number;
}

uint32_t flowtuple_data_get_src_ip(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->src_ip;
}

uint32_t flowtuple_data_get_dest_ip(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);

    if (data->has_slash_eight) {
        return (data->dst_ip.y.b << 16) | (data->dst_ip.y.c) << 8 | data->dst_ip.y.d;
    } else {
        return data->dst_ip.x;
    }
}

uint16_t flowtuple_data_get_src_port(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->src_port;
}

uint16_t flowtuple_data_get_dest_port(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->dst_port;
}

uint8_t flowtuple_data_get_protocol(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->proto;
}

uint8_t flowtuple_data_get_ttl(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->ttl;
}

uint8_t flowtuple_data_get_tcp_flags(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->tcp_flags;
}

uint16_t flowtuple_data_get_ip_len(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->ip_len;
}

uint32_t flowtuple_data_get_packet_count(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->pkt_cnt;
}

int flowtuple_data_is_slash_eight(flowtuple_data_t *data) {
    CHECK(data != NULL, return 0);
    return data->has_slash_eight;
}
