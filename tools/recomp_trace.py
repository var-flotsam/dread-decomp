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
  keeps them. Rerun the recomp's setup with the same --out and --base (the original game folder: it keeps the
  RomFS and the generated code and compiles the changed files; its own `_data` folder as --base fails). `--undo` takes the
  lines out again and deletes `code_zz_trace.c`. The generated C stays in that folder: it is game code,
  never copied into this repository.
- `instrument --build DIR --snapshot ADDR[,ADDR...] --cpu-h FILE` also captures, at the first entry of each
  of those functions (our addresses), the state a real run gives it: x0-x30, q0-q31 and the 256 bytes above
  sp, then every guest page the thread touches until the function returns (each copied before its first
  access, so as it was at entry). It redefines the runtime's MP() address macro right after its definition
  in FILE (the recomp's runtime cpu.h, which every generated file includes) and writes
  `fn_<addr>.snap` into DREAD_SNAPSHOTS (default: the working directory), the file
  tools/pcode_emu.py's load_snapshot reads; copy them to build/port/snapshots/ for tools/port_lift.py.
  Every function also gets a guard that, while a snapshot runs, records each of its direct calls in
  `fn_<addr>.calls` beside it: the target, x0, x1 and q0-q3 on return, and the bytes the call changed
  (in the function's frame by offset from its entry sp, elsewhere by address), which the oracle replays
  in place of made-up results.
  They hold game memory: they stay in build/, never in the repository. `--undo` restores FILE too.
- `map --trace FILE` turns the log into our addresses (ours = theirs - 0x8005000 + 0x7100000000, checked
  on function starts and an import thunk; journal, "What the Dread recomp found") and joins each with
  data/functions: name, status, subsystem. `--until NAME|ADDR` cuts the order at the first entry of that
  function (a milestone such as the title screen). Writes build/recomp_trace/order.csv. Other modules'
  addresses (the SDK, the multimedia module) are kept, labelled by module. Import stubs (the PLT entries
  data/functions names `<EXTERNAL>::...`) are marked `import`: the port's runtime provides those.
- `next --order CSV [--boot JSON]` lists what our port meets next: the functions in recomp order that are
  not usable yet (status U, W or M), import stubs left out. With boot_distance.json, it starts after the furthest of our boot's
  traps in that order.

Only addresses, names, statuses and counts are written or printed, never game bytes.

Usage:
  python tools/recomp_trace.py instrument --build "D:/Games/Metroid Dread/Metroid Dread_build"
  python tools/recomp_trace.py instrument --build "D:/Games/Metroid Dread/Metroid Dread_build" --undo
  python tools/recomp_trace.py instrument --build DIR --snapshot 7100b89f84,7101191cc0 --cpu-h DIR/../recompiler/runtime/cpu.h
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


SNAP_C = r"""/*DREAD_TRACE*/ /* entry snapshots (instrument --snapshot): registers, then each page as first touched */
#include <string.h>
#define DSNAP_MAX 8192
#define DSNAP_PAGE 4096ull
__thread int dread_snap_on;
static __thread u8* dsnap_buf;
static __thread unsigned long long dsnap_page[DSNAP_MAX], dsnap_set[2 * DSNAP_MAX], dsnap_addr, dsnap_x[32];
static __thread V128 dsnap_v[32];
static __thread u8 dsnap_stack[256];
static __thread int dsnap_n;
static unsigned long long dsnap_done[64];
static int dsnap_ndone;
static void dsnap_touch(u8* M, unsigned long long p){
  unsigned long long h = (p >> 12) * 0x9E3779B97F4A7C15ull, k = h % (2 * DSNAP_MAX);
  while(dsnap_set[k]){ if(dsnap_set[k] == p + 1) return; k = (k + 1) % (2 * DSNAP_MAX); }
  if(dsnap_n >= DSNAP_MAX) return;
  dsnap_set[k] = p + 1;
  memcpy(dsnap_buf + (unsigned long long)dsnap_n * DSNAP_PAGE, M + p, DSNAP_PAGE);
  dsnap_page[dsnap_n++] = p;
}
/* The function's direct callees (depth 2): what each returned and the memory it changed, for the oracle
   to replay in place of made-up call results. Each page a callee touches is copied before its first touch
   in that call; when the call returns, the bytes that differ (in aligned 8-byte words) are written out. */
#define DSNAP_CALL_MAX 4096
static __thread int dsnap_depth, dsnap_cn, dsnap_ctrunc;
static __thread unsigned long long *dsnap_cpage, *dsnap_cset, dsnap_csp, dsnap_ctarget, dsnap_cx[8];
static __thread u8 *dsnap_cbuf, *dsnap_cframe;
static __thread FILE* dsnap_calls;
static __thread CPU* dsnap_cpu;  /* (the thread's CPU: the cleanup gets only its guard) */
#define DSNAP_FRAME 65536ull
static void dsnap_ctouch(u8* M, unsigned long long p){
  unsigned long long h = (p >> 12) * 0x9E3779B97F4A7C15ull, k = h % (2 * DSNAP_CALL_MAX);
  while(dsnap_cset[k]){ if(dsnap_cset[k] == p + 1) return; k = (k + 1) % (2 * DSNAP_CALL_MAX); }
  if(dsnap_cn >= DSNAP_CALL_MAX){ dsnap_ctrunc = 1; return; }
  dsnap_cset[k] = p + 1;
  memcpy(dsnap_cbuf + (unsigned long long)dsnap_cn * DSNAP_PAGE, M + p, DSNAP_PAGE);
  dsnap_cpage[dsnap_cn++] = p;
}
u8* dread_snap_touch(u8* M, unsigned long long a){
  dsnap_touch(M, a & ~(DSNAP_PAGE - 1));
  dsnap_touch(M, (a + 15) & ~(DSNAP_PAGE - 1));
  if(dsnap_depth >= 2 && dsnap_calls){
    dsnap_ctouch(M, a & ~(DSNAP_PAGE - 1));
    dsnap_ctouch(M, (a + 15) & ~(DSNAP_PAGE - 1));
  }
  return M + a;
}
/* Runs of changed bytes between old and now (n bytes at address base), widened to aligned 8-byte words;
   with skip_lo..skip_hi left out (the function's frame and its callees', written as "frame"). */
static void dsnap_runs(FILE* f, const u8* old, const u8* now, unsigned long long base, unsigned long long n,
                       unsigned long long skip_lo, unsigned long long skip_hi, int* first, long long rel){
  for(unsigned long long i = 0; i < n; i += 8){
    if(!memcmp(old + i, now + i, 8) || (base + i < skip_hi && base + i + 8 > skip_lo)) continue;
    unsigned long long j = i + 8;
    while(j < n && memcmp(old + j, now + j, 8) && !(base + j < skip_hi && base + j + 8 > skip_lo)) j += 8;
    if(rel) fprintf(f, "%s[%lld, \"", *first ? "" : ", ", (long long)(base + i) - rel);
    else fprintf(f, "%s[\"0x%llx\", \"", *first ? "" : ", ", base + i);
    for(unsigned long long k = i; k < j; k++) fprintf(f, "%02x", now[k]);
    fputs("\"]", f);
    *first = 0;
    i = j - 8;
  }
}
int dread_snap_call(CPU* c, unsigned long long e){
  if(++dsnap_depth != 2 || !dsnap_calls) return 1;
  dsnap_cpu = c; dsnap_ctarget = e; dsnap_csp = c->x[31]; dsnap_cn = 0; dsnap_ctrunc = 0;
  memcpy(dsnap_cx, c->x, sizeof dsnap_cx);
  memset(dsnap_cset, 0, 2 * DSNAP_CALL_MAX * sizeof *dsnap_cset);
  unsigned long long top = dsnap_x[31] + 256, lo = dsnap_csp < top - DSNAP_FRAME ? top - DSNAP_FRAME : dsnap_csp;
  if(lo < top) memcpy(dsnap_cframe, c->mem + lo, top - lo);
  return 1;
}
void dread_snap_ret(int* g){
  if(!*g) return;
  if(dsnap_depth-- != 2 || !dsnap_calls) return;
  FILE* f = dsnap_calls;
  CPU* c = dsnap_cpu;
  fprintf(f, "{\"target\": \"0x%llx\", \"x_in\": [", dsnap_ctarget);
  for(int i = 0; i < 8; i++) fprintf(f, "%s\"0x%llx\"", i ? ", " : "", dsnap_cx[i]);
  fprintf(f, "], \"x\": [\"0x%llx\", \"0x%llx\"], \"q\": [", (unsigned long long)c->x[0], (unsigned long long)c->x[1]);
  for(int i = 0; i < 4; i++) fprintf(f, "%s\"0x%016llx%016llx\"", i ? ", " : "", (unsigned long long)c->v[i].d[1], (unsigned long long)c->v[i].d[0]);
  fprintf(f, "], \"sp\": \"0x%llx\", \"truncated\": %d, \"frame\": [", dsnap_csp, dsnap_ctrunc);
  unsigned long long top = dsnap_x[31] + 256, lo = dsnap_csp < top - DSNAP_FRAME ? top - DSNAP_FRAME : dsnap_csp;
  int first = 1;
  if(lo < top) dsnap_runs(f, dsnap_cframe, c->mem + lo, lo, top - lo, 0, 0, &first, (long long)dsnap_x[31]);
  fputs("], \"mem\": [", f);
  first = 1;
  for(int i = 0; i < dsnap_cn; i++)
    dsnap_runs(f, dsnap_cbuf + (unsigned long long)i * DSNAP_PAGE, c->mem + dsnap_cpage[i], dsnap_cpage[i], DSNAP_PAGE,
               top > 0x1000000ull ? top - 0x1000000ull : 0, top, &first, 0);
  fputs("]}\n", f);
}
int dread_snap_begin(CPU* c, unsigned long long addr){
  if(dread_snap_on) return 0;
  for(int i = 0; i < dsnap_ndone; i++) if(dsnap_done[i] == addr) return 0;
  if(dsnap_ndone < 64) dsnap_done[dsnap_ndone++] = addr;
  if(!dsnap_buf) dsnap_buf = (u8*)malloc(DSNAP_MAX * DSNAP_PAGE);
  if(!dsnap_buf) return 0;
  if(!dsnap_cbuf){
    dsnap_cbuf = (u8*)malloc(DSNAP_CALL_MAX * DSNAP_PAGE); dsnap_cframe = (u8*)malloc(DSNAP_FRAME);
    dsnap_cpage = (unsigned long long*)malloc(DSNAP_CALL_MAX * sizeof *dsnap_cpage);
    dsnap_cset = (unsigned long long*)malloc(2 * DSNAP_CALL_MAX * sizeof *dsnap_cset);
  }
  memset(dsnap_set, 0, sizeof dsnap_set); dsnap_n = 0; dsnap_addr = addr;
  memcpy(dsnap_x, c->x, sizeof dsnap_x); memcpy(dsnap_v, c->v, sizeof dsnap_v);
  memcpy(dsnap_stack, c->mem + c->x[31], sizeof dsnap_stack);
  dsnap_depth = 0; dsnap_calls = 0;
  if(dsnap_cbuf && dsnap_cframe && dsnap_cpage && dsnap_cset){
    const char* d = getenv("DREAD_SNAPSHOTS");
    char path[1024];
    snprintf(path, sizeof path, "%s/fn_%llx.calls", d && *d ? d : ".", addr - 0x8005000ull + 0x7100000000ull);
    dsnap_calls = fopen(path, "wb");
    if(dsnap_calls) fputs("DCALLS1\n", dsnap_calls);
  }
  dread_snap_on = 1;
  return 1;
}
void dread_snap_end(int* g){
  if(!*g) return;
  dread_snap_on = 0;
  if(dsnap_calls){ fclose(dsnap_calls); dsnap_calls = 0; }
  const char* d = getenv("DREAD_SNAPSHOTS");
  char path[1024];
  snprintf(path, sizeof path, "%s/fn_%llx.snap", d && *d ? d : ".", dsnap_addr - 0x8005000ull + 0x7100000000ull);
  FILE* f = fopen(path, "wb");
  if(!f) return;
  fputs("DSNAP1\n{\"x\": [", f);
  for(int i = 0; i < 31; i++) fprintf(f, "%s\"0x%llx\"", i ? ", " : "", dsnap_x[i]);
  fputs("], \"q\": [", f);
  for(int i = 0; i < 32; i++) fprintf(f, "%s\"0x%016llx%016llx\"", i ? ", " : "", (unsigned long long)dsnap_v[i].d[1], (unsigned long long)dsnap_v[i].d[0]);
  fprintf(f, "], \"sp\": \"0x%llx\", \"stack\": \"", dsnap_x[31]);
  for(int i = 0; i < 256; i++) fprintf(f, "%02x", dsnap_stack[i]);
  fprintf(f, "\", \"pages\": %d}\n", dsnap_n);
  for(int i = 0; i < dsnap_n; i++){ fwrite(&dsnap_page[i], 8, 1, f); fwrite(dsnap_buf + (unsigned long long)i * DSNAP_PAGE, 1, DSNAP_PAGE, f); }
  fclose(f);
}
"""

# Into the runtime's cpu.h: right after its MP() (every RD*/WR* macro and the inline exclusive and DC ZVA
# helpers defined below it go through MP(), so a snapshot sees each page), and the guard's functions at the end.
CPU_H_MP = """/*DREAD_TRACE*/ extern __thread int dread_snap_on; u8* dread_snap_touch(u8* M, unsigned long long a);
/*DREAD_TRACE*/ #undef MP
/*DREAD_TRACE*/ #define MP(a) (dread_snap_on ? dread_snap_touch(M, (u64)(a)) : M + (u64)(a))
"""
CPU_H_END = """/*DREAD_TRACE*/ int dread_snap_begin(CPU* c, unsigned long long addr); void dread_snap_end(int* g);
/*DREAD_TRACE*/ int dread_snap_call(CPU* c, unsigned long long e); void dread_snap_ret(int* g);
"""
MP_DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+MP\(.*(?:\r?\n)", re.M)


# --- instrument ----------------------------------------------------------------------------------

def hook(name: str, eol: str, snapshot: frozenset = frozenset()) -> str:
    addr = int(name[2:], 16)
    out = (f"{eol} {{ static int t_; if(UNLIKELY(!t_) && e=={addr:#x}ull){{ t_=1; dread_trace_hit({addr:#x}ull); }} }}"
           f" {TAG}")
    if addr in snapshot:  # (recomp addresses; the guard writes the snapshot when the function returns)
        out += (f"{eol} int dread_snap_g_ __attribute__((cleanup(dread_snap_end))) = "
                f"e=={addr:#x}ull ? dread_snap_begin(c, {addr:#x}ull) : 0; {TAG}")
    if snapshot:  # (every function: a snapshot's direct callees record their results; after the guard above)
        out += (f"{eol} int dread_snap_c_ __attribute__((cleanup(dread_snap_ret))) = "
                f"dread_snap_on ? dread_snap_call(c, e) : 0; {TAG}")
    return out


def instrument_text(text: str, snapshot: frozenset = frozenset()) -> tuple[str, int]:
    """One generated file with a hook after every prologue and the declaration on its first line."""
    if TAG in text:
        return text, 0
    n = 0

    def add(m: re.Match) -> str:
        nonlocal n
        n += 1
        return m.group(1) + hook(m.group(2), m.group(3), snapshot) + m.group(3)

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


def instrument(build: Path, undo: bool, snapshot: frozenset = frozenset(), cpu_h: Path | None = None) -> int:
    """snapshot: our addresses."""
    files = sorted(p for p in build.glob("code_*.c") if p.name != TRACE_FILE)
    if not files:
        print(f"recomp_trace: no code_*.c in {build} (the recomp's <name>_build folder)", file=sys.stderr)
        return 2
    if snapshot and not undo and cpu_h is None:
        print("recomp_trace: --snapshot needs --cpu-h (the runtime's cpu.h)", file=sys.stderr)
        return 2
    theirs = frozenset(a - OUR_MAIN + RECOMP_MAIN for a in snapshot)
    total = changed = 0
    includes = ""
    for p in files:
        text = p.read_bytes().decode("latin-1")
        if not includes:
            includes = "".join(line + "\n" for line in text.splitlines() if line.startswith("#include"))
        new, n = (undo_text(text), 0) if undo else instrument_text(undo_text(text) if theirs else text, theirs)
        if new != text:
            p.write_bytes(new.encode("latin-1"))
            changed += 1
        total += n
    if cpu_h is not None:
        text = cpu_h.read_bytes().decode("latin-1")
        base = undo_text(text)
        new = base
        if not undo and theirs:
            m = MP_DEFINE.search(base)
            if m is None or m.group(0).rstrip("\r\n").endswith("\\"):
                print(f"recomp_trace: no one-line #define MP( in {cpu_h}", file=sys.stderr)
                return 2
            new = base[:m.end()] + CPU_H_MP + base[m.end():].rstrip("\r\n") + "\n" + CPU_H_END
        if new != text:
            cpu_h.write_bytes(new.encode("latin-1"))
            print(f"recomp_trace: {cpu_h} {'restored' if undo or not theirs else 'patched (MP)'}")
    trace = build / TRACE_FILE
    if undo:
        if trace.exists():
            trace.unlink()
        print(f"recomp_trace: {changed} files restored, {TRACE_FILE} removed")
    else:
        trace.write_text(TRACE_C + ("/*DREAD_TRACE*/\n" + includes + SNAP_C if theirs else ""), encoding="ascii")
        print(f"recomp_trace: {total} functions hooked in {changed} files; {TRACE_FILE} written. "
              f"Rebuild with the recomp's setup, same --out and --base as the first build.")
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
        # The PLT: the span of the 16-byte import stubs, plus 16-byte symbol entries right before it (the
        # function list gives the PLT's first entry a linker symbol, `end` on 2.1.0, not an <EXTERNAL> name).
        plt = [int(r["address"], 16) for r in self.rows
               if r["name"].startswith("<EXTERNAL>::") and int(r["size"]) == 16]
        self.plt = (min(plt), max(plt) + 16) if plt else (0, 0)
        if plt:
            i = self.starts.index(self.plt[0])
            while i > 0 and int(self.rows[i - 1]["size"]) == 16 and self.rows[i - 1]["name_source"] == "symbol" \
                    and self.starts[i - 1] + 16 == self.starts[i]:
                i -= 1
            self.plt = (self.starts[i], self.plt[1])

    def is_import(self, row: dict) -> bool:
        a = int(row["address"], 16)
        return row["name"].startswith("<EXTERNAL>::") or self.plt[0] <= a < self.plt[1]

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
            "import": bool(row) and index.is_import(row),
        })
        if until is not None and module == "main" and a == until:
            break
    return order


def write_order(order: list[dict], out: Path) -> None:
    out.parent.mkdir(parents=True, exist_ok=True)
    cols = ["order", "seq", "us", "thread", "module", "address", "name", "status", "subsystem", "start", "import"]
    with out.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=cols)
        w.writeheader()
        w.writerows(order)


def summary(order: list[dict]) -> list[str]:
    imports = sum(1 for o in order if o["module"] == "main" and o["import"])
    main = [o for o in order if o["module"] == "main" and not o["import"]]
    by: dict[str, int] = {}
    for o in main:
        by[o["status"] or "?"] = by.get(o["status"] or "?", 0) + 1
    off = sum(1 for o in main if not o["start"])
    threads = len({o["thread"] for o in order})
    others = sum(1 for o in order if o["module"] != "main")
    lines = [f"recomp trace: {len(order)} functions entered ({len(main)} in main, {imports} import stubs, {others} in "
             f"other modules), {threads} threads; main by status: "
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
    todo = [o for o in main[start:] if o["status"] in ("U", "W", "M") and o.get("import") not in ("True", True)]
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
    a.add_argument("--snapshot", default="", help="ADDR[,ADDR...] (ours): capture each one's entry state (needs --cpu-h)")
    a.add_argument("--cpu-h", type=Path, help="the recomp runtime's cpu.h (MP() is redefined at its end)")
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
        snap = frozenset(int(a, 16) for a in args.snapshot.split(",") if a)
        return instrument(args.build, args.undo, snap, args.cpu_h)
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
