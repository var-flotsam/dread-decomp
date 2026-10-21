# Contributing

Thanks for helping. This page is the procedure for working from a fork. The rules in the first
section are not negotiable: a pull request that breaks one is closed, and its content may have
to be purged from the history.

Local AI coding agents (small models with short contexts) start with `AGENTS.md` instead.

## Rules

1. **No game data, ever.** Never commit, paste into an issue or PR, or attach: game files or
   assets, executables (`.nso`, `.nro`, `.elf`), containers (`.nca`, `.nsp`, `.xci`, ...), keys,
   firmware, or dumps of any kind. Game data lives only in `orig/` and `build/`, which are
   ignored by git.
2. **No disassembly or decompiler output in tracked files.** `src/` and `include/` are our own
   C++. Don't paste Ghidra, IDA or other decompiler output into them, and don't commit assembly
   listings, diffs or `tools/ghidra/decompile.py` output. A few instructions quoted in a PR
   discussion to explain a match are fine.
3. **No downloads of protected material.** Use only what you dumped yourself. Don't link to
   game files, keys or Nintendo SDK files anywhere in the project.
4. **Generated directories are outputs.** `src/reflection/`, `src/reflection_types/`,
   `src/reflection_statics/`, `src/clones/`, `src/lifted/` and the `status` column of `data/functions/` are
   produced by tools. Change the tool, regenerate, and commit both.
5. **No regressions.** `tools/check.py --version 2.1.0 --update` must pass with no
   `REGRESSION` lines before you open a PR.
6. Names you can't justify keep address-based placeholders (`fn_7100123456`, `field_0x18`).
   Real names come from RTTI, reflection data, strings or
   [mercury-engine-data-structures](https://github.com/randovania/mercury-engine-data-structures).

The pre-commit hook and CI enforce part of rules 1 and 4 (paths, file types, sizes). The rest
is on you and on review.

## Fork procedure

### 1. Fork and clone

Fork `var-flotsam/dread-decomp` on GitHub, then:

```sh
git clone --recurse-submodules https://github.com/<you>/dread-decomp.git
cd dread-decomp
git remote add upstream https://github.com/var-flotsam/dread-decomp.git
git config core.hooksPath tools/hooks      # the pre-commit content check; never bypass it
```

### 2. Toolchain

Python 3.12 in a venv at `.venv`, CMake, Ninja, and the project compiler (upstream Clang 9.0.1,
fetched by a script; the Nintendo SDK compiler is not used). Ghidra is optional (only for
reading code). Exact versions and the install commands are in `CLAUDE.md` and
`docs/HANDOFF.md` ("Rebuilding everything from scratch").

```sh
python -m venv .venv
.venv/Scripts/python -m pip install lz4 mercury-engine-data-structures capstone pyelftools   # Windows path; bin/ elsewhere
.venv/Scripts/python tools/fetch_toolchains.py
.venv/Scripts/python tools/fetch_sources.py
```

On macOS (Apple Silicon) use `.venv/bin/python`. `fetch_toolchains.py` fetches the official
x86_64 macOS build of Clang 9.0.1 (there is no arm64 one), which runs under Rosetta 2
(`softwareupdate --install-rosetta`); downloads go to `~/Library/Caches/dread-decomp`. This
path is untested on hardware: `check.py` must reproduce the Windows O count before you rely on it.

### 3. Your dump

Dump the base game and the 2.1.0 update from your own console. Then import and verify it.
`setup.py` checks the files against `data/hashes.json` and refuses anything else.

```sh
.venv/Scripts/python tools/setup.py --version 2.1.0 --exefs <your exefs dir> --romfs <your romfs dir>
.venv/Scripts/python tools/verify_dump.py --version 2.1.0
.venv/Scripts/python tools/nso2elf.py orig/2.1.0/exefs/main orig/2.1.0/main.elf
.venv/Scripts/python tools/check.py --version 2.1.0
```

If `check.py` passes on a fresh clone, your setup matches everyone else's.

### 4. Work on a branch

```sh
git fetch upstream
git switch -c <topic> upstream/main
```

- Pick work from `docs/HANDOFF.md` ("Next steps"), or open an issue first for anything large
  (a new generator, a new subsystem, a change to shared headers) so two people don't do the same
  thing.
- Commit messages start with the phase and step: `Phase 4.3: ...`.
- Keep commits focused: a tool change and its regenerated output belong together; unrelated
  changes don't.
- Before each commit: `tools/check.py --version 2.1.0 --update`, no `REGRESSION` lines.

### 5. Stay current

```sh
git fetch upstream
git rebase upstream/main          # or merge, if you've already shared the branch
.venv/Scripts/python tools/check.py --version 2.1.0 --update
```

`data/functions/*.csv` statuses often conflict. Don't resolve them by hand: take either side,
then rerun `check.py --update`, which rederives every status from `src/`. If a generator's
output conflicts, regenerate it (the order is in `CLAUDE.md`).

When several machines or people work at once (the owner's workstreams are in
`docs/HANDOFF.md`, "Workstreams"):

- Merge `main` into your branch often and keep PRs small.
- After merging `main`, take `main`'s `data/functions/*.csv`, rerun only the generator you own,
  then `tools/check.py --update` (or `tools/verify.py --regen <generator>`). Never hand-merge the CSVs.
- `gen_clones.py` and `gen_lift.py` skip functions defined elsewhere in `src/`, so a rerun drops
  duplicates.
- Branch prefixes are `pc/` and `mac/`.
- A machine that changes focus updates its Workstreams row in its next PR.

### 6. Open a pull request

Push to your fork and open a PR against `var-flotsam/dread-decomp:main`. Fill in the template:
what you matched (functions and bytes; `tools/progress.py` before and after), which tools you
changed, and confirm the checklist. CI runs the content-policy checks; it can't run
`check.py`, because CI has no game data, so reviewers rely on your report.

### 7. Review and merge

- Every change to `main` goes through a pull request. Direct pushes and force pushes to `main`
  are blocked.
- A maintainer reviews and merges, with a merge commit. The maintainer checks out your branch
  and runs `check.py` locally before merging generator changes.
- Changes to `tools/hooks/`, `.github/`, `LICENSE`, `CONTRIBUTING.md` and `CLAUDE.md`'s rules
  need the maintainer's review (see `.github/CODEOWNERS`).

## Reporting a problem

If you find game data, keys or listings anywhere in the repository or its history, **don't
copy or quote it**. Open an issue that only says where it is (path and commit), or contact the
maintainer privately. It will be removed from the history.
