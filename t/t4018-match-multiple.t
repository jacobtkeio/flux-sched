#!/bin/sh
#set -x

# Adapted from t4016-match-without-allocating.t

test_description='Test matching-without-allocating multiple timeslots at once'

. `dirname $0`/sharness.sh

grug="${SHARNESS_TEST_SRCDIR}/data/resource/grugs/tiny.graphml"
jobspec="${SHARNESS_TEST_SRCDIR}/data/resource/jobspecs/basics/test001.yaml"
malform="${SHARNESS_TEST_SRCDIR}/data/resource/jobspecs/basics/bad.yaml"
duration_too_large="${SHARNESS_TEST_SRCDIR}/data/resource/jobspecs/duration/test001.yaml"
duration_negative="${SHARNESS_TEST_SRCDIR}/data/resource/jobspecs/duration/test002.yaml"

test_under_flux 1

test_debug '
    echo ${grug} &&
    echo ${jobspec} &&
    echo ${malform}
'

test_expect_success 'loading resource module with a tiny machine config works' '
    load_resource \
load-file=${grug} prune-filters=ALL:core \
load-format=grug subsystems=containment  \
policy=high maximum-matches=6
'

test_expect_success 'match-without-allocating -n works with a 1-node, 1-socket jobspec' '
    flux ion-resource match without_allocating -n4 ${jobspec} | grep -o "{.*starttime" | tee out1 &&
    flux ion-resource match without_allocating --number=4 ${jobspec} | grep "{" | tee out2 &&
    test $(wc -l out1 | cut -d" " -f1,1) -eq 4 &&
    test $(wc -l out2 | cut -d" " -f1,1) -eq 4
'

# MWOAE blocks out the time span of each prospective match completely
# after the match is emitted. Since the first match extends to the end
# of the graph, no time span is available for another match and the
# command exits successfully with one result.
test_expect_success 'basic match-without-allocating-extend -n works' '
    test $(flux ion-resource match without_allocating_extend -n4 ${jobspec} | grep -o "{.*" | wc -l) -eq 1
'

test_expect_success 'match-allocate works (all resources)' '
    flux ion-resource match allocate ${jobspec} &&
    flux ion-resource match allocate ${jobspec} &&
    flux ion-resource match allocate ${jobspec} &&
    flux ion-resource match allocate ${jobspec}
'

test_expect_success 'match-allocate fails when all resources are allocated' '
    test_expect_code 16 flux ion-resource match allocate ${jobspec}
'

test_expect_success 'match-without-allocating -n succeeds when all resources are allocated' '
    flux ion-resource match without_allocating -n 5 ${jobspec} | grep "{" | tee out3 &&
    test $(wc -l out3 | cut -d" " -f1,1) -eq 5
'

test_expect_success 'match-without-allocating -n succeeds when n>max' '
    flux ion-resource match without_allocating -n9999 ${jobspec} | \
        grep "{" | tee out4 &&
    test $(wc -l out4 | cut -d" " -f1,1) -eq 6
'

# With one time gap before a reservation and one after, MWOAE should match two slots
# 1: M....E    R    M.....E
# 2: A    R    R
# 3: A    R    R
# 4: A    R    R
# t: 0    3600 7200 10800 graph_end
# With A=Allocated, R=Reserved, M=Matched, and E=Extended Match
# Note: cancel 3&9 opens the first gap from t=0-3600 on node 1
test_expect_success 'match-without-allocating-extend -n works with multiple matches' '
    for i in {1..8}; do flux ion-resource match allocate_orelse_reserve ${jobspec}; done &&
    flux ion-resource cancel 3 &&
    flux ion-resource cancel 9 &&
    test $(flux ion-resource match without_allocating_extend -n4 ${jobspec} | grep "{.*" | wc -l) -eq 2
'

test_expect_success 'detecting of a non-existent jobspec file works' '
    test_expect_code 3 flux ion-resource match without_allocating -n2 foo
'

test_expect_success 'handling of a malformed jobspec works' '
    test_expect_code 2 flux ion-resource match without_allocating -n2 ${malform}
'

test_expect_success 'handling of an invalid resource type works' '
    test_expect_code 1 flux ion-resource match without_allocating -n2 \
        "${SHARNESS_TEST_SRCDIR}/data/resource/jobspecs/basics/bad_res_type.yaml"
'

test_expect_success 'invalid duration is caught' '
    test_must_fail flux ion-resource match without_allocating -n2 ${duration_too_large} &&
    test_must_fail flux ion-resource match without_allocating -n2 ${duration_negative}
'

test_expect_success 'removing resource works' '
    remove_resource
'

test_expect_success 'loading resource module with policy=low works' '
    load_resource \
load-file=${grug} prune-filters=ALL:core \
load-format=grug subsystems=containment  \
policy=low maximum-matches=6
'

test_expect_success 'match-without-allocating matches different resources with policy=low' '
    flux ion-resource match without_allocating -n4 ${jobspec} | grep -o "{.*starttime" | tee out5 &&
    test_expect_code 1 diff out1 out5
'

test_expect_success 'removing resource works' '
    remove_resource
'

test_done
