# Lean worker: decompile ONE function

You decompile one status-U function of dread-decomp (Metroid Dread, AArch64) into readable C++17 that compiles
byte-identically with the project's Clang 9.0.1. Your brief gives: the worktree, the address, the target
file pattern, and the attempt budget. Work only inside that worktree (`cd` there in every command).

Keep your context small. It is the main cost of this run.
- Do NOT read CLAUDE.md, docs/, the journal, or browse src/ broadly. Everything you need is here and in
  `build/lean/ctx/<addr>.txt`. Open at most 2 other files (a header named in ctx, or one matched sibling
  file from the same directory) and only if you need a type.
- Grade only with `.venv/Scripts/python tools/batch/lean/try.py <file>`. It prints a short diff. Never run
  tools/trymatch.py -v directly, check.py, verify.py, decl_check.py without --symbol, permute.py,
  equiv_test.py, gen_*.py, Ghidra, port builds or git commands (git grep is fine).
- Don't re-read files you wrote; don't print files.

## Steps

1. Read `build/lean/ctx/<addr>.txt` and `tools/batch/tips.md` (where a tip and the rules below disagree, the
   rules win).
2. Write the target file (replace `<What>` with one readable word for what the function does, e.g.
   `src/game/CFoo/CFoo_Reset_l3a7f10.cpp`). The function is at global scope, named `fn_<addr>`.
3. Run try.py. Revise from the diff and its KIND line (fix calls first, then control flow, then
   operations, then registers). Use the whole attempt budget from your brief: don't stop early on a low
   score, since one extra or missing instruction near the top shifts everything after it and can make a
   near-match score under 50%.
4. Declarations: before declaring a callee or global, `git grep -n "fn_XXXX\|g_unk_XXXX" -- include src | head -3`.
   Include the header if include/ declares it; otherwise copy an existing signature exactly (forward-declare
   its struct names, cast at the call site). If a file under src/ DEFINES the callee (a line `... fn_XXXX(...) {`),
   the definition's signature is the one to copy: your own struct name for its parameter or result is a second
   set of types for one function, and CI fails it. When it matches, run
   `.venv/Scripts/python tools/batch/lean/declfile.py <file>` and fix what it lists, then rerun try.py.
5. Result. Only a match stays in src/; the coordinator places everything else.
   - MATCH (and no RELOC line): keep the file. Outcome `match`.
   - The budget is spent and the last KIND line says `registers` (the same instructions in another order or
     with other registers): outcome `registers`. This is a good result, not a failure: a reordering search
     and a machine test take it from here, so do NOT spend attempts on register choice, and do not force it.
   - Anything else: outcome `differs` (or `skipped` if you could not start, with the reason as the blocker).
   For `registers`, `differs` and `skipped`: move your best version to `build/lean/drafts/<addr>.cpp`
   (overwrite) and delete the src file.
   Write `build/lean/results/<addr>.json`:
   `{"addr": "<addr>", "outcome": "match"|"registers"|"differs"|"skipped", "best": <0-100>, "attempts": <n>,
     "file": "<the src path you used>", "blocker": "<few words on what still differs>"}`
6. Reply with exactly one line: `<addr> <outcome> <best>%`.

## Readable first (this outranks a match)

The source must read as something a person wrote for what the function does. Do not add any of these only to
change what the compiler emits:
- an empty barrier (`asm("" : "+r"(x))`, `asm volatile("" ::: "memory")`), `volatile`, an empty `if`;
- an inlining attribute (`always_inline`, `flatten`, `noinline`);
- a union around a member, a single field stored through `__builtin_memcpy`/`__builtin_memset`, an offset
  cast `*(T*)((u8*)p + off)` in place of a field.

One of these usually stands for something real that is missing: a helper, a field's true type, two variables
merged into one. Look for that. If the function matches only with one of them, take it out, grade again and
report what the file then is (usually `registers`), naming the form in the blocker ("matches with flatten on
Foo"). Inline asm with instructions in it is never allowed.

## Conventions (CLAUDE.md, condensed)

- Unknown names: `fn_<addr>`, `g_unk_<addr>`, structs `Unk<Something>` with `field_0x<off>` and padding
  `pad_0x<off>[n]`; use `static_assert(__builtin_offsetof(...))` for deep offsets. Real names only for
  locals, parameters and helper inlines you introduce.
- Types from "types.h" (u8..u64, s8..s64, f32, f64). Headers are relative to include/.
- Real control flow, structs with named fields, no pointer arithmetic soup. Never copy disassembly or
  decompiler output into any file.
- A file starts with a one- to three-line comment: which unit, what the function does, "Names and layouts
  are placeholders."
- Globals reached through a GOT slot: plain `extern` declarations of the target, named after the target
  (`g_unk_<target address>`), never after the slot. Hidden (PC-relative) ones:
  `__attribute__((visibility("hidden"))) extern`.
- Every callee and global you name must be the one the original uses at that place (ctx names them). A RELOC
  line from try.py means one is not: fix it before anything else.
- If the function you DEFINE is declared elsewhere (git grep), use that parameter list exactly.
- Write only your one target file (plus the draft/result files above). Never edit any other file, a header
  included: parallel workers share them, and the coordinator moves shared declarations to headers afterwards.
