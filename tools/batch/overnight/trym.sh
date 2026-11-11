#!/bin/sh
# usage: trym.sh file [func-substr]
cd <repos>/dread-decomp-overnight
f=$1; n=$(basename $f .cpp)
.venv/Scripts/python tools/trymatch.py --version 2.1.0 $f -v --obj-dir build/lean/obj/$n 2>&1 > build/overnight/scratch/b0199/$n.out
