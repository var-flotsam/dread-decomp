#!/usr/bin/env python3
"""Boot distance: build the port's Linux executable, run it, and report how far the boot gets.

A smoke run for the PC port (port/README.md, "Linux in Docker"). It builds the executable the way the
README does (tools/port_sweep.py, then tools/port_link.py --executable with the semantic lifts), runs it
with DREAD_TRAP=log under gdb with a time limit, and reports (the link places vtables and named
functions with tools/port_addresses.py, run first when tools/check.py's build/check/<version> exists):

- the furthest milestone reached, from the ordered list MILESTONES below (a function entered, or a
  function returned for the "done" ones). gdb sets a one-shot breakpoint on each milestone's function,
  and on its return address once it is entered, so the game and the runtime need no logging of their own;
- the distinct traps hit (dread_port_missing_function, mismatched calls, missing imports), in the order
  the boot reached them;
- how many semantic lifts ran: one one-shot breakpoint per lift the link kept (objects_lifted.rsp),
  which is what DREAD_PORT_LIFTED (tools/port_lift.py) would log if a boot defined it;
- how the run ended (exit status, signal, the fault address the runtime's handler prints, time-out).

It prints one summary line and at most about 20 more, and writes <out>/boot_distance.json. Only
addresses, names and counts are printed or saved, never game bytes (the fault handler's instruction
word is dropped). The build's own reports go to <out>/sweep.log and <out>/link.log, the run's output to
<out>/boot.log; all of <out> is under build/ and untracked.

--expect MILESTONE exits 1 unless that milestone (a key or a label from MILESTONES) was reached, for
the nightly job (.github/workflows/boot-nightly.yml). A build that fails exits 2. Adding a milestone:
append it to MILESTONES in boot order, then raise the nightly's --expect once the boot reaches it.

On a host that is not Linux the tool runs itself inside the port's Docker image (port/docker/run.sh;
the image needs gdb, so rebuild it after pulling: docker build -t dread-port-linux port/docker).
--out must be under the checkout (the container sees only that); the default is build/boot.

Usage:
  python tools/boot_distance.py --version 2.1.0
  python tools/boot_distance.py --version 2.1.0 --expect fn_7100015c70
  python tools/boot_distance.py --version 2.1.0 --no-build --timeout 300   # rerun the boot only
  python tools/boot_distance.py --version 2.1.0 --no-lifted --out build/boot_nolift
  python tools/boot_distance.py --version 2.1.0 --nvn   # with the NVN shim, in the Vulkan image (lavapipe)
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import signal
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_OUT = ROOT / "build" / "boot"
DEFAULT_LIFTED = ROOT / "build" / "port" / "lifted"
DEFAULT_SHADERS = ROOT / "build" / "port" / "shaders"  # the `shaders` target's output (shaders.manifest)
VULKAN_IMAGE = "dread-port-linux-vulkan"  # port/docker/vulkan: the loader and lavapipe
RUN_SH = ROOT / "port" / "docker" / "run.sh"

# (key, label, symbol, when): in boot order. `symbol` is the function's demangled name without its
# parameters; `when` is "enter" (reached when the function is entered) or "return" (when it returns).
MILESTONES: list[tuple[str, str, str, str]] = [
    ("nninitStartup", "nninitStartup done", "nninitStartup", "return"),
    ("static-init", "static initialisers done", "main", "enter"),
    ("nnMain", "nnMain entered", "nnMain", "enter"),
    ("romfs", "RomFS mounted", "nn::fs::MountRom", "return"),
    ("system-startup", "system start-up done (fn_71001325d0)", "fn_71001325d0", "return"),
    ("game-main", "game main fn_7100000980 entered", "fn_7100000980", "enter"),
    ("fn_7100015c70", "fn_7100015c70 entered", "fn_7100015c70", "enter"),
    ("fn_7100015c70-done", "fn_7100015c70 done", "fn_7100015c70", "return"),
    ("frame", "first frame: the per-frame callback fn_7100000d40 entered", "fn_7100000d40", "enter"),
    ("frame-done", "first frame done", "fn_7100000d40", "return"),
]

TRAP = re.compile(r"^dread: trap \d+ \(continuing\): (\S+?)(, mismatched call)?$")
STOP = re.compile(r"^dread: call to (?:a function with no source yet: (fn_[0-9a-f]+)|an import the port does not "
                  r"define yet: (\S+)|(fn_[0-9a-f]+) through a declaration)")
# glibc's own message before it aborts (a heap check: free() of a pointer malloc never returned, ...)
LIBC = re.compile(r"^(?:(?:free|malloc|realloc|munmap_chunk|malloc_consolidate)\(\)|double free|corrupted )")
FAULT = re.compile(r"^dread: (.+?) at address (0x[0-9a-f]+)")
EVENT = re.compile(r"^(\d+\.\d+) (\S+) (0x[0-9a-f]+)$")
TRAP_LINES = 8  # trap addresses printed (all of them are in the JSON)


def rel(p: Path) -> str:
    """p relative to the checkout, without following links (build/port/lifted may be a junction)."""
    try:
        return Path(os.path.abspath(p)).relative_to(ROOT).as_posix()
    except ValueError:
        return str(p)


# --- parsing (no gdb, no game data: tools/tests/test_boot_distance.py) ---------------------------

def parse_log(lines: list[str]) -> dict:
    """The runtime's own lines in the run's output: traps in order, an aborting trap, a fault."""
    traps: list[dict] = []
    seen: set[tuple[str, bool]] = set()
    stop = fault = libc = None
    for line in lines:
        line = line.rstrip()
        m = TRAP.match(line)
        if m:
            key = (m.group(1), bool(m.group(2)))
            if key not in seen:
                seen.add(key)
                kind = "mismatched" if m.group(2) else ("function" if m.group(1).startswith("fn_") else "import")
                traps.append({"name": m.group(1), "kind": kind})
            continue
        m = STOP.match(line)
        if m and stop is None:
            stop = m.group(1) or m.group(2) or m.group(3)
            continue
        if libc is None and LIBC.match(line):
            libc = line
            continue
        m = FAULT.match(line)
        if m and fault is None:
            fault = {"signal": m.group(1), "address": m.group(2)}
    return {"traps": traps, "stop": stop, "fault": fault, "libc": libc}


def parse_events(lines: list[str]) -> tuple[dict[str, float], set[int], dict]:
    """The gdb side's event file: milestone keys reached (seconds), lift addresses run, the end."""
    reached: dict[str, float] = {}
    lifted: set[int] = set()
    end: dict = {}
    for line in lines:
        line = line.strip()
        if line.startswith("end "):
            end = json.loads(line[4:])
            continue
        m = EVENT.match(line)
        if not m:
            continue
        t, tag = float(m.group(1)), m.group(2)
        if tag.startswith("m:"):
            reached.setdefault(tag[2:], t)
        elif tag.startswith("l:"):
            lifted.add(int(tag[2:], 16))
    return reached, lifted, end


def furthest(reached: dict[str, float]) -> int:
    """Index in MILESTONES of the last milestone reached, -1 for none."""
    best = -1
    for i, (key, *_rest) in enumerate(MILESTONES):
        if key in reached:
            best = i
    return best


def milestone_index(name: str) -> int | None:
    for i, (key, label, *_rest) in enumerate(MILESTONES):
        if name in (key, label):
            return i
    return None


def summarize(reached: dict[str, float], lifted_ran: int, lifted_linked: int | None, log: dict, end: dict) -> dict:
    f = furthest(reached)
    traps = log["traps"]
    return {
        "furthest": MILESTONES[f][0] if f >= 0 else None,
        "furthest_label": MILESTONES[f][1] if f >= 0 else None,
        "milestones": [{"key": k, "label": lab, "symbol": sym, "when": when, "reached": k in reached,
                        "seconds": reached.get(k)} for k, lab, sym, when in MILESTONES],
        "traps": {"distinct": len(traps), "functions": sum(t["kind"] == "function" for t in traps),
                  "imports": sum(t["kind"] == "import" for t in traps),
                  "mismatched": sum(t["kind"] == "mismatched" for t in traps), "order": traps},
        "aborting_trap": log["stop"],
        "fault": log["fault"],
        "libc": log.get("libc"),
        "lifted": {"linked": lifted_linked, "ran": lifted_ran},
        "end": end,
    }


def report(result: dict) -> list[str]:
    """The summary line and the short report under it (at most about 20 lines)."""
    ms = result["milestones"]
    f = result["furthest"]
    n = sum(m["reached"] for m in ms)
    tr = result["traps"]
    end = result["end"]
    how = ("time-out after %ss" % end.get("timeout") if end.get("timeout")
           else "signal %s" % end["signal"] if end.get("signal")
           else "exit %s" % end["exit"] if "exit" in end else "no end recorded")
    if result["fault"]:
        how += f", {result['fault']['signal']} at {result['fault']['address']}"
    lifted = result["lifted"]
    lifts = (f"{lifted['ran']} of {lifted['linked']} lifts ran" if lifted["linked"] is not None
             else "no lifts linked")
    head = (f"boot distance: {result['furthest_label'] or 'no milestone'} "
            f"({n} of {len(ms)} milestones), {tr['distinct']} traps, {lifts}, {how}")
    lines = [head]
    for m in ms:
        t = f" ({m['seconds']:.1f} s)" if m["reached"] else ""
        lines.append(f"  [{'x' if m['reached'] else ' '}] {m['key']}: {m['label']}{t}")
    lines.append(f"  traps: {tr['functions']} functions, {tr['imports']} imports, {tr['mismatched']} mismatched calls"
                 + (f"; aborted at {result['aborting_trap']}" if result["aborting_trap"] else ""))
    shown = [t["name"] for t in tr["order"][:TRAP_LINES]]
    if shown:
        more = tr["distinct"] - len(shown)
        lines.append("  first traps: " + ", ".join(shown) + (f" (+{more} more)" if more > 0 else ""))
    if result.get("libc"):
        lines.append(f"  C library: {result['libc']}")
    return lines


# --- the gdb side (runs inside gdb: gdb -x tools/boot_distance.py) -------------------------------

def gdb_main() -> None:
    import gdb  # noqa: F401  (gdb's own module)

    spec = json.loads(Path(os.environ["DREAD_BOOT_SPEC"]).read_text())
    events = open(spec["events"], "w", buffering=1)
    t0 = time.monotonic()
    for cmd in ("set pagination off", "set confirm off", "set startup-with-shell off",
                "set disable-randomization off", "set print thread-events off", "set print inferior-events off",
                "handle all nostop noprint pass"):
        gdb.execute(cmd, to_string=True)

    tags: dict[int, list[str]] = {}
    bps: dict[int, object] = {}

    def place(address: int, tag: str) -> None:
        tags.setdefault(address, []).append(tag)
        if address not in bps:
            bps[address] = gdb.Breakpoint(f"*{address:#x}", internal=True)

    for b in spec["breakpoints"]:
        place(b["address"], b["tag"])

    end: dict = {}

    def on_exit(ev) -> None:
        if hasattr(ev, "exit_code"):
            end["exit"] = ev.exit_code
        else:
            try:
                n = int(gdb.parse_and_eval("$_exitsignal"))
                end["signal"] = signal.Signals(n).name if n in signal.Signals._value2member_map_ else str(n)
            except (gdb.error, ValueError):
                end["signal"] = "unknown"

    gdb.events.exited.connect(on_exit)
    try:
        gdb.execute("run", to_string=True)
        while gdb.selected_inferior().pid:
            pc = int(gdb.selected_frame().pc())
            for tag in tags.pop(pc, []):
                if tag.startswith("r:"):  # a "return" milestone's entry: watch its return address
                    place(int(gdb.parse_and_eval("*(unsigned long *)$sp")), "m:" + tag[2:])
                else:
                    events.write(f"{time.monotonic() - t0:.3f} {tag} {pc:#x}\n")
            bp = bps.pop(pc, None)
            if bp is not None and pc not in tags:
                bp.delete()
            gdb.execute("continue", to_string=True)
    except gdb.error as e:
        end.setdefault("gdb_error", str(e).splitlines()[0] if str(e) else "error")
    end["seconds"] = round(time.monotonic() - t0, 1)
    events.write("end " + json.dumps(end) + "\n")
    events.close()


# --- the run (Linux) ------------------------------------------------------------------------------

def nm_symbols(exe: Path) -> dict[str, list[tuple[int, str]]]:
    """Demangled name without its parameters -> [(address, nm type)] for the executable's definitions."""
    nm = shutil.which("llvm-nm") or shutil.which("nm")
    out = subprocess.run([nm, "--defined-only", "-C", str(exe)], capture_output=True, text=True, check=True).stdout
    syms: dict[str, list[tuple[int, str]]] = {}
    for line in out.splitlines():
        parts = line.split(" ", 2)
        if len(parts) != 3 or parts[1] not in "TtWw":
            continue
        name = parts[2].split("(", 1)[0]
        d = (int(parts[0], 16), parts[1])
        syms.setdefault(name, []).append(d)
        if "::fn_" in name:  # callee::fn_<addr> and the like, under " ::fn_<addr>"
            syms.setdefault(" ::" + name.rsplit("::", 1)[1], []).append(d)
    return syms


def resolve(syms: dict[str, list[tuple[int, str]]], name: str) -> int | None:
    """The address of a function by its demangled name; an fn_<addr> name may also sit in a namespace."""
    cands = syms.get(name) or (syms.get(" ::" + name) if name.startswith("fn_") else None)
    if not cands:
        return None
    rank = {"T": 0, "W": 1, "t": 2, "w": 3}
    return min(cands, key=lambda d: rank[d[1]])[0]


def lifted_linked(link: Path) -> list[int]:
    """Addresses of the lifts tools/port_link.py kept (objects_lifted.rsp)."""
    rsp = link / "objects_lifted.rsp"
    if not rsp.exists():
        return []
    return sorted({int(m.group(1), 16) for m in re.finditer(r"fn_([0-9a-f]+)\.o", rsp.read_text())})


def run_step(title: str, cmd: list[str], log: Path) -> bool:
    t = time.monotonic()
    with open(log, "w") as f:
        r = subprocess.run(cmd, cwd=ROOT, stdout=f, stderr=subprocess.STDOUT)
    dt = time.monotonic() - t
    if r.returncode != 0:
        tail = log.read_text(errors="replace").splitlines()[-5:]
        print(f"boot distance: {title} FAILED (exit {r.returncode}, {dt:.0f} s; {rel(log)}):")
        for line in tail:
            print("  " + line[:200])
        return False
    print(f"{title}: {dt:.0f} s ({rel(log)})")
    return True


def build(args, out: Path) -> bool:
    py = sys.executable
    sweep = out / "sweep"
    link = out / "link"
    if not run_step("sweep", [py, "tools/port_sweep.py", "--version", args.version, "--stdlib", "libcxx9",
                              "--objects", rel(sweep), "--json", rel(out / "sweep.json")], out / "sweep.log"):
        return False
    addresses = args.addresses
    if not addresses and (ROOT / "build" / "check" / args.version).is_dir():
        # vtables and functions named other than fn_<addr>, from tools/check.py's objects; without them
        # the default allocator's constructor is a residue trap and the boot stops in nninitStartup
        addresses = rel(out / "addresses.json")
        if not run_step("addresses", [py, "tools/port_addresses.py", "--version", args.version, "--out", addresses],
                        out / "addresses.log"):
            return False
    cmd = [py, "tools/port_link.py", "--version", args.version, "--stdlib", "libcxx9", "--objects", rel(sweep),
           "--out", rel(link), "--executable"]
    if args.lifted:
        cmd += ["--lifted", args.lifted]
    if addresses:
        cmd += ["--addresses", addresses]
    if args.nvn:
        cmd.append("--nvn")
    else:
        print("addresses: no build/check/<version> (tools/check.py) and no --addresses; the link places fewer names")
    return run_step("link", cmd, out / "link.log")


def run_boot(args, out: Path) -> dict | None:
    link = out / "link"
    exe = link / "dread"
    if not exe.exists():
        print(f"boot distance: no executable at {rel(exe)} (build it first, without --no-build)")
        return None
    if not shutil.which("gdb"):
        print("boot distance: gdb not found (rebuild the image: docker build -t dread-port-linux port/docker)")
        return None
    syms = nm_symbols(exe)
    bps: list[dict] = []
    for key, _label, sym, when in MILESTONES:
        a = resolve(syms, sym)
        if a is not None:
            bps.append({"address": a, "tag": ("m:" if when == "enter" else "r:") + key})
    unresolved = [key for key, _l, sym, _w in MILESTONES if resolve(syms, sym) is None]
    lifts = lifted_linked(link) if args.lifted else []
    placed = 0
    for a in lifts:
        x = resolve(syms, f"fn_{a:x}")
        if x is not None:
            bps.append({"address": x, "tag": f"l:{a:x}"})
            placed += 1
    events = out / "events.txt"
    spec = out / "gdb_spec.json"
    spec.write_text(json.dumps({"events": str(events), "breakpoints": bps}))
    events.unlink(missing_ok=True)
    env = dict(os.environ, DREAD_TRAP="log", DREAD_BOOT_SPEC=str(spec))
    if args.nvn:
        env["DREAD_NVN_SHADERS"] = str(Path(args.shaders).resolve())  # the shim's shader manifest
    cmd = ["timeout", "-k", "10", str(args.timeout), "gdb", "-nx", "-batch", "-q", "-x", str(Path(__file__).resolve()),
           "--args", str(exe)]
    log = out / "boot.log"
    with open(log, "w") as f:
        r = subprocess.run(cmd, cwd=ROOT, env=env, stdin=subprocess.DEVNULL, stdout=f, stderr=subprocess.STDOUT)
    reached, ran, end = parse_events(events.read_text().splitlines() if events.exists() else [])
    if r.returncode in (124, 137) and not end:
        end = {"timeout": args.timeout}
    result = summarize(reached, len(ran), len(lifts) if args.lifted else None,
                       parse_log(log.read_text(errors="replace").splitlines()), end)
    result["unresolved_milestones"] = unresolved
    result["lifted"]["placed"] = placed
    result["version"] = args.version
    return result


def in_docker(args) -> int:
    """Run this tool inside the port's image (port/docker/run.sh), from Git Bash on Windows."""
    bash = None
    git = shutil.which("git")
    if git and os.name == "nt":  # Git for Windows' bash; `bash` on PATH may be WSL's
        for parent in Path(git).resolve().parents:
            if (parent / "bin" / "bash.exe").exists():
                bash = str(parent / "bin" / "bash.exe")
                break
    bash = bash or shutil.which("bash")
    if not bash:
        print("boot distance: no bash for port/docker/run.sh (install Git for Windows)")
        return 2
    out = Path(os.path.abspath(args.out))
    try:
        out.relative_to(ROOT)
    except ValueError:
        print(f"boot distance: --out {args.out} is outside the checkout; the container cannot see it")
        return 2
    inner = ["python3", "tools/boot_distance.py", "--version", args.version, "--out", rel(out),
             "--timeout", str(args.timeout), "--lifted", args.lifted or ""]
    for flag, value in (("--expect", args.expect), ("--addresses", args.addresses)):
        if value:
            inner += [flag, value]
    if args.no_build:
        inner.append("--no-build")
    env = None
    if args.nvn:
        inner += ["--nvn", "--shaders", rel(Path(os.path.abspath(args.shaders)))]
        env = dict(os.environ)
        env.setdefault("DREAD_PORT_IMAGE", VULKAN_IMAGE)
    return subprocess.run([bash, RUN_SH.relative_to(ROOT).as_posix(), *inner], cwd=ROOT, env=env).returncode


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n", 1)[0])
    ap.add_argument("--version", required=True, help="game version (orig/<version>)")
    ap.add_argument("--out", default=str(DEFAULT_OUT), help="output directory (default build/boot)")
    ap.add_argument("--timeout", type=int, default=180, help="seconds the boot may run (default 180)")
    ap.add_argument("--lifted", default=str(DEFAULT_LIFTED) if DEFAULT_LIFTED.exists() else "",
                    help="tools/port_lift.py output to link (default build/port/lifted when present; '' for none)")
    ap.add_argument("--no-lifted", action="store_true", help="link without the semantic lifts")
    ap.add_argument("--addresses", default="", help="passed to tools/port_link.py --addresses")
    ap.add_argument("--no-build", action="store_true", help="run the executable already in <out>/link")
    ap.add_argument("--expect", help="exit 1 unless this milestone (key or label) was reached")
    ap.add_argument("--nvn", action="store_true",
                    help="link the NVN shim (tools/port_link.py --nvn) and run in the Vulkan image (lavapipe;"
                         f" DREAD_PORT_IMAGE, default {VULKAN_IMAGE})")
    ap.add_argument("--shaders", default=str(DEFAULT_SHADERS),
                    help="with --nvn: the directory of shaders.manifest (default build/port/shaders)")
    ap.add_argument("--docker", choices=("auto", "yes", "no"), default="auto",
                    help="run inside port/docker/run.sh (auto: on any host but Linux)")
    args = ap.parse_args()
    if args.no_lifted:
        args.lifted = ""
    if args.lifted:
        args.lifted = rel(Path(args.lifted))
    if args.expect and milestone_index(args.expect) is None:
        print(f"boot distance: unknown milestone {args.expect!r}; one of: {', '.join(m[0] for m in MILESTONES)}")
        return 2
    if args.docker == "yes" or (args.docker == "auto" and not sys.platform.startswith("linux")):
        return in_docker(args)

    if args.lifted and not (ROOT / args.lifted / "manifest.json").exists():
        print(f"boot distance: no tools/port_lift.py output in {args.lifted} (manifest.json); --no-lifted links none")
        return 2
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    if not args.no_build and not build(args, out):
        return 2
    result = run_boot(args, out)
    if result is None:
        return 2
    (out / "boot_distance.json").write_text(json.dumps(result, indent=1) + "\n")
    for line in report(result):
        print(line)
    if result["unresolved_milestones"]:
        print("  not in the executable: " + ", ".join(result["unresolved_milestones"]))
    if args.expect:
        want = milestone_index(args.expect)
        got = furthest({m["key"]: 0 for m in result["milestones"] if m["reached"]})
        if got < want:
            print(f"boot distance: expected {MILESTONES[want][0]}, reached {result['furthest'] or 'none'}")
            return 1
    return 0


if __name__ == "__main__":
    if "gdb" in sys.modules:
        gdb_main()
    else:
        sys.exit(main())
