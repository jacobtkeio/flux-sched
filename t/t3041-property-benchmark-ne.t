#!/bin/sh

# Adapted from t3012

test_description='Benchmark of property operations on Sierra'

. $(dirname $0)/sharness.sh

test_under_flux 1

cmd_dir="${SHARNESS_TEST_SRCDIR}/data/resource/commands/resource_property"
grug="${SHARNESS_TEST_SRCDIR}/data/resource/grugs/sierra.graphml"
query="../../resource/utilities/resource-query"

test_expect_success 'starting resource module works' '
    load_resource load-file=${grug} load-format=grug subsystems=containment policy=high
'

test_expect_success 'creating a property works' '
    flux ion-resource set-property /Sierra0/rack0/node0 property0=value0 &&
    flux ion-resource get-property /Sierra0/rack0/node0 property0
'

test_expect_success 'search for a nonexisting property 100 times' '
    for i in {1..100}; do flux ion-resource find property=no_such_property; done
'

test_expect_success 'removing resource module works' '
    remove_resource
'

test_done
