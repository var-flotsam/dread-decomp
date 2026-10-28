#!/usr/bin/env python3
"""Recomp trace: the order the game enters its functions, from a local build of the Dread recomp.

yozander2009-stack/Metroid-Dread-Recomp (docs/references.md) recompiles the same 2.1.0 `main` to C and
plays the whole game. A build of it on the user's own dump, with one line added to every generated
function, logs the first entry of each function: the real boot order from `nnMain` to the title screen
and beyond, which our port's boot (tools/boot_distance.py) can be compared with. Three steps:

- `instrument --build DIR` rewrites the generated C in the recomp's `<name>_build` folder in place:
  after each function's prologue, a once-only call to `dread_trace_hit(<its address>)`; it also writes
  `code_zz_trace.c`, which defines that function (setup compiles every `code_*.c` it finds). The
  instrumented build writes `<seq> <microseconds> <thread> <address>` lines to the file named by
  DREAD_TRACE (default `recomp_trace.txt` in the game's working directory), flushed per line so a crash
  keeps them. Rerun the recomp's setup on its own `_data` folder to rebuild the DLL. `--undo` takes the
  lines out again and deletes `code_zz_trace.c`. The generated C stays in that folder: it is game code,
  never copied into this repository.
- `map --trace FILE` turns the log into our addresses (ours = theirs - 0x8005000 + 0x7100000000, checked
  on function starts and an import thunk; journal, "What the Dread recomp found") and joins each with
  data/functions: name, status, subsystem. `--until NAME|ADDR` cuts the order at the first entry of that
  function (a milestone such as the title screen). Writes build/recomp_trace/order.csv. Other modules'
  addresses (the SDK, the multimedia module) are kept, labelled by module.
- `next --order CSV [--boot JSON]` lists what our port meets next: the functions in recomp order that are
  not usable yet (status U, W or M). With boot_distance.json, it starts after the furthest of our boot's
  traps in that order.

Only addresses, names, statuses and counts are written or printed, never game bytes.

Usage:
  python tools/recomp_trace.py instrument --build "D:/Games/Metroid Dread/Metroid Dread_build"
  python tools/recomp_trace.py instrument --build "D:/Games/Metroid Dread/Metroid Dread_build" --undo
  python tools/recomp_trace.py map --trace recomp_trace.txt [--until fn_7100224de4] [--out build/recomp_trace/order.csv]
  python tools/recomp_trace.py next --order build/recomp_trace/order.csv [--boot build/boot/boot_distance.json] [--limit 40]
"""

from __future__ import annotations

import argparse
import bisect
import csv
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from dread import functions  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_OUT = ROOT / "build" / "recomp_trace" / "order.csv"

# The recomp's module bases for 2.1.0 (its metroid_perf.py checks these with the build IDs).
RECOMP_MAIN = 0x8005000
OUR_MAIN = 0x7100000000
MODULES = [("subsdk0", 0x7AA1F000), ("sdk", 0x7B0C6000)]

TAG = "/*DREAD_TRACE*/"
TRACE_FILE = "code_zz_trace.c"
# A generated function's prologue: the translator's own form, and the SWR_FN macro its title patches use.
PROLOGUE = re.compile(r"^(void (F_[0-9A-F]+)\(CPU\* RESTRICT c, u64 e\)\{\r?\n u8\* const M=c->mem;)(\r?\n)", re.M)
PROLOGUE_MACRO = re.compile(r"^(SWR_FN\((F_[0-9A-F]+)\))(\r?\n)", re.M)
FIRST_LINE_END = re.compile(r"\r?\n")

TRACE_C = r"""/*DREAD_TRACE*/ /* dread-decomp tools/recomp_trace.py: first entry of each function, one line each */
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
static unsigned long long now_us(void){ static LARGE_INTEGER f; LARGE_INTEGER t; if(!f.QuadPart) QueryPerformanceFrequency(&f);
  QueryPerformanceCounter(&t); return (unsigned long long)(t.QuadPart / f.QuadPart * 1000000ull + t.QuadPart % f.QuadPart * 1000000ull / f.QuadPart); }
static unsigned long thread_id(void){ return (unsigned long)GetCurrentThreadId(); }
#else
#include <time.h>
#include <pthread.h>
static unsigned long long now_us(void){ struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return (unsigned long long)t.tv_sec * 1000000ull + t.tv_nsec / 1000; }
static unsigned long thread_id(void){ return (unsigned long)pthread_self(); }
#endif
static FILE* g_dread_trace;
static unsigned long long g_dread_seq;
__attribute__((constructor)) static void dread_trace_open(void){
  const char* p = getenv("DREAD_TRACE");
  g_dread_trace = fopen(p && *p ? p : "recomp_trace.txt", "w");
}
void dread_trace_hit(unsigned long long addr){
  if(!g_dread_trace) return;
  unsigned long long seq = __atomic_fetch_add(&g_dread_seq, 1, __ATOMIC_RELAXED);
  fprintf(g_dread_trace, "%llu %llu %lu %llx\n", seq, now_us(), thread_id(), addr);
  fflush(g_dread_trace);
}
"""


# --- instrument ----------------------------------------------------------------------------------

def hook(name: str, eol: str) -> str:
    addr = int(name[2:], 16)
    return (f"{eol} {{ static int t_; if(UNLIKELY(!t_) && e=={addr:#x}ull){{ t_=1; dread_trace_hit({addr:#x}ull); }} }}"
            f" {TAG}")


def instrument_text(text: str) -> tuple[str, int]:
    """One generated file with a hook after every prologue and the declaration on its first line."""
    if TAG in text:
        return text, 0
    n = 0

    def add(m: re.Match) -> str:
        nonlocal n
        n += 1
        return m.group(1) + hook(m.group(2), m.group(3)) + m.group(3)

    text = PROLOGUE.sub(add, text)
    text = PROLOGUE_MACRO.sub(add, text)
    if not n:
        return text, 0
    m = FIRST_LINE_END.search(text)
    eol = m.group(0) if m else "\n"
    return f"void dread_trace_hit(unsigned long long); {TAG}{eol}" + text, n


def undo_text(text: str) -> str:
    """The file as it was: every line instrument_text added removed."""
    if TAG not in text:
        return text
    lines = text.splitlines(keepends=True)
    return "".join(line for line in lines if TAG not in line)


def instrument(build: Path, undo: bool) -> int:
    files = sorted(p for p in build.glob("code_*.c") if p.name != TRACE_FILE)
    if not files:
        print(f"recomp_trace: no code_*.c in {build} (the recomp's <name>_build folder)", file=sys.stderr)
        return 2
    total = changed = 0
    for p in files:
        text = p.read_bytes().decode("latin-1")
        new, n = (undo_text(text), 0) if undo else instrument_text(text)
        if new != text:
            p.write_bytes(new.encode("latin-1"))
            changed += 1
        total += n
    trace = build / TRACE_FILE
    if undo:
        if trace.exists():
            trace.unlink()
        print(f"recomp_trace: {changed} files restored, {TRACE_FILE} removed")
    else:
        trace.write_text(TRACE_C, encoding="ascii")
        print(f"recomp_trace: {total} functions hooked in {changed} files; {TRACE_FILE} written. "
              f"Rebuild with the recomp's setup on its own _data folder.")
    return 0


# --- map -----------------------------------------------------------------------------------------

def read_trace(lines) -> list[tuple[int, int, str, int]]:
    """(seq, microseconds, thread, recomp address) in sequence order, each address once (its first entry)."""
    hits = []
    for line in lines:
        parts = line.split()
        if len(parts) != 4:
            continue
        try:
            hits.append((int(parts[0]), int(parts[1]), parts[2], int(parts[3], 16)))
        except ValueError:
            continue
    hits.sort()
    seen: set[int] = set()
    out = []
    for h in hits:
        if h[3] not in seen:
            seen.add(h[3])
            out.append(h)
    return out


class FunctionIndex:
    def __init__(self, rows: list[dict]):
        self.rows = sorted(rows, key=lambda r: int(r["address"], 16))
        self.starts = [int(r["address"], 16) for r in self.rows]
        last = self.rows[-1]
        self.end = int(last["address"], 16) + int(last["size"])

    def find(self, addr: int) -> dict | None:
        i = bisect.bisect_right(self.starts, addr) - 1
        if i < 0:
            return None
        r = self.rows[i]
        return r if addr < self.starts[i] + int(r["size"]) else None

    def lookup(self, key: str) -> int | None:
        """An address from `fn_<addr>`, a hex address or a name in data/functions."""
        k = key.strip()
        if re.fullmatch(r"(fn_)?(0x)?[0-9a-fA-F]{8,}", k):
            return int(re.sub(r"^(fn_)?(0x)?", "", k), 16)
        for r in self.rows:
            if r["name"] == k:
                return int(r["address"], 16)
        return None


def to_ours(addr: int, index: FunctionIndex) -> tuple[str, int]:
    """(module, address): main in our address space, other modules as module offsets."""
    ours = addr - RECOMP_MAIN + OUR_MAIN
    if OUR_MAIN <= ours < index.end:
        return "main", ours
    for name, base in sorted(MODULES, key=lambda m: -m[1]):
        if addr >= base:
            return name, addr - base
    return "?", addr


def map_trace(hits, index: FunctionIndex, until: int | None) -> list[dict]:
    order = []
    for seq, us, thread, addr in hits:
        module, a = to_ours(addr, index)
        row = index.find(a) if module == "main" else None
        order.append({
            "order": len(order), "seq": seq, "us": us, "thread": thread, "module": module,
            "address": f"{a:#x}",
            "name": row["name"] if row else "",
            "status": row["status"] if row else "",
            "subsystem": row["subsystem"] if row else "",
            "start": bool(row) and int(row["address"], 16) == a,
        })
        if until is not None and module == "main" and a == until:
            break
    return order


def write_order(order: list[dict], out: Path) -> None:
    out.parent.mkdir(parents=True, exist_ok=True)
    cols = ["order", "seq", "us", "thread", "module", "address", "name", "status", "subsystem", "start"]
    with out.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=cols)
        w.writeheader()
        w.writerows(order)


def summary(order: list[dict]) -> list[str]:
    main = [o for o in order if o["module"] == "main"]
    by: dict[str, int] = {}
    for o in main:
        by[o["status"] or "?"] = by.get(o["status"] or "?", 0) + 1
    off = sum(1 for o in main if not o["start"])
    threads = len({o["thread"] for o in order})
    lines = [f"recomp trace: {len(order)} functions entered ({len(main)} in main, {len(order) - len(main)} in other "
             f"modules), {threads} threads; main by status: "
             + ", ".join(f"{k} {v}" for k, v in sorted(by.items()))]
    if off:
        lines.append(f"  {off} main addresses are not a function start in data/functions (the recomp split or "
                     f"merged a function differently)")
    return lines


# --- next ----------------------------------------------------------------------------------------

def read_order(path: Path) -> list[dict]:
    with path.open(newline="", encoding="utf-8") as fh:
        return list(csv.DictReader(fh))


def next_up(order: list[dict], boot: dict | None, limit: int) -> list[str]:
    main = [o for o in order if o["module"] == "main"]
    pos = {o["name"]: i for i, o in reversed(list(enumerate(main))) if o["name"]}
    pos.update({f"fn_{int(o['address'], 16):x}": i for i, o in reversed(list(enumerate(main)))})
    start = 0
    lines = []
    if boot:
        names = [t["name"] for t in boot.get("traps", {}).get("order", [])]
        if boot.get("aborting_trap"):
            names.append(boot["aborting_trap"])
        found = [(pos[n], n) for n in names if n in pos]
        if found:
            i, n = max(found)
            start = i + 1
            lines.append(f"our boot's furthest trap in recomp order: {n} at {i + 1} of {len(main)}")
        else:
            lines.append("none of our boot's traps is in the recomp order; listing from the start")
    todo = [o for o in main[start:] if o["status"] in ("U", "W", "M")]
    lines.append(f"{len(todo)} functions not usable yet after that point; the next {min(limit, len(todo))}:")
    for o in todo[:limit]:
        lines.append(f"  {int(o['order']) + 1:6d}  {o['address']}  {o['status']}  {o['name']}"
                     + (f"  ({o['subsystem']})" if o["subsystem"] and o["subsystem"] != "unknown" else ""))
    return lines


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    a = sub.add_parser("instrument", help="add (or with --undo remove) the trace hooks in the recomp's generated C")
    a.add_argument("--build", required=True, type=Path, help="the recomp's '<name>_build' folder")
    a.add_argument("--undo", action="store_true")
    m = sub.add_parser("map", help="the trace in our addresses, joined with data/functions")
    m.add_argument("--trace", required=True, type=Path)
    m.add_argument("--until", help="stop at the first entry of this function (name, fn_<addr> or address)")
    m.add_argument("--out", type=Path, default=DEFAULT_OUT)
    n = sub.add_parser("next", help="functions in recomp order that are not usable yet")
    n.add_argument("--order", type=Path, default=DEFAULT_OUT)
    n.add_argument("--boot", type=Path, help="tools/boot_distance.py's boot_distance.json")
    n.add_argument("--limit", type=int, default=40)
    args = ap.parse_args()

    if args.cmd == "instrument":
        return instrument(args.build, args.undo)
    if args.cmd == "map":
        index = FunctionIndex(functions.rows())
        until = None
        if args.until:
            until = index.lookup(args.until)
            if until is None:
                print(f"recomp_trace: {args.until} is not a function in data/functions", file=sys.stderr)
                return 2
        with args.trace.open(encoding="ascii", errors="replace") as fh:
            order = map_trace(read_trace(fh), index, until)
        write_order(order, args.out)
        print("\n".join(summary(order)))
        print(f"  written to {args.out}")
        return 0
    boot = json.loads(args.boot.read_text()) if args.boot else None
    print("\n".join(next_up(read_order(args.order), boot, args.limit)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
