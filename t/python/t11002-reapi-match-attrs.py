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

"""Test CFFI bindings to dfu_match_attrs."""

import unittest

from fluxion.reapi.match_attrs import ReapiMatchAttributes

from fluxion.reapi.match_op import (
    MATCH_UNKNOWN,
    MATCH_ALLOCATE,
    MATCH_ALLOCATE_W_SATISFIABILITY,
    MATCH_ALLOCATE_ORELSE_RESERVE,
    MATCH_SATISFIABILITY,
    MATCH_WITHOUT_ALLOCATING,
    MATCH_WITHOUT_ALLOCATING_FUTURE,
    END_MATCH_OP_T
)

from pycotap import TAPTestRunner

class TestDFUMatchAttrsFII(unittest.TestCase):
    """Test CFFI bindings to dfu_match_attrs."""

    def test_relative_match_op_values(self):
        self.assertEqual(MATCH_UNKNOWN, 0)
        self.assertEqual(MATCH_UNKNOWN+1, MATCH_ALLOCATE)
        self.assertEqual(MATCH_ALLOCATE+1, MATCH_ALLOCATE_W_SATISFIABILITY)
        # ...

    def test_string_conversion(self):
        attrs = ReapiMatchAttributes()
        print(attrs.to_str())
        attrs.from_str("{\"jobid\": 1}")
        print(attrs.to_str())


if __name__ == "__main__":
    unittest.main(testRunner=TAPTestRunner())
