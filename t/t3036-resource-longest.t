#!/bin/sh

# Based on t3032-resource-node-locality.t

test_description='Test longest-available resource matching'

. $(dirname $0)/sharness.sh

cmd_dir="${SHARNESS_TEST_SRCDIR}/data/resource/commands/match_longest"
exp_dir="${SHARNESS_TEST_SRCDIR}/data/resource/expected/match_longest"
grugs="${SHARNESS_TEST_SRCDIR}/data/resource/grugs/tiny.graphml"
query="../../resource/utilities/resource-query"

#
# Selection Policy -- Longest availble resources first (-P longest)
#     The matches that stay available the longest amount of time
#     are preferred, regardless of their specified duration.
#     (e.g. if node0 is scheduled to run its next job in two hours
#      and node1 is scheduled to run its next job in three hours,
#      this policy will choose node1 even if the job is only 1 hour)

cmds001="${cmd_dir}/cmds01.in"
test001_desc=""
test_expect_success "${test001_desc}" '
    sed "s~@TEST_SRCDIR@~${SHARNESS_TEST_SRCDIR}~g" ${cmds001} > cmds001 &&
    ${query} -L ${grugs} -S CA -P longest -t 001.R.out < cmds001 &&
    cat 001.R.out && false
    #test_cmp 001.R.out ${exp_dir}/001.R.out
'

test_done
