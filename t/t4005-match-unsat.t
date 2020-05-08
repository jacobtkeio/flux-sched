#!/bin/sh
#set -x

test_description='Test the basic functionality of allocate_with_satisfiability
'

ORIG_HOME=${HOME}

. `dirname $0`/sharness.sh

#
# sharness modifies $HOME environment variable, but this interferes
# with python's package search path, in particular its user site package.
#
HOME=${ORIG_HOME}

grug="${SHARNESS_TEST_SRCDIR}/data/resource/grugs/tiny.graphml"
jobspec1="${SHARNESS_TEST_SRCDIR}/data/resource/jobspecs/basics/test001.yaml"
jobspec2="${SHARNESS_TEST_SRCDIR}/data/resource/jobspecs/satisfiability/test001.yaml"

#
# test_under_flux is under sharness.d/
#
test_under_flux 1

#
# print only with --debug
#
test_debug '
    echo ${grug} &&
    echo ${jobspec1} &&
    echo ${jobspec2} &&
'

test_expect_success 'loading resource module with a tiny machine config works' '
    flux module load sched-fluxion-resource load-file=${grug} load-format=grug \
prune-filters=ALL:core subsystems=containment policy=high
'

test_expect_success 'satisfiability works with a 1-node, 1-socket jobspec' '
    flux ion-resource match allocate_with_satisfiability ${jobspec1} &&
    flux ion-resource match allocate_with_satisfiability ${jobspec1} &&
    flux ion-resource match allocate_with_satisfiability ${jobspec1} &&
    flux ion-resource match allocate_with_satisfiability ${jobspec1}
'

test_expect_success 'satisfiability returns EBUSY when no available resources' '
    test_expect_code 16 flux ion-resource \
match allocate_with_satisfiability ${jobspec1} &&
    test_expect_code 16 flux ion-resource \
match allocate_with_satisfiability ${jobspec1} &&
    test_expect_code 16 flux ion-resource \
match allocate_with_satisfiability ${jobspec1} &&
    test_expect_code 16 flux ion-resource \
match allocate_with_satisfiability ${jobspec1}
'

test_expect_success 'satisfiability returns ENODEV on unsatisfiable jobspec' '
    test_expect_code 19 flux ion-resource \
match allocate_with_satisfiability ${jobspec2} &&
    test_expect_code 19 flux ion-resource \
match allocate_with_satisfiability ${jobspec2} &&
    test_expect_code 19 flux ion-resource \
match allocate_with_satisfiability ${jobspec2} &&
    test_expect_code 19 flux ion-resource \
match allocate_with_satisfiability ${jobspec2}
'

test_expect_success 'removing resource works' '
    flux module remove sched-fluxion-resource
'

test_done
