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

/*! Wrapper for optional traversal attributes.
 *  Passing around a pointer to dfu_match_attrs allows adding new attributes
 *  without breaking ABI compatibility.
 *    match_overhead     Double to store performance overhead in terms of
 *                       elapsed time needed to complete the match operation.
 *                       Not used by traverser.
 *    match_within       Only return matches that start between now and now+within.
 *                       If within < 0, don't apply this filter.  However, if
 *                       within == INT64_MIN, also search for a 'within'
 *                       value in the jobspec's user attributes dictionary.
 */
struct dfu_match_attrs {
    double match_overhead;
    int64_t match_within;
};

const struct dfu_match_attrs default_match_attrs = {0.0f, INT64_MIN};

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
