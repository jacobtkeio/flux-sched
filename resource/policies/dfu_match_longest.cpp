/*****************************************************************************\
 * Copyright 2025 Lawrence Livermore National Security, LLC
 * (c.f. AUTHORS, NOTICE.LLNS, LICENSE)
 *
 * This file is part of the Flux resource manager framework.
 * For details, see https://github.com/flux-framework.
 *
 * SPDX-License-Identifier: LGPL-3.0
\*****************************************************************************/

extern "C" {
#if HAVE_CONFIG_H
#include <config.h>
#endif
}

#include "resource/policies/dfu_match_longest.hpp"

namespace Flux {
namespace resource_model {

int dfu_match_longest_cb_t::dom_finish_slot (subsystem_t subsystem, scoring_api_t &dfu);

int dfu_match_longest_cb_t::dom_finish_vtx (vtx_t u,
                                            subsystem_t subsystem,
                                            const std::vector<Flux::Jobspec::Resource> &resources,
                                            const resource_graph_t &g,
                                            scoring_api_t &dfu) {
    int64_t score = MATCH_MET;
    struct planner_t *p = g[u].schedule.plans;
    int64_t end = planner_unavail_time_first (p, m_match_time, 1);
    score = end - match_time;
    if (score < dfu.overall_score ())
        dfu.set_overall_score (score);
    decr ();
    return (score > 0) ? 0 : -1;
}

}  // namespace resource_model
}  // namespace Flux

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
