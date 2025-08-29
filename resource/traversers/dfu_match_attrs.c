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

json_t *match_attrs_to_json (struct dfu_match_attrs attrs)
{
    json_t *obj = NULL;
    if (obj =
            json_pack ("{s:f s:I}", "overhead", attrs.match_overhead, "within", attrs.match_within))
        return obj;
    return NULL;
}

struct dfu_match_attrs *match_attrs_from_json (json_t *json)
{
    struct dfu_match_attrs *attrs =
        (struct dfu_match_attrs *)malloc (sizeof (struct dfu_match_attrs));
    if (!attrs)
        return NULL;
    *attrs = default_match_attrs;

    if (json_unpack (json,
                     "{s?f s?I}",
                     "overhead",
                     &(attrs->match_overhead),
                     "within",
                     &(attrs->match_within))
        == 0)
        return attrs;
    return NULL;
}
