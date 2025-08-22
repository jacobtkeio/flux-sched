/*****************************************************************************\
 * Copyright 2025 Lawrence Livermore National Security, LLC
 * (c.f. AUTHORS, NOTICE.LLNS, LICENSE)
 *
 * This file is part of the Flux resource manager framework.
 * For details, see https://github.com/flux-framework.
 *
 * SPDX-License-Identifier: LGPL-3.0
\*****************************************************************************/

#ifndef DFU_MATCH_LONGEST_HPP
#define DFU_MATCH_LONGEST_HPP

#include "resource/policies/base/dfu_match_cb.hpp"

namespace Flux {
namespace resource_model {

struct dfu_match_longest_cb_t : public dfu_match_cb_t {
    int dom_finish_slot (subsystem_t subsystem, scoring_api_t &dfu);
    int dom_finish_vtx (vtx_t u,
                        subsystem_t subsystem,
                        const std::vector<Flux::Jobspec::Resource> &resources,
                        const resource_graph_t &g,
                        scoring_api_t &dfu);
    int64_t match_time = std::numeric_limits<int64_t>::max ();
};

}  // namespace resource_model
}  // namespace Flux

#endif  // DFU_MATCH_LONGEST_HPP

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
