#!/usr/bin/env python3
###############################################################
# Copyright 2026 Lawrence Livermore National Security, LLC
# (c.f. AUTHORS, NOTICE.LLNS, COPYING)
#
# This file is part of the Flux resource manager framework.
# For details, see https://github.com/flux-framework.
#
# SPDX-License-Identifier: LGPL-3.0
###############################################################

"""CFFI build script for _match_op extension module."""

from cffi import FFI

ffi = FFI()

# Set source - this tells CFFI to compile a real extension module
# that links against libmatch_op.a
ffi.set_source(
    "_fluxion._match_op",
    """
    #include "resource/policies/base/match_op.h"
    """
)

# C declarations from match_op.h
cdefs = """
typedef enum match_op_t {
    MATCH_UNKNOWN,
    MATCH_ALLOCATE,
    MATCH_ALLOCATE_W_SATISFIABILITY,
    MATCH_ALLOCATE_ORELSE_RESERVE,
    MATCH_SATISFIABILITY,
    MATCH_WITHOUT_ALLOCATING,
    MATCH_WITHOUT_ALLOCATING_FUTURE,
    END_MATCH_OP_T
} match_op_t;
"""

ffi.cdef(cdefs)

if __name__ == "__main__":
    ffi.emit_c_code("_match_op.c")
