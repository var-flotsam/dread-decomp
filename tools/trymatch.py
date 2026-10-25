#!/usr/bin/env python3
"""Build one source file with the project toolchain and diff it against the original.

The quick loop for hand-matching: compile a single .cpp (in src/ or a scratch file anywhere)
exactly as tools/check.py does (tools/buildcfg.py), then diff every function it defines that
maps to an original address. Symbols map the same way as in check.py: `fn_<addr>` in the
name, or a unique demangled name from data/functions/. Nothing in data/ is updated; run
check.py for that.

The object goes to build/trymatch/<version>/<name>.o (or under --obj-dir, so that parallel runs on
different sources keep their objects apart), never next to the source.

Relocation targets are checked as in check.py: a function that matches byte-for-byte also
has to name the original's callees, globals and GOT slots, or it gets a `RELOC` line under
its MATCH line and counts as not matching (`--no-reloc` skips this).

Usage:
  python tools/trymatch.py --version 2.1.0 src/flag_id/fn_7100227448.cpp
  python tools/trymatch.py --version 2.1.0 scratch.cpp -v              # side-by-side diff
  python tools/trymatch.py --version 2.1.0 scratch.cpp -- -fno-inline  # extra compiler flags
Exit status: 0 when every mapped function matches, 1 otherwise (2 on a build failure).
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("source", type=Path)
    ap.add_argument("-v", "--verbose", action="store_true", help="print side-by-side disassembly of mismatches")
    ap.add_argument("--no-reloc", action="store_true", help="skip the relocation target check")
    ap.add_argument("--obj-dir", type=Path, help="where the object goes (default build/trymatch/<version>/)")
    ap.add_argument("flags", nargs="*", help="extra compiler flags (after --)")
    args = ap.parse_args()

    src = args.source.resolve()
    obj = (args.obj_dir or ROOT / "build" / "trymatch" / args.version) / (src.stem + ".o")
    obj.parent.mkdir(parents=True, exist_ok=True)
    r = subprocess.run(buildcfg.cxx_command(src, obj, args.flags), capture_output=True, text=True)
    if r.returncode != 0:
        print(r.stderr, end="")
        return 2
    if r.stderr:
        print(r.stderr, end="")

    rows, _ = check.load_csv()
    by_name = check.csv_by_name(rows)
    funcs = asmdiff.read_object(obj)
    codes = {f.symbol: f.code for f in funcs}
    original = asmdiff.Original(args.version)

    results, names, full_names, targets = check.diff_with_targets(funcs, original, by_name)
    mismatched = 0
    for res in results:
        status = "MATCH" if res.matching else f"{res.score:5.1%}"
        print(f"{status:>6}  {res.address:#x}  {names.get(res.symbol, res.symbol)}  "
              f"orig {res.orig_size} B, ours {res.our_size} B, {len(res.diff_insns)} differing insns")
        if args.verbose and not res.matching:
            asmdiff.print_side_by_side(res, original, codes[res.symbol])
        if res.matching and not args.no_reloc:
            rep = asmdiff.check_relocs(res, original, targets)
            if rep.mismatches:
                mismatched += 1
                for m in rep.mismatches:
                    sym = check.strip_params(full_names.get(m.symbol, m.symbol))
                    print(f"  RELOC  {asmdiff.format_mismatch(m, sym)}")
    unmapped = len(funcs) - len(results)
    matched = sum(r.matching for r in results) - mismatched
    print(f"{matched}/{len(results)} functions match" + (f" ({unmapped} not mapped to an address)" if unmapped else "")
          + (f"; {mismatched} with a wrong relocation target" if mismatched else ""))
    return 0 if results and matched == len(results) else 1


if __name__ == "__main__":
    sys.exit(main())
