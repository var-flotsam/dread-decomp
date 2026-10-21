# 00 Triage: check the setup and report

## Goal

Prove this machine builds `src/` to the same bytes as the PC, then report the state of the
tree in a few lines. Change no code.

## Files

None to edit. You may create `docs/tasks/notes/00-triage.md`.

## Commands

Use `PY=.venv/bin/python` (macOS) or `PY=.venv/Scripts/python` (Windows).

```sh
git status --short | wc -l                                   # must print 0
git log --oneline -1
$PY tools/check.py --version 2.1.0 | tail -1                 # about 1-2 minutes
$PY tools/check.py --version 2.1.0 | grep -c "REGRESSION"    # must print 0
$PY tools/verify.py --version 2.1.0                          # about 2-4 minutes on its first run
```

## Expected

At main `71f7115` the `check.py` summary line is:

```
80414 functions in src/: O 80409, m 4, M 1, W 0
```

On a later commit the numbers may be higher. What matters: no `REGRESSION` lines,
no `BUILD FAILED` lines, and `verify.py` prints `RESULT: PASS`.

## If it fails

- `BUILD FAILED`: the toolchain is broken. Print the first 10 lines of
  `$PY tools/check.py --version 2.1.0 | grep -A10 "BUILD FAILED"`. Check that
  `~/Library/Caches/dread-decomp/toolchains/llvm-9.0.1/bin/clang++ --version` runs (Rosetta 2).
- `REGRESSION` lines: this machine's compiler output differs from the PC. Do no other card.
  Record the count and the first 5 addresses only.
- `policy` fails: a file in the tree breaks the content rules. Record the file name only.

## Done when

You wrote `docs/tasks/notes/00-triage.md` with: the date, `git log --oneline -1`, the
`check.py` summary line, the `REGRESSION` count and the `verify.py` lines (all of them; they
are short). Commit it alone:

```sh
git add docs/tasks/notes/00-triage.md
git commit -m "Phase 4.3: triage report"
```

Then tell the owner PASS or FAIL in one line.

## Time-box

20 minutes, plus the build times. If anything needs more, stop and report.
