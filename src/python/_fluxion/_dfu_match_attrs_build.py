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

"""CFFI build script for _dfu_match_attrs extension module."""

from cffi import FFI

ffi = FFI()

# Set source - this tells CFFI to compile a real extension module
# that links against libdfu_match_attrs.a
ffi.set_source(
    "_fluxion._dfu_match_attrs",
    """
    #include "resource/traversers/dfu_match_attrs.h"
    #include "resource/policies/base/match_op.h"
    """,
    libraries=["dfu_match_attrs", "match_op"],
)

# C declarations from dfu_match.h
cdefs = """
typedef enum match_op_t match_op_t;

struct dfu_match_attrs {
    const char *jobspec;
    int64_t jobid;
    int64_t at;
    match_op_t op;
};

extern const struct dfu_match_attrs default_match_attrs;

const char *match_attrs_to_str (struct dfu_match_attrs attrs);

struct dfu_match_attrs *match_attrs_from_str (const char *str);
"""

ffi.cdef(cdefs)

if __name__ == "__main__":
    ffi.emit_c_code("_dfu_match_attrs.c")
