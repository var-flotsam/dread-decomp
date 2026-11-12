#!/usr/bin/env python3
"""select_large.py [--seed N] [--bands 601-1024:10] [--exclude FILE] > build/lean/fable_addrs.txt

A size-stratified random sample of the large (> 600 B) status-U functions on the boot path
(tools/hubs.py --boot): 8 of 601 B-1 KB, 8 of 1-2 KB, 4 of 2-4 KB. Random within each band, so the
trial's match rate stands for the band, not for the easiest hubs. One hex address per line.
--bands replaces the bands (LO-HI:N,...); --exclude skips the addresses listed in FILE (an earlier trial's).
"""
import json
import random
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
BANDS = [(601, 1024, 8), (1025, 2048, 8), (2049, 4096, 4)]


def main():
    seed = int(sys.argv[sys.argv.index("--seed") + 1]) if "--seed" in sys.argv else 1110
    out = subprocess.run([str(ROOT / ".venv" / "Scripts" / "python.exe"), "tools/hubs.py", "--version", "2.1.0",
                          "--boot", "--min-size", "601", "--json", "--top", "0"],
                         cwd=ROOT, capture_output=True, text=True, check=True).stdout
    funcs = json.loads(out)["functions"]
    bands = BANDS
    if "--bands" in sys.argv:
        spec = sys.argv[sys.argv.index("--bands") + 1]
        bands = [(int(r.split("-")[0]), int(r.split("-")[1].split(":")[0]), int(r.split(":")[1]))
                 for r in spec.split(",")]
    skip = set()
    if "--exclude" in sys.argv:
        skip = set(Path(sys.argv[sys.argv.index("--exclude") + 1]).read_text().split())
    funcs = [f for f in funcs if f["address"].replace("0x", "") not in skip]
    rng = random.Random(seed)
    for lo, hi, n in bands:
        band = sorted((f for f in funcs if lo <= f["size"] <= hi), key=lambda f: f["address"])
        for f in rng.sample(band, min(n, len(band))):
            print(f["address"].replace("0x", ""))


if __name__ == "__main__":
    main()
