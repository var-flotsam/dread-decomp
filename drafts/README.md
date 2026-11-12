# drafts/

Hand-written functions that do not match yet. They were written by the agent runs of 2026-11-09 to
2026-11-11 and until now lived only in gitignored `build/` folders of four worktrees. They are kept here so
the work is not lost and so the tools that will judge and finish them have something to run on.

Nothing here is built by `tools/check.py`, the port or CI, and nothing here counts as progress. A draft
moves to `src/` when it matches, or when it is verified equivalent (`CLAUDE.md`, "Completion").

## Layout

`drafts/<unit folder>/fn_<address>.cpp`, the folder its function would have under `src/`
(`tools/tu_map.py --function <address>`). One draft per function: where a function had several, the
best-graded one is here. Includes of headers in `src/` are written relative to this folder, so a draft
that moves to `src/` needs them rewritten. `fn_7101077a70_shared.h` is one draft's own header, from its
batch's scratch folder.

To grade a draft:

```sh
python tools/trymatch.py --version 2.1.0 drafts/game/CAttackComponent/fn_7101136bb4.cpp -v
```

## What is here (graded 2026-11-11)

`INDEX.csv` has one row per draft: address, size, file, what separates it from the original, similarity,
the number of differing instructions, and the run it came from. It is a record of that day's grading, not
a generated file: regrade a draft rather than trust its row.

| What separates the draft from the original | Drafts | Bytes |
|---|---|---|
| Registers or instruction order only (the same instructions, constants and offsets) | 105 | 36.8 KB |
| Other instructions (widths, types, expressions) | 42 | 19.7 KB |
| Control flow | 27 | 27.4 KB |
| Compare or branch form only | 17 | 6.9 KB |
| Number of calls | 4 | 7.8 KB |
| Nothing: it matches, but is held back (see its note in `INDEX.csv`) | 2 | 0.9 KB |
| Total | 197 | 99.5 KB |

A pilot batch on 2026-11-12 added 42 more (`from` = `pilot50` in `INDEX.csv`; not in the table above): 10 registers
or order only, 20 other instructions, 7 control flow, 5 number of calls; 6 of the 42 at 90% or better.

154 of the 197 are at 90% similarity or better. Two more drafts matched cleanly and went to `src/`
(`fn_7100ad3404`, `fn_7100df81fc`). The two held back then (`fn_7100df31b8`, `fn_7100eeaed0`) went to `src/` on 2026-11-12, once the
declarations they disagreed with were settled; so did four of the five matches the 40-function test
left here. One match is still held: `fn_71008c3dcc` (see its note in `INDEX.csv`).

The first row is what the "verified equivalent" tier and a permuter are for. A registers-only difference is
a strong sign of equivalence but not proof of it: an inconsistent register swap changes behaviour, so
these still need the machine test before they count (`python tools/equiv_test.py --version 2.1.0 --drafts`;
on 2026-11-12 it passed 17 of the 115 registers-only drafts, failed 1 and could not judge 97). Nine of those 17
are in `src/` since that day; the notes of the rows that are left say what holds each one back.

## Rules

The repository's rules apply here as everywhere: our own code only, no disassembly or decompiler output
in any file, comments included. Don't add a draft that is far from its original (the large-function
drafts under 50% are here because they were all there was): a draft is worth keeping when someone can
finish it.
