---
name: dread-match
description: Hand-match Metroid Dread functions in var-flotsam/dread-decomp on one address slice, near copies first, in a tight compile-and-diff loop with batched PRs.
---

# dread-match

Fast, cheap hand-matching for `var-flotsam/dread-decomp`. One session owns one address slice
and nothing else, so many sessions can run at once. Read nothing else unless stuck:
not `docs/HANDOFF.md`, not the other streams' notes. `CLAUDE.md` loads by itself; its hard
rules still apply.
The same text works as a pasted prompt in a session without this skill: fill in the inputs.

## Inputs (ask once if missing)
- `SLICE`: `LO-HI` hex addresses, and the source folder `src/game_<LO>/` (create it).
- `BRANCH`: the branch to push to. Whether you may merge your own PRs (default: no).

## Choosing slices (once, by whoever launches the sessions)
Split the `U` bytes evenly so each session gets the same amount of work:
```sh
N=8; cat data/functions/main_*.csv | awk -F, -v N=$N '$6=="U" && $2>=36 && $2<=2048 && $5!~/^(script|engine)/{a[++n]=$1;s[n]=$2;t+=$2} END{k=1;printf "0x7100000000";for(i=1;i<=n;i++){c+=s[i];if(c>=t*k/N&&k<N){printf " %s",a[i+1];k++}}print " 0x7200000000"}'
```
Give each session one adjacent pair as `LO-HI`, its own branch, and never two sessions the same slice.

## Hard rules (short form)
- No game data in git: nothing from `orig/` or `build/`, no disassembly, no `asmdiff -v` or
  Ghidra output in files, commits or PR text. Scratch work goes in `build/` (gitignored).
- `src/` is your own C++. Placeholder names (`fn_<addr>`, `field_0x18`, `UnkFoo_0x40`).
- Never `--no-verify`. Never edit generated dirs (`src/reflection*/`, `src/clones/`,
  `src/lifted/`, `src/tables/`), `include/`, `tools/`, `CLAUDE.md` or `docs/`.
- Define only functions whose address is inside your slice, and only in your folder.
  Helpers you need go in a header in your folder (`#include "UnkFoo.h"`). You may include
  any existing header, never edit one.
- No subagents. One function at a time.

## Setup (Linux cloud, ~5 min)
```sh
unzip -q <dread-cloud-2.1.0.zip> -d .        # the user's private inputs: orig/ and build/
python3 tools/nso2elf.py orig/2.1.0/exefs/main orig/2.1.0/main.elf
git config core.hooksPath tools/hooks && git submodule update --init
pip install lz4 mercury-engine-data-structures capstone pyelftools
T=~/dread-decomp/toolchains; mkdir -p $T && cd $T && curl -LO https://github.com/llvm/llvm-project/releases/download/llvmorg-9.0.1/clang+llvm-9.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz \
  && tar xf clang+llvm-9.0.1-*.tar.xz && mv clang+llvm-9.0.1-x86_64-linux-gnu-ubuntu-16.04 llvm-9.0.1 && cd -
python3 tools/fetch_sources.py   # libc++ 9.0.1 headers
export PATH=$HOME/dread-decomp/toolchains/llvm-9.0.1/bin:$PATH     # llvm-cxxfilt
python3 tools/check.py --version 2.1.0 | tail -3                   # must report 0 failures
```
On the owner's Windows PC everything is installed: use `.venv/Scripts/python` for `python3`,
run `verify.py` in the background (8-13 min, longer than a foreground command's timeout), and
edit files with the Edit tool or Python, not `sed -i` (Git Bash). Skip to the loop. If the zip path is unknown, ask; never download game data.

## Pick candidates (best yield first)
1. Run `python3 tools/nearcopy.py --version 2.1.0 LO HI` (~20 s; never while `verify.py` runs, which rewrites the
   CSVs). Each line: similarity, size, the `U` function, the closest `O` function of the same
   size, `refs` (calls and data targets that differ) and the file defining the original.
2. Work top-down, skipping rows whose original is in a generated dir (`src/clones/`,
   `src/lifted/`, `src/reflection*/`): list those in `build/generated_copies.txt` for the report.
   A 100% row still usually differs in struct offsets or element sizes: expect 2-4 compiles.
3. **Check refs before trusting a match.** `trymatch`/`check.py` mask relocated fields, so a copy
   that calls the wrong function or reads the wrong global still scores `O`. When `refs > 0`,
   run `python3 tools/nearcopy.py --version 2.1.0 --refs U O` and declare the copy's own callees (`fn_<addr>`)
   and data (`unk_<addr>`) in the differing positions (for example the FilePathStrId pool
   instead of the StrId pool). Do this before committing every copy.
4. Put a family (one original, several copies) in one file: a template or macro instantiated
   per copy, the first comment line naming the original.
5. When near copies run below ~80%, take plain `U` functions of 128 B-1.5 KB in the slice
   (`data/functions/main_XX.csv`, status `U`, skip `script/` and `engine/` subsystems) and
   start from `python3 tools/gen_lift.py --version 2.1.0 --best ADDR,...`
   (writes `build/lift_best/fn_<addr>.cpp`, ~3 s; give it real types before keeping it).
6. Rerun `tools/nearcopy.py` after each merged batch: new `O` functions expose new copies.

## Loop per function
```sh
python3 tools/trymatch.py --version 2.1.0 src/game_<LO>/fn_<addr>.cpp      # score per function
python3 tools/trymatch.py --version 2.1.0 src/game_<LO>/fn_<addr>.cpp -v   # side-by-side, read only
```
Exit 0 means every function in the file is byte-identical with relocations masked (so check
refs, step 3 above). `-v` is long: `| grep "!!"` shows only the differing lines.
Change one thing per compile. Known levers, all found in this codebase:
- Wrong inlining: `__attribute__((always_inline))` on the helper, or `flatten` on a
  wrapper the compiler refuses to inline into; `visibility("hidden")` for unit-local callees.
- Merged or reordered loads/stores: a user-written move/copy constructor instead of the
  implicit one, `may_alias` word types, or `asm("" : "+r"(p));` to pin an address.
- Globals reached through the GOT are plain `extern char unk_<addr>[];` declarations.
- Loop indices are often `u64`; parameters by reference vs value change the frame.
- Stack order follows declaration order of locals.
- Float `fmin`+`b.lt` vs `fminnm`+`b.mi`: write the comparison out by hand.

## Stop rule
Time-box each function to 20 minutes or 8 compiles without a better score, whichever is
first. Then delete it (do not commit `NON_MATCHING`: only `O` counts toward progress),
append `addr score reason` to `build/stuck.txt`, and move on. Never come back in the same
session. If three functions in a row get stuck, switch to the next family.

## Verify and batch
Every 15-30 matched functions (or ~2 hours):
```sh
git fetch origin main && git merge origin/main
git checkout origin/main -- data/functions    # only on a CSV conflict; never hand-merge them
python3 tools/verify.py --version 2.1.0       # check --update + progress + policy
```
It must end in PASS with no regressions (its "O a -> b" banner compares against its own
baseline, not the CSVs; trust the regression line and the progress %) and no duplicate definitions (a duplicate means
another slice or a generator already defines it: delete yours). Then commit only your folder,
`data/functions/*.csv` and any `src/lifted/` deletions `verify.py` made (its prune step):
```sh
git add src/game_<LO>/ data/functions/ src/lifted/ && git commit -m "Phase 4.4: slice <LO> near copies, batch N (K functions)"
git push -u origin BRANCH
```
Open one PR per batch to `main` (fill the repo's PR template; give progress before -> after
from `verify.py`). Merge only if the user allowed it in this session, with a merge commit,
after `verify.py` passed on the merged branch. Start the next batch from the updated branch.
Do not edit `docs/HANDOFF.md`; the PR body is the record.

## Report
At the end (or when the user's limit nears): functions matched, KB, progress %, PR links, and
the top of `build/stuck.txt` in one short message.
