/*****************************************************************************\
 * Copyright 2026 Lawrence Livermore National Security, LLC
 * (c.f. AUTHORS, NOTICE.LLNS, LICENSE)
 *
 * This file is part of the Flux resource manager framework.
 * For details, see https://github.com/flux-framework.
 *
 * SPDX-License-Identifier: LGPL-3.0
\*****************************************************************************/

#include "dfu_match.h"

const char *jobid_str = "jobid";
const char *op_str = "op";
const char *at_str = "at";
const char *reserved_str = "reserved";
const char *overhead_str = "overhead";

json_t *match_params_to_json (struct dfu_match_params params)
{
    return json_pack ("{s:I s:I s:I}",
                      jobid_str,
                      params.jobid,
                      op_str,
                      params.op,
                      at_str,
                      params.at);
}

struct dfu_match_params *match_params_from_json (json_t *json)
{
    struct dfu_match_params *params =
        (struct dfu_match_params *)malloc (sizeof (struct dfu_match_params));
    if (!params)
        return NULL;
    *params = default_match_params;

    if (json_unpack (json,
                     "{s:I s:I s:I}",
                     jobid_str,
                     &(params->jobid),
                     op_str,
                     &(params->op),
                     at_str,
                     &(params->at))
        == 0)
        return params;
    return NULL;
}

json_t *match_result_to_json (struct dfu_match_result result)
{
    return json_pack ("{s:I s:b s:f}",
                      at_str,
                      result.at,
                      reserved_str,
                      result.reserved,
                      overhead_str,
                      result.overhead);
}

struct dfu_match_result *match_result_from_json (json_t *json)
{
    struct dfu_match_result *result =
        (struct dfu_match_result *)malloc (sizeof (struct dfu_match_result));
    if (!result)
        return NULL;
    *result = default_match_result;

    if (json_unpack (json,
                     "{s:I s:b s:f}",
                     at_str,
                     &(result->at),
                     reserved_str,
                     &(result->reserved),
                     overhead_str,
                     &(result->overhead))
        == 0)
        return result;
    return NULL;
}
