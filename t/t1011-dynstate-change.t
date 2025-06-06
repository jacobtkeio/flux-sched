#!/bin/sh

test_description='Test Fluxion on Dynamic Resource State Changes'

. `dirname $0`/sharness.sh

mkdir -p config

hwloc_basepath=`readlink -e ${SHARNESS_TEST_SRCDIR}/data/hwloc-data`
# 4 brokers, each (exclusively) have:
# 1 node, 2 sockets, 44 cores (22 per socket), 4 gpus (2 per socket)
excl_4N4B="${hwloc_basepath}/004N/exclusive/04-brokers-sierra2"

export FLUX_SCHED_MODULE=none
test_under_flux 4 full -o,--config-path=$(pwd)/config

test_expect_success 'dyn-state: generate jobspecs' '
	flux run --dry-run -N 4 -n 4 -c 44 -g 4 -t 1h \
	    sleep 3600 > basic.json &&
	flux run --dry-run -N 1 -n 1 -c 44 -g 4 -t 1h \
	    sleep 3600 > 1N.json &&
	flux run --dry-run -N 4 -n 4 -c 45 -g 4 -t 1h \
	    sleep 3600 > unsat.json &&
	flux run --dry-run -N 4 -n 4 -c 44 -g 4 -t 1h --queue=debug \
	    sleep 3600 > basic.debug.json &&
	flux run --dry-run -N 1 -n 1 -c 44 -g 4 -t 1h --queue=debug \
	    sleep 3600 > 1N.debug.json &&
	flux run --dry-run -N 4 -n 4 -c 44 -g 4 -t 1h --queue=batch \
	    sleep 3600 > basic.batch.json &&
	flux run --dry-run -N 1 -n 1 -c 44 -g 4 -t 1h --queue=batch \
	    sleep 3600 > 1N.batch.json
'

test_expect_success 'load test resources' '
	load_test_resources ${excl_4N4B}
'

# Note: updating test to specify ALL:core. The default (ALL:core,ALL:node)
# causes requests of drained resources to be unsatisfiable rather than the
# expected behavior of blocking under fcfs.
test_expect_success 'dyn-state: loading fluxion modules works' '
	load_resource match-format=rv1 prune-filters=ALL:core &&
	load_qmanager
'

test_expect_success 'dyn-state: a full-size job can be scheduled and run' '
	jobid1=$(flux job submit basic.json) &&
	flux job wait-event -t 2 ${jobid1} start
'

test_expect_success 'dyn-state: node drain does not kill the job' '
	flux resource drain 1 &&
	test_must_fail flux job wait-event -t 1 ${jobid1} finish
'

test_expect_success 'dyn-state: killing the job on the drained node works' '
	flux cancel ${jobid1} &&
	flux job wait-event -t 10 ${jobid1} clean
'

test_expect_success 'dyn-state: undrain' '
	flux resource undrain 1
'

test_expect_success 'dyn-state: the drained node with a job not used' '
	jobid1=$(flux job submit 1N.json) &&
	flux job wait-event -t 10 ${jobid1} start &&
	rank=$(flux job info ${jobid1} R \
	    | jq " .execution.R_lite[0].rank ") &&
	rank=${rank%\"} && rank=${rank#\"} &&
	jobid2=$(flux job submit 1N.json) &&
	jobid3=$(flux job submit 1N.json) &&
	jobid4=$(flux job submit 1N.json) &&
	flux job wait-event -t 10 ${jobid4} start &&
	flux resource drain ${rank} &&
	flux cancel ${jobid1} &&
	flux job wait-event -t 10 ${jobid1} clean &&
	jobid5=$(flux job submit 1N.json) &&
	test_must_fail flux job wait-event -t 1 ${jobid5} start
'

test_expect_success 'dyn-state: cancel all jobs' '
	flux cancel ${jobid2} &&
	flux cancel ${jobid3} &&
	flux cancel ${jobid4} &&
	flux cancel ${jobid5} &&
	flux job wait-event -t 10 ${jobid2} clean &&
	flux job wait-event -t 10 ${jobid3} clean &&
	flux job wait-event -t 10 ${jobid4} clean &&
	flux job wait-event -t 10 ${jobid5} clean &&
	flux resource undrain ${rank}
'

test_expect_success 'dyn-state: unsatifiability check works' '
	jobid1=$(flux job submit unsat.json) &&
	flux job wait-event -t 2 ${jobid1} clean &&
	flux job eventlog ${jobid1} | grep unsatisfiable
'

test_expect_success 'dyn-state: drain prevents a full job from running' '
	flux resource drain 0 &&
	jobid1=$(flux job submit basic.json) &&
	test_must_fail flux job wait-event -t 1 ${jobid1} start &&
	flux cancel ${jobid1}
'

test_expect_success 'dyn-state: a full job blocks a later job under fcfs' '
	jobid1=$(flux job submit basic.json) &&
	jobid2=$(flux job submit 1N.json) &&
	test_must_fail flux job wait-event -t 1 ${jobid2} start &&
	flux cancel ${jobid1} &&
	flux cancel ${jobid2}
'

test_expect_success 'dyn-state: correct unsatifiability after drain' '
	jobid1=$(flux job submit unsat.json) &&
	flux job wait-event -t 2 ${jobid1} clean &&
	flux job eventlog ${jobid1} | grep unsatisfiable &&
	flux resource undrain 0
'

test_expect_success 'dyn-state: removing fluxion modules' '
	remove_qmanager &&
	remove_resource
'

test_expect_success 'dyn-state: loading fluxion modules works' '
	load_resource match-format=rv1 &&
	load_qmanager queue-policy=easy
'

test_expect_success 'dyn-state: a full job skipped for a later job under easy' '
	flux resource drain 3 &&
	jobid1=$(flux job submit basic.json) &&
	jobid2=$(flux job submit 1N.json) &&
	flux job wait-event -t 10 ${jobid2} start &&
	flux cancel ${jobid2} &&
	flux job wait-event -t 10 ${jobid2} clean &&
	flux resource undrain 3 &&
	flux job wait-event -t 10 ${jobid1} start &&
	flux cancel ${jobid1} &&
	flux job wait-event -t 10 ${jobid1} clean
'

test_expect_success 'dyn-state: removing fluxion modules' '
	remove_qmanager &&
	remove_resource
'

test_expect_success 'configure queues' '
	cat >config/queues.toml <<-EOT &&
	[queues.batch]
	[queues.debug]

	[policy.jobspec.defaults.system]
	queue = "batch"

	[sched-fluxion-qmanager]
	queue-policy-per-queue = "batch:easy debug:fcfs"
	EOT
	flux config reload &&
	flux queue start --all
'

# Note: updating test to specify ALL:core. The default (ALL:core,ALL:node)
# causes requests of drained resources to be unsatisfiable rather than the
# expected behavior of blocking under fcfs.
test_expect_success 'dyn-state: loading fluxion modules works' '
	load_resource match-format=rv1 prune-filters=ALL:core &&
	load_qmanager
'

test_expect_success 'dyn-state: a full job blocks a later job for fcfs queue' '
	flux resource drain 2 &&
	jobid1=$(flux job submit basic.debug.json) &&
	jobid2=$(flux job submit 1N.debug.json) &&
	test_must_fail flux job wait-event -t 1 ${jobid1} start &&
	flux cancel ${jobid1} &&
	flux cancel ${jobid2} &&
	flux job wait-event -t 1 ${jobid2} clean
'

test_expect_success 'dyn-state: a job skipped for a later job for easy queue' '
	jobid1=$(flux job submit basic.batch.json) &&
	jobid2=$(flux job submit 1N.batch.json) &&
	flux job wait-event -t 1 ${jobid2} start &&
	flux cancel ${jobid1} &&
	flux cancel ${jobid2} &&
	flux job wait-event -t 1 ${jobid2} clean
'

test_expect_success 'dyn-state: removing fluxion modules' '
	remove_qmanager &&
	remove_resource
'
test_expect_success 'unconfigure queues' '
	rm -f config/queues.toml &&
	flux config reload
'

test_expect_success 'dyn-state: loading fluxion modules works' '
	load_resource match-format=rv1 &&
	load_qmanager
'

test_expect_success 'dyn-state: a job skipped for a later job for easy queue' '
	jobid1=$(flux job submit basic.json) &&
	test_must_fail flux job wait-event -t 1 ${jobid1} start
'

test_expect_success 'cleanup active jobs' '
	cleanup_active_jobs
'

test_expect_success 'dyn-state: removing fluxion modules' '
	remove_qmanager &&
	remove_resource
'

test_done

