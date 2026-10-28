---
name: run-dread-decomp
description: Build, check and drive the dread-decomp matching toolchain. Use when asked to run, build or test dread-decomp, check for regressions, try/match a function, diff an object against the original, run objdiff, view Ghidra's decompilation, show progress, or screenshot objdiff.
---

dread-decomp has no runnable game yet (the PC port is empty). What you run is the matching
toolchain: compile `src/` with the project's Clang 9.0.1 and diff it against the user's dump.
Drive it with `.claude/skills/run-dread-decomp/smoke.py`, which checks the setup, proves that the
match loop catches a mismatch, runs the full regression check and reports progress. All paths
are relative to the repo root. Shell: Git Bash on Windows 11 (the only platform this was run
on).

Read `CLAUDE.md` first: no game data, decompiler output or assembly in tracked files.
Everything below writes only to `build/` (gitignored).

## Prerequisites

The user supplies the dump, keys, Ghidra and LLVM; you never download them. `smoke.py env`
lists each prerequisite and the command that creates it when it is missing:

```bash
.venv/Scripts/python .claude/skills/run-dread-decomp/smoke.py env
```

Every line should say `ok`. Lines marked `optional` (objdiff, the Ghidra project) only gate
the commands that need them. The full from-scratch rebuild is in `docs/reference.md`
("Rebuilding everything from scratch").

## Run (agent path)

Everything at once (about 40 s; exit 0 = PASS):

```bash
.venv/Scripts/python .claude/skills/run-dread-decomp/smoke.py all
```

| command | what it does |
|---|---|
| `env` | prerequisites (see above) |
| `selftest` | `trymatch` on a known-matching source (expects `1/1 functions match`, exit 0) and on a copy with one store changed (expects `0/1`, exit 1) |
| `check` | `tools/check.py`: builds all of `src/` (~36 s), fails on any `REGRESSION` line, prints the `O/m/M/W` totals |
| `try FILE [-v] [-- flags]` | `tools/trymatch.py` on one source (in `src/` or a scratch file) |
| `objdiff UNIT` | `objdiff-cli` one-shot diff of one unit; match % per function |
| `progress` | `tools/progress.py` headline (game code only, `L` excluded) |

The inner loop when matching one function:

```bash
.venv/Scripts/python .claude/skills/run-dread-decomp/smoke.py try src/base/reflection/UnkReflFunction.cpp
.venv/Scripts/python tools/trymatch.py --version 2.1.0 src/base/reflection/UnkReflFunction.cpp -v
```

`trymatch` exit codes: 0 = all mapped functions match, 1 = a mismatch (`-v` prints a
side-by-side diff), 2 = compile error (Clang's message is printed). Put scratch sources
in the session scratchpad or `build/`, never in `src/`.

Reading the original (for you only; never paste it into tracked files):

```bash
.venv/Scripts/python tools/ghidra/decompile.py --version 2.1.0 0x7100a19530 > build/smoke/decompile.txt
```

Takes about 10 s. Two viewers can run at the same time.

objdiff (per-unit view, the same units the GUI shows):

```bash
.venv/Scripts/python tools/objdiff_gen.py --version 2.1.0
.venv/Scripts/python .claude/skills/run-dread-decomp/smoke.py objdiff base/reflection/UnkReflFunction
"$LOCALAPPDATA/dread-decomp/tools/objdiff-cli-3.8.2.exe" report generate -p . -o build/smoke/objdiff_report.json -f json-pretty
```

`objdiff_gen.py` takes about 2 minutes (it builds every unit and synthesises target objects)
and must run again after `src/` changes.

Generated sources: after changing `tools/refl_attrs.py`, `tools/gen_reflection.py` or the
reflection headers, regenerate and check again:

```bash
.venv/Scripts/python tools/gen_reflection.py --version 2.1.0
.venv/Scripts/python .claude/skills/run-dread-decomp/smoke.py check
```

Before committing `src/` changes, record the statuses (rewrites `data/functions/`):

```bash
.venv/Scripts/python tools/check.py --version 2.1.0 --update
```

## Screenshot (objdiff GUI)

computer-use can't be granted the objdiff GUI: it's a portable exe, not a Start-menu app.
Launch it, then capture its window with `winshot.ps1` (`PrintWindow`, so it also works when
the window is behind others):

```bash
"$LOCALAPPDATA/dread-decomp/tools/objdiff-3.8.2.exe" > build/smoke/objdiff_gui.log 2>&1 &
powershell -NoProfile -ExecutionPolicy Bypass -File .claude/skills/run-dread-decomp/winshot.ps1 -ProcessName objdiff-3.8.2 -Out build/smoke/objdiff.png
```

Stop it afterwards (PowerShell): `Get-Process -Name objdiff-3.8.2 | Stop-Process`.

## Run (human path)

Open the objdiff GUI and point its project settings at the repo root (it reads
`objdiff.json`). On first launch it shows "Missing project settings": it doesn't pick up the
working directory. Setting the project wasn't tested here, because it changes the user's
objdiff config.

## Gotchas

- **objdiff under-reports matches.** Functions that `check.py`/`trymatch` mark `O` show
  99.2–99.9% in objdiff (for example `reflection/CharClassRechargePlayerLife`). objdiff scores
  relocation and data-reference differences that `tools/asmdiff.py` masks, and the target
  objects point string literals at an undefined `.rodata.str1.1`. Use `check.py`/`trymatch`
  for status and objdiff for viewing. `smoke.py objdiff` exits 1 whenever any function is
  below 100%.
- **objdiff counts more functions than `check.py`** (18,987 against 17,105): constructors compile
  to C1/C2 aliases, which `check.py` counts once by address.
- **`check.py` and `progress.py` disagree on `O`** (17,105 against 17,096): matching
  `nn::atk`/library functions keep `L` and are excluded from progress.
- **`decompile.py` output is game-derived.** Redirect it into `build/` and read it there;
  never commit it or quote more than a few lines.
- **`smoke.py selftest` relies on `src/base/reflection/UnkReflFunction.cpp` matching.** If
  that file changes, point `KNOWN` in `smoke.py` at another single-function source that matches.
- **objdiff opens a 22×22 window first.** Capturing too early gives a 22×22 PNG. `winshot.ps1`
  waits up to 15 s for a window at least 200×200.
- **Use `.venv/Scripts/python`**, not the system Python: the tools need `capstone`,
  `pyelftools` and `pyghidra` from the venv.

## Troubleshooting

- **`RuntimeError: Could not determine home directory`** (any tool) or **Ghidra:
  `"APPDATA" environment variable is not set`**: the shell's environment was stripped. The
  tools need `USERPROFILE`, `APPDATA` and `LOCALAPPDATA` (toolchains live under
  `%LOCALAPPDATA%\dread-decomp\`).
- **`winshot.ps1`: `no window for objdiff-3.8.2`**: the GUI isn't running, or took more than
  15 s to open a window.

- **The objdiff GUI shows "Missing project settings"**: it doesn't read the working
  directory. Use `smoke.py objdiff UNIT` (CLI), or set the project in the GUI (human path).
- **`request_access` says objdiff "doesn't match any installed or running application"**:
  it isn't in the Start menu. Use `winshot.ps1`.
- **`trymatch` prints `error: use of undeclared identifier ...` and exits 2**: a compile error
  in the source, not a mismatch.
