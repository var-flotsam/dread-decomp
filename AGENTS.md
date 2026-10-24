# AGENTS.md: guide for local coding agents

You are a coding agent working on dread-decomp, a matching decompilation of Metroid Dread 2.1.0
(Nintendo Switch, AArch64). Source in `src/` must compile with Clang 9.0.1 to the same bytes as
the original game. Read this file and one task card in `docs/tasks/`. `docs/HANDOFF.md` is the
short current-state page and is safe to read; do not read `docs/JOURNAL.md` (the long history)
unless a card tells you to. The Mac workstream this guide was written for is retired
(`docs/tasks/README.md`); the rules below still hold for any small-context agent.

## Hard rules

1. Never commit, copy into tracked files, or print in full any game file, asset, key,
   firmware, assembly listing or decompiler output. Game data lives only in `orig/` and `build/`.
2. `src/` and `include/` hold our own C++. Never paste Ghidra or disassembler output there.
3. Never download game images, keys, firmware or Nintendo SDK files.
4. Never push, merge, rebase shared branches or open pull requests. The owner does that.
5. Never use `git commit --no-verify`. If the pre-commit hook rejects a file, remove that file.
6. Never edit generated folders by hand: `src/reflection/`, `src/reflection_types/`,
   `src/reflection_statics/`, `src/clones/`, `src/lifted/`. Change the generator and rerun it.
7. Edit only the files your task card lists. Never touch `tools/gen_lift.py` or `src/lifted/`
   (the PC owns them).
8. Names you are not sure of stay address-based: `fn_7100123456`, `field_0x18`, `UnkStruct_0x40`.
9. If an input is missing (a path, a tool, a file), stop and ask. Do not guess.

## Output discipline

- Print counts, not listings. Pipe long output through `| tail -5` or `| wc -l`.
- Never print a generated `.cpp` file, a CSV file or a disassembly in full.
- `tools/verify.py` already prints a short summary. Trust it.

## First-time setup on macOS (Apple Silicon)

The macOS toolchain path is untested on hardware. Step 7 is the test: do not start any task
until it passes.

1. Install Xcode command line tools (`xcode-select --install`), Python 3.12
   (`brew install python@3.12`) and Rosetta 2 (`softwareupdate --install-rosetta --agree-to-license`).
   Clang 9.0.1 has no arm64 macOS build; the x86_64 build runs under Rosetta 2.
2. Clone and configure:
   ```sh
   git clone --recurse-submodules https://github.com/var-flotsam/dread-decomp.git
   cd dread-decomp
   git config core.hooksPath tools/hooks
   python3.12 -m venv .venv
   .venv/bin/python -m pip install lz4 mercury-engine-data-structures capstone pyelftools
   ```
3. Copy the owner's own dump from external storage or the network to a folder outside the
   repo, for example `~/dread-dump/`. It holds `exefs/` and `romfs/` of title `010093801237C000`.
   ```sh
   rsync -a /Volumes/<drive>/<dump folder>/ ~/dread-dump/       # external drive
   rsync -a <pc-host>:<dump folder>/ ~/dread-dump/              # or over the network
   ```
   Never copy the dump into the repo yourself, and never `git add` it.
4. Import and verify it (both must print no errors):
   ```sh
   .venv/bin/python tools/setup.py --version 2.1.0 --exefs ~/dread-dump/exefs --romfs ~/dread-dump/romfs
   .venv/bin/python tools/verify_dump.py --version 2.1.0
   ```
5. Prepare the binary and analysis data:
   ```sh
   .venv/bin/python tools/nso2elf.py orig/2.1.0/exefs/main orig/2.1.0/main.elf
   .venv/bin/python tools/fetch_toolchains.py      # Clang 9.0.1 + objdiff, into ~/Library/Caches/dread-decomp
   .venv/bin/python tools/fetch_sources.py         # Lua and libc++ 9.0.1 headers
   .venv/bin/python tools/recon/run.py --version 2.1.0
   ```
6. If `git status` shows `data/toolchains.json` or `data/sources.json` changed, a download
   hash differs: stop and tell the owner.
7. Run the triage card, `docs/tasks/00-triage.md`. It must report the expected O count.

Ghidra is optional. `tools/gen_functions_csv.py` is not needed (the CSVs are tracked).

## The work loop

Use `PY=.venv/bin/python` below.

1. Start from a clean tree: `git status --short | wc -l` prints `0`.
2. Make a branch: `git switch -c mac/<card-name>`.
3. Open one card in `docs/tasks/`. Read only the files it lists.
4. Write a test source in `build/scratch/<name>.cpp` (gitignored) for the card's example
   address. Name the function `fn_<address>` so the tools can find it.
5. Build and diff that one function until it says `MATCH`:
   ```sh
   $PY tools/trymatch.py --version 2.1.0 build/scratch/<name>.cpp | tail -3
   $PY tools/trymatch.py --version 2.1.0 build/scratch/<name>.cpp -v | head -60   # side by side
   ```
6. Move the matching code into the generator and header the card names. Then run the gate:
   ```sh
   $PY tools/verify.py --version 2.1.0 --regen <generator>
   ```
7. If it prints `RESULT: PASS`, commit only the card's files and `data/functions/`:
   ```sh
   git add <files from the card> data/functions/
   git commit -m "Phase 4.3: <what you matched> (<N> functions)"
   ```
8. If it prints `RESULT: FAIL`, follow its `NEXT:` line. Never commit a failing state.
9. Pick the next family or card. Stop after the card's time-box.

After the owner merges `main` into your branch, run `$PY tools/verify.py --version 2.1.0
--regen <your generator>`. Never fix conflicts in `data/functions/*.csv` by hand: take
`main`'s copy (`git checkout origin/main -- data/functions/`), then run verify.

## When you are stuck

Stop when any of these happen: the time-box is over, three attempts in a row change nothing,
`verify.py` fails twice for the same reason, or a step needs a file not on the card.

Then write a note in `docs/tasks/notes/<card-name>.md` with: the card, the address you
tried, what you changed, the last `trymatch.py` summary line (counts only), and your best
guess at the cause. Use your own words; no disassembly, no decompiler output. Undo
unfinished code (`git checkout -- <files>`), commit the note alone
(`git commit -m "Phase 4.3: notes on <card-name>"`) and stop.

## Reference

- Status codes in `data/functions/*.csv`: `O` matching, `m`/`M` non-matching, `U` not done,
  `L` library.
- Mark a function that cannot match with `// NON_MATCHING(minor): <reason>` on the line above
  it (generators keep only matching code, so this is rare in your work).
- Compiler flags live in `tools/buildcfg.py`. Never change them.
- Full rules: `CLAUDE.md`. Fork procedure: `CONTRIBUTING.md`.
