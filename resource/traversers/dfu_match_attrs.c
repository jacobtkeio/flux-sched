/*****************************************************************************\
 * Copyright 2026 Lawrence Livermore National Security, LLC
 * (c.f. AUTHORS, NOTICE.LLNS, LICENSE)
 *
 * This file is part of the Flux resource manager framework.
 * For details, see https://github.com/flux-framework.
 *
 * SPDX-License-Identifier: LGPL-3.0
\*****************************************************************************/

#include "dfu_match_attrs.h"

#define JS_STR "jobspec"
#define ID_STR "jobid"
#define AT_STR "at"
#define OP_STR "op"

json_t *match_attrs_to_json (struct dfu_match_attrs attrs)
{
    return json_pack ("{s:s s:I s:I s:I}",
                      JS_STR,
                      attrs.jobspec,
                      ID_STR,
                      attrs.jobid,
                      AT_STR,
                      attrs.at,
                      OP_STR,
                      attrs.op);
}

struct dfu_match_attrs *match_attrs_from_json (json_t *json)
{
    struct dfu_match_attrs *attrs =
        (struct dfu_match_attrs *)malloc (sizeof (struct dfu_match_attrs));
    if (!attrs)
        return NULL;
    *attrs = default_match_attrs;

    if (json_unpack (json,
                     "{s?s s?I s?I s?I}",
                     JS_STR,
                     &(attrs->jobspec),
                     ID_STR,
                     &(attrs->jobid),
                     AT_STR,
                     &(attrs->at),
                     OP_STR,
                     &(attrs->op))
        == 0)
        return attrs;
    return NULL;
}
