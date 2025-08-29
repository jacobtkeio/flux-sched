/*****************************************************************************\
 * Copyright 2026 Lawrence Livermore National Security, LLC
 * (c.f. AUTHORS, NOTICE.LLNS, LICENSE)
 *
 * This file is part of the Flux resource manager framework.
 * For details, see https://github.com/flux-framework.
 *
 * SPDX-License-Identifier: LGPL-3.0
\*****************************************************************************/

#ifndef DFU_MATCH_ATTRIBUTES_H
#define DFU_MATCH_ATTRIBUTES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <jansson.h>

#include "resource/policies/base/match_op.h"

/*! Wrapper for optional traversal attributes.
 *  Passing around a pointer to dfu_match_attrs allows adding new attributes
 *  without breaking ABI compatibility.
 *    jobid              E
 *    at
 *    op
 */
struct dfu_match_attrs {
    const char *jobspec;
    int64_t jobid;
    int64_t at;
    match_op_t op;
    int64_t within;
};

const struct dfu_match_attrs default_match_attrs = {NULL, -1, 0, MATCH_UNKNOWN, INT64_MIN};

/*! Encode a dfu_match_attrs struct as a json object.
 *  The caller must decref the new json object.
 *  \param attrs    Attribute struct to encode.
 *  \return         Newly allocated json object representing attrs;
 *                  NULL on error.
 */
json_t *match_attrs_to_json (struct dfu_match_attrs attrs);

/*! Decode a json object as a dfu_match_attrs struct.
 *  The caller must free the new struct pointer.
 *  \param json     Json object to decode.
 *  \return         Newly allocated, populated struct ptr;
 *                  NULL on error.
 */
struct dfu_match_attrs *match_attrs_from_json (json_t *json);

#ifdef __cplusplus
}
#endif

#endif  // DFU_MATCH_ATTRIBUTES_H
