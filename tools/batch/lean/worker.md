# Lean worker: decompile ONE function

You decompile one status-U function of dread-decomp (Metroid Dread, AArch64) into readable C++17 that compiles
byte-identically with the project's Clang 9.0.1. Your brief gives: the worktree, the address, the target
file pattern, and the attempt budget. Work only inside that worktree (`cd` there in every command).

Keep your context small. It is the main cost of this run.
- Do NOT read CLAUDE.md, docs/, the journal, or browse src/ broadly. Everything you need is here and in
  `build/lean/ctx/<addr>.txt`. Open at most 2 other files (a header named in ctx, or one matched sibling
  file from the same directory) and only if you need a type.
- Grade only with `.venv/Scripts/python build/lean/try.py <file>`. It prints a short diff. Never run
  tools/trymatch.py -v directly, check.py, verify.py, decl_check.py without --symbol, gen_*.py, Ghidra,
  port builds or git commands (git grep is fine).
- Don't re-read files you wrote; don't print files.

## Steps

1. Read `build/lean/ctx/<addr>.txt` and `build/lean/tips.md`.
2. Write the target file (replace `<What>` with one readable word for what the function does, e.g.
   `src/game/CFoo/CFoo_Reset_l3a7f10.cpp`). The function is at global scope, named `fn_<addr>`.
3. Run try.py. Revise from the diff and its KIND line (fix calls first, then control flow, then
   operations, then registers). Use the whole attempt budget from your brief: don't stop early on a low
   score, since one extra or missing instruction near the top shifts everything after it and can make a
   near-match score under 50%.
4. Declarations: before declaring a callee or global, `git grep -n "fn_XXXX\|g_unk_XXXX" -- include src | head -3`.
   Include the header if include/ declares it; otherwise copy an existing signature exactly (forward-declare
   its struct names, cast at the call site). When it matches, run
   `.venv/Scripts/python build/lean/declfile.py <file>` and fix what it lists, then rerun try.py.
5. Result:
   - MATCH (and no RELOC line): keep the file.
   - Otherwise: move your best draft to `build/lean/drafts/<addr>.cpp` (overwrite) and delete the src file.
     Never leave NON_MATCHING code in src/.
   Write `build/lean/results/<addr>.json`:
   `{"addr": "<addr>", "outcome": "match"|"minor"|"differs"|"skipped", "best": <0-100>, "attempts": <n>,
     "file": "<src path>" or null, "blocker": "<few words for a non-match>"}`
6. Reply with exactly one line: `<addr> <outcome> <best>%`.

## Conventions (CLAUDE.md, condensed)

- Unknown names: `fn_<addr>`, `g_unk_<addr>`, structs `Unk<Something>` with `field_0x<off>` and padding
  `pad_0x<off>[n]`; use `static_assert(__builtin_offsetof(...))` for deep offsets. Real names only for
  locals, parameters and helper inlines you introduce.
- Types from "types.h" (u8..u64, s8..s64, f32, f64). Headers are relative to include/.
- Readable code: real control flow, structs with named fields, no pointer arithmetic soup, no copied
  disassembly, no inline asm with instructions. Empty barriers are allowed when nothing else matches:
  `asm("" : "+r"(x))` or `asm volatile("" ::: "memory")`, each with a one-line comment above it saying what
  ordering it keeps. Never copy disassembly or decompiler output into src/.
- A file starts with a one- to three-line comment: which unit, what the function does, "Names and layouts
  are placeholders."
- Globals reached through a GOT slot: plain `extern` declarations of the target. Hidden (PC-relative)
  ones: `__attribute__((visibility("hidden"))) extern`.
- If the function you DEFINE is declared elsewhere (git grep), use that parameter list exactly.
- Write only your one target file (plus the draft/result files above). Never edit any other file.
