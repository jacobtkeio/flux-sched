###############################################################
# Copyright 2026 Lawrence Livermore National Security, LLC
# (c.f. AUTHORS, NOTICE.LLNS, COPYING)
#
# This file is part of the Flux resource manager framework.
# For details, see https://github.com/flux-framework.
#
# SPDX-License-Identifier: LGPL-3.0
###############################################################

import errno as _errno_mod
import json

# Import from compiled extension module (API mode)
from _fluxion._dfu_match_attrs import ffi, lib as _lib

# Import match options
from fluxion.reapi.match_op import (
    MATCH_UNKNOWN,
    MATCH_ALLOCATE,
    MATCH_ALLOCATE_W_SATISFIABILITY,
    MATCH_ALLOCATE_ORELSE_RESERVE,
    MATCH_SATISFIABILITY,
    MATCH_WITHOUT_ALLOCATING,
    MATCH_WITHOUT_ALLOCATING_FUTURE
)


class ReapiMatchAttributes():
    match_attrs = ffi.new("struct dfu_match_attrs *", _lib.default_match_attrs)

    def __init__(self, jobspec=None, jobid=None, at=None, op=None):
        print (_lib.default_match_attrs)
        print (_lib.match_attrs_to_str(_lib.default_match_attrs))
        if jobspec:
            self.match_attrs.jobspec = jobspec
        if jobid:
            self.match_attrs.jobid = jobid
        if at:
            self.match_attrs.at = at
        if op:
            self.match_attrs.op = op

    def to_str(self):
        return _lib.match_attrs_to_str(self.match_attrs[0])

    def from_str(self, s):
        self.match_attrs = _lib.match_attrs_from_str(s.encode('utf-8'))
