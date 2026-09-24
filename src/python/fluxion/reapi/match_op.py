###############################################################
# Copyright 2026 Lawrence Livermore National Security, LLC
# (c.f. AUTHORS, NOTICE.LLNS, COPYING)
#
# This file is part of the Flux resource manager framework.
# For details, see https://github.com/flux-framework.
#
# SPDX-License-Identifier: LGPL-3.0
###############################################################

from _fluxion._match_op import ffi, lib as _lib

MATCH_UNKNOWN = _lib.MATCH_UNKNOWN
MATCH_ALLOCATE = _lib.MATCH_ALLOCATE
MATCH_ALLOCATE_W_SATISFIABILITY = _lib.MATCH_ALLOCATE_W_SATISFIABILITY
MATCH_ALLOCATE_ORELSE_RESERVE = _lib.MATCH_ALLOCATE_ORELSE_RESERVE
MATCH_SATISFIABILITY = _lib.MATCH_SATISFIABILITY
MATCH_WITHOUT_ALLOCATING = _lib.MATCH_WITHOUT_ALLOCATING
MATCH_WITHOUT_ALLOCATING_FUTURE = _lib.MATCH_WITHOUT_ALLOCATING_FUTURE
END_MATCH_OP_T = _lib.END_MATCH_OP_T
