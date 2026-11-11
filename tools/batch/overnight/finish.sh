#!/usr/bin/env bash
# finish.sh <bid>: merge results into the ledger, spot-check one file, commit the worker's new files.
set -u
cd <repos>/dread-decomp-overnight
P=.venv/Scripts/python
bid=$1
info=$($P build/overnight/coord.py merge "$bid" | tr -d '\r')
echo "$info"
files=$(echo "$info" | $P -c "import json,sys;print(' '.join(json.load(sys.stdin)['files']))" | tr -d '\r')
n=$(echo "$info" | $P -c "import json,sys;print(json.load(sys.stdin)['matched'])" | tr -d '\r')
units=$(echo "$info" | $P -c "import json,sys;print(', '.join(json.load(sys.stdin)['units']))" | tr -d '\r')
[ -z "$files" ] && { echo "nothing to commit"; exit 0; }
ok=""
for f in $files; do
  [ -f "$f" ] || { echo "MISSING $f"; continue; }
  case "$f" in src/*_ov$bid.cpp) ;; *) echo "UNEXPECTED PATH $f (skipped)"; continue;; esac
  ok="$ok $f"
done
[ -z "$ok" ] && { echo "nothing to commit"; exit 0; }
first=$(echo $ok | awk '{print $1}')
out=$($P tools/trymatch.py --version 2.1.0 "$first" --obj-dir build/overnight/obj/spot_$bid 2>&1 | tr -d '\r' | tail -1)
echo "spot-check $first: $out"
echo "$out" | grep -qE "^([0-9]+)/\1 functions match" || { echo "SPOTCHECK-WARN: not committed"; exit 3; }
# untracked files of this batch not in the listed set are reported, never added
for s in $(git status --porcelain --untracked-files=all -- src include | grep "_ov$bid" | awk '{print $2}'); do
  case " $ok " in *" $s "*) ;; *) echo "stray: $s";; esac
done
git add $ok
git commit -q -m "Phase 4.7: overnight batch $bid: $n functions ($units)

Co-Authored-By: Claude Sonnet 5.5 <noreply@anthropic.com>" && git log --oneline -1
d=$($P build/overnight/declfile.py $ok 2>&1 | tr -d '\r')
echo "$d" | tail -1
echo "$d" | grep -q "declfile: 0 problems" || echo "$ok" | tr ' ' '\n' | grep . >> build/overnight/declfix_queue.txt
