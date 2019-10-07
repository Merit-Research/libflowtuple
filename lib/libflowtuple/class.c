/*
 *  class.c
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

#include <arpa/inet.h>

#include "fttypes.h"
#include "util.h"

flowtuple_magic_t flowtuple_class_get_magic(flowtuple_class_t *ftclass) {
    CHECK(ftclass != NULL, return 0);
    return ftclass->magic;
}

flowtuple_class_type_t flowtuple_class_get_class_type(flowtuple_class_t *ftclass) {
    CHECK(ftclass != NULL, return 0);
    return ntohs(ftclass->class_type);
}

uint32_t flowtuple_class_get_key_count(flowtuple_class_t *ftclass) {
    CHECK(ftclass != NULL, return 0);
    return ftclass->key_count;
}
