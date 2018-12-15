/*
 *  flowtuple.h
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

#ifndef FLOWTUPLE_H
#define FLOWTUPLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>  /* uintX_t types */

/*
 * Flowtuple library
 */

/*
 * Opaque structures
 */

/** Flowtuple handle object */
typedef struct _flowtuple_handle_t flowtuple_handle_t;
/** Flowtuple header object */
typedef struct _flowtuple_header_t flowtuple_header_t;
/** Flowtuple trailer/footer object */
typedef struct _flowtuple_trailer_t flowtuple_trailer_t;
/** Flowtuple interval start/end object */
typedef struct _flowtuple_interval_t flowtuple_interval_t;
/** Flowtuple class start/end object */
typedef struct _flowtuple_class_t flowtuple_class_t;
/** Flowtuple data object */
typedef struct _flowtuple_data_t flowtuple_data_t;
/** Flowtuple record object */
typedef struct _flowtuple_record_t flowtuple_record_t;

typedef enum _flowtuple_errno_t {
    FLOWTUPLE_ERR_OK = 0,
    FLOWTUPLE_ERR_MEM,
    FLOWTUPLE_ERR_WRONG_MAGIC,
    FLOWTUPLE_ERR_CORRUPT,
    /* file */
    FLOWTUPLE_ERR_FILE_OPEN,
    FLOWTUPLE_ERR_FILE_READ,
    FLOWTUPLE_ERR_FILE_EOF,
} flowtuple_errno_t;

flowtuple_errno_t flowtuple_errno(flowtuple_handle_t *handle);
const char *flowtuple_strerr(flowtuple_errno_t err);

/*
 * Enumerations
 */

/** Flowtuple record types. */
typedef enum _flowtuple_record_type_t {
    FLOWTUPLE_RECORD_TYPE_NULL,
    FLOWTUPLE_RECORD_TYPE_HEADER,
    FLOWTUPLE_RECORD_TYPE_INTERVAL,
    FLOWTUPLE_RECORD_TYPE_TRAILER,
    FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_CLASS,
    FLOWTUPLE_RECORD_TYPE_FLOWTUPLE_DATA,
} flowtuple_record_type_t;

/** Flowtuple magics */
typedef enum _flowtuple_magic_t {
    FLOWTUPLE_MAGIC_SIXT = 0x53495854,
    FLOWTUPLE_MAGIC_SIXU = 0x53495855,
} flowtuple_magic_t;

/** Flowtuple class types */
typedef enum _flowtuple_class_type_t {
    FLOWTUPLE_CLASS_TYPE_BACKSCATTER,
    FLOWTUPLE_CLASS_TYPE_ICMPREQ,
    FLOWTUPLE_CLASS_TYPE_OTHER,
} flowtuple_class_type_t;

/*
 * Structures
 */

/** Initialize flowtuple file for reading.
 * @param filename Filename of input
 * @return New flowtuple handle
 */
flowtuple_handle_t *flowtuple_initialize(const char *filename, flowtuple_errno_t *err);

/** Free a flowtuple handle structure.
 * @param handle Handle to be freed
 */
void flowtuple_release(flowtuple_handle_t *handle);

flowtuple_record_t *flowtuple_get_next(flowtuple_handle_t *handle);

typedef void (*flowtuple_handler)(flowtuple_record_t *, void *);
long flowtuple_loop(flowtuple_handle_t *handle, long cnt, flowtuple_handler callback, void *args);

/** @addtogroup flowtuple_api_handle Options
 * Libflowtuple handle getters
 * @{
 */

/** Get file uri from handle object */
const char *flowtuple_handle_get_uri(flowtuple_handle_t *handle);
/** Get previous record retrieved (needs to be freed) */
flowtuple_record_t *flowtuple_handle_get_last_record(flowtuple_handle_t *handle);

/** @} */

/** Free a record object.
 * @param record Record to be freed
 */
void flowtuple_record_free(flowtuple_record_t *record);

/** @addtogroup flowtuple_api_record Record
 * Libflowtuple record getters
 * @{
 */

/** Get type of record */
flowtuple_record_type_t flowtuple_record_get_type(flowtuple_record_t *record);

/** Get header pointer from record */
flowtuple_header_t *flowtuple_record_get_header(flowtuple_record_t *record);
/** Get trailer/footer pointer from record */
flowtuple_trailer_t *flowtuple_record_get_trailer(flowtuple_record_t *record);
/** Get interval pointer from record */
flowtuple_interval_t *flowtuple_record_get_interval(flowtuple_record_t *record);
/** Get class pointer from record */
flowtuple_class_t *flowtuple_record_get_class(flowtuple_record_t *record);
/** Get data pointer from record */
flowtuple_data_t *flowtuple_record_get_data(flowtuple_record_t *record);

/** @} */

/** @addtogroup flowtuple_api_header Header
 * Libflowtuple header getters
 * @{
 */

/** Get major version from header object */
uint8_t flowtuple_header_get_version_major(flowtuple_header_t *header);
/** Get minor version from header object */
uint8_t flowtuple_header_get_version_minor(flowtuple_header_t *header);
/** Get local initial time from header object */
uint32_t flowtuple_header_get_local_init_time(flowtuple_header_t *header);
/** Get length of interval from header object */
uint16_t flowtuple_header_get_interval_length(flowtuple_header_t *header);
/** Get trace uri string length from header object */
uint16_t flowtuple_header_get_traceuri_len(flowtuple_header_t *header);
/** Get trace uri from header object */
const char *flowtuple_header_get_traceuri(flowtuple_header_t *header);
/** Get number of plugins used from header object */
uint16_t flowtuple_header_get_plugin_count(flowtuple_header_t *header);
/** Get plugin magic numbers from header object */
uint32_t *flowtuple_header_get_plugins(flowtuple_header_t *header);

/** @} */

/** @addtogroup flowtuple_api_trailer Trailer/Footer
 * Libflowtuple trailer getters
 * @{
 */

/** Get packet count from trailer object */
uint64_t flowtuple_trailer_get_packet_count(flowtuple_trailer_t *trailer);
/** Get accepted packet count from trailer object */
uint64_t flowtuple_trailer_get_accepted_count(flowtuple_trailer_t *trailer);
/** Get dropped packet count from trailer object */
uint64_t flowtuple_trailer_get_dropped_count(flowtuple_trailer_t *trailer);
/** Get time of first packet from trailer object */
uint32_t flowtuple_trailer_get_first_packet_time(flowtuple_trailer_t *trailer);
/** Get time of final packet from trailer object */
uint32_t flowtuple_trailer_get_last_packet_time(flowtuple_trailer_t *trailer);
/** Get final time of processing from trailer object */
uint32_t flowtuple_trailer_get_local_final_time(flowtuple_trailer_t *trailer);
/** Get runtime from trailer object */
uint32_t flowtuple_trailer_get_runtime(flowtuple_trailer_t *trailer);

/** @} */

/** @addtogroup flowtuple_api_interval Interval
 * Libflowtuple interval getters
 * @{
 */

/** Get interval number from interval object */
uint16_t flowtuple_interval_get_number(flowtuple_interval_t *interval);
/** Get time from interval object */
uint32_t flowtuple_interval_get_time(flowtuple_interval_t *interval);

/** @} */

/** @addtogroup flowtuple_api_class Class
 * Libflowtuple class getters
 * @{
 */

/** Get magic from class object */
flowtuple_magic_t flowtuple_class_get_magic(flowtuple_class_t *ftclass);
/** Get class type from class object */
flowtuple_class_type_t flowtuple_class_get_class_type(flowtuple_class_t *ftclass);
/** Get key count from class object */
uint32_t flowtuple_class_get_key_count(flowtuple_class_t *ftclass);

/** @} */

/** @addtogroup flowtuple_api_data Data
 * Libflowtuple data getters
 * @{
 */

/** Get class pointer from data object */
flowtuple_class_t *flowtuple_data_get_class_start(flowtuple_data_t *data);
/** Get number from data object */
uint32_t flowtuple_data_get_number(flowtuple_data_t *data);
/** Get source ip from data object */
uint32_t flowtuple_data_get_src_ip(flowtuple_data_t *data);
/** Get destination ip from data object */
uint32_t flowtuple_data_get_dest_ip(flowtuple_data_t *data);
/** Get source port from data object */
uint16_t flowtuple_data_get_src_port(flowtuple_data_t *data);
/** Get destination port from data object */
uint16_t flowtuple_data_get_dest_port(flowtuple_data_t *data);
/** Get protocol from data object */
uint8_t flowtuple_data_get_protocol(flowtuple_data_t *data);
/** Get time to live from data object */
uint8_t flowtuple_data_get_ttl(flowtuple_data_t *data);
/** Get TCP flags from data object */
uint8_t flowtuple_data_get_tcp_flags(flowtuple_data_t *data);
/** Get IP length from data object */
uint16_t flowtuple_data_get_ip_len(flowtuple_data_t *data);
/** Get number of packets from data object */
uint32_t flowtuple_data_get_packet_count(flowtuple_data_t *data);
/** Is the destination IP in slash eight format? */
int flowtuple_data_is_slash_eight(flowtuple_data_t *data);

/** @} */

#ifdef __cplusplus
}
#endif

#endif