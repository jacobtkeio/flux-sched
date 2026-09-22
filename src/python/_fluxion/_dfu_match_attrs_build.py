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

"""CFFI build script for _dfu_match_params extension module."""

from cffi import FFI

ffi = FFI()

# Set source - this tells CFFI to compile a real extension module
# that links against libdfu_match_params.a
ffi.set_source(
    "_fluxion._dfu_match_params",
    """
    #include <jansson.h>
    #include "resource/traversers/dfu_match_attrs.h"
    """,
    libraries=["dfu_match_attrs", "jansson"],
)

# C declarations from dfu_match.h
cdefs = """
"""

ffi.cdef(cdefs)

if __name__ == "__main__":
    ffi.emit_c_code("_dfu_match_attrs.c")
