#!/usr/bin/env bash
# step.sh <bid>: finish the batch, requeue unattempted (if >=4), start the next queued batch, print its id
cd <repos>/dread-decomp-overnight
P=.venv/Scripts/python
bash build/overnight/finish.sh $1 2>&1 | tail -2 | cut -c1-90
$P build/overnight/requeue.py $1 | tr -d '\r'
n=$($P build/overnight/coord.py next 1 | awk '{print $1}' | tr -d '\r')
[ -n "$n" ] && $P build/overnight/coord.py start $n && echo "NEXT $n"
$P build/overnight/coord.py stats
