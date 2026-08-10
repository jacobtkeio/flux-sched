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
#include <stdbool.h>
#include <jansson.h>
#include "resource/policies/base/match_op.h"

/*! Wrapper for traversal match parameters.
 *  Passing around a pointer to dfu_match_params allows adding new parameters
 *  without breaking ABI compatibility.
 *    id          Job ID to use for the scheduling operation.
 *    op          The scheduling operation to run.
 *    at          The earliest time at which the job should be scheduled.
 */
struct dfu_match_params {
    int64_t jobid;
    match_op_t op;
    int64_t at;
};

/*! Wrapper for traversal match results.
 *  Passing around a pointer to dfu_match_params allows adding new result fields
 *  without breaking ABI compatibility.
 *    at           When the job was scheduled; in the future if reserved.
 *    reserved     Whether the job was reserved or not.
 *    overhead     The performance overhead in terms of elapsed time needed to
 *                     complete the match operation. Not used by traverser.
 */
struct dfu_match_result {
    int64_t at;
    bool reserved;
    double overhead;
};

const struct dfu_match_params default_match_params = {-1, MATCH_UNKNOWN, 0};

const struct dfu_match_result default_match_result = {0, false, 0.0f};

/*! Encode a dfu_match_params struct as a json object.
 *  The caller must decref the new json object.
 *  \param params   Parameter struct to encode.
 *  \return         Newly allocated json object representing params;
 *                  NULL on error.
 */
json_t *match_params_to_json (struct dfu_match_params params);

/*! Decode a json object as a dfu_match_params struct.
 *  The caller must free the new struct pointer.
 *  \param json     Json object to decode.
 *  \return         Newly allocated, populated struct ptr;
 *                  NULL on error.
 */
struct dfu_match_params *match_params_from_json (json_t *json);

/*! Encode a dfu_match_result struct as a json object.
 *  The caller must decref the new json object.
 *  \param result   Result struct to encode.
 *  \return         Newly allocated json object representing result;
 *                  NULL on error.
 */
json_t *match_result_to_json (struct dfu_match_result result);

/*! Decode a json object as a dfu_match_result struct.
 *  The caller must free the new struct pointer.
 *  \param json     Json object to decode.
 *  \return         Newly allocated, populated struct ptr;
 *                  NULL on error.
 */
struct dfu_match_result *match_result_from_json (json_t *json);

#ifdef __cplusplus
}
#endif

#endif  // DFU_MATCH_ATTRIBUTES_H
