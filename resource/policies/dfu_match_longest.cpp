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

#include "resource/planner/c/planner.h"
#include "resource/policies/dfu_match_longest.hpp"

namespace Flux {
namespace resource_model {

dfu_match_longest_cb_t::dfu_match_longest_cb_t ()
{
}

dfu_match_longest_cb_t::dfu_match_longest_cb_t (const std::string &name) : dfu_match_cb_t (name)
{
}

dfu_match_longest_cb_t::dfu_match_longest_cb_t (const dfu_match_longest_cb_t &o)
    : dfu_match_cb_t (o)
{
}

dfu_match_longest_cb_t &dfu_match_longest_cb_t::operator= (const dfu_match_longest_cb_t &o)
{
    dfu_match_cb_t::operator= (o);
    return *this;
}

dfu_match_longest_cb_t::~dfu_match_longest_cb_t ()
{
}

int dfu_match_longest_cb_t::dom_finish_graph (subsystem_t subsystem,
                                              const std::vector<Flux::Jobspec::Resource> &resources,
                                              const resource_graph_t &g,
                                              scoring_api_t &dfu)
{
    int score = MATCH_MET;

    for (auto &resource : resources) {
        unsigned int qc = dfu.qualified_count (subsystem, resource.type);
        unsigned int count = calc_count (resource, qc);
        if (count == 0) {
            score = MATCH_UNMET;
            break;
        }
        dfu.choose_accum_best_k (subsystem, resource.type, count);
    }
    dfu.set_overall_score (score);
    return (score == MATCH_MET) ? 0 : -1;
}

int dfu_match_longest_cb_t::dom_finish_slot (subsystem_t subsystem, scoring_api_t &dfu)
{
    std::vector<resource_type_t> types;
    dfu.resrc_types (subsystem, types);
    for (auto &type : types)
        dfu.choose_accum_all (subsystem, type);
    return 0;
}

int dfu_match_longest_cb_t::dom_finish_vtx (vtx_t u,
                                            subsystem_t subsystem,
                                            const std::vector<Flux::Jobspec::Resource> &resources,
                                            const resource_graph_t &g,
                                            scoring_api_t &dfu)
{
    struct planner_t *p = g[u].schedule.plans;
    int64_t score = MATCH_MET;
    int64_t overall;
    int64_t match_time = get_match_time ();
    int64_t plan_end = planner_base_time (p) + planner_duration (p);

    if (match_time > plan_end || match_time < 0)
        return -2;

    for (auto &resource : resources) {
        if (resource_type_t{resource.type} != g[u].type)
            continue;

        // jobspec resource type matches with the visiting vertex
        for (auto &c_resource : resource.with) {
            // test children resource count requirements
            const auto &c_type = resource_type_t{c_resource.type};
            unsigned int qc = dfu.qualified_count (subsystem, c_type);
            unsigned int count = calc_count (c_resource, qc);
            if (count == 0) {
                score = MATCH_UNMET;
                break;
            }
            dfu.choose_accum_best_k (subsystem, c_type, count);
        }
    }

    int64_t end = planner_unavail_time_first (p, match_time, 1);

    // If end == graph_end it should actually be very large
    if (end == -1 && errno == ENOENT)
        end = plan_end;

    overall = (score == MATCH_MET) ? (end - match_time) : score;
    dfu.set_overall_score (overall);
    decr ();
    return (score == MATCH_MET) ? 0 : -1;
}

}  // namespace resource_model
}  // namespace Flux

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
