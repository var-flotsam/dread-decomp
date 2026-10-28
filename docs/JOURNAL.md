# Journal: session and batch log

History of the Phase 4 work, moved out of `docs/HANDOFF.md` on 2026-10-24 so that the handoff holds only
the current state. Append new entries at the top of the "Session log" (newest first); add each matching
finding worth reusing to the index. Nothing here is edited after the fact except to fix a link.

## Findings index

Matching findings worth reusing, by topic. Each line links to the entry that records it.

**String ids and pools**
- `UnkStrIdBase` with `CStrId`/`UnkStrId` on top; refcount as a union member so the decrement reloads: [work log item 0](#work-log-by-stream-through-2026-10-23), "Matching notes".
- Which pool a file names must be checked, not assumed (`check.py` now does): [Matching does not check relocated targets](#matching-does-not-check-relocated-targets).
- A move-assign re-reads the pool for the other's empty string; a file path id keeps the pool read: [batch 16](#slice-1-near-copies-batch-16).
- Lazy id check must be a pointer compare (`data == &pool->empty`): [Inventory string and projectile models](#inventory-string-and-projectile-models).

**Vectors, arrays, lists**
- `UnkVector`: trivially copyable elements by `memcpy`, others moved one by one, 32-bit counter; `Insert`'s append path is `PushBack`'s shape: work log item 0.
- `Reserve` must be `always_inline` or copies become 4-byte tail calls: [batch 12](#slice-1-near-copies-batch-12).
- A list passed as a reference parameter lets the step load before the first capacity test; each branch pushes its own last value: [Hash lists by height](#hash-lists-by-height).
- List fields as union members so they reload after node stores; `alignas(16)` for the stack slot: [Flag lists](#flag-lists).
- An array of non-trivially-destructible members becomes a loop: declare separate members: [Link groups](#link-groups).

**Iterators and iterator values**
- Elements copied member by member need a user-written move constructor; `flatten` wrappers reproduce inlined template moves: [batch 15](#slice-1-near-copies-batch-15).
- 64-bit keys only match through a key struct with its own `operator!=`: [batch 14](#slice-1-near-copies-batch-14).
- Counted references copy as "null, then assign" with the count in a union: [batch 16](#slice-1-near-copies-batch-16).

**libc++ sorts**
- Sort through a minimal class iterator (`UnkIter`) to fix pointer walks, inlining and unsigned halving; internal comparators inline `__insertion_sort_3`: [Sort through a class iterator](#sort-through-a-class-iterator) and [Above 2 KB](#above-2-kb-outside-the-six-hand-slices-srcgame_s7-2026-10-21).

**Copy constructors and assignment (record layouts)**
- A defaulted `operator=` shows the layout: memcpy runs are plain fields, interleaved copies are wrapper types, an uncopied 8-byte gap is a vptr: [Copy-assignment of property records](#copy-assignment-of-property-records).
- Vptr stores of a middle base survive only when an untyped copy follows: same entry.

**Static initialisers and tables**
- Each vector of ids built from a list sits in its own immediately called lambda; lists of one length in a unit need one element type; packed 4-byte-aligned struct when a u32 precedes the ids: [Above 2 KB](#above-2-kb-outside-the-six-hand-slices-srcgame_s7-2026-10-21).
- Header ids built "sources first", enum converters local to the unit (`visibility("hidden")`): [batch 13](#slice-1-static-initialisers-batch-13).
- Giant table functions are generated (`tools/gen_tables.py`); the id table's destructor still differs in register allocation: [Giant functions](#giant-functions-probe-2026-10-21-cloud-thread-biggest-functions-probe).

**Reflection attributes and registration**
- `AddField` takes the name by value; `Add` by const reference as a {type, ptr} temporary; struct-typed defaults and `CVector2D` globals copied member by member: [Field registrations the reflection parser stopped at](#field-registrations-the-reflection-parser-stopped-at).
- Collection types are globals built by static initialisers, all reached through the GOT: [Collection-type registration](#collection-type-registration).

**Transform nodes and math**
- `UnkXform` layout (4x4 matrix, angle 0x40, scale 0x4c, flags 0x58); the same memory viewed through two struct paths defeats alias analysis, so use one view per function: [hubs batch 2](#slice-1-hubs-batch-2).
- Taking both ids by reference in a comparison keeps the owner load from being hoisted; a loop over a matrix becomes memcpy, so write `UnkCopyMatrix` out: [hubs batch 3](#slice-1-hubs-batch-3).

**Switches and block layout**
- Write switch cases in the order of their bodies in the binary to get Clang's compare tree: [Above 2 KB](#above-2-kb-outside-the-six-hand-slices-srcgame_s7-2026-10-21) (music volume overrides).
- `__builtin_expect`, `always_inline` and early returns do not change register allocation in long inlined destructor runs: [Exact copies of matched functions](#exact-copies-of-matched-functions).

**Lifter and generators**
- Alias, sort and copy rewrite rounds; "the generic lifter is near its limit": work log item 1.
- Semantic mode and its P-code oracle; four flat-mode bugs byte identity hid; the oracle's reach (mutation test): [port M1 step 3](#session-log-newest-first).
- Parallel agents in one checkout: file-disjoint steps, `trymatch.py --obj-dir`, one `check.py` at the end: [sixteenth session](#session-log-newest-first).
- Byte-mode argument pass-through (a lift names the registers its callee reads and it still holds); capstone 5.0.7's wrong register access for `cmp`/`tst` and shift/extend aliases: [lifter pass-through](#session-log-newest-first).
- The P-code oracle's inputs and rules: mutations placed where a word was read, a compare-constant sweep, out-parameter bounds, runs cut off, unknown callees; each rule's measured trade-off: [lifter oracle inputs](#session-log-newest-first).
- Multi-register results (x0:x1, s0-s3) and the result register, read off what callers do right after a call; a void lift of a function whose result is read; every `U` function's outcome: [lifter gaps round 3](#session-log-newest-first).

**Declarations and calling conventions**
- A caller declared with fewer parameters can rely on an argument register the caller left set (six callers of `fn_7100216978` passed the vector in `x1` unawares); declaring the real signature keeps the bytes: [sixteenth session](#session-log-newest-first).
- A union-typed scoped reference adds a reload in its destructor; cast at the call instead. A cast that lets Clang tail-merge two paths can be undone by an anonymous-union member: [sixteenth session](#session-log-newest-first).
- A header type name must not be one that other files define locally (`UnkComponent` had 41 local definitions): the port links one program; a hidden-visibility attribute stays load-bearing in the header: [hub batch 6](#session-log-newest-first).
- A global the generated sources declare keeps their type in its header (`CType` for reflected type globals), or every generated includer stops compiling; a typed call that changes a minor function's code can go through a function-pointer view of the header's declaration: [hub batch 5](#session-log-newest-first).
- On the x86-64 port a mismatched declaration is a register mismatch only across argument classes (`float` vs integer or pointer); count and pointee differences are harmless for address-taking references: [sixteenth session](#session-log-newest-first).
- Clone kinds and the census method; families tried and left: work log items 0 and 2.
- A function passed as a template argument keeps its relocation target only if the template takes the function's own type (no inline wrapper): [hub batch 3](#session-log-newest-first).
- A callee without an address (`__cxa_atexit`) is an unchecked relocation; spell it by its `fn_` name (`fn_7100000250`). Compare the relocation totals before and after a header refactor: [TU helper fold](#session-log-newest-first).
- Store order between two distinct globals is the original's per unit, steered by source order: keep both orders as named helpers rather than one: [TU helper fold](#session-log-newest-first).
- A field's pointer type can be load-bearing (alias info): a hash kept in a `void*` word stays `void*`, cast at the store: [hub batch 4](#session-log-newest-first).
- Before an agent round, `baseline.py save --all` (every hand-written source, 2 minutes) is the cheapest complete evidence: [hub batch 4](#session-log-newest-first).
- A keyed array's lookup: `Find` returns `&values[i]` or `&values[count]`, the caller compares with `&values[count]`, ids compared as raw `u64`; a `u8` flag with `__builtin_expect(f & 1, 1)` gives `tbz`: [boot hubs, round 1](#session-log-newest-first).
- Clang 9 ignores an `asm` label on a constructor or destructor: alias the C1/D1 symbols to `fn_<addr>` under `__clang_major__ <= 9`: [boot hubs, round 1](#session-log-newest-first).
- A const global read as words: reading it through a `reinterpret_cast<const u32*>` of the const object kept the original's reloads, with no barrier: [boot hubs, round 2](#session-log-newest-first).
- An out-of-line hash-map or vector initialiser: a nested map-head struct makes the stores relative to the map base; an `asm("" : "+r"(p))` on a pointer to a far field hoists its `add` as the original does: [boot hubs, round 2](#session-log-newest-first).
- An agent's "behaviour identical" on a 10-45% minor is worth an adversarial check (decompile, `abi_facts.py`, the `-v` diff): round 2's ten all held, and the review found a `void` declaration that really returns its first argument: [boot hubs, round 2](#session-log-newest-first).
- When the callee is in the same unit the original compiler infers facts about it (nocapture, a by-value argument left alone, a constant result) that change the caller; `__attribute__((noescape))`, `inline __attribute__((noinline))` and definitions kept in address order (dead-argument elimination walks them in order) reproduce some: [port M1 boot-match](#session-log-newest-first).
- Reassociate ranks commutative float operands by IR order: loading values into locals first, or `Merge(a, b)` helpers, fix `fadd`/`orr` operand order; inlined templates are optimised before inlining and flip it again: same entry.
- A port hazard in matching code: AArch64 `udiv` by zero gives 0, x86-64 traps; guard the division only off `__aarch64__` so the Switch bytes stay: [boot hubs, round 2](#session-log-newest-first).

**Port: shaders**
- `.bshdat` layout, the GLSL in each stage's debug section, and the two rewrites glslang needs (block `location` to the compiler's binding, `GL_NV_explicit_typecast`): [shader packages parsed](#session-log-newest-first).
- NVN's compiler numbers a stage's active uniform blocks 0..n-1 in declaration order, whatever the source says; take bindings from the reflection, never from the source: same entry.
- `nvnProgramSetShaders` gets each stage's control section by pointer; key stages by FNV-1a 64 over its first 2,048 bytes. NVN block sizes are the SPIR-V span rounded to 16. The engine selects variants itself (`fn_71001b0f24`): [port M2](#session-log-newest-first).
- A source fix for one shader is a regular expression over the construct, never the shipped lines (those are game data): same entry.
- Vulkan rejects fragment inputs the vertex stage does not write (GL allows unread ones): drop those NVN found inactive and the vertex stage does not cover, never all inactive ones (code still names them); enable `maintenance4` for wider outputs: [binding layer against Vulkan](#session-log-newest-first).
- A check that compares generated data with the generator's own claims passes vacuously; compare with the artefact (the SPIR-V's decorations) and prove it fails on damaged input: same entry.
- What an NVN number of the drawing side means: find the engine wrapper that converts its own reflected enumeration (`msapi::api::types::*`, `idx::*`; value names in mercury-engine-data-structures' `dread_types.json`) to it; the game's deferred shaders give the depth range and window origin: [port M2 drawing](#session-log-newest-first).
- NVN's bound state is the queue's, not a command buffer's: the engine rebinds a state only when it changed and alternates two command buffers: same entry.
- The NVN globals `g_unk_716e29xxxx` are named by the loader's GOT slots and their relative relocations (`data/nvn_procs.csv`); count GOT-slot call sites with a short window, not adjacent pairs; never write into a game-owned NVN object's storage: [NVN shim's start-up path](#session-log-newest-first).

**Port: runtime and loader**
- Another Dread project's addresses map to ours by one constant (the recomp: ours = theirs - 0x8005000 + 0x7100000000); the game's PC-side problems it found (a video wait under a lock, per-texel texture uploads, a deserializer growing 10 at a time, frame pacing): [Dread recomp findings](#session-log-newest-first).
- A function-local static of an ordinary (non-inline) function is a local symbol that its object reaches section-relative: no link step can redirect it. Make the function inline on the port (`inline __attribute__((used))`) so the static is weak and `--defsym` binds it: [port M1 round 4](#session-log-newest-first).
- Clang 17 stopped packing a packed struct's non-POD members (GCC's rule), so a newer Clang lays out the matching sources differently; `-fclang-abi-compat=9.0` restores the matching compiler's layouts: same entry.
- A bounds test `cmp #N; b.hs` before a table load is a `switch` turned into a lookup table (an array index gives `cmp #N-1; b.hi`): same entry.
- The game's static initialisers are plain `fn_<addr>` functions that `.init_array` calls; a port that links them as code runs none of them until something calls the table (`dread_port_initialisers`): [port M1 round 3](#session-log-newest-first).
- The game defines its own `operator new`/`delete` and `malloc` family over its allocator: route them by address to the game's code and the C++ names to a runtime heap that frees each pointer where it came from: same entry.
- A constructor or destructor under its mangled C1/D1 name has no address in its name; the byte-matching callers' relocation targets give it: same entry.
- The engine's thread object puts `ThreadType` at +0, light events at +0x1c0 and +0x1cc: the SDK's thread block is 0x1c0 bytes, a light event 12; event clear mode 1 is auto clear: [port M1 step 4](#session-log-newest-first).
- main's 146,684 dynamic relocations: every slot in `.data`, 128,576 relative ones into `.text` (vtables, callbacks), every symbol relocation an import; the game's vtables are used as they are, so the port needs the Itanium C++ ABI (not MSVC): same entry.
- A freestanding core builds into a Windows DLL with clang, `lld-link /noentry /nodefaultlib` and a `kernel32.lib` from `llvm-dlltool`: no SDK needed to test it from Python: same entry.
- The SDK runs the application's `nninitStartup` before the static initialisers (Dread's sets up the memory heap and the default allocator), so the port's loader calls it right after mapping the image: [port M1 boot-match](#session-log-newest-first).
- A game global a matched file defines itself (a `static`, a function-local static) is the executable's on the port, while lifted code and `g_unk_<addr>` users reach the image's: keep such state in the image off `__aarch64__`: same entry.
- A matched call to an import goes through a PLT stub, so harvesting addresses from matched code must skip those (`actual_name` set) or the import becomes an alias of itself: same entry.
- Newer Clang (the port's 18) calls an `asm` label on a constructor or destructor declaration, Clang 9 ignores it: a label binds a member to its `fn_<addr>` on the port with the Switch bytes unchanged: same entry.
- Clang 18 can hang on an `asm` label on an inline constructor: alias the complete-object symbol under `__clang_major__ <= 9` and define a function that runs the constructor otherwise: [port M1 round 2](#session-log-newest-first).
- Byte-identical lifts drop argument registers at call sites (the file's `namespace callee` declaration names fewer than the callee reads) as well as in definitions; AArch64 keeps them, x86-64 loses them. A lifted constructor call after an allocation is the common case: same entry.
- capstone 5.0.7's register access is wrong for aliases: `cmp`/`cmn`/`tst` write their first operand instead of reading it, and `fmov #imm`, `lsl #imm` and `sxtw` read their destination. Anything built on `regs_access()` (`tools/abi_facts.py`) under-counts arguments read first by a compare: same entry.
- The boot's run-time roots are function pointers (`nnMain`'s main, per-frame and shutdown callbacks, main's stored callbacks), and some traps are reached only through pointers: a static worklist from the roots is a lower bound, `DREAD_TRAP=log` gives the order: same entry.
- Function-local statics of header-inline functions are weak symbols in the executable on the port, while lifted code uses the image's: a split object unless the symbol is bound to the image address: same entry.
- x86-64 assumes 16-byte alignment for any global array of 16 bytes or more, even one only declared, while the image keeps AArch64 alignments: declare such image arrays unsized, or Clang 18 may store into them with `movdqa` (a SIGSEGV at address 0): [port M1 round 6](#session-log-newest-first).
- Integer and float arguments take separate register sequences on both AArch64 and x86-64: `(int, float, bool)` and `(ptr, ptr, float)` are the same call: same entry.
- A struct-returning function (x8 on AArch64) called through a pointer from lifted code needs its hidden result pointer passed explicitly on x86-64 (rdi), or every argument shifts by one: same entry.
- Several templates can carry one address's tag (`refl_GetPrimitiveType<G>` and its inline helper `refl_MakePrimitiveType<G>`, which Clang 18 keeps out of line); an alias to one that returns a class by value puts its hidden result pointer in the caller's rdi and writes the object over the caller's frame. A crash whose return address is a function's start, with a type's hook table on the stack, is the sign: [port M1 round 5](#session-log-newest-first).

## Session log (newest first)

**Lifter: the boot trace's instruction gaps (2026-10-28, branch `claude/project-thread-zgkpk3`, after #250).**
The recomp's boot trace names 37 unmatched functions the boot runs that no lifted set accepts; 11 were
the lifter's (diagnosed on the PC, measured there). All semantic mode:
- **Jump table reached from two bounds checks** (`71001af824`, `71008e7708`): an earlier `cmp; b.ls`
  branches straight to the table's `adr`, which the switch replaces, so its label was never emitted and
  the lift did not compile. Labels are now emitted on the table's skipped instructions.
- **Small integers loaded into a float register** (`7100027a7c`, `7100027d08`, `71001bd634`,
  `71001bd644`): `ldr h|b` then `ucvtf|scvtf s` was taken only when adjacent. Now other instructions not
  touching the register may sit in between, and `sshll v.4s, v.4h, #0` (after `sshll v.8h, v.8b, #0`
  for a byte) makes it an s16 or s8.
- **Variadic save area** (`710007cb4c`, `710007d788`, `71010c0270`, `71010c0b40`): `stp q<n>, q<n+1>`
  of q0-q7 not written before stores each register's double (the low 64 bits, what `va_arg` reads);
  `setup_frame` counts a q access as 16 bytes.
- **Lane arithmetic** (`71000f88b8`): `fadd|fsub|fmul|fdiv v.2s|4s` on known lanes, and `mov v.d[1],
  v.d[0]`; a scalar `ldr s<n>` before a lane load into v<n> gives lane 0 (the others zero).
- **`rev16`** (`710007d788`, past its variadic save): the bytes of each halfword swapped.
- **`faddp`** (`71000f88b8`, next): pairwise sums of known lanes, vector and scalar forms.
- **A virtual forwarder's pass-through registers** (the boot's next stop after #250): `fn_71000e8fa8`
  calls slot 0x28 with seven arguments; the target `fn_7100087ecc` (lifted with four) forwards to its
  items' slot 0x40 as `(x0, x1 + 8, x2, x3 & 1)` and leaves x4-x6 as they came, and x4 is the
  allocator of a buffer three calls down. With no direct caller `fwd` had no evidence, so on x86-64
  x4-x6 were lost. abi_facts' new `open` (an indirect call and no direct caller) counts every integer
  argument register as read by an indirect call (cache version 11), and semantic mode passes them on.
- `end` is not a function to lift: both symbols of that name lack a size in the image's symbol table
  (a 16-byte entry at `7101238d80`, and the image's end marker), which the size cap reads as too big.

**Lifter: vectors built lane by lane (2026-10-28, branch `claude/project-thread-zgkpk3`, after #244).**
The movi and ld1 rejects left after #244 build a vector from 32-bit floats and store it whole: `movi
v3.2d, #0`, then `mov v3.s[0..2], v<m>.s[0]`, then `str q3, [sp]` (a Vector3 padded to 16 bytes, passed
by address: `71000f3c7c`, `71000f3ce4`), and `ld1 {v0.s}[0]` then `ld1 {v0.s}[1]` from another address
(`71000fa05c`; `fn_71000f7800` too, hand-written since). `State.lanes` keeps a register's four lanes
(semantic mode; dropped at a join, a label and any other write), `Lifter.lane_step` reads the lane
forms and stores `str q` (four lanes) or `str d` (two) lane by lane; `float_widths` leaves those
stores out. Also: `port/runtime/src/heap.cpp` includes `<cstddef>` for `std::max_align_t` (the
Linux image's clang++ with libstdc++ refused it).
- **An indirect call given this function's own result address in x8** (the boot's next stop after #244:
  `fn_710001ab24` calls a resource descriptor's callback at +0x40, `fn_71001b2e4c`, which builds an
  `UnkResult`). Semantic mode made a struct-returning indirect call only when x8 held a local's
  address, so with x8 set back to the incoming result address the call named neither, and on x86-64
  every argument moved up a register (the hidden result pointer is rdi). `Lifter.ret_ptrs` and
  `FlatLifter.ret_regs` now follow copies of that address, and x8 set to one before a `blr` makes the
  call build its result there, as for a local.
  The same call made as a tail call (`mov x8, x20; br x2` in `fn_710001ab24`) now returns the
  callee's result, built in place as for `blr` and followed by `return ret;` (refused if x8 holds
  anything else). A first try emitted `return ((UnkResult (*)(...))(ptr))(...)`; beside the other
  paths' `return ret;` that cost Clang's NRVO, ret became a stack local, and the oracle rejected
  `710001ab24` and `710110f1c8` (their other paths passed or wrote the local, not the caller's buffer).

**Boot functions the lifter rejects, by hand, batch 2 (2026-10-28, branch `claude/project-thread-xllbvf`, second PR).**
Ten more from the boot-slice list and the boot's next trap, `fn_710006f390` (the vibration thread's loop). All of them
are written from the original's behaviour, checked against its disassembly, and marked `NON_MATCHING(major)`:
`fn_71000f01d0` (binding a reflected call's leading arguments, `src/base/reflection/UnkReflCallArgs.cpp`),
`fn_710010d9fc` (building the box tree, next to its sort in `src/game_s7/fn_710010e470.cpp`), `fn_7100ee6d00` and
`fn_7100ee8044` (what a change of sub-area loads and unloads), `fn_7100fd6f74` (whether a music request is
already playing), `fn_7100ed5a04` (the shots' audio presets), `fn_7100e0fc00` (the loading screen's tip labels),
`fn_7100ebc7f0` (a minimap segment as a quad) and `fn_710006f390` (`src/base/input/UnkVibrationThread.cpp`). The
loop reads the SDK data import `nn::hid::VibrationNode::DefaultVibrationSampleInterval` through its symbol name,
so the port's runtime has to define it (5 ms on the console). The boot reaches none of the five largest
functions still open (`fn_7100223c2c`, `fn_7100ebcd70`, `fn_71001b83f8`, `fn_7100ea1d78` and `fn_7100e9e164`), so they
are left until a trap needs them. `fn_710007d788` waits for the lifter's generic fix for its instruction gap.
Finding: `tools/abi_facts.py` gives the exact registers a callee reads (`AbiFacts(...).facts(addr).args`).
A callee that skips x0 or x2 is called with null in those places, as the existing declarations have them.

**What the Dread recomp found about the game on PC (2026-10-28, branch `claude/project-thread-lrmp48`).** At the
owner's request, the public [Metroid-Dread-Recomp](https://github.com/yozander2009-stack/Metroid-Dread-Recomp)
(yozander2009-stack, head `c5dfcf0`; SwitchRecomp by MZ Labs, `docs/references.md`) was read for what it knows about
the game. It is a static recompilation of the same 2.1.0 build: every ARM64 function translated to C and compiled
for the PC, on a high-level emulation of the console (kernel, services, a Maxwell GPU over OpenGL or Vulkan). Its
functions are named `F_<addr>` for a `main` based at `0x8005000`, so **ours = theirs - 0x8005000 + 0x7100000000**
(checked: function starts agree, and its `F_806B91C` is our `nn::os::LockMutex` thunk). Nothing was copied: these
are facts read from its comments and patch descriptions, mapped to our addresses. Its patch set edits generated
code that is not in its repository, so a few places are known only by what they do.
- **Video completion wait under a lock** (`fn_7100224de4`): when bit 0 of its argument is set it waits with a
  timeout of 50 (`fn_71002202d8`) while the caller `fn_710021f858` still holds the mutex at `+0x960` of the same
  object. The recomp drops that mutex around the wait (`fn_7100066920` to unlock, `nn::os::LockMutex` to lock
  again) or video playback stalls. A wait inside the multimedia module (`subsdk0`), reached from `fn_7100226b70`, has the
  same pattern with a global rendering lock (the object whose pointer is at `0x7101cc5d58`), released and taken
  again with `fn_7100159be0` / `fn_7100159bd0` (lock order kept: outer before inner). Relevant once `nn::movie` runs.
- **Per-texel texture uploads** (`fn_71001e193c`, return address `0x71001e1e38`): a dirty rectangle of a
  1024-wide RGBA8 texture (format `0x25`) is written with one `nvnTextureWriteTexels` per texel, through the NVN
  table slot at `0x7101cc5300`. The recomp batches it by row. The shim's texel write should be cheap per call,
  or batch the same way.
- **Vector growth in a deserializer**: an array the resource deserializer appends to grows by 10 elements at a
  time, so big loads copy quadratically. Not located in our addresses yet (the recomp names it by line of its
  generated code).
- **Frame pacing**: the loading-screen render thread (elevators, trains, area loads) sleeps 30 ms per frame;
  cinematics ask the present-interval setter `fn_7100159420` for interval 2 (30 FPS), and `fn_710119a320` is the
  check that a 60 FPS cutscene patch makes answer yes. The FXAA-enabled flag is stored at renderer `+0xc00` by
  `fn_710014e86c` (`0x710014e888`). The recomp keeps the 60 Hz tick and changes these only as opt-in settings.
- **Cutscene skip**: `fn_71010692a0` is the game's can-skip check and `fn_71011986e0` its seen-before lookup;
  `fn_7100ec9a24` is the intro page controller's finish, called from `fn_7100ec9e38`.
- The game's inflate (zlib) and `memcpy` are in the SDK module, which our port replaces anyway; the recomp swaps
  them for native ones for speed.

Findings:
- Another project's function addresses map to ours by one constant when it loads `main` elsewhere; check it
  against a few function starts and an import thunk before trusting it.
- A recomp's patch descriptions name behaviour, and that is fair to record with our addresses. Its generated code,
  and patch anchors containing it, are game-derived: read for the address only, never copied.

**Port M1 round 6: through the boot milestones, and the NVN shim linked (2026-10-28, branch `claude/project-thread-3x0lf8`).**
Boots of main with the hand-writing thread's branch (`fn_71000f7800`, the vibration thread) and the lifter's sets
(`build/port/lifted_all` with `lifted_insn3` merged over it), each stop's cause found before its fix:
- **The null read under `fn_7100100804`** was fallout: it locks the sound system's mutex at +0x400568, which
  `fn_71000f7800` sets up. With that function hand-written, the set-up checks the audio output first. The new
  `nn::audio` runtime (`port/runtime/src/audio.cpp`) fails `OpenDefaultAudioOut`, so the game sets its sound-off flag and
  stays silent until M4.
- **`fn_710006f390`'s data import.** The vibration thread reads `nn::hid::VibrationNode::DefaultVibrationSampleInterval`
  (5 ms). `port/runtime/src/hid.cpp` defines it, and `tools/port_link.py`'s import table now takes the runtime's data
  definitions as well as its functions.
- **`nvnBootstrapLoader`.** `tools/port_link.py --nvn` compiles the NVN shim into the executable and links the Vulkan
  loader. The Vulkan loader's `vk*` names are left to it, not given residue traps. `tools/boot_distance.py --nvn` runs
  it in the Vulkan image (which needed rebuilding for gdb). Lavapipe was chosen over a surface because it needs nothing
  new and the shim's checks already pass on it. The shim creates its device (llvmpipe, Vulkan 1.4).
- **A `movdqa` to an 8-aligned array.** x86-64 assumes a global array of 16 bytes or more is 16-byte aligned, even an
  `extern` one, and Clang 18 vectorised `fn_7100161520`'s stores into `unk_716e2aadf8` (0x...df8). The image's arrays
  have AArch64's alignment. Ten declarations of image arrays at addresses that aren't 16-aligned are now unsized
  (`T name[/* N; ... */]`), which drops the assumption. `tools/check.py` is unchanged. Two range-for loops became counted ones. `g_unk_71726c2888[2]` stays
  sized, because unsized it changed `fn_71003f3778`'s bytes.
- **Two "mismatched call" traps that weren't.** A declaration `(int, float, bool)` against a lift's `(ptr, ptr, float)`
  lands in the same registers on both CPUs, since integer and float arguments are numbered separately.
  `signature_class` now compares each class's own sequence when the positions differ and everything fits in registers.
  175 aliases are affected.

Result (`boot_distance.py --nvn`, merged lifts): all 8 milestones, 0 traps, 297 lifts ran. The stop is in the render
resource loader. The lifted `fn_710001ab24` calls a resource callback through a pointer (`fn_71001b2e4c`, which returns
`UnkResult` through x8) and passes x0/x1 but not x8. On x86-64 the hidden result pointer is rdi, so the callee's
arguments shift and `fn_7100020320` reads a string id at address 7. That is the lifter's to fix (indirect calls
after an x8 store). Previous stops of this round, in order: `fn_71000f7800` → `nvnBootstrapLoader` → `fn_7100161520`'s
alignment fault → `fn_71001b27ac` (lifter `rev`, accepted in `lifted_insn3`) → this.

**Lifter: retry for cut-off runs, no float result from a void function, indirect calls with stack
arguments (2026-10-28, branch `claude/project-thread-zgkpk3`).** Fixes from the port's current stop and
from the hand-writing thread's reading of the 13 boot functions #236 refuses. Written in a cloud
session without the dump and measured on the PC: the boot slice (2,329 functions) goes from 2,266
accepted (main's manifest) to 2,283, none lost; the retry alone gains `71001ea42c`, `passed_on` the six
s0 voids (`710033187c`, `7100ee4d34`, `7100d8d854`, `7100d8db30`, `7100e10150`, `7100ee5dcc`) and the
three x0 ones, the stack arguments the 7 below. Oracle rejections go from 25 to 17, "result s0/d0" verdicts from 10 to 3.
- **Long loops.** `fn_71002584a0` (2.8 KB, the function the port traps in before the bad string id)
  was inconclusive: "both runs were cut off" on every input. They were cut by events, not steps (a
  loop past `MAX_EVENTS`, 256 calls; the finished inputs end in 20-43 steps). `pcode_emu.Machine.scale`
  now multiplies the step, event and snapshot limits of one run, and `Oracle.check` reruns up to
  `RETRY_INPUTS` (4) cut-off inputs, original and lift, with `RETRY_SCALE` (8) times the limits when
  no input agreed. Two cut-off runs still decide nothing; the "lift ran out of steps" rule uses the
  lift's own limit.
  `fn_71002584a0` itself stays inconclusive: its loop calls through a table read from the input
  (1,203 distinct targets in 2,048 calls at 8x), so made-up memory never ends it; it needs a
  hand-written source.
- **Float results that are not there.** Nine of the 13 refused functions are void (the hand-writing
  thread, from Ghidra), yet `result_main` gave s0 for all nine, so #236 refused their void lifts. A
  trace of its evidence on the PC: no caller reads s0 itself. All of it is "passed on to the next
  call", where the next callee is variadic (its prologue saves every argument register, so its
  `args` holds all of v0-v7) or a callee none of whose direct callers ever sets s0 (`71004e4a38`);
  the rest reaches them through tail calls and returns from such callers. `AbiFacts.passed_on` drops
  s0 from that evidence in both cases (`SHAPE_VERSION` 4). Semantic mode only; byte mode does not
  read `result_main`. A first try, a callee-side check of whether the function writes s0 itself,
  could not tell a void function doing float arithmetic in s0 from one returning it, and was dropped.
  The three x0-side cases (`7100eea4c8`, `7100eea618`, `710109c9e0`) are hand-written in #240; the
  lifter now accepts them as well (a source in `src/` wins).
- **Indirect calls with stack arguments** (105 functions refused, 7 on the boot path). An indirect
  callee has no facts, so `Lifter.stack_args_before` now counts the 8-byte slots from `[sp]` up that
  the straight-line code before the `blr` stores, and `callee_need("")` passes them as a direct
  callee's are (all eight integers, then `st<i>`). A local stored just above the arguments is passed
  too; a callee taking fewer ignores it. Semantic mode only. Of the 105, 98 are accepted (all 7 on the
  boot path: `71000f17c8`, `71001ef898`, `710027c298`, `7100d58cfc`, `7100ea39a4`, `7100ee5bc4`,
  `71011f8bdc`); 3 stop at "stack arguments outside the frame" (`7100148600`, `7100211590`,
  `710074fd08`), 3 are rejected (two pass a d register where the callee reads it as s, or the reverse:
  `71003e69e4`, `7100656f3c`; `710096cdec`) and one is inconclusive (`71008626bc`).
- **What the boot reaches through pointers.** The boot's next trap after the string id fix,
  `fn_71000f7800` (`CSoundSystemATK`), is outside the boot slice: only an indirect call reaches it.
  `port_lift.py --slice pointers` adds what the slice's functions reach through the addresses they
  take (`pointed_to`: an adrp with an add or a GOT load naming a function or a vtable), to a fixed
  point: 12,404 functions against the boot slice's 2,226 (3 s). `fn_71000f7800` is in it. Of the
  10,178 added, the slice-all set `lifted_r5a` accepts 9,826 already; 86 more are indirect calls with
  stack arguments (from before the fix above), 75 oracle rejections, 58 inconclusive, 50
  undecodable. `--slice boot` stays the default and the progress figure; `pointers` is an upper
  bound (a vtable taken is not a method called).
- **Instructions semantic mode now lifts**, from the forms the pointer-reached rejects stop at (one
  per function): LLVM's trap word `0xe7ffdefe` (unallocated, so capstone stopped: 49 four-byte stubs
  and one function's last word) as `__builtin_trap()`; a GOT load whose slot names a symbol rather
  than a RELATIVE address (`fn_71000f7800`, the boot's next trap, and `710006f390`), as the module's
  own definition or the import by name; `fmov s, w` of an inf or NaN constant; `movi v<n>.2d, #0`;
  `rev`; `ldpsw`; `ld1 {v<n>.s}[0]` (as s<n>); `str q<n>` of a zeroed register. Measured on the 79
  functions: fmov 8/8, rev, ldpsw and the function ending in the trap word accepted. The oracle now
  ends a run at the trap word or a `brk` (`pcode_emu`, end "trap"), for the 49 stubs, and resolves a
  lift's data import to the value the original reads from its GOT slot (`port_lift`, GLOB_DAT and
  ABS64 symbols). The port runtime defines `nn::util::detail::Float1Divided2Pi` (1/(2 pi)), which
  `fn_71000f7800` reads. Its other data import seen, `nn::hid::VibrationNode::
  DefaultVibrationSampleInterval` (`710006f390`), stays undefined: its type and value are not known.
  Still refused: `ldr b/h` not followed by a conversion (13), `adr` (9, local code addresses),
  `stp q` (7, variadic saves), far `cbz` (2).
- Tool tests: `tools/tests/test_abi_facts.py` (result_main on hand-assembled callers) and
  `tools/tests/test_pcode_emu.py` (the per-run limits in `compare`), `tools/tests/test_port_lift.py`
  (`pointed_to`). `progress.py --handoff` now writes LF on Windows too (it wrote CRLF, a whole-file diff).

**Boot functions the lifter rejects, by hand, batch 1 (2026-10-28, branch `claude/project-thread-xllbvf`).** The
boot-slice manifest (`port_lift.py --slice boot`, lifts of 2026-10-28) left 40 functions outside the accepted set,
the seven indirect calls with stack arguments and the string-id crash aside. Four of them (`fn_71000763c0`,
`fn_7100d7ce40`, `fn_7100b8deec`, `fn_7100b8dff0`) were already hand-written in #238, so 36 were open. This batch
covers 22 of them, plus the new boot trap `fn_71000f7800`:
- `O`: the eight Lua bindings that push a manager's setting (`src/script/lua_bind.cpp`; their `src/lifted`
  copies lost arguments on x86-64), `fn_7100219ef4` (render manager), `fn_710019c198` (the screen's corners in
  clip space, `src/engine/UnkScreenCorners.cpp`), `fn_71001ba864` (a view aimed at a point), `fn_71011b250c`
  (a GUI pooled list's constructor), `fn_7100e3d8a8` (a minimap rectangle's dashed diagonal),
  `fn_7100fd51a0` (the music manager's tunables file, `src/game/sound/CMusicManagerFile.cpp`) and
  `fn_7100fc7920` (the scenario load request, `src/game/UnkScenarioLoadRequest.cpp`).
- `m`: `fn_71000ffa68` (a sound's stop command), `fn_7100d668d4` (the polygon collection's type-8 leaf
  visitor), `fn_7100070a28` (the vibration player's update, `src/base/input/UnkRumblePlayer.cpp`),
  `fn_7100ee2ebc` and `fn_71011a224c` (the sub-area manager's and the scenario's collision file loads),
  `fn_710086697c` (a collider group from the collision file) and `fn_71001a6644` (reading a material).
- `M`: `fn_71000f7800`, the sound system's set-up (3,632 bytes, `src/base/snd/CSoundSystemATK.cpp`). It is
  not in the boot-slice worklist (nothing reaches it by a direct call), but it is where the boot traps after
  round 5. It is written from the original's behaviour and checked call by call against the disassembly, not
  byte-matched. On the port it turns sound off cleanly: it sets `g_unk_7108b6ab60` and returns false either when
  `nn::audio::OpenDefaultAudioOut` fails or when the silent `nn::atk` stand-in's `fn_710120ade0` returns 0.
  The listener's direction is `nn::util::SinCosEst(0)` inlined, written as its exact result.

Findings:
- A `RELOC` line's "original" address is the target the original's GOT slot or page holds, not a symbol and
  an offset: the `+0x..` after it is the instruction's offset. Name the global after that address.
- A bool read for a variadic push compiles to a plain `ldrb` only through a `bool` lvalue; a `u8` read is
  normalised with `cmp`/`cset`. The helper must not be a template over the owner's type (one instantiation
  kept the normalisation).
- A parameter tested with `tbz w1, #0` and nothing else is a `bool`; with `u32 & 1` Clang inverted the branch
  layout (`fn_710019c198`).
- Two identical branch bodies are hoisted by SimplifyCFG unless each branch has its own local; the slots are
  merged again by stack colouring (`fn_71000ffa68`).
- The order of member destruction decides where an inlined list destructor lands: give the list its own
  destructor and let the entry's be empty (`fn_7100fc7920`).
- A callee that does not read x0 (`tools/abi_facts.py` lists the registers it reads) still takes a first
  parameter in the existing declarations; passing null there changes nothing but the bytes.
- `!(a - e <= b && a + e >= b)` and the same test written the other way round give the same compare chain
  with the operands swapped; `fmax` (not `fmaxnm`) comes from `!(x <= c) ? x : c`.

**Port M1 round 5: the bad string id was a wrong alias (2026-10-28, branch `claude/project-thread-3x0lf8`).** With #240's hand-written `fn_71002584a0` the boot no longer traps there, but it still crashed in the same place, so that function was not the cause. The crash is in the walk over a class's field records (`fn_7100096f30` → lifted `fn_71000e7420` → the attribute handler `fn_710008ef70`, here a field's `CreateElement` attribute holding `CreateNewActor`). A hardware watchpoint on `fn_710008ef70`'s return address caught the write: `refl_MakePrimitiveType<UnkRefl_fn_7100009658>`, called from the lifted `fn_71000979a0`, storing a whole `CType` (0xc8 bytes) over the stack. The handler then returned into the string id type's copy hook (slot 0x48), whose `src` was the type's vtable; its first entry is `fn_710009ecb8`, the "string id holding a function address".

The cause is in `tools/port_link.py`. The lifted callers name the getter `callee::fn_7100009658(unsigned long)`, and two port definitions carry its tag: the getter `refl_GetPrimitiveType<G>()` and its inline helper `refl_MakePrimitiveType<G>(const CStrId&)`, which Clang 9 inlines and Clang 18 keeps out of line. The alias ranking matched parameter counts, so it picked the helper, which returns `CType` by value through a hidden pointer in rdi (x8 on AArch64). Ten lifted getters were aliased this way. Among several tagged candidates, the ranking now puts one returning a class by value last (`returns_class_by_value`, test in `tools/tests/test_port_link.py`). No alias targets the helper any more.

The boot (`tools/boot_distance.py`, main plus this fix, lifts of 2026-10-28) now runs 274 lifts instead of 240. It reaches `fn_7100015c70` as before, then traps in `fn_71000f7800` (3,632 bytes, unit 99, the sound system's `CTunableSoundSystemATK`; no accepted lift). After that it hits a null read (0x10) in a C library call under the lifted `fn_7100100804` (from `fn_7100107a04`), probably the trapped function's missing result.

**Two skills from obra/superpowers (2026-10-28, branch `claude/project-thread-4k0wen`).** At the owner's request, `.claude/skills/systematic-debugging` and `.claude/skills/writing-skills` are adapted from superpowers 7.0.1 (MIT, notice in `.claude/skills/LICENSE.superpowers`, entry in `docs/references.md`). `systematic-debugging` makes a session find the root cause before it tries a fix, and its new last section maps that onto this repository: the cheapest reproduction per symptom (one trap with `DREAD_TRAP=log`, `port_lift.py --debug ADDR`, `trymatch.py` on one source, one generator), the port's layers (lift or source, sweep, link, runtime, boot), and stream ownership of a fix. It is for traps, oracle rejections, regressions and tool failures, not for an ordinary non-matching function (`dread-match`). `writing-skills` is for creating or tightening a skill such as `dread-match` or `dread-hubs`: run a pressure scenario on a subagent without the skill, then with it. The rest of superpowers was evaluated and left out: its session-start hook loads into every parallel session, brainstorming waits for sign-off, and its plan, TDD, subagent and worktree skills overlap HANDOFF, the byte diff and oracle, and the streams.

**CActorLayer's reflection by hand: `fn_71002584a0` (2026-10-28, branch `claude/project-thread-pvyj4u`).** The port
traps in this function just before the bad string id, and the lifter can't verify it: the oracle's made-up
tables never end its loops. It is `O` in the new `src/game/CActorLayer.cpp`. `gen_reflection` skipped it
because of its first statement, a class attribute whose value is a list of one reflected value (a string id
set up by its type's +0x38 hook, listed by `fn_7100ad3404`). Matching findings:
- The value and the list match as temporaries of one `AddTyped<List, TypeGetter>(key, List(Value(name, type)))`
  call. With named locals, the stack slots or the destruction order differed.
- The name has to come before the type in the value's constructor, so the type getter runs after the id is built.
- A `const char*` handed down through an inline helper is stored early. `static_cast<const char*>(p)` makes a fresh
  temporary, stored after the key getter as a literal would be.

**The three x0 cases the lifter's fix leaves refused, by hand (2026-10-28, branch `claude/project-thread-pvyj4u`, second PR).**
The lifter thread's fix (branch `claude/project-thread-zgkpk3`) now checks whether a function writes s0
itself before it treats s0 as the result, which covers six of the nine void functions above. It does
not cover the three where x0 was taken for a result, because the same rule would wrongly catch
functions that return `this`. Those three are now in `src/`: `fn_7100eea4c8` (a sub-area's settings to
the environment manager) and `fn_7100eea618` (its camera settings to the main camera's
CMetroidCameraCtrl), both `O` in `src/game/CSubAreaManager.cpp`, and `fn_710109c9e0` (the pop of the
Lua console's scratch stack, `m` in `src/base/UnkLuaDoString.cpp`). The first argument of the two sub-area
steps is an empty `UnkLeftoverArg`, because their caller leaves x0 as the previous call returned it.

**Dashboard for both goals; README status card (2026-10-28, branch `claude/project-thread-vt2bik`).**
The private dashboard (see "Status dashboard" below) predated the reorganization: it showed matching
only, read the handoff's old phase table (every port row came out as an unstarted grey chip) and took
the workstream table's second column as "Area" when it is now "Files", dropping "Next". It now opens
with the two goals side by side. Decompilation: matching and usable shares, the boot path, the daily
trend. PC port: the Linux build's furthest point (handoff), the share of code with an accepted
semantic lift for every `U` function and for the boot worklist, and what the lifter can't take yet by
reason. `tools/dashboard_export.py` writes `areas.csv` (with a decomp/port goal per area; replaces
`phases.csv`), `workstreams.csv` with `next`, `lifts.csv` from the lift manifests, and
`boot_milestones.csv`/`boot_run.csv` from `build/boot/boot_distance.json` once the nightly has run;
handoff parsing moved to `tools/dread/handoff.py`. The lift manifests on the owner's PC (2026-10-28)
read 15,594 of 16,088 accepted (all) and 2,266 of 2,323 (boot worklist, which adds the run-time and
graphics roots to the boot slice), newer than the handoff's figures. `tools/status_card.py` is a slim
version for the README (`docs/status.svg`, from tracked files only); the `policy` job writes it to each
pull request's job summary with the change's effect on the whole binary, and the merging session
refreshes the README with `--write` after `progress.py --handoff` (a card committed by every branch
would conflict, like the handoff's table).

**Boot functions the lifter refuses since #236, by hand (2026-10-28, branch `claude/project-thread-pvyj4u`).**
#236 stopped accepting void lifts of functions whose callers read a result. On the boot slice that
refused 13 functions (12 accepted before, and `fn_7100ee5dcc`, inconclusive before). Read in Ghidra,
only four really return a float in s0: `fn_71000763c0` (a timed curve's step, `src/base/UnkCurveModifiers.cpp`),
`fn_7100d7ce40` (an entity's factor, `src/game/CEntity.cpp`, `O`) and `fn_7100b8deec`/`fn_7100b8dff0`
(animation times in frames, seconds or a fraction, new `src/game/UnkAnimTime.cpp`). The three others
are `m` and behave the same. The other nine are void, so the lifter's result detection was wrong for
them: `fn_710033187c` and `fn_7100ee4d34` take s0 as a float argument, five return with s0 left by a
callee that takes a float constant, and `fn_7100eea4c8`'s leftover x0 is passed on unused. They wait
for the lifter's detection fix.

**Port M1 round 4: every registered type named (2026-10-28, branch `claude/port-m1-round4`).** The
lifter's newer full set (#236, 15,594 lifts) linked with no trap before round 3's null dereference.
The null was a class with no name: 2,354 of the 4,581 types on the reflection registry's list
(`fn_710009ec00`, head `0x7101d5bdc0`) had none. A class getter's descriptor is a function-local
static of an ordinary function, which on the port is a local symbol in the executable, reached
section-relative, so the link cannot bind it. Meanwhile the game's lifted code registers and reads
the image's copy, which nothing constructed. `DREAD_REFL_CLASS_GETTER` now makes the getter
`inline __attribute__((used))` off `__aarch64__`; its descriptor and guard become weak symbols, and
the round-3 binding takes them (8,950 statics bound). Found and fixed on the way:
- **The size check misread fields as neighbours.** It refused four descriptors that `g_unk_` field
  names point into. A descriptor's size is asserted, so only another static counts there.
- **Clang 17 stopped packing a packed struct's non-POD members**, to match GCC. A static
  initialiser's merged statics then shifted by 4 bytes on the port, and one class was registered at
  its descriptor plus 4. The port compiles with `-fclang-abi-compat=9.0` (the sources' layouts are
  Clang 9's).
- **`CRntString`'s destructor** had 278 uses at `fn_710007ecd4` and two at one function's own
  address. `tools/port_addresses.py` now ignores a self-reference when the uses disagree.
- **`fn_7100153a90`** (the five NVN heaps by index) is a `switch` that Clang turns into a table:
  `cmp #5; b.hs` rather than the `cmp #4; b.hi` of an array index. Matched.
- **The fault handler scans the stack** for code addresses, which names callers the unwinder
  cannot reach.

The boot now writes through a string id whose data word is a code address, under `fn_7100096f30` (the
walk over a class's attribute and field records).

**Port M1: a silent stand-in for `nn::atk` (2026-10-28, branch `claude/port-atk-silent`).** The
owner chose a silent stand-in for the sound middleware until M4. `tools/port_link.py` turns every
status-`L` function in `nn::atk`'s range without a definition into a stub that returns zero
(`xor eax, eax; xorps xmm0, xmm0; ret`), whether a name reaches it (180) or only the game's code
pointers do (985, added to the loader's function table). The boot's ten `nn::atk` traps in the
static initialisers are gone, and the loader's trap thunks went from 1,105 to 674. Two game
functions without a lift trap next (`fn_7100001be8`, `fn_7100153a90`), then the null dereference in
`fn_71000a4b60`.

**Lifter: every `U` function, and semantic gaps round 3 (2026-10-28, branch `claude/lifter-gaps-3`).**
`port_lift.py --slice all` lifts every `U` function except the SDK imports and checks each with the
oracle. On main after #233 the run took 75 minutes (the first P-code fetch for 16,000 functions included):
- **15,352 of the 15,996** functions without a definition in `src/` are accepted (96%; 7.20 MB of
  8.08 MB).
- What was left, by bytes:
  - a caller-saved register read after a call: 138 functions, 203 KB;
  - inconclusive: 111, 195 KB;
  - over the size cap: 14, 150 KB;
  - an indirect call with stack arguments: 104, 122 KB;
  - unsupported instructions: 72, 83 KB, mostly `adr`;
  - rejected by the oracle: 75, 64 KB;
  - a register read before it is written: 37, 18 KB;
  - undecodable: 65, 2 KB.

This round takes the largest gaps first:
- **Results in more than one register.** The 138 "caller-saved register read after a call" break
  down as follows:
  - 75 read x1 after the call: a 16-byte result in x0:x1;
  - 36 read s1-s3: a float aggregate, a Vector4 returned in s0-s3;
  - the rest pass a clobbered register on as an argument.

  `abi_facts.result_shape` reads the shape off the callers: a forward scan of their straight-line
  code after `bl f`, past tests, up to a join, for x1 or s1-s3 read before written. Compiled code
  never keeps a value in a caller-saved register across a call, so such a read is a result. A
  caller that returns it (`bl f` then `ret`, or `b f`) adds its own shape. The scan is not
  liveness, whose evidence at indirect calls could invent an x1. The callee's semantic lift
  returns `lift_r2` (two u64) or `lift_f32x<n>`, every call declares it so
  (`ret_r2::fn_<addr>`, which port_link aliases like `callee::`), and a tail call to the same
  shape returns it. On AArch64 that is x0:x1 and s0-s3 as in the original, so the oracle
  compares them; on x86-64 it is rax:rdx and xmm0/xmm1. An argument register a call clobbered is
  passed as 0, garbage in the original as well. 136 of the 138 are accepted.
- **The result register.** `abi_facts.result_main` says which of x0 and s0 callers read; x0
  passed on to the next call counts (a function returning `this`). The lift returns that one where
  the last register written was the other.
- **No void lift of a function whose callers read its result.** Before, when the value variant
  failed the oracle (a wrong result), the void variant was accepted with no result compared, and
  the x86-64 callers got garbage. The evidence must be direct (`result_main` or a shape):
  `ret_used`'s liveness also counts a float register a later callee's facts read and nobody
  wrote (`fn_7100075f78`, a field initialiser). On the boot slice this refuses 13 functions that
  had been accepted.
- **Inputs that crash.** 86 of the inconclusive functions crash on every input, nearly always by
  dereferencing a call's result or a word the run read as a pointer (a small number). The oracle
  now repairs such a run: each of those values is set to a valid pointer, and again while the run
  crashes further on (64 runs at most). That recovers 11 of the 111.
- **Jump tables** whose bound check has a store between the `cmp` and the `b.hi` (20 of the 30
  `adr` refusals).
- **Every `U` function after this round** (`--slice all`, 71 minutes): **15,499 of 15,996
  accepted** (96.9%; 7.43 MB of 8.08 MB, up from 7.20 MB). Median coverage is 89%. The 497 left
  (0.65 MB):
  - inconclusive: 100 functions, 177 KB;
  - over the size cap: 14, 150 KB;
  - indirect calls with stack arguments: 105, 124 KB;
  - rejected by the oracle: 109, 93 KB. That is up from 75: these are the value variants of
    functions whose void lift is no longer accepted;
  - unsupported instructions: 52, 56 KB;
  - a tail call to another result shape: 22, 12 KB. A follow-up commit returns the wider
    result's first field, which accepts 18 of them;
  - undecodable: 65, 2 KB;
  - 30 others.
- **The boot set** (`build/port/lifted`, from this branch's code):
  - 2,266 lifts (1.21 MB);
  - 2,171 of the slice's 2,220 functions accepted (1.20 MB of 1.26 MB), up from 2,134;
  - 49 left (60 KB);
  - 95 of 103 overrides accepted;
  - median coverage 87%.

  Byte mode is unchanged: `gen_lift.py --add` on 6,000 candidates leaves `src/lifted/` as it is.

**Port M1 round 3: the port's foundations (2026-10-28, branch `claude/port-m1-round3`).** The
lifter's new lifts (#227-#233; `build/port/lifted` for the boot slice, then `lifted_all` for every
`U` function: 15,447 of 16,105 accepted, 95 overrides) took the boot past round 2's stops. What
stopped it next was the port, not missing functions:
- **The pack device.** `base::file::CPackManagerDevice` (unit 66) is reached through its vtable, so
  no worklist had it: 12 functions matched by a workflow (11 `O`, one `m` checked by a skeptic).
- **Two heaps.** The game defines its own `operator new`/`delete` and `malloc`/`free` over its
  default allocator, and the port linked them to the host's, so glibc's `free` got a block of the
  game's heap (`munmap_chunk(): invalid pointer`, in a deleting destructor). They now have sources
  (nine `O`, `realloc` `m`), and the runtime's replaceable C++ operators allocate through the game's
  allocator once it exists and free each pointer where it came from (`port/runtime/src/heap.cpp`).
- **Statics split.** The link binds 4,242 function-local statics and guards of inline functions to
  the image (`--defsym` over the weak definition, both or neither; see the README).
- **Constructors without an address.** A definition under its mangled C1 name (which Clang 9
  aliased to `fn_<addr>`) now takes the address its byte-matching callers reach it at: 54, among
  them `CFilePathStrId`'s constructor `fn_71000166c8`.
- **No static initialisers ran.** The game's 2,760 initialisers are plain `fn_<addr>` functions
  (none is a C++ `_GLOBAL__sub_I`) that `.init_array` calls on the console. On the port nothing
  called them, which a null default id showed. The port's main now runs them in order before
  `nnMain` (`dread_port_initialisers`), skipping the ones without a source. Four of those were the
  game's, marked `L` by the `runtime?` inference (units 0 to 3: the empty string and file-path ids,
  the start record "samus"/"StartPoint0"): matched (4 `O`, one `m`).

The fault handler reports aborts and names the C library's frames. With traps logged the boot now
runs 2,759 of 2,760 initialisers and `nnMain` into the start-up, then dereferences null in lifted
code. Ten `nn::atk` functions the initialisers call trap (`L`, the owner's decision, M4). So do two
game functions the lifter could not lift (`fn_7100001be8` rejected, `fn_7100153a90`
inconclusive).

**Hand-written files moved by unit (2026-10-28, branch `claude/project-thread-kzc3uy`).**
`tools/unit_move.py` moved 822 `fn_<addr>.cpp` files out of the eleven address-slice directories
into 405 unit directories named by `data/tu_map.csv` (high confidence and a class or namespace
hint only: `src/game/<Class>/`, `src/game/navmesh/<Class>/`, `src/base/<ns>/...`), with `git mv`
and their relative includes rewritten. 504 stay (300 in unnamed units, 157 medium and 45 low
confidence, `CAnimationComponent.cpp`, and `src/game_s2/fn_71003f1b00.cpp`, whose deliberate second
declaration `decl_check --base` would count again under a new path). The 15 local headers stay in
their directories: moving them to `include/` turned 40 local re-declarations into header
re-declarations for `decl_check`, which is hub-migration work. Evidence: `tools/check.py
--compile-only` before and after and `tools/objcompare.py`: all 90,578 functions identical in code
and relocations; the x86-64 sweep compiles all 3,979 files; `decl_check --base origin/main` PASS.

**Development overhead: docs, CI, shared tool code (2026-10-28, branch `claude/project-thread-kzc3uy`).**
From the owner-requested peer review, in order of time saved. The handoff (613 lines, edited by 56
commits in ten days) is one screen: stable sections moved to `docs/reference.md`, the per-stream next
steps to the entry below, and its status table is written by `tools/progress.py --handoff`; only the
merging session edits it. `CLAUDE.md` keeps the rules and a dozen daily commands; every tool's usage
is in `docs/commands.md`, generated from the docstrings by `tools/gen_commands.py` (CI checks it).
`.gitattributes` marks generator output `linguist-generated` (GitHub collapses it; the pathspec
`':(attr:!linguist-generated)'` drops it from local diffs). A hosted `build` workflow compiles all
3,979 sources with Clang 9.0.1 (`tools/check.py --compile-only`, the official Linux tarball now
pinned, about 90 s on 4 cores) and for x86-64 (`tools/port_sweep.py`, about 2 minutes), and runs
`tools/tests` (24 unittest cases, no game data; asmdiff's tests compile a fixture). `tools/dread/`
holds the one loader of `data/functions/` (six tools moved to it, outputs compared before and after)
and `python -m tools <tool>` runs any script under the repository's venv. `AGENTS.md` and
`docs/tasks/` are retired (macOS steps kept in `docs/reference.md`). The move of `src/game_*` files by unit
and the boot-distance run followed (entries above, `tools/boot_distance.py`). The status column in
its own file was dropped (owner, 2026-10-28): both columns are generated, only 5 of 83 merges since
2026-10-18 had both sides change the same `data/functions/` file, `tools/check.py --update` already
resolves those, and about 20 tools read the column.

**Handoff next-steps snapshot (2026-10-28, branch `claude/project-thread-kzc3uy`).** The handoff's
per-stream "Next steps" section as it stood on 2026-10-27, moved here verbatim when the handoff was cut
to one screen. The handoff now keeps only the next few items per stream and links here.

**Stream A (boot slice and hubs).** Round 1 (2026-10-26, #199) matched the eight top hubs of
2026-10-25 and two siblings; round 2 (2026-10-26, journal) took the next 18 boot hubs and round 1's
follow-ups: 14 `O`, 10 `NON_MATCHING(minor)`, one dropped, each in a file named for its class or
unit with one header (`CRntFile`, `CFilePathStrId`, `CType`, `CGameManager`, `CBlackboard`,
`CGameBlackboard`, the music, model and rumble managers, the file device, the registry). Next,
from `tools/hubs.py --version 2.1.0 --boot` (2026-10-26, after round 2): `fn_7100ee1f34` (2,096 B,
19 unmatched callers; the `CSubAreaManager` unit's 0xae8-byte object, dropped in round 2, layout
notes in the journal), `fn_71000ead80` (416 B, 13), `fn_71000ea448` (2,032 B, 12),
`fn_7100076acc` (544 B, 12), `fn_710007c1ac` (476 B, 12, the `CRntFile` unit), `fn_710009d52c`
(168 B, 12, `CType`'s unit), `fn_7100087850` (160 B, 11, the registry's lookup), `fn_71011a2fbc`
(20 B, 11), `fn_710027a33c` (204 B, 11), `fn_7101196fd8` (48 B, 11), `fn_7100decab0` (428 B, 10,
the game manager's target getter, declared in `UnkGameManager.h`). Round 2's own follow-ups: a
second pass on its low-scoring minors (`fn_710023b580` 10.6%, `fn_710009d7a8` 16.4%,
`fn_7100deba8c` 16.5%, `fn_710007c714` 23.8%, `fn_7100fd3804` 24.8%, `fn_710007e7f8` 43.5%);
the music manager singleton `g_unk_71726bc560` still has three local types (five files, two of
them with their own view of the 0xf18-byte object); `UnkGameManager` lacks the map at +0x1a70 that
`fn_7100e039e4` reads through its own view; `src/game_aca57c/fn_7100ea56f0.cpp`'s `UnkSuffix` is a
`CRntString`; `fn_710008b88c` has no header. Binary-wide the largest are `fn_71010c0270` (120 B,
354 unmatched callers, almost all `lua_bind::*`) and `fn_71006f4b0c` (592 B, 211). 895 boot-slice
`U` functions (300 KB) are "ready" (`--ready --boot`).

Canonical headers. Batch 1 (2026-10-24, #184): the 13 symbols `include/` declared twice, the
actor's target getter (`include/game/UnkActor.h`), the matrix getter (`UnkMatrix44.h`), the
counted-resource free (`UnkCountedResource.h`), the game manager (`UnkGameManager.h`) and every
class getter (generated `UnkClassGetters.h`). Batch 2 (2026-10-25, #187), 25 symbols: the five
`CRntString` operations (`CRntString.h`); the `CStrId` constructor from a `CRntString`, the
global empty id and the string pool (`CStrId.h`); the reflected-value builder (`UnkReflValue.h`)
and the blackboard qualified id (`include/game/UnkGameBlackboard.h`); the animation component's
class test and track progress (`include/game/UnkAnimationComponent.h`) and the GUI focus setter
(`UnkGuiDisplayObject.h`); the type-erased array copy (`include/base/UnkErasedVector.h`), the two
handle assignments (`UnkRefHandle.h`) and the render manager (`include/game/UnkRenderManager.h`);
the sphere position setter (`UnkSphere.h`), the scene toggle notifiers
(`include/engine/scene/UnkToggle.h`), the converter registration (`UnkReflConverter.h`), the
property registration (`UnkReflStatics.h`), the AI component's navigation getter
(`UnkAIComponent.h`) and the nav-mesh manager (`UnkNavMeshManager.h`). Five matched on the way:
`fn_7100216978`, `fn_71001eef58`, `fn_71001eef74`, `fn_7100ebf278`, `fn_7101177ea0` (and
`fn_71011cf2a4` as `m`). The batch ran six file-disjoint steps in parallel and five overlapping
ones in sequence (`tools/trymatch.py --obj-dir` keeps parallel objects apart).

Batch 4 (2026-10-26) gave every conflicting symbol with a hand-written definition one declaration
with the definition's types (29 symbols, 19 new headers such as `UnkTimelineComponent.h`,
`UnkCollisionRecord.h`, `UnkSpitAttack.h`, `UnkGuiHandle.h`; journal). Batch 5 (2026-10-26, journal)
chose signatures for 34 symbols without one (Ghidra, `abi_facts.py`, the callers), among them the
deferred set's add and remove, the GUI root (`UnkGuiRoot.h`: layout, global, five root functions),
the display-object container, the matrix product and scale, the segment's normal and constructor,
the timeline component's lookups, reflection type globals, the suit properties, behaviour-tree
nodes, and moved `CAttack.h` to `include/game/`. Batch 6 (2026-10-26, journal): about 70 symbols and batch
5's review follow-ups, among them every `ITEM_` property (48, `UnkItemProperties.h`, with
`ItemQualifiedId`/`ItemSource` instead of `SuitAs`), the reflected type globals
(`UnkReflTypeGlobals.h`; `g_unk_7101d5e2e0` had 57 copies), the action-node base constructor
`fn_7100393e68` (27 nodes on `UnkBehaviorTreeNodeBase`), the boss AI component, the sphere's radius
setter (16 files), the weak-handle unlink, the GUI event, child lookup and key list, and
`UnkGuiFocus`/`CAttack.h`'s handle types onto `UnkWeakHandle.h`; `UnkReflStatics.h` includes the small
`UnkTimelineCalls.h`. Next, by `tools/decl_check.py --list`: `g_unk_71726bc570` (the stats global, 3
views), `g_unk_71727bd078` (a tunable), `fn_7100ee1f34`, `g_unk_7101d47360` and `g_unk_71726bc560`
(managers; `fn_7100ee1f34` is also a boot hub), `fn_7100010e58`, `fn_7100017390`, `fn_71002aeb9c`,
`fn_71003ac8f4`, `fn_7100ad61f8`, `g_unk_7172794d40` (2 signatures each). Batch 6's own follow-ups
(its review): `src/game_5821e4/CAttackDtor.h`'s `UnkAttackOwner` is a second view of
`UnkWeakHandle` (and `fn_710063904c.cpp`'s `fn_71010cb2fc(void*, void*)` is why
`UnkAttackListenerRaw::Destroy` is a template); the three `src/game_s7` callers of `fn_71010c4250` cast
their `UnkProp` (`src/game_s7/UnkPropS7.h`) to `UnkComponentProp`: move it into the header; two action
nodes (`fn_710037a410`, `fn_710037ca84`) cast to the node base instead of deriving; the lifted
`fn_7100394bd4` is defined without parameters, the header takes `void*` (port aliasing by address);
`UnkStrIdTable` and `CBlackboard.h`'s `UnkPropertyArrays` are one parallel-array template with two
strides; the GUI callers cast their own views to `UnkGuiDisplayObject*` (derive them instead).
`src/game_aca57c/fn_7100b2c6a0.cpp` calls `fn_71011d968c` through an untyped function pointer and
`fn_71011cde44` typed (its minor functions went from 98.2% to 96.5% for that; an asm-labelled second
declaration would have kept the bytes but hides from `decl_check`).
Older follow-ups:
`fn_710021693c` and `fn_7100216d30` into `UnkSphere.h`; `fn_71004fa3b8` (9 identical local
declarations); the `UnkActor` slots at +0x240 and +0x298 typed `UnkAIComponent*` and
`UnkNavigation*`; `fn_7100394bd4` (2 signatures); `fn_7100ad3588` into `UnkErasedVector.h` and
`fn_71006eb8cc` into `UnkRefHandle.h`; `fn_710001a270` and the string neighbours `fn_710007ea60`,
`fn_710007e6c4`; `InitPropertyEx`'s `on_init` parameter type in `UnkTUInit.h` (one caller casts);
the render manager's accessors into `UnkRenderManager.h`. From the port link: 182 names that
resolve to no address (`UnkIdTable::UnkIdTable()`, `UnkMapIter<...>::Advance()`, ...) need an
`fn_<addr>` name; `src/game_s7/fn_71011bc0b0.cpp` defines `fn_71011bc830` by an
`asm(".set ...")` alias to a libc++ `__sort3` instantiation that an x86-64 compile does not emit.
Leftovers that keep a second declaration on purpose: `src/game_s2/fn_71003f1b00.cpp`
(`u32`-returning `fn_710009d50c`, load-bearing for its `m`), ten callers of `fn_7100d7c990` with
their own word-typed views of the actor's translation, two callers of `fn_710001737c` with a local
count view; `src/game_s7/UnkGuiS7.h` keeps `using UnkClass = CClass` for two includers.

The mechanical part of a migration is a tool since 2026-10-25: `tools/hub_migrate.py` removes the
local declarations, includes the header and keeps only byte-identical files (`tools/baseline.py`
is the per-file evidence, parallel, with copies to restore from). Two passes so far. The first
(var-flotsam/dread-decomp#190) migrated 66 files for 15 symbols in a minute. The second (branch
`claude/tuinit-unify`) first moved the symbols both static-initialiser helper families name into
two small headers they both include: `include/base/reflection/UnkReflTypeDtors.h` (the type
objects' destructors `fn_7100096604`, `fn_7100099468`, `fn_710009891c`, `fn_710009a368`,
`fn_710010c424`; `base/UnkCloneHelpers.h` includes it too) and `include/game/UnkTUSymbols.h` (the
function records `fn_710000cf00`/`fn_710000d470`/`fn_710000d4f4`, `UnkPropertyHead` and the property
list head `g_unk_71726db788`); `unk_static_init`'s property structs point `next` at
`UnkPropertyHead` and its helpers take the head as `UnkPropertyHead*&` (pointer retypes only; the
helper bodies and their store order are untouched, because they are load-bearing). That unblocked
96 files (the class-type destructor alone was declared locally in 57), all 195 includers of the two
families byte-identical (`baseline.py`, tag `tuinit`). Batch 3 (2026-10-26) hand-migrated the 40
files the tool left (placeholder types renamed to the headers' real ones, five agents on disjoint
file sets, about 7 minutes; journal). The two helper families now share one set of property and
tunable helpers, `include/game/UnkTUProperty.h` (2026-10-26, branch `claude/tuinit-fold`): each
property initialiser in both store orders (`InitProperty`/`InitPropertyEx` link first,
`...VtableFirst` store the vtable first; the source order steers the scheduler). What the families
still hold apart is the TU header data (`InitHeader*`), the type registrations (`InitEnum`,
`InitFlagSet`, `InitClass`, `InitVector` vs `unk_static_init`'s) and two placeholder views of a
type object (`UnkReflType` vs `UnkEnumType`); folding those is the next readability step, verified
the same way (`baseline.py`, every includer of both families). Cheap follow-up: 729 calls in unit
files still spell the atexit stub `__cxa_atexit`, which `check.py` cannot resolve; `fn_7100000250`
makes each a checked relocation (the helper headers' 24 sites added 1,055 checks, 0 mismatched).

**Stream B (core engine types).** Left from the string-id and reflection work: the reflected
function array's growth (`fn_7100097f80`), the function's constructor and destructor
(`fn_710009a5b4`, `fn_710009aae8`), the allocator functions next to `operator new`
(`fn_7100083870`, `fn_71000839c0`, `fn_7100083a10`, `fn_7100083a70`), the string pool's remaining
`m` functions (insert `fn_71000801e4`, compare `fn_710007f240`, release `fn_71000803a4`). CRC64
and the string-id constructors sit next to `nnMain`, so `gen_functions_csv.py` infers `runtime?`
and marks them `L` although they match (`fn_7100000850`, the constructor from a `CRntString`,
matches since 2026-10-25 and still reads `L`); fix that inference.

**Stream E (tooling).** Done 2026-10-25: `tools/baseline.py` (trymatch snapshots for a file set,
compare, restore; headers expand to their includers; `--obj-dir` per source so agents never
collide) and `tools/hub_migrate.py` (the mechanical hub migration on top of it, with a `BROKEN`
guard for files that do not compile before the edit), plus the batch procedure in
`.claude/skills/dread-hubs`. Done 2026-10-24: every generator verifies relocation targets
(`asmdiff.diff_object(strict=True)`), the three generator target bugs are fixed (`gen_clones`'s
destructor scan now reads each string-id member's pool, `gen_statics`'s fact extractor drops
stale register state, `gen_tables` reads the video map's globals from the GOT) and `check.py`
enforces relocation targets everywhere. Next: a dynamic boot trace from Eden's GDB stub to
replace the static lower bound; `check.py`'s resolver could map the mangled constructor and
destructor names of address-bound members (`UnkStringPoolHolder`'s C1/D1) to their addresses, so the
66 includers' references are checked; directory moves driven by `data/tu_map.csv` once a unit is
complete (one rename PR per unit); `tools/gen_functions_csv.py` subsystem inference from
`tu_map.csv` instead of neighbours (68,731 functions are still "unknown"); `trymatch.py`-style
relocation checking in `tools/nearcopy.py`'s candidates if it compiles anything; `check.py`'s
definition rule counts a function pointer in a template argument (`UnkFoo<&fn_<addr>>`) as a
definition of that address, which `tools/port_link.py` already excludes.

**Stream D (port).** M0 is done (#183). M1 step 1 is done (2026-10-25, #186): `tools/port_link.py`
links the 3,804 sweep objects relocatably (`ld.lld -r`, no duplicate symbol, 60,402 undefined),
classifies what is missing, generates `--defsym` aliases (37,531 references to functions defined
under another symbol: the declaration debt and the lifter's `callee::` convention), absolute data
symbols (15,450 `g_unk_` addresses) and trap thunks, and links again to a residue of 304 (71
vtables, 49 standard library names, 2 `nn::hid` destructors, 182 names without an address).
M1 step 2 is done (2026-10-25): `lib/lua` and `lib/dlmalloc` are compiled into the link and the
build (`dread_libs`), and `fn_7100000250` (`__cxa_atexit`) jumps to the host's. On `main` after hub
batch 2 the boot slice's 4,211 game functions are 1,109 defined, 941 reached through an alias only,
4 the host's and **2,157 without a source** (1.24 MB; 2,148 `U`, 8 `L`, one `O`), listed by
callers in `build/port/link/boot_missing.csv`, the lifter's coverage worklist. The residue is 375
(the 304 above plus Lua's C library calls). M1 step 3 is done (2026-10-25): `tools/port_lift.py`
lifts the slice's `U` functions in `gen_lift`'s semantic mode and accepts a lift when Ghidra-P-code
execution of it and of the original agree (`tools/pcode_emu.py`, oracle (b); design and limits in
`docs/PORT_PLAN.md`, usage in `port/README.md`). 1,777 of 2,148 pass (839 KB); with
`tools/port_link.py --lifted build/port/lifted` the slice has 380 functions without a source. The
gaps step (exact flags, jump tables, float pass-through, four oracle fixes) brings it to 1,941
lifts (1.04 MB) and **216 slice functions without a source** (196 KB, `boot_unlifted.csv`). The
oracle covers a median 67% of an original's instructions. The lifter's second gaps round
(2026-10-27, branch `claude/lifter-semantic-gaps`) handles the gaps listed then: stack arguments,
`x8` results in tail position, jump tables through register copies, shrink-wrapped prologues,
frames sized by shifted immediates, float widths, and half and byte loads converted to float. On
the boot slice with the run-time roots (2,366 `U` functions, 1.40 MB at the time) it gets 2,295
accepted lifts (97%). The oracle's inputs and a fresh `build/port/lifted` follow (stream M);
`tools/port_lift.py --debug ADDR` shows a rejected function's first difference. A semantic lift the manifest marks `"overrides": "src/lifted/<file>.cpp"` replaces that
byte-identical lift in the link (2026-10-27, `port/README.md` "Overrides"): the port's fix for
lifts that drop arguments. M1 step 4 (2026-10-26, var-flotsam/dread-decomp#201, #203 and #206): the runtime defines every `nn::os` and
`nn::oe` import the slice reaches (port_link's residue lists none); `dread_game` and the sweep
build with `-fno-strict-aliasing`; the data loader (`port/runtime/include/dread_port_loader.h`,
`port/README.md` "The data loader") maps the dump's main NSO at `0x7100000000` before the game's
static initialisers and translates the 128,576 code pointers in `.data` through the function table
`port_link.py` now writes (`image_tables.s`); `python tools/test_port_loader.py --version 2.1.0`
runs it on the dump in 2 s (26 checks, no SDK). `nn::fs` is in too (all 22 imports; `rom` is the
dump's `romfs/`, saves go to `$DREAD_SAVE`). So is the rest of the start-up `fn_71001325d0` (`nn::mem`,
`nv`, `nn::account`, `nn::util`, `nn::time`, `nn::diag`). `nn::hid` too (one handheld pad set by
`dread_port_set_npad_state`, no vibration devices). Every SDK import of the slice is in the runtime
(`nn::prepo` drops reports, `movie` has no tracks yet); `tools/port_imports.py` checks stubs against
the dump's mangled import names. `tools/port_addresses.py` places the vtables and
named functions the link had no address for from matched code (residue 439 to 226: 191 host names,
9 vtables and 26 names, 12 of them a placeholder name the sources reuse for different classes:
rename them, `port/README.md` "Addresses from matched code"). **The port links and runs on Linux in Docker** (`port/README.md`,
"Linux in Docker"; WSL 2 and Docker installed by the owner on 2026-10-26). The boot-match batch
(2026-10-27, branch `claude/port-m1-boot-match`) hand-matched 194 of the 213 boot-slice functions
that had neither a source nor a lift (112 `O`, 82 `m`, each `m` re-checked by a skeptic; 19 of 2
to 16 KB left, `build/port/linux/link/boot_unlifted.csv`), gave the 40 declarations it made
conflicting one header each, and matched `nninitStartup`. The boot now maps the dump, runs
`nninitStartup` (the memory heap and the default allocator, as the SDK does before the static
initialisers), every static initialiser, `nnMain` and the whole system start-up, and traps at the
game's main function `fn_7100000980` (764 B, `L` in `data/functions/` but the game's). Crashes
print their address and return addresses (`port/runtime/src/fault.cpp`). Boot-match round 2
(2026-10-27, branch `claude/port-m1-game-main`, journal) matched `fn_7100000980` and the 141
functions the boot then reached by direct calls from its run-time roots (`nnMain`'s three callbacks
and main's four), 95 `O` and 47 `m`. It also stubbed `nn::nfp` (26 of 26). The boot now runs into
the start-up `fn_7100015c70`; under `DREAD_TRAP=log` six functions without a source come first,
then a null dereference in lifted code after one of them returned 0. Next for M1: round 3,
the 127 functions on the same roots' static list (74 KB, the scratch script of round 2 over the new
`traps.s`) and the ones reached through pointers (`fn_7100075730`, `fn_710008a460`,
`fn_710002f0dc`, `fn_71000666b0`, found by running), the same workflow; then follow the traps.
Two blockers come from the lifted code itself, not from missing functions:
- **Dropped arguments.** Byte-identical lifts dropped arguments at their call sites (277 boot-path
  callers) and in their definitions (164 on the boot path), and `tools/abi_facts.py` under-counted
  216 functions (capstone's alias bug). The lifter's pass-through regeneration (2026-10-27) leaves
  103 functions, 10 on the boot path (`build/port/passthrough.json`); their semantic lifts carry
  `"overrides"` in port_lift's manifest, which port_link has to honour (stream M). Until it does,
  write a trapped path's callers by hand, as unit 167's wrappers were
  (`src/engine/UnkGraphicsApi.cpp`).
- **Split statics.** Function-local statics of header-inline functions are executable symbols on
  the port, while lifted code uses the image's. Bind them to the image with `--defsym` from
  `tools/port_addresses.py`'s harvest.

`nn::audio` (6 reached through lifted code) still needs stubs, and also the game globals that a
matched file defines itself while other code reaches them by address (`port/README.md`, "Game data
defined by the sources").
M2 groundwork (2026-10-26, branch `claude/port-bshdat`): `tools/bshdat.py --version 2.1.0
--compile` parses the 478 shader packages, extracts the 671 programs' 1,342 GLSL sources and the
compiler's reflection to `build/shaders/2.1.0/`, and compiles 1,341 stages to OpenGL and Vulkan
SPIR-V with glslang 16.6.0 after two rewrites (block bindings from the reflection,
`GL_NV_explicit_typecast`); FXAA's fragment stage needs a rewrite rule. Decided 2026-10-26: Vulkan
first, the SPIR-V built from the user's dump by the port's build (`bshdat.py --compile`'s Vulkan
mode), no compiler in the executable. Done since (#202, #204): the build step is the port's
`shaders` target (`tools/bshdat.py --install`: the FXAA fixup makes all 1,342 stages compile; each
module is checked against NVN's reflection; `shaders.manifest` beside the executable), and
`port/nvn/` (`dread_nvn`) holds the manifest reader and the binding layer: NVN's per-stage slots on
one Vulkan descriptor set per stage, stages recognised by their control section (the only part
`nvnProgramSetShaders` gets by pointer). Variant selection is the engine's own (`fn_71001b0f24` on
`CProgramResource`, `port/nvn/README.md`): the shim needs nothing for it, but the engine's loader
and selector (`fn_71001b0050`, `fn_71001b0f24`, `fn_71001b0f7c`, the program switch `fn_71001cc608`)
are `U` and in port_lift's boot slice. The game calls 150 of its 523 NVN functions.
Since then (#217 and the Vulkan check): the binding layer runs in the Linux image
(`nvn_manifest_check`) and against lavapipe with the validation layers (`nvn_vulkan_check`,
`port/docker/vulkan/Dockerfile`, the owner's go-ahead): all 671 programs' modules, layouts,
descriptor sets and pipelines, no validation message, after two fixes it found (inactive fragment
inputs the vertex stage does not write are dropped by the build step; the device needs
`maintenance4`). Then the start-up path of the shim itself: the entry layer (`nvnBootstrapLoader`,
`nvnDeviceGetProcAddress`, a named trap for every function not written), the device over Vulkan,
memory pools with a GPU address space and buffers (the binding layer's address resolution), queues
and syncs, command buffers, the state objects and the window: 106 functions, 74 of the 146 the game
calls (`data/nvn_procs.csv` from `tools/nvn_procs.py`, which also names every `g_unk_716e29xxxx` of
the matched start-up). `nvn_startup_check` replays that start-up `fn_71001587f0` and three frames
against the shim on lavapipe with validation: all checks pass, no message. With the texture part
(textures as Vulkan images, the texture and sampler pools, samplers; formats from
mercury-engine-data-structures and by use, targets from the game's own textures) the replay reaches
no unimplemented function: 143 functions, 108 of the 146 the game calls. With the drawing part
(2026-10-27): all 146 (182 in all). Programs, pipelines, Vulkan command buffers, render targets,
clears, barriers, uniform updates, vertex streams and draws, each number's meaning taken from the
engine's own reflected enumerations and the wrapper that converts them (`port/nvn/README.md`,
"Drawing"; `include/nvn_shim/draw.h`). `nvn_startup_check` with the shaders draws all 671 programs
in full and depth-only on lavapipe with no validation message. Next for M2: the alpha test (a discard
the build step adds to the fragment stages, with a push constant), a Vulkan surface for the window,
asset texel upload (the console's tiled layout) and the sampler enumerations; and, once the boot
reaches `fn_71001587f0`, `dread_nvn` joins `tools/port_link.py --executable` (agreed with the M1
session).
Windows (decided 2026-10-27: Linux is the only running target until the title screen; Windows is
compile-checked alongside, #225): `tools/port_sweep.py --target
x86_64-w64-mingw32` compiles with llvm-mingw 20261006 (`tools/fetch_toolchains.py
llvm-mingw-20261006`, Itanium ABI, libc++ 9.0.1 over MinGW-w64's headers) and scans for what LLP64
and a 16-bit `wchar_t` change silently. All 3,979 sources compile and the scan finds nothing: the
game's wide text goes through the `char32_t` helpers in `include/base/Char32.h` (the original calls
where `wchar_t` is 32 bits). Keep both at zero: rerun it when a PR adds hand-written code. Next
Windows steps, after the title screen on Linux: the runtime's LLP64-mangled import names
(`tools/port_imports.py --windows`: the runtime compiles; 41 of its 200 imports are lost by name,
each with one `long long` counterpart, for `image_tables.s` to list under the console's name), Win64/COFF
versions of `port_link.py`'s `traps.s`, `residue.s` and `image_tables.s`, the fault handler as
structured exception handling, and a Windows link (`port/README.md`, "Windows (compile only)").

**Stream M (generators).** The bugs above and `--prune`/regeneration after merges. The lifter's
semantic mode (`opts={"semantic"}`, driven by `tools/port_lift.py`) gives the port oracle-checked
lifts of `U` functions; its gaps are listed under stream D. Byte mode now names, at each call, the
argument registers the callee reads that the caller still holds (2026-10-27, journal "Lifter:
arguments lost in byte-mode lifts"; regenerated, 46,443 lifted, 73 more `O`, none lost). Byte-identical
lifts had relied on registers their C never named (AArch64 keeps them; an x86-64 compile does not):
before, 3,220 `src/lifted` definitions read more than they declared and 8,362 made a call that
dropped one; after, **103** functions lose an argument (36 definitions, 95 callers; 10 on the boot
path), listed by `tools/port_lift.py --passthrough` in `build/port/passthrough.json`. port_lift
lifts those in semantic mode and marks each accepted lift `"overrides": "src/lifted/lifted_XXX.cpp"`
in its manifest, for `tools/port_link.py` to link instead of the `src/lifted` copy (the M1 session's
change, branch `claude/port-lift-overrides`). 93 of the 103 have an accepted lift, the 10
boot-path ones included. `tools/abi_facts.py` corrects capstone 5.0.7's register access
(`cmp`/`cmn`/`tst`, shifts and extends by immediate, `fmov #imm`), sees the cases of a switch, and
serves every tool from one table computed in address order (`build/port/abi_facts.pickle`).
port_lift's worklist is now a slice (2026-10-27): `--slice boot` (the default) is every `U`
function reachable by direct calls from `nnMain`, from the seven run-time roots, from the four
graphics functions and from the four the boot reaches only through pointers (2,226 functions,
1.26 MB); `--slice all` is every `U` function but the SDK imports (16,002, 8.08 MB);
`--worklist build/port/link/boot_missing.csv` gives the old one. Next for the lifter: the semantic
gaps, the oracle's inputs, and both slices run (branches `claude/lifter-*`). Open leftovers of
every generator round are recorded in `docs/JOURNAL.md` (work log items 0–3).

**Lifter: the oracle's inputs (2026-10-27, branch `claude/lifter-oracle-inputs`).** The first
oracle ran six seeds per function and accepted a lift when every conclusive run agreed. Its reach
was measured with `--mutants`, one-point faults planted in accepted lifts: on 300 boot-slice
functions it caught 46% of them. This round gives it more inputs, fixes what a triage of the boot
slice's rejected and inconclusive lifts found (36 of 40 were oracle artifacts, 4 lifter bugs), and
fixes what two review workflows over the diff found. Every review finding was checked by two
skeptics, and where a fix trades soundness for false rejects, the variants were measured.
- **Mutations.** After the seeds, `Oracle.originals` changes one input at a time and keeps the run
  when it reaches an instruction no run reached before. The input is an argument register, the
  result of one of the first 16 calls (`("c", n)` overrides) or a memory word the run read. The
  value comes from `mutation_values`: the function's own constants and their neighbours, 0, -1,
  boundaries and pointers. The value is placed where the run read the word, so a 32-bit field in
  the upper half of its 8-byte word can change. Before the random search, a sweep sets each of the
  first 8 words read to each constant the function compares with. A thread loop like `while (state
  != 3)` (the pointer root `fn_71000666b0`) had run into the event cap on every input. The search
  stops after 64 tries, or 24 tries in a row that find nothing (`--no-fuzz` turns both off; the
  manifest records `fuzz`). Guard words, relocated words
  (the GOT), read-only data and text are never mutated. The lift reads its own GOT and folds jump
  tables into a switch, so mutating them sent only the original elsewhere and rejected correct
  lifts (`fn_71001604e0` and `fn_7100d8ced8` are accepted now).
- **Out-parameters.** A pointer to a local passed to a call gets a pattern written through it.
  The pattern stops at the saved return address (`frame_record`), at callee-saved save slots and
  at the next local whose address the code takes, and the lift's run follows the original's
  choice of arguments and extents (`call_log`, `guide`). An object passed on the stack is
  compared up to the next address the original took that the lift did not take at the same
  offset, so a field's address passed earlier (`&s.name`) no longer ends the comparison.
- **Unknown callees** (imports outside the modelled C library, cycle-closers, indirect targets)
  are compared on x0, on what the call's block set, and on `x8` when it holds a local. A register
  written before a conditional branch is not compared, since it may be the other path's. Without
  that rule the boot slice rejects 2 correct lifts, e.g. `fn_7101059794`, whose vtable getters
  see the previous iteration's float temporaries. An unconditional `b` and a tail call's own
  branch keep what was written.
- **Runs that end early.** A run is cut off after 60,000 steps, 256 events or 2 MB of memory
  snapshots, and `Machine.cut` records which. One boot-slice function had made a worker reach
  23 GB; kept runs now drop their pages. The comparison rules are:
  - both runs cut off: the input decides nothing. On the review's example, a 300-call loop against
    a 280-call one had agreed at the event cap;
  - one run cut off: what both recorded must agree. Memory the cut-off run wrote that the other
    run had not written by its next call or return is a difference (addresses only; not after a
    fault, nor after a modelled `memset` clipped at 64 KiB);
  - a lift that runs out of steps where the original ended within a twentieth of them fails;
  - an original that faults is compared up to its fault, whatever the lift did; a lift cut off
    before that point is skipped;
  - a lift that reads the caller's frame beyond the original's stack arguments, or returns
    outside where the original returned, fails;
  - a skip names its reason, and an inconclusive verdict shows it.
- **Lifter fixes** (semantic mode):
  - an indirect call given a local in `x8` builds its result there (a placement new through the
    pointer), but only when `x8` is exactly a local's address, not read since, and set on both
    paths of a join;
  - an indirect call after a store to `[sp]` is refused. That store is a stack argument, which an
    indirect callee's lift would drop; 7 lifts on the boot slice had been accepted that way;
  - flat mode refuses a caller-saved register read after a call before it is written (`x1` of a
    16-byte result).
- **Left as they are, measured:**
  - writing the out-parameter pattern up to the save slots instead of the next taken address
    rejects a correct lift: the lift's save slots sit elsewhere;
  - relocated `.data` words both runs read stay unmutated (a narrower rule needs the GOT's range).
- **Measured** on the 300 boot-slice functions of the first test (`--only` that list, `--mutants
  300`):
  - seeds only (`--no-fuzz`): 291 accepted, median coverage 77% of the original's instructions,
    80 functions run completely, 46% of one-point faults caught (77% on functions run completely);
  - with mutations, before the review fixes: 292 accepted, 86%, 104 complete, 51% (78%);
  - final, on the 288 still `U`: 282 accepted of 283 compiled, 85%, 96 complete, 51 below half,
    53% (78%).

  On the same 288 functions the review fixes moved coverage from 85% to 83% (mutated GOT slots
  had reached paths the program cannot take), and the sweep and the placed values brought it back
  to 85%.
- **The fresh `build/port/lifted`** (`port_lift.py --version 2.1.0`, the boot slice and the
  overrides; 10 minutes with warm caches): 2,229 of 2,329 accepted (1.16 MB).
  - On the slice, 2,134 of the 2,220 functions without a source are accepted (1.15 MB of
    1.26 MB). Every run-time root, graphics function and pointer root is either accepted or
    defined in `src/`.
  - Of the 86 left:
    - 35 read a caller-saved register after a call (16-byte results);
    - 23 are inconclusive: 15 crash on every input before anything is compared, 7 loop past the
      step limit on every input, 1 reads its caller's frame;
    - 10 read a register before writing it;
    - 7 make an indirect call with stack arguments;
    - 6 are rejected and 5 hit other lifter gaps.
  - Against the set built before the review rounds (2,247), 19 functions that had been accepted
    are not:
    - 7 dropped stack arguments;
    - 12 had agreed only on runs cut off (7) or on inputs the original crashed on (5).
  - Overrides: 95 of 103 accepted. All 8 others differ at a call's float argument.
  - Median coverage is 85%.

**Port M1: semantic lifts override byte-identical ones in the link (2026-10-27, branch
`claude/port-lift-overrides`).** The lifter session's pass-through fix marks a semantic lift
`"overrides": "src/lifted/<file>.cpp"` when byte mode cannot name a register the code relies on.
`tools/port_link.py --lifted` now links such a lift in place of the `src/lifted/` definition. A copy
of the file's sweep object gets that address's definitions made local (`llvm-objcopy
--localize-symbol`), and the lifted link takes the objects again with the copy. The address is then
left without a global definition, so every reference is aliased to the lift.

Two findings on the way:
- A call within one section carries no relocation under Clang 18 (the assembler resolves it), so a
  "still referenced in its own object" check has to read the disassembly as well.
- `find_tool` now also looks where the compiler's symlink leads (`/usr/lib/llvm-18/bin` in the
  Docker image), which holds `llvm-objcopy`.

Tested two ways:
- a hand-made manifest of three stand-in lifts: two replaced, one refused because the named file
  does not define the address;
- the real lift set, which has no overrides: every output identical to the original tool's, the
  executable included.

**Port: the runtime's import names on Windows (2026-10-27, branch `claude/port-windows-imports`).**
A report only (the owner chose it over runtime or `port_link.py` changes, which wait for the Linux
title screen). `tools/port_imports.py --windows` compiles `port/runtime/src/` for
`x86_64-w64-mingw32` with llvm-mingw (the sweep's Windows flags, libc++ 9.0.1 headers) and compares
the runtime's definitions there with the imports it answers on Linux (`image_tables.s`'s `nn::`,
`nv::` and `movie::` names). All 18 sources compile. Of 200 imports, 159 keep their name and 41 are
lost (`movie` 11, `nn::fs` 9, `nn::os` 9, `nn::mem` 3, `nn::util` 3, `nn::hid` 2, `nn::prepo` 2,
`nv` 2): each is a `u64`/`s64` parameter, mangled `y`/`x` where the console has `m`/`l`
(`nn::fs::ReadFile`, `nn::os::CreateThread`, `nn::mem::StandardAllocator::Allocate`, the message
queues, `nv::InitializeGraphics`). Each has exactly one Windows definition that reads the same with
`long long` taken as `long`, and no runtime import keeps the console's name through a spelled-out
32-bit `long`. The fix is therefore one rule in `image_tables.s` on a Windows target (the console's
name pointing at the `long long` definition), not 41 runtime edits. The C library names and the
C++ ABI names the loader resolves are the same on both and are not compared.

**Lifter: semantic gaps, round 2 (2026-10-27, branch `claude/lifter-semantic-gaps`).** These are
the gaps the first round left on the boot slice, each with the lifter change that closes it in
semantic mode. Byte mode is unchanged: `gen_lift.py --add` on 6,000 random candidates with this
code leaves `src/lifted/` as it is.
- **`x8` results in tail position.** `b callee` to a function returning through `x8` becomes
  `return sret::callee(...)` when `x8` is the function's own incoming result address. Otherwise
  it becomes a placement new at `x8` followed by a return.
- **Stack arguments.** abi_facts counts the 8-byte slots of the caller's frame that a function
  reads (`stack_slots`; cache version 10). A semantic definition declares them as `u64 st0, ...`
  after `p0`-`p7`, padding the integers to eight, and a call passes the caller's outgoing slots
  `*(u64*)(stk + off)`. The pcode oracle compares those slots as arguments 200+i. Review fixes:
  - a definition declares as many slots as abi_facts tells callers to pass, and `ldp`/`stp` name
    each slot (an `ldp` of two slots had declared one);
  - the oracle's out-parameter pattern took a stack slot's pointer from a register that does not
    exist, so every call passing a local's address on the stack faulted in both runs and nothing
    after it was compared.
- **Frames.** `sub sp, sp, #1, lsl #12` counts its shift (481 functions had fake stack slots).
  A shrink-wrapped prologue (an early check, then the frame) starts at the first frame
  instruction, and an exit before it keeps the entry's sp (a review fix: the adjustment was
  counted twice, and `stp x29, x30` was lifted as a store past the locals).
- **Jump tables** whose index or base reaches the `br` through `mov` copies are found.
- **Float widths.** A float register's width is its first read, ignoring frame saves
  (`stp d9, d8`) and following `mov vD.16b, vS.16b`. Flat-mode float locals read before any write
  start at 0. `ldr h`/`ldr b` + `ucvtf` is lifted as one integer-to-float conversion. A page
  address used as a value (`adrp` alone) is a global.
- **The oracle** fails a lift whose return address was overwritten ("return outside") where the
  original returned. Before, that input counted as inconclusive.
- On the boot slice with the run-time roots (2,366 `U` functions, 1.40 MB, before the boot-match
  round 2), 2,295 lifts were accepted (97%).

**Port: the Windows wide-text helpers (2026-10-27, branch `claude/port-wide-text`).** The last five
places the Windows scan flagged were the game's `char32_t` text handed to the C library's wide
functions: `CLabel.cpp`'s two `swscanf` calls (`"%x"` for a font glyph's code in a `@N<hex>]` tag,
`"%f"` for a sprite's scale in `@name:scale]`) and `fn_7100067460`'s `wcsstr`. On Windows those take
16-bit units. New header `include/base/Char32.h`: `char32_scan_hex`, `char32_scan_f32` and
`char32_strstr`. Under `__WCHAR_MAX__ > 0xffff` each is the original call, `always_inline` (the
callers ignore the scans' results, so the `== 1` folds away); otherwise the scans narrow the leading
ASCII (128 bytes at most) and `sscanf` it with the same format, and the search compares 32-bit units.
`fn_7100067460`'s string type is now `const char32_t*` (its mangled name changes from `PKw` to `PKDi`;
`check.py` matches it by address, still `O`). Evidence: `baseline.py` (both files unchanged),
`check.py --update` (no REGRESSION, DUPLICATE or RELOC; no status change), a native Windows test of
the helpers built with llvm-mingw (hex, float, a non-ASCII stop, empty input, the search with an
astral character and an empty needle: all pass), and both sweep targets compile the two files. The
header joins `include/types.h` in the scan's exemptions; the Windows scan now finds nothing.

**Lifter: port_lift's worklist as a slice (2026-10-27, branch `claude/lifter-worklist`).**
`tools/port_link.py`'s `boot_missing.csv` follows direct calls from `nnMain` only, so it missed
what the boot reaches through the pointers `nnMain` hands the SDK (main, the per-frame and the
shutdown function) and main's four callbacks: the 127 functions of the port's round 3. port_lift
now builds its own worklist with `--slice boot` (the default). It takes the `U` functions reachable
by direct calls from `nnMain`, from those seven run-time roots, from the graphics loader and
selector (`fn_71001b0050`, `fn_71001b0f24`, `fn_71001b0f7c`, `fn_71001cc608`), and from the four
functions the Linux run trapped in through pointers (`fn_7100075730`, `fn_710008a460`,
`fn_710002f0dc`, `fn_71000666b0`). That is 2,226 functions, 1.26 MB, ordered by callers in the
slice. The roots themselves are listed whatever their status (`fn_7100000d40` and `fn_7100000cf0`
are game code that `data/functions/` marks `L`). `--slice all` takes every `U` function except
the SDK imports (16,002, 8.08 MB), and `--worklist CSV` the old way.

**Lifter: arguments lost in byte-mode lifts (2026-10-27, branch `claude/lifter-passthrough`).** A
byte-identical lift can rely on argument registers its C never names: on AArch64 a register the
caller leaves set reaches the callee anyway, so the bytes match, but an x86-64 compile passes only
what a call names. abi_facts' liveness against the definitions found 3,592 lifted definitions that
read more argument registers than they declare (164 on the boot path), and the port M1 session's
call-site audit found the other half: 8,885 callee declarations naming fewer registers than the
callee reads, among them the graphics system's constructor called without its object.
- **Pass-through.** At each call, byte mode now names every argument register the callee reads
  (`tools/abi_facts.py`) that the function still holds, its own incoming arguments included, and an
  indirect call gets what the function's callers set for it (abi_facts' new `fwd`: registers the
  callers write before the call and do not read again; `fn_7100005c40` takes the float its callers
  pass and forwards it to its virtual call). A callee returning through `x8` gets
  `UnkResult f(...) { return sret::callee(...); }` (C++17 elision keeps the tail call) or a
  placement new at `x8`. Every variant is tried with pass-through first and, where it named more,
  as before, so nothing that matched stops matching. On a 2,000-function sample (seed 3) 1,455
  match with pass-through against 1,452 plain. Regenerated: 46,443 byte-identical lifts (46,380
  before), 73 functions `U` to `O`, none lost; definitions reading more than they declare went
  from 3,220 to 36 and callers dropping a held register from 8,362 to 95 (103 functions, 10 on
  the boot path).
- **capstone 5.0.7's register access is wrong for some aliases** (the M1 session's report, then a
  census over `main`): `cmp`/`tst`/`cmn` (147,844 sites) mark their first register written and not
  read, and `lsl`/`lsr`/`asr`, `ubfx`/`sbfx`/`ubfiz`/`sbfiz`, `sxtb`/`sxth`/`sxtw` by immediate and
  `fmov #imm` mark their destination read. `abi_facts.Insn.of` corrects them by mnemonic (`movk`,
  `bfi`/`bfxil`, lane inserts and `fmla` really read theirs); `fn_71001d2dcc` reads 2 integer and
  1 float argument, not 1 and 2.
- **Review fixes** (three reviewers over the diff, each finding checked): lazily computed facts
  depend on the order they are asked for (a call cycle leaves the function that closes it
  without facts, and the caller's result is kept), so every tool now takes them from one table
  computed for all functions in address order (`AbiFacts.complete`, cached); a switch's `br` had
  no successors, so registers read only in its cases were never arguments (a bug from before);
  callers' evidence crossed into a block nothing falls into and missed Clang's FPR copy
  `mov v0.16b, v8.16b`; float widths come from the first read (`fcvt d0, s0` writes first).
- **What byte mode cannot express** (an incoming value on one path and a call's garbage on the
  other, a plain fallback) gets a semantic lift: `tools/port_lift.py` lists the src/lifted
  functions that still lose arguments (`build/port/passthrough.json`: definitions, and calls that
  drop a register the caller holds, from abi_facts' new `held`), adds them to its worklist and
  marks an accepted lift `"overrides": "src/lifted/lifted_XXX.cpp"` in the manifest, for
  `tools/port_link.py` to link instead (the M1 session's change). Semantic mode itself keeps the
  function's argument registers from the entry on and joins a value with a clobbered path instead
  of passing 0. On the 103 left after the regeneration, 93 semantic lifts are accepted, all 10
  boot-path ones among them; 8 fail at a call's float argument and 2 are inconclusive (a return
  outside the function).
- The three "float to `char*`" callers (`fn_7100005c40`, `fn_7100573200`, `fn_7100a1fe98`) never
  passed `x0`; they do now. The `lift_guard` constructors are placed by `tools/port_addresses.py`
  (C1/D1 from matched code); the one name left, `lift_guard::Singleton::~Singleton()`, is
  `fn_71000407f0` (96 KB, `U`, over the lifter's size cap). `fn_7100394bd4` is a bare `ret`: it
  reads no register, so the link's alias from the header's `void*` is exact.
- Also: `port_lift.py --only/--top/--debug` write to `build/port/lifted_partial/` (before, a debug
  run replaced the set the port links); the P-code and facts caches are replaced in one step.

**Port: the Windows compile check (2026-10-27, branch `claude/port-windows-sweep`).** The owner
decided to keep Linux as the only running target until the title screen and to compile-check
Windows alongside it. Windows means MinGW-w64 (the Itanium C++ ABI the game's vtables need), so the
toolchain is llvm-mingw 20261006 (Clang 23.1.3, UCRT; published 2026-10-07; SHA-256
`317492c4…d17a`, GitHub's published digest, in `data/toolchains.json`), fetched by
`tools/fetch_toolchains.py llvm-mingw-20261006`. `tools/port_sweep.py --target x86_64-w64-mingw32`
uses its `clang++`, libc++ 9.0.1's headers over MinGW-w64's C headers by default (Win32 thread API),
no `-fPIC`, and writes to `build/port/windows/`. Findings:
- **13 files failed**, all one problem: a local `operator new(unsigned long, void*)`. Placement new
  must take `size_t`, `unsigned long long` on Windows. Spelled `u64`.
- **COFF definitions.** The first run reported 2,417 symbols defined in several objects: inline
  functions, their statics and MinGW's `.refptr` stubs are COMDAT on COFF, and `llvm-nm` prints
  them as strong. The sweep now drops symbols in COMDAT sections (`llvm-objdump --syms`, the
  section's AUX `comdat` field) and demangles the rest with `llvm-cxxfilt` (`llvm-objdump -C` does
  not demangle COFF symbol tables). What is left matches the Linux sweep: `fn_7100227448` and the
  three frozen sample pairs.
- **Silent LLP64.** The compiler sees none of it, so the sweep now scans `src/` and `include/` (code
  only) for `long`, `l`/`ul` literals and `wchar_t`. Fixed, all byte-identical (`tools/baseline.py`,
  42 files unchanged; `check.py --update` clean): two iterators' `long` difference type and offsets
  (`src/game/CActor.cpp`, `src/game_s7/UnkIter.h`), `u64 crc = ~0ul` (`0xffffffff` on Windows, in
  `fn_7100001570`), `capacity * 8ul` (a 32-bit product) in four files and `include/base/UnkHashMap.h`,
  two `0xffffffff00000000ul`. Generated sources had nothing to fix.
- **Left:** the game's wide text is 32-bit `char32_t`, and `CLabel.cpp` (`swscanf`) and
  `fn_7100067460` (`wcsstr`) pass it to the C library's 16-bit wide functions on Windows. It needs
  `char32_t` helpers, not a spelling change.
- The Win64 calling convention changes no source; it matters to `port_link.py`'s generated
  assembly. The next Windows steps are listed in `port/README.md`, "Windows (compile only)".
- Running it: the machine ran short of memory during the session (another session's job held
  80 GB), and 10 files failed with "insufficient system resources"; rerun with fewer `--jobs`, they
  compile.

**Port M1: boot-match round 2, and the boot past the game's main function (2026-10-27, branch
`claude/port-m1-game-main`).** The game's main function `fn_7100000980` is matched (`src/engine/HostArguments.cpp`), and
`nn::nfp` has stubs for all 26 imports (no reader: the library initialises and lists no device).
`nnMain` passes three callbacks as function pointers (main `fn_7100000980`, per frame
`fn_7100000d40`, shutdown `fn_7100000cf0`) and main stores four more, so the direct-call slice from
`nnMain` misses their callees. From those seven roots, 139 functions trapped (149 KB). The round-2
workflow took them, plus `fn_7100deac9c` and `fn_7100219294`, whose lifted copies drop an argument.
It ran 14 agents by translation unit and a skeptic for each claimed `m` (61 agents, 13.6 M tokens,
2.9 hours): 95 `O`, 47 `m`, three left (`fn_7100e91f18` 4.7 KB, `fn_7100c9eb08` 3.5 KB,
`fn_7101100e04` 3.6 KB). Three `m` claims were refuted and fixed:
- `fn_71000934b8`, the attribute insert behind every `UnkReflAttrs::Add`: it read the value after
  the slot's destroy, which writes and frees.
- `fn_710013b0e0` and `fn_710007e7f8`: they stored the GOT slot `g_unk_7101cbb250` plus 0x10 as
  the string allocator's vtable. The slot holds the vtable `g_unk_7101a194b8`, now declared once in
  `CRntString.h`.
- `fn_71011ed1e8`: a `u64` field at +0x30c was aligned to +0x310. It is now two words.

Unit 167 (0x7100150b50) holds the graphics system's entry points. Seven of them are now written by
hand (all `O`), because their lifted copies drop arguments: the creator called the constructor
without the object, the scale setter dropped the value, and five factories called their
constructors without the object. They are in `src/engine/UnkGraphicsApi.cpp`, with
`include/engine/UnkGraphicsSystem.h`.

The boot now runs through the game's main function into the start-up `fn_7100015c70`. With
`DREAD_TRAP=log` it reaches six functions without a source (`fn_71000a4598`, `fn_71000a497c`,
`fn_7100075730`, `fn_710008a460`, `fn_7100087124`, `fn_710002f0dc`), then dereferences null in the
lifted `fn_710008cbe8`, under `fn_7100015fc0`, after a trap returned 0. Four of the six are reached
through pointers, which the direct-call graph does not follow. Round 3's static list from the same
roots holds 127 functions (74 KB).

Findings for the port:
- **Byte-identical lifts drop arguments at call sites too.** A lifted file declares each callee
  once (`namespace callee { ... }`). 8,885 of the 37,525 declarations name fewer registers than the
  callee reads (8,627 integer, 692 float), and 57 call a callee that returns through `x8`; 277
  boot-path functions make such calls (`build/port/callsite_audit.json`). On the definition side,
  3,592 of 46,379 lifted definitions read more argument registers than they declare (164 on the
  boot path; `build/port/passthrough.json`). AArch64 keeps the registers, so the bytes match;
  x86-64 loses them. The scripts are in `build/port/audits/`, sent to the lifter session for its
  byte-mode fix.
- **`tools/abi_facts.py` under-counts arguments where capstone 5.0.7 gets an alias wrong.** `cmp`,
  `cmn` and `tst` report their first operand as written, not read; `fmov` from an immediate and the
  `ubfm`/`sbfm` aliases (`lsl`, `sxtw`) report their destination as read. Measured over 101,366
  functions with a corrected decoder: 216 read more integer arguments than reported, 1,961 fewer,
  and 959 fewer floats (`build/port/abi_alias_impact.json`). The oracle uses the same facts on both
  sides, so it cannot see a dropped argument. Sent to the lifter session, which owns the file.
- **Clang 18 hangs on an `asm` label on an inline constructor.** `src/base/UnkResFactoryRegistry.cpp`
  aliases the complete-object symbol for Clang 9, and newer Clang gets a function that runs the
  constructor.
- **Function-local statics in header-inline functions** (the string-pool and file-path-pool
  holders, the reflection type ids, `NoContact`'s empty contact, the game manager's id table) are
  weak symbols in the executable, while lifted code reaches the image's copies. The pool holders
  are guarded by the pool global, which lives in the image. A type id compared across hand and
  lifted code would differ. `tools/port_addresses.py` could place these symbols from matched code,
  and `tools/port_link.py` could bind them with `--defsym`, which overrides an object's definition
  in lld. Not done yet.
- **37 functions are `L` through the `runtime?` neighbour inference** of `gen_functions_csv.py`.
  `nnMain` and `nninitStartup` count as runtime, so the unnamed run around them is too. Ten of them
  are matched game code (`nnMain`, `nninitStartup`, `fn_7100000980`, ...), and they stay `L` because
  `check.py` never updates an `L` row. `port_link.py`'s host symbols (`fn_7100000250` is
  `__cxa_atexit`) depend on that `L`, so fix that first.

**Port M1: the boot-match batch, and the boot past the system start-up (2026-10-27).** The Linux
executable's first run trapped at the allocator, and `boot_unlifted.csv` listed 213 boot-slice
functions with neither a source nor a semantic lift. A workflow matched them by translation unit
(14 agents over file-disjoint unit groups, `tools/tu_map.py` naming each new file), then sent one
skeptic per claimed `m` to refute "same behaviour": 112 `O`, 82 `m`, 19 left (2 to 16 KB, not
attempted in the time-box). Two `m` claims were refuted and fixed:
- `fn_71000e7b5c`: the accessor's value was scoped inside a branch, so its slot was reused for the
  walk object.
- `fn_7100b414f8`: `fn_71004f1748` was declared returning by value, where it constructs in place,
  so the arguments were in the wrong registers.

The batch had added 40 conflicting declarations. A second workflow (seven file-disjoint groups,
`hub_migrate.py --plan` first, `baseline.py` as evidence) gave each one header: `CColor4F`,
`CVector4D`, the GUI label and sprite headers, `UnkReflObjectRead.h`, `COBox2D.h` and others.
Conflicting symbols went from 156 to 147. A full `baseline.py` compare of the 1,497 hand-written
sources showed one change, `CGUIManager.cpp` building again after hub batch 6 retyped its event.

The boot then went three traps further:
- **The allocator.** The new `fn_7100083ba0` sat in a file of its own that read the allocator
  through `g_unk_7101d58ed0`, while its unit's file kept the allocator in a `static`: two objects
  on the port. It moved into `UnkDefaultAllocatorGlobal.cpp`, which keeps its state in the image
  off `__aarch64__`, because the lifted entry points read it there.
- **`nninitStartup`.** It sets up the heap and the allocator, and the SDK calls it before the
  static initialisers: matched, given the three `nn::os` memory calls, and called by the loader.
- **The sub-allocator's constructor.** Its mangled name had no address; the header now binds it
  with an `asm` label, which Clang 18 calls and Clang 9 ignores.

`tools/port_addresses.py` had placed five SDK imports at their PLT stubs, and they linked as
aliases of themselves (now skipped). Crashes now print where they happened (`fault.cpp`). Port
fixes from the agents' notes: x86-64 definitions for the libc++ sort helpers that
`fn_71011bc0b0.cpp` names by `.set`; a port copy of lauxlib's static `getS` (`fn_71010a57d0`);
`fn_7100077038`'s float argument, which its caller had passed implicitly.

The boot now runs `nninitStartup`, every static initialiser, `nnMain` and the whole system start-up,
and traps at the game's main function `fn_7100000980`. Progress: whole binary 54.21% `O`, boot
slice 28.27% `O` and 34.95% usable (25.19% before). The agents' notes list helper types now
repeated across files (the object-file reader family in 8 files, the game manager's id table in 4,
the 2D collision types under three names): a header batch for the next round.

**Port M2: drawing (2026-10-27).** The 38 NVN functions the game calls that the shim lacked were all
on the drawing side. Each one's arguments come from its call sites (`tools/nvn_procs.py --sites`
now lists them), read in the engine wrappers around it. The numbers come from the engine itself:
it keeps render state in reflected enumerations whose value names mercury-engine-data-structures
lists (`blendFactor::EBlendFactor`, `blendOp::EBlendOp`, `cmpMode::ECmpMode`,
`stencilOp::EStencilOp`, `cullMode::ECullMode`, `fillMode::EFillMode`, `idx::EPrimitiveType`,
`idx::EDataType`) and converts each in one place: `fn_710015a500` (blend factors), `fn_710015a680`
(equations), `fn_710015a8c0` and `fn_710015acf0` (compare), `fn_710015afd0` and the table at
`0x7101647dc8` (stencil ops), `fn_710015ab00` (cull), `fn_710015aa80` (fill), `fn_7100154020` and
the table at `0x7101647bac` (primitives), the same function (index types). Vertex formats reuse the
texture format numbers (`fn_7100161e60`: `0xa`, `0x16`, `0x22`, `0x2e` for one to four floats,
`0x25` RGBA8). The depth mode 0 and window origin 1 have no names, but the game's deferred shaders
rebuild clip-space z as `2 * depth - 1` and map row 0 to clip-space y = +1, so the shim uses
`VK_EXT_depth_clip_control` and flips the viewport. Two readings rest on the engine's names alone:
SUBDST/SUBSRC as subtract and reverse subtract, INCRSAT/DECRSAT as clamping. The engine binds
render state only when it changed and alternates two command buffers, so the shim keeps one bound
state for the queue. Command buffers are Vulkan command buffers (dynamic rendering, Vulkan 1.3
dynamic state, a pipeline cache keyed by the rest); images stay in the GENERAL layout with a full
barrier at each render-target change, because the deferred passes sample their targets; pools
import the game's storage (`VK_EXT_external_memory_host`); uniform updates are shadowed and copied
per draw; one timeline semaphore carries submits, fences and syncs. Drawing every program found that
empty and mismatched sampler slots need a placeholder of the declared kind (694 `samplerCube`, 16
`sampler2DShadow`). `nvn_startup_check` with the shaders: a clear and texel writes read back, all
671 programs drawn in full and depth-only (1,342 draws, 1,318 pipelines), no validation message on
lavapipe. Open: the alpha test (Vulkan has none; the build step must add a discard), a surface for
the window, asset texel upload, the sampler enumerations.

**Port M2: textures, pools and samplers (2026-10-26).** The format numbers come from the engine.
Its converter `fn_7100161410` turns a texture header's (type, sRGB flag, channel layout) into the
number it passes to `nvnTextureBuilderSetFormat`. The layouts are the engine's reflected enum
`EComponents` (`fn_710016468c` registers `E_COMPONENTS_R8_G8_B8_A8`, `_R32F`, `_U16_V16` ...).
That gives `0x1`, `0xd`, `0x12`, `0x25`, `0x6d`, `0x3b`, `0x3c`, `0xa` = R32F, the BC numbers,
and with the sRGB flag `0x37`/`0x38`/`0x46`/`0x48`. Correction from the review: the first version
took mercury-engine-data-structures' XTX numbers as NVN's, which holds only for linear files, so
2,063 sRGB base-colour textures would have failed. The engine's render-target table at
`0x7101647fec` (17 entries) gives the colour targets `0x25` and the depth target `0x35`. The
swizzle function `fn_71001604e0` shows one-channel, two-channel and depth (`0x33`/`0x35`: R,R,R,A).
`0x3f` is a colour render target, since the start-up registers a fast clear for it (provisionally
RGBA16F). The game's strings name no NVN format. Seven render-target numbers stay unknown and fail
by name. The targets come from all 5,754 of the game's textures: 1 is 2D (5,480); 8 is a cube map (274,
square with six faces and BC6H in every case). A sampler is made on the stack, registered and finalised at once
(`fn_710015e9fc`), so the sampler pool owns a copy. The game's filter, wrap and compare values are
the engine's packing (it remaps compare 1-7 into NVN's numbering), which nothing names. With
textures in, the start-up replay reaches no unimplemented function; the 38 called functions still
missing are all on the drawing side.

**Port M2: the NVN shim's start-up path (2026-10-26).** The graphics start-up `fn_71001587f0` is
matched in `src/game_s7/`, so its NVN calls are known with their arguments. The 523 names of the
table loader `fn_7100154770` map to the globals they fill through each GOT slot's relative
relocation, so every `g_unk_716e29xxxx` the start-up calls is a named NVN function.
`tools/nvn_procs.py` decodes the loader's seven-instruction steps from `main.elf` and writes
`data/nvn_procs.csv`, which agrees with the Ghidra reading on all 523. Counting call sites needs a
window: an adjacent `adrp`/`ldr` pair misses `BindUniformBuffer`, whose `adrp` the compiler
scheduled earlier. A window of up to 8 instructions that stops when the register is overwritten
finds 146 functions used. The loose scan's `SamplerPoolInitialize` hits came from a stale register.
The engine reads its own fields right after the 0x100-byte NVN memory pool object (`+0x100`, the
storage base, `fn_7100160f60`), so the shim keeps all its state in side tables keyed by the storage
address and never writes into game storage. The frame loop is `fn_7100159920`, `fn_7100159ac0` and
`fn_7100159bf0`; no caller reads a sync wait's or an acquire's result. `nvnDeviceGetInteger`
answers eleven names; their meaning comes from how the game uses each: `0xf`/`0x10` descriptor
sizes, `0x11`/`0x12` reserved descriptors, `0x24`/`0x25` maximum pool sizes, `0x13`/`0x14` command
and control alignments, `0x51` the queue's command memory size. `nvn_startup_check` replays the
start-up and three frames on lavapipe with validation: everything passes, and it stops at 11
texture functions. A static library drops objects that only register themselves from static
initialisers, so each part has an explicit `Register…Procs()`, called once by the entry layer.

**Port M2: the binding layer against Vulkan (2026-10-26, var-flotsam/dread-decomp#217 and the Vulkan
check).** With the Linux image from #214, the binding layer ran for the first time. `nvn_manifest_check`
covers every stage: found by its control bytes, layouts and writes checked against the SPIR-V's own
decorations. The review showed the first version compared the manifest with itself; seven damaged
manifests are now caught. With the owner's go-ahead, `port/docker/vulkan/Dockerfile` adds Ubuntu's
Vulkan loader, Mesa's lavapipe and the validation layers. `nvn_vulkan_check` creates every program's
modules, set layouts, descriptor sets (through `BuildWrites`) and full and vertex-only pipelines in
about 10 s. Its first run found two interface rules GL and NVN accept and Vulkan does not.
32 fragment inputs in 16 deferred-lighting and ambient packages are not written by the vertex stage,
or are written narrower; all are inactive in NVN's reflection. Dropping every inactive input broke
628 stages, whose code still names inputs NVN optimised away, so the rewrite drops only those the
vertex stage does not cover (vertex output widths from NVN's type codes 36-39 = 1-4 floats). Two
programs write a wider vector than they read, which needs `maintenance4`. After both: 671 of 671,
no validation message. The install now checks each program's two SPIR-V interfaces itself.

**Port M1: the first executable (2026-10-26).** With WSL 2 and Docker on the PC, the port builds
in a Docker image (`port/docker/`: Ubuntu 24.04, Clang 18) over the checkout. Findings: the
host's libstdc++, and libc++ 18's headers, cannot compile the 18 sources that instantiate libc++
9's internal sort (`std::__sort3` and siblings take a policy parameter since libc++ 15), so the
link builds with libc++ 9.0.1's headers over glibc (`--stdlib libcxx9`) against the host's
libc++; GCC's `crtbegin.o` cannot sit above 4 GB (32-bit absolute relocations), so the
executable uses the PIC start files without being a PIE; and an executable below the image
gets its `brk` heap inside it (`0x7116e6c000`), so it sits above the `.bss` at `0x7173000000`.
The first run maps the dump, runs every static initialiser, enters `nnMain` and the system
start-up, matched for the purpose (`nnMain`, `fn_71000005d0`, `fn_71001325d0`, `fn_7100132730`:
the start-up's callbacks are this unit's hidden functions, so their addresses come PC-relative,
and the SDK calls are declared in the form of the imports' mangled names), and stops at
`fn_7100083ba0`. Several functions `data/functions/` marks `L` with a `runtime?` subsystem next
to `nnMain` and the allocator are the game's own (`fn_71000005d0`, `fn_7100000610`, which builds
the game blackboard, `fn_7100083ba0`).

**Port M1: addresses from matched code (2026-10-26).** The 71 vtables and 177 names the link had
no address for are mostly placed by `tools/port_addresses.py`: for each byte-matching function
the relocation check is asked about each unresolvable symbol as if it were at 0, and the
mismatch it reports is the original's target (no new decoding code: `asmdiff.check_relocs` does
the pairing). 9,297 symbols placed from 89,299 functions; with them `port_link.py`'s residue
is 226 (from 439). The 12 symbols whose uses disagree are real naming problems: placeholder
classes reused across sources for different classes (one `vtable for UnkCallbackSet` symbol
would serve two vtables in a whole-program link, an ODR violation the matching build cannot
see), and `fn_710026ead0`, which calls itself `CRntString::~CRntString` twice. Pitfall met on
the way: a version that only placed port_link's residue read the residue of a link that
already used its output and wrote an empty file; it now places every unresolved symbol.

**Port M1: `nn::prepo`, `movie`, the language and an import check (2026-10-26).** With these the
runtime defines every SDK import the boot slice reaches. `movie`'s player
(`src/game_71001e2acc/`) subclasses the library's Extractor and Decoder and keeps Buffers at a
0x40-byte stride; the port's Buffer is complete and every source has zero tracks, so
`fn_71002233c4` reports its code 5 for each. `nn::oe::GetDesiredLanguage` returns an 8-byte code
that `fn_710119c41c` compares with 14 codes as one word. New `tools/port_imports.py` compares the
dump's import names (with their mangled parameter types) with the runtime's: every implemented
namespace matches completely (285 of 547 imports), which confirms the signatures written from
the callers and the mangled names. Left for later: `nn::audio` 149 and `nn::nfp` 26 (outside the
slice), `nn::os` memory blocks and multi-waits, `nn::util` math tables and gzip.

**Port M1: `nn::hid` (2026-10-26).** All 34 `nn::hid` functions in `port/runtime/`. The
parameter lists came from the dump's dynamic symbols (the imports' mangled names: read locally,
the names only), which settles what Ghidra's pseudocode leaves open (`this`, references,
`nn::util::BitFlagSet<32, NpadStyleTag>` by value, the `IntrusiveList` of node connections) and
is the source to use for every remaining import. From the callers: `fn_710006daf0` supports
full key and handheld (3) on nine ids starting with 0x20; `fn_710006deac` copies the state as
five words (0x28 bytes) and reads full key on style bit 0, handheld on bit 1, and wants exactly
one id connected; `VibrationPlayer::GetFileInfo` returns through x8 and the game reads +0xc.
`VibrationPlayer` moved from `src/game_s1/fn_71000707dc.cpp` into
`include/base/input/UnkHidVibration.h` with the classes' member declarations (the two
destructors and `fn_71000733e4` still match; `check.py` clean). `port_link.py`'s residue no
longer lists an `nn::` function by name.

**Port M1: the system start-up's services (2026-10-26).** `nn::mem`, `nv`, `nn::account`,
`nn::util`, `nn::time` and `nn::diag` in `port/runtime/`, so every import `fn_71001325d0` makes
resolves. From the bytes (capstone over the dump, read only): the start-up initialises the
StandardAllocator at `0x710ab70000` with a 32 MB region and hands it to `nv` through three
callbacks that only shuffle registers, `(size, alignment, user)` to `Allocate(this=user, size,
alignment)`, `(pointer, user)` to `Free`, `(pointer, size, user)` to `Reallocate`; Ghidra's
pseudocode hides the `this` and showed the arguments in another order. `nn::diag::detail::AbortImpl`
is imported twice (4 arguments, and 6 with a result and a format): `port_link.py` used to send
both thunks to the first runtime definition; it now ranks the overloads against the caller's
reference (`runtime_overload`), and resolves `nv`/`movie` imports to the runtime as it does
`nn::` ones. Slice imports with a runtime definition: all but `nn::hid` 12, `nn::prepo` 12,
`movie` 24 and the host's C and C++ library.

**Port M1: `nn::fs` (2026-10-26).** All 22 `nn::fs` imports in `port/runtime/` over host
directories, from the callers' uses (Ghidra, read only): the engine's file object
`fn_7101235090`..`fn_71012355b0` (handle at +8, position at +0x20, cached size at +0x18 with
0xffffffff for unknown), the sound banks (`fn_7100100e74`, `rom:/packs/sounds/<name>.bfgrp` read
whole), the save system (`fn_7100fc3510` mounts "savedata"; `fn_710002dedc` to `fn_710002f2b8`
commit after every change; the writer opens with mode 6 and truncates with `SetFileSize`). The
file object decodes results itself (module 2, description from bit 9; 1, 4000-4999 and 7 get
codes of its own), so the port returns the console's values. `fn_710002e6e0` fixes
`DirectoryEntry` at 0x310 bytes with the type at +0x304 (0 = directory). The start-up
mounts only if the cache allocation succeeds, so `QueryMountRomCacheSize` answers one page, not
zero. libc++ 9's `<filesystem>` needs `_LIBCPP_HAS_MUSL_LIBC` over musl's headers, which
`tools/buildcfg.py` already passes.

**Port M1 step 4 (2026-10-26, var-flotsam/dread-decomp#201, #203 and #206).** Stream D taken
over from the lifter session's notes. (1) `nn::os` and `nn::oe`: every import the boot slice reaches
(30 and 20 with the GOT slots) and every one a lifted source calls, in `port/runtime/`. Layouts from
the engine's thread wrapper `fn_7100066744` (Ghidra, read only): it initialises two light events at
+0x1c0 and +0x1cc of its object and passes +0 to `CreateThread`, so `ThreadType` is 0x1c0 bytes and
`LightEventType` 12; mode 1 is auto clear. The port keeps a pointer to its host object and a tag
word in the game's storage, so a call on an uninitialised object stops with its name. `nn::oe`'s
notification loop `fn_71001327c0` quits on message 4 and re-reads the mode on 30: the runtime sends
4 once after `dread_port_request_exit()`. `std::thread` brings `pthread_*`/`sched_yield` into the
link: `port_link.py` counts them as the C library. (2) `-fno-strict-aliasing` for `dread_game` and
the sweep. (3) The data loader. Measured on 2.1.0's main (`tools/nso2elf.py` parsing): text
`0x123ac10`, rodata at `0x123b000`, data at `0x1a19000`, `.bss` to `0x72a19000`; 145,813 relative
relocations (128,576 into `.text`, all slots in `.data`), 489 PLT, 364 absolute (245
`__cxa_pure_virtual`, the C++ ABI's type-info vtables, a few libc++ stream vtables), 18 GOT, all
symbol relocations imports; 2,760 `.init_array` entries (the port runs its own compiled
initialisers, not these). The PC-relative data the link references runs from `0x71015af379` to
`0x7172a112e8` (5,257 of those symbols in `.bss`), which leaves about 214 MiB below the image or
235 MiB above it for the port's code. The core is freestanding (its own LZ4 block decoder, two
passes so the thunks are counted before they are mapped), so `tools/test_port_loader.py` builds it
into a DLL with clang and lld alone and maps the dump inside the Python process: 26 checks in 2 s,
and a mutation that skips every 64th code pointer fails 3 of them. With the table `port_link.py`
writes, 91,166 function addresses can be named; with the test's half-table, 63,278 of the 128,576
code pointers translate and 65,842 thunks are written. Gotcha: in the Bash tool a Python heredoc
lost its `\t` and `\n` escapes (they became real tabs and newlines in `port_link.py`); patch
scripts with escapes go through the Write tool.

**Boot hubs, round 2 (2026-10-26, branch `claude/boot-hubs-2`).** Five sonnet agents, one per group
of `tools/hubs.py --boot` hubs and round 1's follow-ups, grouped by `tools/tu_map.py` unit on disjoint
files (the `CRntFile` unit and the string pool; path ids and `CType`; the game manager and the
blackboards; the three large manager constructors; the actor, rumble, file-device, registry and
sound hubs). 14 `O`, 10 `NON_MATCHING(minor)`, one dropped, about 10 KB; whole-tree baseline
`bh2-full` unchanged except the five class files that gained functions. Matched: `CRntFile`'s
begin and write (`UnkHashStream_0x38` is now an alias of `base::global::CRntFile`), the
`CFilePathStrId` constructors from a `CRntString` and from a C string (`fn_710001a270`,
`fn_71000166c8`, the latter a delegating constructor aliased under Clang 9), `CType`'s dynamic-type
getter, the game manager's mode test and target pointer, the two `CBlackboard` lookup twins, the
`CGameBlackboard` qualified-name builder, the debug model manager's constructor (1,572 B), the
registry, file-device and rumble-manager constructors. Minor, each checked by an adversarial review
(decompile, `abi_facts.py`, the diff) as same arguments, memory, calls and results: the string-pool
set setup (99.1%), `UnkStrIdVec` reserve (98.5%), `CRntFile` reserve (98.4%), the sanitised path
lookup (76.2%), the `CRntString` prefix assign (43.5%), the music manager's constructor (24.8%;
matches up to its slot loop), `CRntFile`'s byte-swapping read (23.8%), the game manager's object
lookup (16.5%), `CType`'s typed assign (16.4%), the actor's weak-reference getter (10.6%). Dropped:
`fn_7100ee1f34` (2,096 B, the `CSubAreaManager` unit's 0xae8-byte object: a vtable, vectors with
flags 0xf from +8 in steps of 0x28, a 0x1b-flag file-path-handle vector at +0x388 reserved to 0x12,
an inline-buffer block at +0x410, maps at +0x8e0 and +0x930). Levers: the rumble constructor
needed a nested map-head struct (stores relative to the map base), a hand-written guarded static
and its defaults read as words; the file device an `asm("" : "+r"(p))` on a far field's pointer; the
string-pool setup a cached old-bucket pointer and the set typed together with its node pool. The
agents left new conflicting signatures (callees declared locally with their own types); the
orchestrator settled them: one header each, the other declaring files moved with a cast at the
call where the file has its own view of the object (`decl_check`: conflicting 249 to 245, extra
copies 1,903 to 1,892). The read-only zero vector `g_unk_710164779c` got
`include/base/math/UnkZeroVector3.h` (agreed with hub batch 5); the rumble constructor kept the
original's reloads reading it through a `const u32*` cast of the const object. The review's fixes:
one `nn::hid` vibration header instead of two clashing local stand-ins
(`include/base/input/UnkHidVibration.h`), two type names that clashed with other files'
(`UnkPooledChainedMap`, `UnkOverlayModel`), the registry's pointer array as a `base::UnkVector`,
`fn_710007f3b4` returning its string, and `CRntFile`'s element count, a division the port would
trap on when the size is zero (guarded off AArch64). Agents running `decompile.py` at once hit
Ghidra's project lock: retry after a pause. The agents' file writes came out CRLF: normalise
before baselines. Boot slice 1,889 to 1,903 `O` (24.96% to 25.19%, `O`+`m` 27.49% to 28.09%); whole
binary 53.909% to 53.930%. About 20 minutes of wall clock for the slowest agent, 1.1 M agent tokens.

**Port M2: shader build step, NVN binding layer, variant selection (2026-10-26, var-flotsam/dread-decomp#202
and #204).** Three pieces of the graphics stream, each reviewed by an adversarial subagent before its
PR. (1) `tools/bshdat.py --install DIR` is the port's build step (`shaders` target): every stage
compiled once for Vulkan with the loader's rewrites, read back by a small SPIR-V reader and checked
against NVN's reflection (bindings, the `Shader` capability only, block sizes: NVN's size is the
SPIR-V layout's span rounded up to 16 in all 2,891 active blocks), then `shaders.manifest`. FXAA
3.11's `FxaaTexOff` passes a parameter as `textureLodOffset`'s offset; every call passes a literal,
so the fixup turns the function into a macro (`ConstOffset` in the SPIR-V, same texels). The first
version quoted the two shipped lines in a tracked file; the fixups are now regular expressions over
the construct. All 1,342 stages compile. (2) Reading the engine: `.bshdat` files are
`msapi::res::CProgramResource` (`fn_71001b2e60` registers the extension). The engine parses each
stage blob's reflection itself (`fn_710015c9b0`, including uniform names; NVN has no
uniform-location API). It copies section 0's code part into GPU memory and passes NVN
`{code address, control pointer}` per stage (`fn_710015d760`). Section 0's record words +0x30,
+0x34 and +0x38 are the control offset (0), the code offset and the code size. So the shim recognises
a stage by its control section: FNV-1a 64 over the first 2,048 bytes separates all 1,010 distinct
ones (1,342 stages; shorter prefixes give 764). Stages with one control section have the same code,
SPIR-V and interface. The program record's empty slot 0 is the engine's stage type 2, bound as NVN
stage 2 with bit 4 (geometry). Call sites through the NVN table (code loading each slot's GOT
entry): 150 of 523 functions are used. Bindings use only
`BindUniformBuffer(cmd, stage, slot from the reflection, address, size)` (`fn_710015d900`),
`BindTexture(cmd, stage, slot 0-15, GetTextureHandle(texture id, sampler id))` (`fn_7100161250`),
`UpdateUniformBuffer` and the two pools. A depth-only pass unbinds the fragment stage with
`BindProgram(cmd, null, 2)`. `port/nvn/` (`dread_nvn`) holds the manifest reader and the binding
layer, compile-checked only. (3) Variant selection: `fn_71001b0050` turns the variant map into
`{key, key & key2, program}` entries and three engine-ID-to-name tables (IDs above 0x11a less
0x2cd). The header's `+0x18` names the default program, and its `+0x20` list is a third, empty, name
list. `fn_71001b0f24` returns the last entry from the top down to entry 1 whose `key & key2` equals
the key, else entry 0, else the default. The renderer keeps the current key at `+0x317af78` of its
context (`fn_71001cc608` applies it; set by `fn_710014d1a0`, `fn_71001cafa0` and `fn_71001d4be4`,
reset by `fn_71001cde48`). The shim needs nothing for variants; the loader and selector are `U` and
belong on the lifter's list. Khronos Vulkan-Headers 1.4.328 are pinned. They were installed into the
owner's real cache by a one-shot scheduled task; the first attempt unregistered the task before it
finished, so wait for the task's state instead of sleeping.

**Port decision 3 (2026-10-26, the owner).** Vulkan is the first graphics backend and the shaders
are compiled to SPIR-V at build time from the user's dump (`tools/bshdat.py --compile`'s Vulkan
mode, the FXAA rule added there), so the executable carries no shader compiler. OpenGL was set
aside: 1,298 stages need the NVIDIA-only `GL_NV_gpu_shader5` under GL, and macOS stops at GL 4.1.
glslang 16.6.0 was installed into the owner's real tool cache from this session through a one-shot
scheduled task (tool shells only see a sandboxed `%LOCALAPPDATA%`).

**Boot hubs, round 1 (2026-10-26, branch `claude/boot-hubs-1`).** Four sonnet agents, one per group
of `tools/hubs.py --boot` hubs on disjoint files, each matching its functions and declaring them once
(the `dread-match` loop, time-boxed): nine `O` and one `m`, 2.4 KB, every declaring file moved onto
the new header and byte-identical (whole-tree baseline `bh1-full`). The string-pool holder's
destructor `fn_7100080004` is `O` and its constructor `fn_710007fe68` (55 unmatched boot callers)
`m` at 98.1% (one `mov` scheduled before a pre-indexed store; an `asm("" : "+r"(p))` barrier after
the first store moved it from 20% to 98.1%); the `CBlackboard` section lookups `fn_7101047270`,
`fn_71010472e0`, `fn_7101047208`; `base::global::CRntFile`'s seek `fn_710007c00c` (the name is the
unit initialiser's class string); the navmesh cell's edge lookup and its const twin; the sound
system's command-queue write; the game manager's id-map lookup. Levers: a keyed array's lookup
matches when written as `Find` returning `&values[i]` or `&values[count]` and compared with
`&values[count]` by the caller (two agents found it independently; an index loop, `it == end` or a
`CStrId::data` compare let LLVM fold or hoist), with the ids compared as raw `u64` words; a flag
tested as `u8` with `__builtin_expect(flag & 1, 1)` gives `tbz` where `bool` gives `cbz`. Clang 9
ignores an `asm` label on a constructor or destructor (callers still name C1/D1), so the definition
aliases the C1/D1 symbols to `fn_<addr>` under `__clang_major__ <= 9`, as
`src/game_s7/fn_7100442780.cpp` does; the port's compiler honours the label. `tools/ghidra/decompile.py`
fails from a worktree under `.claude/` (Ghidra rejects a path element starting with a dot): run it
from the main checkout. About 70 minutes of wall clock for the slowest agent, 0.96 M agent tokens.
Boot slice 1,881 to 1,889 `O`.

**`hub_migrate.py --plan` (2026-10-27, branch `claude/tools-hub-migrate-plan`).** Batch 6's mechanical
pass took an hour because `--header`/`--declaration` take one symbol per run. A plan file of
`SYMBOL<TAB>header<TAB>declaration` lines now runs every symbol at once. Each edited file's text is
rendered from its original with the kept symbols' edits. One baseline is saved. In each round, only the
files whose text changed and the includers of headers whose text changed are rebuilt. A changed file
goes back alone; files that still differ drop the symbols whose header edits reach them, and the rest
are tried again. Symbols dropped together with others are retried one run each, so the result equals one
run per symbol. Checked on two plans, from a clean tree each time: three unrelated symbols gave the same
diff as single runs (70 s against 80 s; the symbols share almost no files), and four vtable globals in the
same three files took 3 s against 11 s. With a plan, the generated sources that include an edited header
are listed and the smallest two per header are rebuilt, the check batch 5's reflected-type globals
needed.

**Hub batch 6 (2026-10-26, branch `claude/hub-batch-6`).** Batch 5's procedure again, on about 70
symbols plus batch 5's review follow-ups: a whole-tree baseline (1,406 files), one scripted
`hub_migrate.py` run over 65 chosen signatures (about an hour: one symbol per run, since `--header`
and `--declaration` take one; 61 file declarations migrated), seven sonnet agents on file-disjoint
sets, an adversarial review and one agent for its fixes. decl_check: conflicting 220 to 156 (256 to
158 extra signatures), extra copies 1,819 to 1,574, header re-declarations 1 to 1. The 48 `ITEM_`
properties' names come from their static initialiser's `InitPropertyEx` strings (a small script made
the list); their readers viewed each property as an item-definition class, now two accessors of the
property's own qualified id. `g_unk_7101d5e2e0` (57 files, 49 of them `char[]` passed to a local
registration prototype's `char*`) became one `CType` with `&` at the calls. A hidden-visibility
attribute is load-bearing in a header like in a local declaration: without it the users load the
global through the GOT (86% in two functions). The action nodes put members at +0x2c, inside a
0x38-byte base: a base of 0x2c data bytes with an empty user-provided constructor (never called) is
non-POD, so the derived struct reuses its tail padding as the polymorphic originals do (reflection
sizes CBehavior and CBaseAction 0x30); `= default` would make it POD and push members to +0x30. Two
nodes reorder six zero stores when derived and cast at the call instead. `fn_7100b2c6a0.cpp`'s minor
functions are sensitive to the parameter type of every call in their inlined set-up: an untyped
function-pointer view kept `fn_71011d968c`'s bytes (batch 5) but not `fn_71011cde44`'s; an
asm-labelled block-scope `extern` under another name did, but `decl_check` cannot see it, so the typed
call stays and the functions dropped from 98.2% to 96.5% (same behaviour, still `m`). The review
caught a header comment that had a list's add and remove the wrong way round, an id pointer typed as
a C string, and two placeholder type names (`UnkComponent`, `UnkTrackQuery`) that other files define
differently (an ODR clash for the one program the port links).

**Hub batch 5 (2026-10-26, branch `claude/hub-batch-5`).** The first batch of conflicting symbols
without a hand-written definition: the signature comes from Ghidra's view (read only), the argument
registers and result use from `tools/abi_facts.py`, and the callers' types. 34 symbols. Procedure as
the owner set it: a whole-tree baseline (`baseline.py save --tag hb5-full --all`, 1,395 files, 4
minutes); then `hub_migrate.py SYM --header H --declaration D` for 22 chosen signatures in one
scripted run (about 40 minutes; 22 file declarations migrated, three symbols done outright:
`fn_7100e52cf4`, `g_unk_7101d47350`, the empty element destructor `fn_71002874dc`); then five sonnet
agents on file-disjoint sets for the `LEFT` files (GUI and string ids, math and timeline, reflection
globals and blackboard, behaviour tree and slots, then `CAttack`), and a sixth for the GUI root
global the GUI set turned up; then an adversarial review of the headers. Every agent ended with its
own baseline unchanged; the whole-tree compare stayed unchanged; `check.py --update` clean.
decl_check: conflicting 249 to 224 (307 to 261 extra signatures), extra copies 1,903 to 1,830,
header re-declarations 1 to 1. Findings: the GUI "watch set" and "tree" placeholders were all
`base::UnkDeferredSet<void>` (its out-of-line Remove `fn_71004f3ea4` and Add `fn_71004f3cf0`); the
"named components" lookup `fn_71008640a4` is the collision container's lookup by name (into
`UnkCollisionRecord.h`); `fn_71000267f8` reads only the ids of its source, so it takes the pair
while its target is the pair with the resource; the GUI root (`g_unk_717271e8f8`) got a header with
its layout and its five root-taking functions, and `fn_71011b69d8` takes it. A global the generated
reflection sources declare (`g_unk_7101d5c998`, `g_unk_71727952a8`) must keep their type (`CType`)
in the header, or every generated includer fails to compile: `hub_migrate.py` and `baseline.py`
rebuild hand-written includers only, so `check.py` caught it. One file
(`src/game_aca57c/fn_7100b2c6a0.cpp`) changes its `NON_MATCHING(minor)` functions (98.2% to 96.5%,
an address register no longer reused) when it calls `fn_71011d968c` with the header's typed
arguments, and also with a `void` cast, a named local or a `static_cast` through `void*`; calling the
header's function through a `void (*)(void*, void*)` view keeps the bytes and the one declaration.
The segment constructor `fn_7100125b1c` is bound with an asm label on `UnkSegment2D`'s constructor
(the `CStrId.h` precedent), so callers construct the segment. The frozen Phase 3 sample
`src/flag_id/fn_71005a998c.cpp` now derives its object from the header's `UnkBehaviorTreeAI`
(identical output; `compiler_matrix.py` compiles it with `include/`). Coordinated with boot hubs round
2 (append-only edits to `CBlackboard.h` on both sides; `g_unk_710164779c` left to that session).

**Shader packages parsed (2026-10-26, branch `claude/port-bshdat`; port M2 groundwork).**
`tools/bshdat.py` parses all 478 `.bshdat` packages from their bytes and writes every stage's GLSL
to `build/shaders/2.1.0/<package>/p<key>.vert|.frag` with a `manifest.json` per package; 9 s for
the set, 50 s with `--compile`. A package is an `MSHD` header of seven file offsets (the program
list, a variant map from a key to a program, a pointer to the last program, a zero, two lists of
uniform names and one of vertex attribute names, each name with an engine ID), the compiled stage
blobs (0x80-aligned, padding 0xff) and the tables behind them (lists of 16-byte nodes, 48-byte
program records with three stage slots: slot 0 always empty, then vertex and fragment). The parser
accounts for every byte of every package. Correction to the 2026-10-25 count: the 1,342 `void
main` are stages; there are **671 programs** (287 packages with one, 190 with two, the second
adding `DEFERRED_AS_FORWARD` in 189 of them, one with four). Each stage blob is the NVN compiler's
output: a header, four sections, of which the reflection (active uniform blocks, uniforms with
samplers, inputs and outputs, and the bindings assigned per stage) and the debug section are
decoded. The debug text is the assembly listing followed by a source file table, one
`#MSDB: (<name>:<file id>:<length>:SOURCE_FILE|INCLUDED_FILE)` line and one C string literal per
file; the decoded length matches the record in all 3,279 files. Sources include
`common_funcs.h` (1,272 stages), `common_ibl.h` (629), `common_deferred.h` and
`common_shadows.h` (15); the tool inlines them, and moves `#version` (440 core in 1,327 stages,
450 in 15) ahead of the engine's variant prefix (`#define HAS_NORMAL` ... `NUM_BONE_COMPONENTS
4`), which the compiler received before it. 1,298 stages enable `GL_NV_gpu_shader5`, but none
uses a 64-bit handle or integer: the compiled SPIR-V declares only the `Shader` capability, so the
bindless textures of NVN are an API matter for the shim, not a shader one. With glslang 16.6.0
(pinned in `tools/fetch_toolchains.py`): as extracted 250 of 1,342 stages compile for `-G` and
`-V` and no complete program, because every program declares uniform blocks as
`layout(location = N) uniform Block {`, which GLSL rejects. Replacing that with the binding the
compiler assigned leaves 1,075 stages (409 programs); the rest use C-style casts (`(int) x`, 266
stages, 261 programs), which `#extension GL_NV_explicit_typecast` (glslang implements it)
accepts. With both rewrites 1,341 stages and 670 programs compile for OpenGL and Vulkan; the last
is `pp_fxaa_nx`'s fragment stage (FXAA 3.11's `textureLodOffset` with an offset that is not a
constant expression; NVIDIA's compiler folds it), which needs a hand fix. The binding model the
NVN shim must reproduce: the compiler ignored the block numbers and gave each stage's active
blocks bindings 0, 1, 2, ... in declaration order (all 2,891 active blocks; 69 differ from the
declared number, in the deferred-lighting shaders and where an earlier block is unused); samplers
keep their explicit `layout(binding = N)` (1,904 active; declared numbers 0 to 12 with 11
unused, and the arrays at 8 and 12 reach 10 and 13); a stage has at most five blocks. NVN numbers
blocks and textures per stage and separately, Vulkan has one (set, binding) space per pipeline:
compiled with plain `-V`, 664 of 670 programs put two differently named resources (block type or
sampler names) on one (set, binding), 583 of them within one stage (a sampler and a block) and 81
only across the two stages; one descriptor set per stage with samplers from binding 16
(`--resource-set-binding`, `--shift-combined-sampler-binding`) removes every collision. Not
decoded: blob sections 0 and 1 (kind 0's size is always a multiple of 8, so it may be the
machine code; nothing more is known), the header constants of blob and debug
section, the reflection's last 0x108 bytes and nine record words, the binary debug part
(`0x0000c0de`), what the two halves of a variant key mean (always equal), header `+0x18`/`+0x20`
and program slot 0, and where the name tables' IDs come from (one fixed engine enumeration; the
lists also name uniforms no stage uses).

**Hub batch 4 (2026-10-26, branch `claude/hubs-4`).** Two steps. First, mechanical: the four symbols
`include/` itself declared in two or three headers got one canonical header each with
`hub_migrate.py --header` (`fn_71000839a0` into `UnkVector.h` next to the allocator getter;
`fn_710000051c`, `fn_7100001570` and `g_unk_7101d47200` into `CStrId.h`), and so did the tunable
base constructor `fn_7100136ecc` (into `UnkTUProperty.h`): 71 files migrated, two by hand. One of
those (`fn_7100343f7c`) shows a load-bearing type: storing the CRC64 into a `u64` field instead of
the original `void*` one changes alias info and the code (63.6%), so it casts at the store. The run
also exposed a `hub_migrate.py` bug: a file whose output changed made the tool undo the
`--declaration` it had appended to the canonical header while keeping the 29 files it had migrated
onto it, which then no longer compiled; the fix restores a changed file alone first (its own
declaration next to the header's usually builds as before) and takes all dependants back when the
header must go. Second, five sonnet agents on file-disjoint groups gave the 29 conflicting symbols
with a hand-written definition one declaration with the definition's types; most needed a shared
type, so the step added 19 headers (`UnkTimelineComponent.h`, `UnkSlots.h`, `UnkCollisionRecord.h`,
`UnkSpitAttack.h` merging four local views, `UnkFreedVectorPair.h` replacing five identical copies,
`UnkCallbackSetVector.h` taking about 235 duplicated lines out of two files, `UnkGuiHandle.h`,
`UnkResType.h` out of `UnkStaticInitS7.h`, and others) and inserted declarations into four existing
ones. About 13 minutes and 1.15 M agent tokens; every agent finished with nothing kept. A whole-tree
baseline (all 1,353 hand-written sources, 2 minutes to save or compare) taken before the agents
started is the batch's evidence, and is cheap enough to take before every agent round: unchanged.
decl_check: conflicting 286 to 251, extra copies 2,055 to 1,906, header re-declarations 5 to 1.
What is left has no hand-written definition (39 symbols with a lifted one, 211 with none).

**TU helper fold (2026-10-26, branch `claude/tuinit-fold`).** `include/game/UnkTUInit.h` and
`unk_static_init` (`src/game_5821e4/UnkStaticInit.h`, `src/game_s7/UnkStaticInitS7.h`) each defined
the tunable and named-property statics; both now include one header, `include/game/UnkTUProperty.h`.
The layouts were the same (`UnkProperty2` is `UnkPropertyEx<0x18>`, `UnkProperty3`
`UnkPropertyEx<0x160>`); the bodies differed only in where the vtable store falls relative to the
list link. That order is the original's, unit by unit, and not a property of the class: the same
property class (same vtables, member constructor `fn_71002ef5bc`) is built link-first in
`fn_71004fd8f0` and vtable-first in `fn_7100adcf10`. The property and the list head are distinct
globals, so the scheduler may reorder the stores, and the source order steers it. So each
initialiser keeps both bodies verbatim under two names, and the extended form takes the member
constructor as an argument (one helper instead of one per member size). `UnkChargeComponent.h`'s
own `struct UnkTunable` (one tunable's rates at +0x9c) clashed with the shared one; as
`UnkChargeTunable : UnkTunable` at the same offsets it lets `fn_7100536550.cpp` take
`g_unk_71726e0aa8` from that header. All 197 includers byte-identical on the first compile.
Finding: `__cxa_atexit` is an unresolved callee for `check.py` (no address), so every helper call
spelled that way went unchecked. Respelling the helpers' 24 call sites as `fn_7100000250` added
1,055 checked relocations, 0 mismatched. Folding the first 11 calls of the global family the
other way had silently removed 11 checks, which the relocation totals showed.

**Hub batch 3 (2026-10-26, branch `claude/dread-decomp-hub-batch-3-85bf13`).** The hand pass on
what `tools/hub_migrate.py --header-redeclared` left: 23 symbols, 52 declarations in 43 files, all
a placeholder type the header's real type rejects. Five sonnet agents on file-disjoint groups (by
union of the files that share a symbol, plus the three `src/` headers involved), each verifying with
its own `baseline.py` tag; the orchestrator's tag `hb3-all` (46 files with the headers' includers)
confirmed the whole set byte-identical. Wall clock about 7 minutes, 680k agent tokens; 40 files
migrated, none restored. Fixes were type renames: `nn::os::MutexType` for the mutex wrappers,
`base::UnkAllocator`, `UnkBlockFreeList`, `base::UnkNamedEntry`, `UnkReflValue`,
`UnkTunableRegistry`, `UnkReflTypeIdArg` locals for the reflection statics' masks, and
`UnkWeakHandle.h`'s ref and handle types in place of local copies. One file got better:
`fn_71003f2710` embeds the header's `UnkXform` and reads the cached angle through `GetAngle()`
(77.3% to 91.8%). Findings: a function passed as a template argument (`UnkOutOfLineElement<0x18,
fn_71000a1c60>`) keeps its relocation target only if the template takes the function's own type;
an inline wrapper names a different function. `CFilePathStrId` has no refcounting copy assignment,
so `fn_7101037bc0` keeps a derived view with `CStrId`'s `operator=`. Header follow-ups the agents
reported: the registry setters take `base::UnkNamedEntry*` where two callers hold a `CRntString`
(cast); `g_unk_7101a1a4c8` is `char[]` in `UnkCloneHelpers.h` (users write `+ 0x10`);
`g_unk_7101d47350` (3 signatures) wants a header; the local `fn_710013c3c8` declaration in
`fn_7100eaa7a8.cpp` disagrees with its definition; the reflection statics' `UnkReflTypeIdArg`
parameters read a `u32`. `baseline.py save` also died on its last line when `build/` is a link
outside the worktree (fixed). decl_check: conflicting 308 to 286, header re-declarations 27 to 6.

**Port M1, semantic-lifter gaps (2026-10-25, branch `claude/port-lift-gaps`).** On top of the
semantic lifts: every condition now follows the flags exactly (carry from an unsigned compare of
the result, overflow through a wider signed sum; `ands`/`tst` clear C and V), flat mode keeps NZCV
in `bool` locals so a condition in another block (`cmp; b L` ... `L: b.ls`) reads the right flags,
Clang's compressed jump tables (`adr; ldrb|ldrh [table, index]; add, lsl #2; br`) become a `switch`
with the targets read from the table, a float register only passed on to a callee goes as `f64`
(all its bits, whatever width the callee reads), and an argument register unwritten in address
order is the incoming argument (flat mode's "a call happened" is linear, not per path). Oracle
fixes, each found from a rejected lift that was right: Ghidra spaces `q<n>` 32 bytes apart (room
for SVE), so `v1`-`v7` had been set and compared at the wrong offsets; a float left in `v0` by a
call and read by the next callee is not an argument the caller chose; an input where the original
faults and the lift does not compares only what came before the fault (C may drop or move a load
whose value decides nothing); `__cxa_guard_*` and `__cxa_atexit` have signatures. Result: 1,941 of
2,148 accepted (1,042,620 bytes), the slice's functions without a source 380 to 216. The mutation
test first looked worse (35%): the flag locals are mutation sites that are mostly never read, so
they are excluded now; on the top 300 the rate is back to 49% (83% on fully covered functions).

**Port M1 step 3 (2026-10-25, branch `claude/lifter-semantic-coverage-225e8f`): semantic lifts.**
`tools/port_lift.py` gives the boot slice's `U` functions a source that behaves like the original:
`gen_lift`'s lifters in semantic mode, every variant compiled for AArch64, and the first one whose
behaviour agrees with the original's under Ghidra P-code execution (`tools/ghidra/pcode.py`,
`tools/pcode_emu.py`) compiled for x86-64 into `build/port/lifted/` (gitignored, a manifest with the
generator's version and source hashes; open decision 2 keeps its default). `tools/abi_facts.py`
supplies callee argument registers (with their width) and whether callers use a result.
`tools/port_link.py --lifted` links the lifts and reports the slice with and without them. Result:
1,777 of 2,148 accepted (839,396 bytes); slice functions without a source 2,157 to 380. The existing
lifter already compiled 1,550 of the 2,148, it only lacked a way to accept them; `x8`-result calls
(C++17 copy elision into the caller's slot), guards and exclusives as atomics, `ccmp`/`ccmn` and a
dozen instructions brought the compiled count to 1,957. Findings: the oracle exposed four flat-mode
lifter bugs that byte identity hides (a compare reading a register its own `subs` rewrote, a
post-indexed access using the updated base, a 64-bit read cast to the type of a write elsewhere,
callee-saved spills into the locals dropped as frame saves); its own false alarms came from scratch
registers (only the original's argument registers count), indirect-call targets, recursion, frame
layout (a stack argument compares the object both sides wrote, stack addresses as equal) and
32-bit arguments (low half only, from `abi_facts`' width). A mutation test shows its reach: 45% of
300 injected faults rejected, 79% where the inputs ran the whole function; coverage is the limit
(median 67%). Ghidra parses instruction words in memory with no project, 100 instructions in 40 ms;
`tools/ghidra/` must never be importable as `ghidra` next to the Java packages, so the extractor runs
as its own process.

**Port M1 step 2 (2026-10-25, branch `claude/port-libs`): the vendored libraries in the link.**
`tools/port_link.py` compiles `lib/lua` (all 29 files) and `lib/dlmalloc/mspace.c` as C++ with
`-fno-strict-aliasing`, as the matching build does, into the first link; `port/CMakeLists.txt` has
the same as `dread_libs`. All 30 compile for `x86_64-linux-gnu` with the project's libc++ and musl
headers, and link 1 still finds no duplicate symbol. The Lua and dlmalloc definitions resolve their
`L` addresses through their unique CSV names (369 addresses). `fn_7100000250` jumps to the host's
`__cxa_atexit`, as `include/base/UnkAtExit.h` identifies it (a new by-address host table). It has
470 callers in the boot slice, so a trap there would have stopped the boot at the first static
initialiser. On `main` after hub batch 2 the slice's functions without a definition went from 2,276
to 2,157. Missing `L` functions fell from 127 to 8, among them `nnMain` itself, which the CSV
marks `L`. Lua brings 24 C library functions the tool did not know (`stdio`, locale, time); they
are listed as the host's, and the residue grows from 304 to 375 by such names only. The sweep on the
same `main` compiles all 3,812 sources.

**Shader sources (2026-10-25, branch `claude/shader-sources`).** Asked how other decomp ports
handle shaders (fixed-function consoles generate programs from combiner or TEV state at runtime;
Xbox 360 recomps wrote their own bytecode recompiler), the dump was checked: every one of the 478
`.bshdat` packages carries its programs' GLSL source next to the compiled Maxwell code (1,342
`void main`, `#version 440 core` and `450`, vertex and fragment stages only, 1,301 programs with
`#extension GL_NV_gpu_shader5`, 130 MB in all, about 80% text). The port therefore compiles the
shipped GLSL with glslang to SPIR-V and needs no Maxwell translator and nothing from an emulator;
`docs/PORT_PLAN.md`'s surface table and open decision 3 record it. Left to write: a `.bshdat`
parser (our own; mercury-engine-data-structures has none) and the NVN shim's binding model,
including the bindless handles behind `GL_NV_gpu_shader5`.

**Seventeenth session (2026-10-25, on the PC, branch `claude/cheap-progress`): making hub batches
cheap.** Review of where hub batch 2 spent its 3.8 hours and 4.7 million tokens: the five-step
sequential chain set the wall-clock, every agent re-implemented the baseline-edit-compare loop by
hand (most of the 1,091 tool calls), and eleven adversarial reviews of mechanical steps found 25
nits and nothing to fix. Changes: `tools/baseline.py` (trymatch snapshots for a file set, in
parallel, with copies to restore from; a header expands to its hand-written includers) and
`tools/hub_migrate.py` (the mechanical migration: remove the local declarations, include the one
header, keep only byte-identical files, print the rest as `LEFT` with the compile error; a file that
does not compile before the edit is `BROKEN` and left alone, and a header is left alone when one
includer breaks); `decl_check.Decl` records each declaration's statement span for it; the batch
procedure is `.claude/skills/dread-hubs`. First pass over the 39 symbols a single header already
declared: 66 files migrated for 15 symbols in about a minute of compute, 135 declarations in 85
files left for a hand pass (75 a local placeholder struct colliding with the header's type, 33 a
placeholder parameter type the header rejects); conflicting 310 to 309, header re-declarations 45
to 35, extra copies 2,304 to 2,212, `O` unchanged. Findings: a tool that restores edited files must
also restore edited headers, or an includer broken by a leftover header edit looks "unchanged"
(same error before and after) in every later comparison; `--defsym` in a relocatable link needs its
target defined by an input, so a stand-in for an undefined symbol is a jump thunk, not an alias.
Recommendations recorded in the handoff's "Start here": migrate per file, not per symbol
(disjoint by construction); review by tier (deterministic compare plus a spot check for mechanical
steps, a full adversarial pass for new tools); publish the repository so green PRs merge themselves
instead of stacking. Second part (branch `claude/tuinit-unify`): 75 of the 135 left declarations
were one problem, the two static-initialiser helper families (`unk_static_init` in
`src/game_5821e4/UnkStaticInit.h` and `src/game_s7/UnkStaticInitS7.h`; global in
`include/game/UnkTUInit.h`) each declaring the same type destructors, function records and property
list head, so a unit that saw both got ambiguities. The symbols moved into
`include/base/reflection/UnkReflTypeDtors.h` and `include/game/UnkTUSymbols.h`, which both families
(and `base/UnkCloneHelpers.h`) include; `next`/`head` in the `unk_static_init` property helpers
became `UnkPropertyHead*` (pointer retypes; bodies and store order untouched); 21 files dropped
their local `g_unk_71726db788`; then `hub_migrate.py --header-redeclared` migrated 75 more files
(`fn_7100096604` was declared locally in 57). All 195 includers byte-identical, `gen_clones.py`
unchanged, `O` unchanged; header re-declarations 45 to 27, extra copies 2,304 to 2,114, 50
declarations in 43 files left (placeholder parameter types the headers reject). Finding: a
baseline compare must drop warnings' line and column, since removing a line above a warning moves
it without changing the code (`baseline.py` does).

**Sixteenth session (2026-10-25, on the PC, branches `claude/hubs-2` and `claude/port-m1`): hub
batch 2 and the port link.** Hub batch 2 (var-flotsam/dread-decomp#187): 25 symbols to one
real-typed declaration each in 11 headers, in eleven steps by workflow agents. Six steps whose
declaring files were disjoint ran in parallel in the main checkout and five that shared files ran
in sequence. Every agent kept its objects apart with the new `trymatch.py --obj-dir`, ran no
generator, `check.py` or git command, and reported its files and a commit message. The
orchestrator reran the generators named, ran `check.py --update` once and committed per step.
Matched: `fn_7100216978` (the positioned sphere's position setter: copies the vector into +0x18
and stores each component times ten, truncated, as three `s32` at +0x40; the 0x50-byte sphere is
embedded at +0x80 of transform nodes), the scene toggle notifiers `fn_71001eef58` and
`fn_71001eef74` (tail calls into one of two adjacent delegate virtuals, the bool choosing the
slot), the nav-mesh manager's constructor `fn_7100ebf278`, the animation component's track
progress `fn_7101177ea0`, the `CStrId` constructor from a `CRntString` `fn_7100000850` (still `L`
in the CSV) and, as `m` at 47.9%, the GUI focus setter `fn_71011cf2a4`; `fn_71000a16c4` (the
converter list registration) went from lifted to hand-written. Conflicting signatures 337 to 310,
header re-declarations 53 to 45; `O` 84,708 to 84,713. Findings: six callers that declared
`fn_7100216978` with one parameter relied on the vector still being in `x1`, and the real
two-parameter declaration leaves their bytes unchanged; two outputs improved instead of staying
identical once calls were typed (`fn_7100e01680.cpp` 53.3% to 94.1%, `fn_7100b2c6a0.cpp`'s seven
minor functions to 98.2/96.8%); a union-typed scoped reference adds a reload in its destructor,
so the six `game_s2` handle callers cast at the call; in `fn_7100b1ae74.cpp` the cast form let
Clang tail-merge two move paths and an anonymous-union member restored the match;
`UnkStaticInit.h` cannot include `UnkTUInit.h` (160 includers use `using namespace
unk_static_init;` and both headers define `UnkTunable`/`UnkProperty`), so their shared record
lives in a small header both include. S1's definition first went into the frozen sample
`src/flag_id/fn_7100216c2c.cpp`; it was moved to `src/game/UnkSphere.cpp`, since
`compiler_matrix.py` compiles the samples by path.

Port M1 step 1 (var-flotsam/dread-decomp#186): `tools/port_link.py`, the whole-program
relocatable link and its report (numbers in `port/README.md`, "The link (M1)", and
`docs/PORT_PLAN.md`). Facts it rests on: a shared-object link with unresolved symbols ignored
fails on the sources' `visibility("hidden")` globals, so the link is `ld.lld -r`; in `-r` mode
`--defsym=A=B` works when an input object defines `B` (also a COMDAT weak definition) and fails
when `B` is undefined, so a jump to a libc name or a trap is an assembly thunk; an absolute
`--defsym` stands in for each data symbol until the data loader maps the dump; 53,244 `--defsym`
fit one response file. Of 37,531 alias references, 27,188 only take the address; of the 10,343
calls 6 pass a `float` where the definition takes an integer or pointer and are trapped instead.


**Status dashboard (2026-10-24, branch `claude/task-status-dashboard-9a819f`).** A private dashboard
artifact (owner's claude.ai account) shows matching progress for the whole binary and the boot slice,
a daily trend, status by subsystem, and the phase and workstream tables of this handoff.
`tools/dashboard_export.py` writes its four datasets to `build/dashboard/` (counts only); refreshing
it means re-running the script after a merge and uploading the files. Its whole-binary share leaves
Ghidra's import stubs out, so it reads about 0.1 point above `tools/progress.py` (53.9% vs 53.8%).

**Fifteenth session (2026-10-24, on the PC, branch `claude/strategy-cleanup`): strategy readthrough
and its changes.** Findings that drove it: 53.8% of game bytes matched, but 9.4 MB of the 10.1 MB is
generated and only 0.76 MB is hand-typed; the boot path from `nnMain` was 27% matched; hand-written
files declared 3,260 symbols with 223 conflicting signatures (one callee with 23 signatures in 34
files) and 3,539 private struct views, so nothing could link; `asmdiff` masked relocations, so a
function could be `O` with the wrong pool; the handoff was a 28k-token append-only log; 178 PRs in
six days at a median of 10 functions and 3 minutes to merge. Changes: `check.py` and `trymatch.py`
verify relocation targets (382,619 checked; 40 mismatches, 39 in generated files from three
generator bugs, 25 hand-written files corrected, mostly globals named after their GOT slot);
`tools/callgraph.py`, `tools/hubs.py`, `progress.py --slice boot`; `tools/decl_check.py` with a CI
step and `check-local.yml` for a self-hosted `check.py`; `tools/tu_map.py` and `data/tu_map.csv`
(2,773 units from 2,760 initialisers); the canonical-header pilot (`fn_710009d50c`: 70 declarations,
12 signatures, to one header, 68 files migrated byte-identical, one left as-is because its wrong
declaration is load-bearing for an `m`); `docs/PORT_PLAN.md` (two routes, the recommended hybrid
and its milestones); the handoff split into this journal and a short state page; new rules in
`CLAUDE.md`/`CONTRIBUTING.md` (one declaration per symbol, files by unit, hubs first, logs here,
bigger PRs). Findings: a `u32`-returning declaration of a `bool` function can be load-bearing
(`fn_71003f1b00.cpp`: with `bool` Clang folds `(ok & flag) == 1` into `tst`); retyping a class
getter where it feeds one call creates a conflict with its other declarations, so retype them all;
`.init_array` entries are in address order with each initialiser last in its TU, and the reflection
boilerplate (attribute helper, then getter) opens a TU; Lua's range is 0x71010a0880–0x71010bee20.

Second part of the session (generator fixes, branch `claude/gen-reloc-fixes`): every generator
now verifies relocation targets through `asmdiff.diff_object(strict=True)` (one `llvm-cxxfilt`
call per object; a byte-identical candidate with a wrong target is rejected and logged), and the
three bugs the check found are fixed. `gen_clones`'s destructor scan assigned every string-id
member the default pool; it now takes the most recent pool load before each member, which also
matched 14 destructors that release ids through both pools (fn_710041b1b8 and others). `gen_statics`'s
`facts()` mistook a GOT page load for a dereference when a register still held an earlier GOT
pointer (the three slots of fn_71009d3ad0 start at a page boundary); every written register now
drops its stale state and a call clobbers x0–x18. `gen_tables` had hand-typed the video map's two
globals 0x200 too low; it reads them from the GOT now. The frozen sample `src/flag_id/fn_71004dd360.cpp`
stored a global's address where the original stores a function's (`fn_71004dd73c`, a callback).
`check.py` no longer exempts generated directories. The self-hosted check runner `dread-pc` was
registered the same day (`docs/maintaining.md`).

Third part (hub migrations, branch `claude/hubs-1`, and port M0, branch `claude/port-m0`): six
canonical-header steps, each byte-identical per file against a trymatch baseline and reviewed.
The 13 symbols `include/` declared twice were reconciled first (`fn_710007ecd4` is
`CRntString`'s destructor; the hash-stream functions moved to `include/base/UnkHashStream.h`;
the block free list and the DSO handle got their own headers), then `fn_7100d7c990` (the actor's
target getter: follows the slot at +0x290 through a weak handle, else returns the player from the
game manager; `include/game/UnkActor.h`), `fn_710002614c` (`UnkMatrix44.h`), `fn_710001737c`
(frees a counted resource through the descriptor registry; `UnkCountedResource.h`),
`g_unk_7101d47310` (the game manager singleton; `UnkGameManager.h` with offset static_asserts for
the fields the 30 files touch) and the class getters (`tools/gen_class_getters.py` writes
`UnkClassGetters.h` from the CSV; 243 files). Conflicting signatures 393 to 337, header
re-declarations 76 to 53, `O` unchanged. Findings: a header included by generated clones must keep
the form the instantiations compile with (re-run `gen_clones.py` after editing it; it emitted
identical files each time); a union-member refcount in a shared struct changes codegen in callers
that read it as a plain word, so two callers keep a local view; ten callers of the actor getter
need word-typed views of the translation for integer loads; `refl_TypeOf` in `UnkTUInit.h` now
takes `template <auto Getter>` so plain-type and class getters both fit. Port M0: `tools/port_sweep.py`
compiles all 3,808 `src/` files for `x86_64-linux-gnu` with the project's libc++ 9.0.1 and musl
headers (the only failures without the runtime are the four SDK headers); `port/` CMake project with
`dread_game`, `dread_runtime` (own `nn::` declarations written from the uses; only 4 Phase 3 sample
files duplicate another source and are excluded) and a banner executable; nothing links on the PC
(no Windows SDK).

**Fourteenth session (2026-10-21, Linux cloud, branch `claude/project-thread-slp42f`):** probe of the
ten largest `U` functions (Next steps, "Giant functions"); new generator `tools/gen_tables.py`
(`src/tables/`, stream 8): the strid table, the loading screen's video map (`video_map` kind;
`include/base/global/CFilePathStrId.h` now keeps its pointer in `UnkFilePathStrIdBase`, no regressions) and the
boss rush setup (`boss_rush`, `m`: register allocation only).
`gen_tables.py` and `check.py --update` were rerun.

**Thirteenth session (2026-10-21, Linux cloud, branch `claude/project-thread-m3b90j`):** core engine
types (Next steps, item 4): the string pool, CRC64 and the allocator getter in `src/base/`
(var-flotsam/dread-decomp#20, #22 and the string heap PR); no generator was rerun.

**Twelfth session (2026-10-21, on the PC, branch `claude/lifter-mismatch-classes`, var-flotsam/dread-decomp#18):** lifter mismatch study
(Next steps, item 1, "Done (twelfth session)"); `gen_lift.py` and `check.py --update` were rerun,
so the numbers below are current.

**Eleventh session (2026-10-21, branch `claude/local-agent-setup-df6136`):** set up two parallel
workstreams (below), `AGENTS.md` and `docs/tasks/` for a small local model on a MacBook,
macOS toolchain support (untested on hardware) and `tools/verify.py`. No matching changes.


**Tenth session (2026-10-20, branch `claude/lifter-float-0b96b0`):** lifter fixes (Next steps,
item 1, "Done (tenth session)"); `gen_lift.py` and `check.py --update` were rerun, so the
numbers below are current.

**Ninth session (2026-10-20, on the PC, branches `claude/sync-local-repo-4f8846`, merged in
var-flotsam/dread-decomp#13, and `claude/shape-round-6`):** the shape-family pass, rounds 5 and 6
(Next steps, item 2, "Done (ninth session)"). Numbers below are current;
`gen_lift.py` was not rerun (it skips what the other generators define).

**Seventh session (2026-10-19, branch `phase-4-3-remaining-reflection`):** the full regeneration
ran (the last steps, `gen_lift.py` and `check.py --update`, in a Linux cloud session with upstream
Clang 9.0.1, which reproduced the Windows results exactly). Numbers below are current. What was
done: Next steps, item 2.

Snapshot as of 2026-10-18 (fifth Phase 4 session; branches `phase-4-4-clones` (var-flotsam/dread-decomp#18, open) and `phase-4-4-skipped-getters` on top of it (not pushed); earlier work merged in var-flotsam/dread-decomp#13–#17). Target: **Metroid Dread 2.1.0**, `main` build ID
`646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`.


## Work log by stream, through 2026-10-23

The numbered items are the former "Next steps" of the handoff, kept as a record of what each stream did,
tried and left. The current next steps are in `docs/HANDOFF.md`.


0. **Clone generator (`tools/gen_clones.py`), done this session.** Exact-clone families (same
   instructions up to branch targets) were 1.3 MB of `U`. To find more, group `U` functions by
   `gen_clones.instructions()` (branch targets made relative) and sort by total bytes. Three kinds of entry:
   a verified reference (exact or shape match), a **probe** (no reference: the instantiation is
   compiled and its mnemonics searched for, so a template matched for one element type finds its
   copies for others) and a **scan** (facts read per candidate; `dtor`). Matching notes:
   - Union members model the original's missing alias info in several places (refcounts,
     `UnkVector`'s count in the registry, owning/handle element pointers, `UnkIterValue::Ref`).
   - `UnkVector`: trivially copyable elements are copied with `memcpy`/`memset`, others moved one by
     one (source walked by pointer, destination by its own pointer, 32-bit counter); `Insert`'s
     append path is `PushBack`'s shape (count re-read before `data` is stored). Trivially copyable
     8-byte elements match as `UnkCloneElem<Tag, 8>` (bytes) where a pointer element doesn't.
   - Left: `UnkVector::EraseAt` (fn_71002b9908, 28 × 192 B: loop counter and source offset in
     separate registers), ~400 destructors with other member kinds (vectors, nested objects),
     the CStrId-valued iterator family (fn_71000c936c, fn_71000cea70) and a long tail of
     families under 8 KB each (re-run the census for current numbers). The remaining call-free vtable
     functions (509) are varied virtual dispatchers, not one template.
1. **Extend the lifter (`tools/gen_lift.py`), largest payoff first.**
   - Done (thirteenth session, cloud): lifting now runs in parallel processes (a full run is
     ~105 min on 4 cores); output is one file per 64 KiB with per-function declarations, so
     `--prune` drops functions defined elsewhere in seconds; `--best` writes the closest attempt of
     any function for hand matching. Inlined guarded-static helpers (`GUARDS`: string pool,
     singleton, type id; replicas in `HEADER`, namespace `lift_guard`) lift as one statement:
     +126 `O`. Negative results: of 800 unlifted candidates none match in any round, and a
     one-field-struct ("field") rewrite for struct-path TBAA added 0 of 578 mismatches. Next, from
     the statics thread: a fourth helper (guard GOT 0x7101cba0c0, a pool getter followed by
     `bl 0x710007fdb4`), tbz up to four instructions after the ldarb, and the DSO handle
     (0x7101a19000) lifted as `""` instead of an extern.
   - Done (twelfth session), mismatch study and second-round rewrites (+1,039 `O`, ~146 KB;
     45.39% to 46.16%). Lifted-but-not-matching functions were classified with a scratch script
     (best attempt per function vs the original; each normalisation step, in order, removes part
     of the differing instructions, and the function's class is the step that removes most).
     Sample A: `--sample 600 --seed 1 --new` (450 mismatches, 172 KB); sample B: 600 new
     candidates of 256 B-1 KB, seed 2 (396 mismatches, 184 KB). Counts / bytes, A then B:

     | Class | Before (A) | Before (B) | After (A) | After (B) | Example |
     |---|---|---|---|---|---|
     | scheduling (same instructions, other order, incl. `ldp`/`stp` pairing) | 170 / 65 KB | 148 / 70 KB | 158 / 64 KB | 148 / 70 KB | fn_7100025bd0 |
     | registers (same instructions, other registers) | 120 / 61 KB | 143 / 67 KB | 111 / 62 KB | 136 / 64 KB | fn_710006f230 |
     | other (several kinds at once; mostly extra `mov`s and missing reloads) | 143 / 45 KB | 102 / 46 KB | 132 / 39 KB | 104 / 47 KB | fn_7100020a68 |
     | extra or missing instructions | 12 / 1 KB | 2 / 1 KB | 11 / 1 KB | 1 / 0 KB | fn_710002e468 |
     | constants materialised differently | 4 / 0 KB | 0 | 4 / 0 KB | 0 | fn_710090ca38 |
     | stack layout (sp offsets, frame) | 1 / 0 KB | 1 / 0 KB | 1 / 0 KB | 1 / 0 KB | fn_7100861e38 |
     | block or branch layout | 0 (365 insns) | 0 (440 insns) | 0 | 0 | fn_7100105a80 |
     | now matching | - | - | 33 / 5 KB | 6 / 2 KB | |

     Three causes inside "scheduling" and "registers" were fixable from the lifter's output, so
     functions that compile without matching get one more round per `ROUND2` rewrite (a round
     never loses a function that matched): **alias** (the biggest: the original reloads values
     after stores of another type, e.g. a pointer re-read after a refcount store, so every access
     goes through a `may_alias` typedef `a_u32` etc.; `-fno-strict-aliasing` alone gave 17 of the
     450), **sort** (field stores of one base in ascending offset order: the scheduler and store
     pairing moved them; as the only mode it loses 31 per 1,000, hence a second round) and
     **copy** (a load run then a store run of the same values becomes `__builtin_memcpy`). Left:
     - Large functions barely move (6 of 396 in sample B): most are flat-mode loops whose block
       layout and induction variables differ, not fixable by rewriting statements.
     - "registers" is mostly a knock-on of ordering (one moved statement renames the rest); the
       remaining scheduling differences are around calls and GOT loads (`adrp`/`ldr` placement).
     - "other": we emit ~3x the original's register `mov`s (values copied into callee-saved
       registers across calls), and the original uses more `add`s and `subs` where we use
       `sub` + `cmp`. Before the alias round the original also had ~25% more loads than us in
       these functions; after it the counts are about even.
     - The full run takes ~80 min on the PC (~40 min first round, ~12 min per rewrite round).
   - Done (tenth session), three cheap fixes (+83 functions, ~45 KB): `MAX_SIZE` is 16 KB (was
     2 KB); a stack load (`ldr x, [sp, #k]`) no longer crashes the GOT-load check (~2,000 more
     functions lift, few match: the `char stk[]` layout differs); code ending in `bl` is a call to a
     `[[noreturn]]` callee (`namespace noret`).
   - Done (tenth session), scalar floats (+471 functions, ~42 KB): `fmov` (immediates, register
     bit casts, mov/movk constants as hex literals), arithmetic incl. fused forms, `fcsel`,
     `fccmp` chains, conversions, `ldp`/`stp` of s/d, float call results (a call is retyped to
     return a float when s0/d0 is read after it: `namespace fret`/`dret`) and float tail calls.
     Clang 9 forms that matter: `fmin`/`fmax` are `!(a >= c) ? a : c`; `fcsel` between 1.0 and 0.0
     is `(float)(cond)`; after `tst w<n>, #1` only a `bool` parameter keeps `fcsel ne`; negated
     float conditions invert the cc (`a != b`, not `!(a == b)`); float accesses to hidden
     globals use an aligned declaration (`namespace h4`/`h8`) so the page offset folds.
     Left: of ~5,400 float functions, ~3,600 now lift but ~90% still differ in scheduling and
     register order (e.g. `fcvt` order for vararg calls, commuted `fadd` operands). Others are
     blocked by integer gaps: guarded statics (`ldarb`), `ccmp`/`ccmn`, jump tables (`adr`).
     Near-miss ideas: a `bool`-parameter variant for integer `tbz`/`csel` on bit 0 (the same
     flip as `fcsel`), aligned declarations for integer global accesses. It lifts 47,431 of 75,661
   candidates (<= 512 B) and 39,598 of those match. Rejection reasons (count / bytes, at 128 B;
   rerun `build/scratch`-style tallies by subclassing `Lifter` and recording why `step` fails):
   - **Guarded function-local statics** (`ldarb`): `tools/gen_statics.py` covers attribute keys
     (61), static string ids (386 from a literal, 55 copied from a global), typed lookups by name
     (203), registries (51), tables built from a registry (`refl_RegistryMapInit`, 37) and internal
     static string ids (plain functions, 13), type-id senders (`refl_SendTypeId`, 11) and
     parts of one singleton (`refl_GetSingletonPart`, 5), typed lookups whose class is internal to
     another TU (`refl_FindTypedHidden`, 5) and attribute keys with a u16 at +0x14 or a global value
     type (`refl_GetAttrKeyFlags`, `key_global`; 17), and game code parsed rather than shape-matched:
     a u32 sent under a hash (`refl_EmitU32`, `parse_emit_u32`; 12 of 15 parsed match). Game code with
     its own static (a string id or file-path id from a literal, or a zeroed word) goes through
     `gen_lift.py`'s Lifter, patched in-process by `own_patch` (`src/reflection_statics/own_*.cpp`):
     14 of 281 candidates up to 1 KB match. Most of the rest lift but differ in the stack frame
     around the guard (the original spills x0/x1 below its locals) or in general lifter gaps. Left (stream 6 census, 2026-10-21): 2,483 `U` functions call
     `__cxa_guard_acquire` (2.28 MB); 2.15 MB of that is one-offs (game code with a static
     inside), and only ~130 KB is in same-shape families of two or more. Older notes: the 437
     class getters in skipped TUs, primitive type getters (`refl_GetType<unsigned>`, 30 × 480 B:
     an inlined `CType` constructor whose kind hash and name share one string), 55 × 356 B
     serialisers and 46 pointer getters whose pointee is in a skipped TU. Add a kind: a template
     in `UnkReflStatics.h`, a reference address in `REFERENCES`, its constants in `facts()`.
   - Stack locals are handled (one `char` array over the gap between save slots, or over the
     bytes accessed; `STACK_LAYOUTS`), but only ~22% of those functions match. Parameters first
     read as `w<n>` are `u32` (this fixed many `mov w2, w1` vs `mov x2, x1` mismatches).
   - Mismatches now dominate (60,768 lifted, 46,219 match after the twelfth session). Only ~1,600 (165 KB) differ in
     register names alone; most differ in instruction selection or scheduling (typed struct
     accesses in the original vs our `char*` arithmetic), so the generic lifter is near its
     limit. Further gains need real types per subsystem (hand decompilation), not variants. Remaining unlifted (~4.6 MB): guarded statics (`ldarb`, 1.2 MB),
     `ccmp`, float arithmetic, early returns of values set on only one path.
   - **`CCollectionType`** (`include/base/reflection/CCollectionType.h`): the descriptors are
     globals built by static initialisers, so only constructors are generated: 178 standalone
     (`gen_types.py`; an element descriptor loaded through the GOT is a global, not a local
     class) and the rest in their element class's TU (`gen_reflection.py`, "local items"; 175 of
     181 local pointer/collection types match). The static initialisers
     that build the descriptors (e.g. `fn_71000e2800`, 14 KB) are still `U`.
   - sret (`x8` read first) and crossing branches/loops are handled (UnkResult built in place;
     `FlatLifter` with gotos), but only ~25% of flat-mode lifts match; the rest differ in
     register allocation or block layout.
   - Missing instructions: `tst`, `subs`, `csel`, `madd`, `udiv`, `mvn`, shifted/extended
     register operands (`add x0, x1, x2, lsl #3`), register-indexed addressing, loops
     (backward branches), float arithmetic.
   - **Mismatches** (~7,800 lifted but not byte-identical): register allocation differences
     (callee-saved registers assigned in another order, ~3,200), store scheduling in large
     constructors (~1,200, mostly descriptor constructors of other kinds), conditional-branch
     layout. A variant that changes declaration order of temporaries may help the first.
   - Raising `MAX_SIZE` beyond 512 adds little (most large functions fail to lift); per-variant
     scratch TUs are capped at 8,000 functions (`SCRATCH_CHUNK`, ELF section-index limit).
2. **Remaining reflection.** Done this session: pointer types whose pointee getter is in the
   same TU (generated into that TU; constructor uses the constant address), the `CPointerType`
   flag at `+0xe0` (true for `std::unique_ptr`), helpers/attribute functions/descriptor
   constructors of skipped TUs (no getters, no dummy statics), registration functions outside
   class getters (mostly container accessors passed by static initialisers) and every
   out-of-line reflected-function initialiser (`refl_attrs.parse_initialiser`). Left:
   - Done (sixth session): class getters in TUs with **other statics**, through padding (see
     Decisions; `layout_gap`/`padding_lines` in `gen_reflection.py`); the 54 container accessors
     (912 B), generated in their element class getter's TU.
   - Done (seventh session): **every located class getter** (2,354 of 2,355). The shape check
     ignores the descriptor's offset from a same-TU parent's (`add xN, x2, #imm`); six base library
     getters inline the name's constructor (`CStrId(UnkStrIdInline{}, ...)` in `CStrId.h`, the
     same body as `fn_71000003d4`, which matches out of line but is marked `L` by neighbour
     inference) and are verified by compiling them, like padded TUs. `tools/recon/reflection.py`
     names class/enum getters MEDS doesn't list (`CGun`, `GUI::CBossRushReport`, ...) and ignores
     functions over 640 B: those have a getter inlined (the "2 non-contiguous TUs",
     `CSubAreaManager`, were such a misdetection; seven `CAbility*` types had shared one wrong
     getter). Primitive `CType` getters (`refl_GetType<float>`, `fn_7100008500`;
     `UnkReflPrimitiveType.h`), pointer getters with the constructor inlined and `CFlagsetType`
     constructors (`gen_types.py`). Registration functions: `CRntString`, `CFilePathStrId`, `CName`
     and `CVector4D` values, function/parameter attributes through `UnkReflFunctionVtbl`, the field
     attribute container at `+0x70`, and a lambda-wrapped form for the stack-slot order
     (`refl_attrs.verify` retries alternatives): 1,911 of 1,957 attribute/field functions, 508 of
     562 others. The inlined string-id constructor (`CFilePathStrId(UnkStrIdInline{}, ...)`, its
     pool created on first use by a guarded static in a cold tail) matches since the thirteenth
     session (7 functions), as do `CName` values stored as a hash (`UnkNameHash`) and copies of
     any global `CStrId` (2), ids copied from a string-id table built on first use
     (`refl_GetStrIdTable`, 3) and enum defaults kept in guarded weak statics (`refl_GetEnumZero`,
     `refl_GetEnumNot8`, 4), the `CRntString` inside the string `g_unk_7101d470e0` points to
     (`refl_GlobalRntString`, 3) and string-list values (`UnkReflStringList` built from an
     initializer list by `fn_7100228ca0`, 4), the same from a string-id table
     (`refl_RntStringOf`, 1) and calls on a field found by its name's hash, setting a
     getter-backed field's function index (`refl_FindField`, 4) (all in `UnkReflAttributes.h`).
     Left there (13, one-offs): `fn_7100408c20` (33 KB), `CVector4D` copied from a constant,
     `fn_71004ded68`, `fn_7100beaf3c`, `fn_71000961f4`, `fn_710044e7fc`, `fn_7100b8bb94` (a u16
     global or'd into a version), and 6 registration functions outside class getters. The 4 that
     parsed but differed (`fn_71004ade14`, `fn_71007a2fb4`, `fn_7100add284`, `fn_7100db69a8`) match by
     hand in `src/game_s7/`: a type getter defined in the class's own file is called but its known
     return passed as a hidden constant, written `(fn_getter(), &g_unk_<addr>)` with the global
     declared `visibility("hidden")`. `refl_attrs` could emit that form itself. List them by failure point with a
     copy of `refl_attrs.parse` whose `return None`s raise their line number.
   - Descriptor helpers of **collection iterator values** (`UnkIterValue<T, K>`, K = index or key):
     move-construct, destroy, copy-assign and compare as `gen_clones` kinds (word, owner,
     string-id and keyed elements; a scan kind for elements with a vtable and string-id members,
     `UnkDtorElem`). Left: keyed move/copy-assign (written in `UnkStrIdMemberOf`, differ in
     scheduling), elements with vectors or out-of-line destructors, and an element destroy keyed by
     a string id that compiles to a 4-byte thunk when it shares a file with an identical one.
   - Done (eighth session): two `gen_clones` scan kinds. `pointee_typed` covers the reflected
     pointer helper that returns the pointee with its dynamic type (`UnkReflTypedPtr.h`, 328
     functions). Its getter is called once per branch, which only matches when the null branch
     calls a second, linker-folded copy of the getter (`folded::fn_<addr>`). `iter_value_destroy_dtor`
     covers iterator-value destroys whose element has an out-of-line destructor (50). The
     helper-slot census (`class_constructors` traits whose status is `U`) now gives 2,355
     functions, 374 KB, in small families (the largest is 5 KB).
   - Done (eighth session): scan kind `iter_element_value` (162 functions): an iterator value's
     element returned as a reflected value (`refl_GetIterElementValue`, `UnkReflValue.h`). The
     element stands in as `UnkCloneBlock` (u64 words; a byte array of the same size forms the
     element address differently and doesn't match).
     Leaf copies load the element type's descriptor through the GOT (143 more).
   - Done (eighth session): scan kinds `field_set_value` (scalar property writes,
     `refl_SetFieldValue`, 187), and `call_and_cast_arg` / `call_and_cast_field` (overrides that
     call their base, then cache a pointer cast to a class with the is-kind-of test
     `fn_710009d50c`, `UnkReflCast.h`, 163).
   - Done (eighth session): setters that clamp an f32 and notify a target by a 64-bit id
     (`refl_SetFieldMinNotify`, 73) or update a lazily created singleton (`refl_SetFieldApply`, 75);
     collection-iterator copy-assignment with out-of-line element destructors (`iter_assign_dtor`,
     52; `UnkCollectionIter` now takes the index type). Deleting destructors free through
     `__builtin_operator_delete` like compiler-generated ones, so stores before the free are
     dropped (40 more `dtor` matches).
   - Done (eighth session): more destructor shapes in `UnkDestructors.h`: no base call
     (`refl_DtorNoBase`, 49 more `dtor`), linked lists freed through their allocator (`list_dtor`,
     57), and classes with a secondary base linked into a refcounted owner's list (`linked_dtor`,
     87); `refl_SetDynamicValue` (`UnkReflCast.h`, 49 identical copies, a reference kind).
     Capstone prints immediates below 10 in decimal, so operand regexes need `(0x...|\d)`.
   - Done (eighth session): `hash_map_clear` (`UnkHashMap.h`, 44), `list_deleting_dtor` (57),
     `iter_element_dyn_value` (44), `iter_value_destroy_strid_elem` (34), and 14 reference kinds for
     families of identical copies (`UnkCloneHelpers.h`, `UnkSharedRecord.h`, `refl_IterSync`; 188).
     Census of identical-copy families (`U`, same normalised instructions): ~0.8 MB in 1,345
     families. Tried and left: the state machine at 0x71002fab10 (44 copies; the original keeps
     `next` in two registers), keyed-array Reserve 0x71001358e0, 0x71002b9908, 0x71000204d4 and
     0x7100287de8 (98%, one swapped pair), 0x71000df388, 0x7100241b30, 0x7100122160, 0x71001f3be4.
   - Done (eighth session, round 2): scan kinds `id_record_assign` (28), `strid_member_ctor` (25),
     `vector_move_poly` (25), `iter_value_destroy_strid2` (24; instantiates `UnkIterValue::Reset`,
     which the original doesn't inline), `iter_value_copy_assign_elem` (19), and 23 more reference
     kinds in `UnkCloneHelpers.h` (~280 functions). `gen_clones.py` now also retries `W` candidates.
     Left: 0x710013208c, 0x71004a84c8 (96%, an inverted branch pair), 0x71001f3a64, 0x71000cbef4,
     0x710035480c (the state-machine shape again), 0x7100020898, 0x7100085930.
   - Done (eighth session, round 3): 32 scan kinds over shape families (593 functions), among them
     `derived_copy_ctor` (68), `hash_key_fields` (55), `callback_base_dtor` (34),
     `callback_list_destroy` (28), `vector_free_poly` (26), the string-id value vector
     (`UnkStrIdValueVec.h`), pool lists, per-TU type initialisers (`type_static_init`), cast-and-register
     and owner destructors. Templates are in `UnkCloneHelpers.h`, `UnkStrIdValueVec.h`, `UnkCache.h`,
     `UnkDestructors.h` and `UnkHashMap.h`. Left: 0x710088af9c (a state machine's update, 15%),
     0x7100020eb0 (string-id hash erase, 44%: block layout), 0x71004f0648 and 0x71000d6310 (block
     layout), and the large per-TU static initialisers 0x71003a8a80, 0x71002d4770, 0x710021c330.
   - Done (eighth session, round 4): `keyed_set_assign` (21), `keyed_set_iter_value_copy` (11),
     `vt_iter_end`/`vt_iter_begin` (11 each), `vt_iter_sync` (8), `owner_vec_insert` (5). Close but
     not matching: 0x7100f5ff2c (`refl_StrIdValueInsert` shape, 4 instructions of register choice)
     and 0x7100f5f5ac (an erase whose index/pointer round trip through an exact division by 24
     Clang 9 folds away). Skipped: 0x710035d230 (static initialiser), 0x710065fb20 (state machine).
     Not yet tried (the run hit its usage limit): 0x71000c3cc0 0x71000c3c14 0x71000d6914
     0x71000d60e4 0x71000d6de0 0x710006d11c 0x710009abf8 0x710008fa30 0x71002464fc 0x7100245c4c
     0x71002e19d0 0x71002e1904 0x71002d3f88 0x71002fa6cc 0x71002ca39c 0x7100300380 0x7100241ddc
     0x7100239cf4 0x710023c3e0 0x71001f30c0 0x7100b8eec8 0x710098e094 0x7100bf7624 0x71003d3cf8
     0x7100e02988 0x7100c79a80 0x7100536450 0x71006cc484 0x71009ba4ac 0x71004f0ab8. About 600 KB
     is left in shape families of five or more `U` members.
   - Done (ninth session, 2026-10-20, round 5, on the PC): most of the round-4 list (~490
     functions). Kinds: `variant_assign`/`variant_assign_pair` (a string id + value or pair
     variant, `UnkStrIdValueVariant`, 36), `keyed_value_array_dtor` (10), `keyed_value_get` (a
     keyed reflected-value lookup per value type, 10), `iter_assign_block` (iterator assignment
     over block elements, 160, of which ~130 matched other kinds before), `vector_clear_poly`
     (25), `value_holder_deleting_dtor` (38), `derived_copy_ctor_48` (26), `strid_copy_ctor`
     (three vtables, 22), `iter_value_destroy_poly_vec_key16` (4), `keyed_pair_iter_assign` (20),
     `set_matrix` (8), `value_map_insert` (a chained hash map of string ids to values, 11) and
     `shared_copy_ctor` (a 0xd0-byte class with an atomically counted pointer, 14).
     The `dtor` scan now reads member offsets below 10 (capstone prints `#8`; 54 more) and treats
     a second vtable store as an inlined base destructor (`refl_DtorInlineBase`, 27 more).
     `gen_statics.py` has a second attribute-key reference for keys whose u32 is 1 (`key_one`, 9).
     Lessons: struct-path TBAA lets Clang skip reloads that the original does after stores to a
     node's links; store through a raw `char*` offset instead. Clang keeps the null check of a
     node only in the form `if (n) new (n) T(...)`. Left: 0x71000d60e4 (the variant's move
     constructor, 63%: one branch schedules the pool load early), 0x71000d6de0 (the map's erase,
     block layout), 0x7100300380 (find by string id, then cast), 0x71003d3cf8 and 0x7100c79a80
     (game code around guarded statics), 0x7100536450 (a TU's static string-id destructors). Not
     yet tried: 0x7100245c4c 0x7100241ddc 0x7100e02988 0x71006cc484 0x71009ba4ac
     0x71004f0ab8. Tooling note: in the Windows Bash tool, heredocs can lose backslashes; write
     patches to Python sources with the Edit tool.
   - Done (ninth session, round 6): a new census that skips everything already in `src/` (486
     functions). Iterator values and iterators: `index_pair_iter_assign` (16),
     `iter_value_copy_assign_index` (14), `iter_value_from_ref` (72), `iter_value_move_ool` and
     `iter_value_move_ool_strid` (an element with an out-of-line move constructor,
     `UnkCloneMoveElem`; 17 + 26, the latter the round-5 leftover 0x71000d60e4),
     `iter_value_destroy_key16` (14), `iter_value_destroy_refptr` (12). Keyed arrays:
     `keyed_find` (26), `keyed_find_then` (20), `keyed_find_assign` (15; the original doesn't
     tail-call the assignment, so the template uses `disable_tail_calls`). Vectors:
     `vector_free_dtor` (10), `vector_clear_dtor` (10). Classes: `call_with_owner_id` (15),
     `strid_dtor_at` (19), `ref_handle_copy_ctor` (20), `value_holder_dtor` (38), `zeroed_ctor`
     (49), `dtor_call_clear` (33). Small helpers: `add_f32` (25) and `pair_counts` (35; most copies
     return the record through x8). Left: 0x71000d15fc (an erase by `memcpy`, 25 copies: the
     original masks the scaled index, as if the loop were idiom-recognised; no source form found),
     0x71000d6a20 (a list walk to the i-th node, 23 copies, 67%: the counter's form),
     0x71000de484 (20), 0x710000dc80 and 0x7100415e18 (property reads returning a reflected value
     through x8), 0x7100142df0 (clamped colour setters), 0x71003e2ea8. In this census, families of
     five or more `U` members still hold ~500 KB; under 100 bytes, ~30 families of 10 or more.
   - Done (2026-10-21, cloud, stream 2, round 7): 13 kinds, +180 functions. `strid_value_insert`
     (`UnkStrIdValueVec.h`; each branch constructs the element itself, 10 × 1 KB), and a new header
     `UnkCloneCalls.h` for small call-shaped families: `set_flag_notify` (17), `set_flag_virtual`
     (13), `call_with_owner_child` (16), `ctor_then_init` (14), `cast_then_call` (14),
     `find_by_id_call` (13, the key's argument register varies), `call_arg_return_one` (14),
     `elem_at` (12), `copy_ctor3` (12; byte copies keep the vtable stores), `link_slot_assign`
     (12; the two state resets are a range check and a switch on 1, 2 read through the raw
     offset) and `get_lookup_f32` (11); `derived_copy_ctor` takes u8/u16 fields (+10).
     `gen_clones.defined_in_src` ignores trailing comments (a commented declaration in
     `src/base/` hid a clone). Left: 0x7100f5ff2c (the insert that calls its push-back, 4
     registers in the shared tail), 0x71008eb574 (clamped colour, 16 × 100 B: the zero register
     and loads are scheduled first), 0x710000e08c (an exact division by 3 Clang folds away),
     0x7100339a50 (a resource load around two string-id types, 9 × 760 B).
   - Done (2026-10-21, cloud, stream 2, round 8): +96 functions. `UnkCloneCalls.h` kinds
     `set_flag_notify_b`, `release_ref`, `copy_ctor2`, `set_bool`, `set_flag_call` (10 each or
     fewer), the reflected properties three pointers away `set_deep_u32`/`get_deep_u32`, `get_false`
     and `keyed_set` (parallel key/value arrays of one width). `call_and_cast_field` also takes a
     descriptor passed as a direct (hidden) address after its getter is called for initialisation
     (+10).
   - Done (2026-10-21, cloud, stream 2, round 9): +56 functions. `value_map_clear` (`UnkHashMap.h`),
     `keyed_array_copy` (the key stored as bytes so the count reloads), `slist_insert_before` (new
     `UnkSList.h`; the count is a union member), `callback_list_dtor` (`UnkCallbackList`'s
     destructor and deleting destructor), `strid_vec_dtor`, and `iter_value_destroy_poly_vec_key16`
     now reads its key and state offsets (`UnkClonePolyVecPad`; +8).
   - Done (2026-10-21, cloud, stream 2, round 10): +29 functions in a new `UnkCloneRecords.h`:
     `strid_pair_record_assign` (8), `keyed_vec_array_clear` (7; the vector loop counts in u32),
     `path_record_copy_ctor` (8; string ids and a file path copied in place) and
     `callback_entry_dtor` (6; `UnkCallbackListShort` plus a string id, with or without a key).
     Looked at and left: 0x7100769fc0 (a per-TU static initialiser). Not yet tried: 0x71000df484,
     0x71000df5ec, 0x71005b4d50, 0x7100af30a0, 0x71011d437c, 0x71000d8428, 0x7100240168,
     0x7100431cec, 0x71000d78ec, 0x71000d5888, 0x71000d1fec.
   - Done (2026-10-21, cloud, stream 2, round 11): +26 functions in `UnkCloneRecords.h`:
     `strid_vec_record_copy_ctor` (6; a string-id vector with an out-of-line Reserve, element
     stored bitwise), `add_function_holder` (7; the first libc++ `std::function` in `include/`,
     copied into a heap holder whose vtable is stored before the members), `keyed_move_assign` (6)
     and `keyed_move_array_clear` (7). Close but left: 0x71000df5ec (a keyed array shrink; the key
     loop compares a 64-bit counter in 32 bits, which no counter type reproduced) and 0x7100240168
     (`UnkSList` set-at: two instructions of scheduling in the inlined push-back; the loop counts
     up, `for (u32 j = 0; j < idx; ++j)`). Skipped: 0x71011d437c (a reflected float setter).
   - Done (2026-10-21, cloud, stream 2, round 12): `strid_vec_variant_reset` (12; a string id and
     byte vector or a word pair, with or without a key). Close but left: 0x71000d15fc (25 × 92 B,
     a vector erase by memmove; the original forms the element offset as `lsl` + `and` mask, which
     Clang 9 folds into the add in every form tried), 0x71002444f4 (the same variant's move
     constructor, state 2 holding pointers; scheduling and a tail-merged load) and 0x7100f5f0f0
     (a keyed record assigned through owned or referenced values; the original duplicates the
     key store per source state). Skipped: 0x7100eab448 (reflected setters that notify by name).
   - Enum static initialisers and the other per-TU static initialisers. These define
     globals, so they need the TU's data definitions (the 160 B ones are mostly lifted already).
   - Slot `0x28`/`0x30`/`0x38`/`0x48`/`0x50` helpers of other classes (need class layouts):
     ~2,400 functions, ~390 KB; a census of `U` helpers by slot and mnemonics (see
     `class_constructors` in `gen_reflection.py`) picks the next families.
   - A shape census (mnemonic sequence per U function, grouped) was the fastest way to pick
     templates this session: the top families were reflected-function initialisers (839 × 380 B)
     and `CClass` constructors (777 × 388 B). `tools/shape_census.py` now does this (`--top`,
     `--max-size`); `--family ADDR` lists the operands that vary across a family's copies.
3. **Lua leftovers:** mostly done (thirteenth session, cloud; continued in the fourteenth). The
   game's Lua needs `-fno-strict-aliasing` (`buildcfg.LUA_FLAGS`), a 64-bit `twoto`, `(float)strtod`
   and a 16-byte aligned jump buffer, and its binary-chunk loader is the older byte-swapping one
   (`lundump.c`: `S->swap`, a swapping `LoadMem`, no `nCcalls` check, `LoadChar` stored as is);
   all in `lib/lua/README.md`. `sigmatch.py` names look-alikes (the `math_*` wrappers, `lua_error`,
   `luaL_register`) from the `luaL_Reg` tables, by order for the rest, and now near-matches by
   mnemonic sequence (mutual best, at least 0.8, 128 bytes or more). 368 of 466 compiled Lua
   functions are named. `LoadFunction` and `luaU_undump` match exactly; `luaB_print` is game code
   (`src/lua/fn_71010a6440.cpp`, `O`: output through a function pointer, not `fputs`). Still
   differing by a few instruction-selection choices: `lua_getinfo` (one pre-indexed load) and
   `luaV_execute` (six hunks: `GETARG_Bx` index, `RKB`/`RKC` extends, a pre-indexed load); neither
   `GETARG_*` macro variants nor loop rewrites fixed them. The 55 other unnamed functions in the
   Lua address range are game code (the game dropped most of `io`/`os`/`package`/`debug`), not
   library look-alikes, and the 8-byte hook getters and the allocator group fold into other code.
   `libcheck_lua.py` could not be rerun (lua.org is not reachable from the cloud sandbox).
4. Core engine types the slice depends on. Done: the inline code of `CStrId`/`UnkStrId` (copy,
   move-assign, destructors) and `UnkVector` with the allocator interface. Done (thirteenth
   session, cloud, `src/base/`): CRC64 (`fn_7100001570`, `O`: reflected table CRC-64, init ~0,
   no final xor; the index must be written `(crc ^ byte) & 0xff`), the string-id constructor
   out of line (`fn_71000003d4`, `O`, a wrapper around the inline `UnkStrIdBase` constructor),
   the allocator getter (`fn_7100083990`, `O`, a hidden global) and the string release
   (`fn_71000803a4`, `m`: 3 instructions of block layout in the inlined set lookup). CRC64 and
   the constructor sit next to `nnMain`, so `gen_functions_csv.py` infers `runtime?` and marks
   them `L`; they match but don't count toward progress until that inference is fixed. Then the
   rest of the pool (`include/base/global/UnkStringPool.h` has the set and node layouts): erase
   (`fn_7100080a00`, `O`), insert (`fn_71000801e4`, `m`: register allocation around the heap's
   GOT address) and the buffer compare (`fn_710007f240`, `m`: a select the original speculates).
   The heap behind the string copies (`g_unk_7101d58d40`, `include/base/global/UnkStringHeap.h`)
   has six size classes (16 to 512-byte blocks carved from one allocation each): class setup
   (`fn_7100083c48`), clear (`fn_7100083d90`) and allocate (`fn_7100084030`) are `O`. Adding a function to
   a class (`fn_7100097550`, `UnkReflArray` at `CClass+0x110`) is `O` in
   `src/base/reflection/fn_7100097550.cpp`: the key search, the inlined move of the 0xb8-byte
   `UnkReflFunction` (full layout there, as local views checked against the headers) and the
   class's `_EditorOnly` attribute read with a default (`Get<T>(key, def)`; a volatile byte read
   keeps the default in memory as the original does). Left: the array's growth
   (`fn_7100097f80`), the function's constructor and destructor (`fn_710009a5b4`,
   `fn_710009aae8`), and the `U` functions next to `operator new`, probably more allocator code (`fn_7100083870`,
   `fn_71000839c0`, `fn_7100083a10`, `fn_7100083a70`).
5. Boot to title screen: startup, memory, file system / `files.toc` / `.pkg`, resource manager,
   Lua + GUI bindings, GUI, render front end, input, localisation, save probe. Estimated
   6,000–9,000 functions (static floor: 4,394 reachable from `nnMain`).
6. Port groundwork: an aurora-style `nn::` + NVN compatibility layer (see `docs/references.md`).


### Giant functions (probe, 2026-10-21, cloud thread "Biggest functions probe")

The ten largest `U` functions held 458 KB (2.4 points). What they are:

| Address | Size | What it is | State |
|---|---|---|---|
| 0x71000407f0 | 96 KB | Implicit destructor of the 1,198-id table below (every id's destructor inlined) | Not generated: structure and block layout match, register allocation doesn't (the pool sits in x8 or x0 in different ~100-id stretches). Tried 9 source shapes of the id destructor |
| 0x7100031894 | 61 KB | Constructor of a struct of 1,198 string ids built from literals with precomputed CRC64 hashes (called from ~700 functions; a guarded static, `fn_7100065ca0`) | **O**, `tools/gen_tables.py` |
| 0x7100057eb8 | 56 KB | Passes each of the same 1,198 literals with an empty 0x30-byte temporary to `fn_7100065b14` (a string escaper) | **O**, `tools/gen_tables.py` |
| 0x71000406dc | 276 B | The ids' constructor (string, length, hash, flag) | **O**, `tools/gen_tables.py` |
| 0x7100db8dec | 63 KB | Boss rush setup: twelve stages, each four to seven `UnkStrId` assignments, flags, a CRC64 and two `UnkVector<UnkItemAmount>` arrays filled by 227 inlined pushes in all (item pointers are GOT entries) | **m**, `tools/gen_tables.py` (`boss_rush`). The calls and blocks are the original's; from the third stage on, register allocation differs (it spills the hoisted array field addresses and rematerialises constants the original keeps), and in the later stages the id release is laid out out of line. Ids had to be `UnkStrId`: with `CStrId` the moved-from temporary's destructor leaves a second store |
| 0x7100ef378c | 52 KB | Loading screen setup: two widgets, then 79 inlined hash-map inserts of (enum key, `CFilePathStrId` video path, `CStrId` label, five flags) | **O**, `tools/gen_tables.py` (`video_map`). Keys 49-71 use `InsertCopy` (the copy taken inside the insert): the original's register allocation differs in that stretch and only that source shape reproduces it, so it is a shape match, not a known source difference. Needed `CFilePathStrId`'s pointer in a base struct with a defaulted copy (as `CStrId`'s `UnkStrIdBase`), which sets the compare order in its copy constructor |
| 0x7100408c20 | 34 KB | Tunable registration (field name, editor description, `refl_GetType<float>` ...) | Reflection-attribute style (stream 4's `refl_attrs.py` shapes); 94 chunk shapes |
| 0x7100c66c60 | 30 KB | Float-heavy game logic (collision, `sqrtf`) | Hand only; not regular (211 shapes in 244 calls) |
| 0x7100b66018 | 28 KB | Inventory GUI (beam list composition), 18 guarded statics | Irregular; hand only |
| 0x7100ae39e0 | 21 KB | Reflection registration of a save-data class (fields with descriptions) | Stream 4 |
| 0x7100e72680 | 17 KB | `CMinimapDef` reflection registration | Stream 4 |

### Above 2 KB, outside the six hand slices (`src/game_s7/`, 2026-10-21)

559 `U` functions above
2 KB held 2.18 MB. Most call-table candidates are singletons; the static initialisers were dropped
(their globals are merged by GlobalMerge, and the original's post-increment folding can't be
reproduced through an `extern char[]`). Done: the loading-screen tips registration
`fn_7100e17d14` (**O**, 60 calls through an `std::initializer_list` pair), and libc++'s
`std::sort` helpers instantiated for a ref-counted handle list (`fn_710117d4d0.cpp`): `__sort3`,
`__sort4`, `__sort5` **O** and `__insertion_sort_incomplete` **m**, each mapped to its address
through an `alias` declaration. The comparator takes its handles by value and has external
linkage (an anonymous namespace changes the load order). `__sort` itself (`fn_710117c430`,
4 KB) is **m**: the original halves the length unsigned and inlines `__insertion_sort_3`, unlike
libc++ 9.0.1, so its body is an adapted copy of libc++'s in `lib/libcxx/sort.h` (owner-approved,
with LLVM's license); it has the same operations in another block layout. Left for hand work:
`fn_7101144a5c`, `fn_7100e68f44`, `fn_7100dd3624`.

Static initialisers above 2 KB (31, 152 KB) are this stream's too (the coordinator's call,
2026-10-22). Sources are written with slice 3's helpers (`src/game_5821e4/UnkStaticInit.h`) plus
`src/game_s7/UnkStaticInitS7.h` (header-id variants, named values, conversions with a type object
on one side, buffer params, empty ids, resource types, (id, value) pair lists). 26 match and 3
are `m` (register allocation or store scheduling only). Findings: a unit whose u32 sits right before its ids (merged
globals) takes a packed, 4-byte-aligned struct; each vector of ids built from a list must sit in
its own immediately called lambda, because the original inlines one initialiser function per
global and the inliner then shares one stack array between lists of the same length (so lists of
one length in a unit need one element type: `UnkIdElem` when any has an empty id); resource-type
source-extension arrays are filled element by element (an initialised array becomes a constant
copy), declared in reverse order; and the lift drops stack stores of strings kept in callee-saved
registers, so take those from the original's code. Three the lifter rejects (an inlined guarded
static: a class name id built on first use, `InitClassIdFrom`) were converted from the original's
code directly. Still `U`: `fn_7100eb4510` (an inlined vector reserve with a virtual allocator) and
`fn_71001e0820` (merged globals: the original folds one id's address into a post-indexed store).

Other functions above 2 KB (2026-10-22), all **O**: `lua_bind::GainAllItemsAndPowerUps`
(`fn_710107a880`, 37 item grants through a lazily built qualified id, `fn_7100ae9a10`), a
property-bag serializer (`fn_7100bda920`), a shape-pair collision dispatch (`fn_7100d541c0`) and
a 64-hit query (`fn_7100aaac80`). The property bag (`src/game_s7/UnkPropBagS7.h`: two vectors and
a lock, keys from the id table `fn_7100031894` builds, setters `fn_710033187c` float,
`fn_710050f4ac` bool, `fn_710050e4dc` string, `fn_7100aa3b9c` u32, destructor `fn_710050e8e4`)
is used by 121 `U` functions (121 KB, most under 2 KB). Findings: the lock is a member with its
own constructor (calling `fn_710007be8c` from the bag's constructor body moves it after the second
vector); a call whose `x0` is left as it is (the dispatch's handlers) takes an empty struct by
value first; a local array of 64 elements with default member initialisers is fully unrolled, and
a counter declared after it lands below it on the stack (declare it first). Tried and left: the
other collision dispatch `fn_7100d51368` (4 KB, inlined geometry per case) and constructors that
inline a vector reserve (`fn_7100ee1f34`).
Also **O**: a component cache (`fn_7100d78a30`, 7.3 KB, 42 slots), where each slot has its own
weak inline `static const CStrId` kind id (`Kind_LIFE()` and so on, reached through the GOT); the
comparison must take both ids by reference (`SameId(Kind(), comp->owner->id)`) or the owner load
is hoisted. `fn_710092f204` (melee hit-effect path from 15 local statics) stays **m**: the
original tests kind 5 before kind 2 and duplicates each item id's out-of-line first-use build.
Property handlers, all **O**: `fn_7101129240`, `fn_71010d3508`, `fn_710111dc08` (11 KB). Each
compares a property's name with keys from the id table and reads the value from the property's
stream (`fn_710007cf30`) or writes it there (`fn_710007cb4c`), falling back to
`fn_71010c4250`; helpers are in `src/game_s7/UnkPropS7.h`. Findings: the id table needs a sized
type (with the empty `UnkReflSingleton`, Clang decides no store can reach a key and drops the
reloads); the key's use count is a union member but the property name's is not, while the
property's name pointer is; the stream functions return an integer (a pointer compare changes
the branch weights and the block layout), and handlers return `u32` (with `bool`, the base
handler's result is masked on return). More callers of these stream functions are under 2 KB
(`fn_7101171fcc`, `fn_71010e4ab8`, `fn_71011608b4`, `fn_71010c4250` itself).
Also **O**: a state-machine setup that creates 18 states and adds them to a keyed list
(`fn_71010e2278`, 5.1 KB), and a GUI panel setup that keeps handles to 18 children found by name
hash (`fn_71011dfeb0`, 5.3 KB). Both use the shared handles in `src/game_s7/UnkHandleS7.h`
(an object's slot at vf_0x28, a handle created on first use). Findings: in the keyed list's add,
the name is copied as raw bytes and the values are union members, or the count isn't reloaded;
a temporary `CFilePathStrId` built inside the call's argument (not a named local) gives the
original's join block; one callee needed its float parameters declared before the pointer to get
the argument copies in order. The child hashes are CRC-64s computed at compile time; strings for
some are in the binary (a reverse lookup over all strings finds them), the rest stay constants.
Related `U` GUI setups that use the same child lookup (`fn_71011d98ec`): `fn_7100b39950` (12 KB),
`fn_7100b49c88`, `fn_7100b4d4a4`, `fn_7100b92e9c`.
The boss rush results screens (`fn_7100b94320`, `fn_7100b921e0`) are **m**; shared declarations are
in `src/game_s7/UnkGuiS7.h`. The button callbacks are `std::function<void()>` built from
`std::bind(&C::method, this)`; libc++'s small buffer holds the bind object. Finding for compositions
looked up through the registry (`fn_71011b69d8`, also the menu setups in `src/game_aca57c/`): take
a reference to the slot after assigning it (`self->composition = ...Get(); auto& c =
self->composition;`), not before. The original keeps the slot's address in a register, set by
pre-indexed stores, and only this order gives that. What's left: `fn_7100b94320` addresses five
loads from the object rather than through that register, and in `fn_7100b921e0` a colour copy is
only partly sunk below a branch.
Music volume overrides by stream id (`src/game_s7/fn_71010182fc.cpp`): the setter
`fn_7101019158` (4.5 KB) is **O** and the getter `fn_71010182fc` (3.7 KB) is **m**. Each is a
73-case switch on the id's CRC-64 (signed). Clang's compare tree comes out the same when the cases
are written in the order of their bodies in the binary. The getter's unmatched ids branch to a
`ret` shared with a case in the middle; ours go to the last one.
Three more long repeated functions are **O** in `src/game_s7/`. `fn_7100fc4b34` (8.4 KB) reads
about 50 tuning values from two property sets: each value is reset, then read by its definition's
qualified id from a section keyed in the id table, through a `Get<Reader>` template. The four
locals of the last two flags have to be function-scope so they get separate stack slots.
`fn_71011415f4` (3.4 KB) clears a component's flag set and re-adds 30 ids. The id is copied with
`memcpy`, then retained, as in the state-list `Add`. `fn_7100d92158` (7.4 KB) registers 29
descriptors: a temporary entry is built by `fn_710002003c`, passed to `fn_710002002c`, and its ids
are released in two steps. Three of the descriptors get a factory singleton created on first use.
Five more libc++ sort families are in `src/game_s7/`, mapped by `alias` as in `fn_710117d4d0.cpp`.
`fn_710031e030.cpp` (entries of two handles and plain data, by a float) is **O** in all six. Its
entry has the implicit copy: the plain members are copied as one block. `fn_7100566930.cpp` and
`fn_71003cd6e0.cpp` sort 0x48-byte entries (two weak handles assigned with `fn_71002a5d1c`, then
13 words), whose copy is member by member with a self-check. Their `__insertion_sort_3` and
`__insertion_sort_incomplete` are **O** and the rest **m** (register choice). The second has an
out-of-line comparator (`fn_71003cd4fc`, **O**) that returns a word: the callers test all of `w0`
(`cbz`), not bit 0 as they would for a `bool`. `fn_710072a110.cpp` sorts object pointers nearest
first: all five **O** (through a class iterator, see below). `fn_710010e470.cpp` sorts boxes
by the centre on one axis: all five **m** (see its note). Other findings:
- The game's `swap` is always inlined, so the element needs an `always_inline` ADL `swap`.
- `__sort` switches to insertion sort below 30 elements for trivially copyable types and below 6
  otherwise. `lib/libcxx/sort.h` now computes this the way libc++ does.
- Temporaries made in one comparison get separate stack slots.
- Writing an expression term by term keeps the original's order.
- With 8-byte elements sorted through raw pointers, our insertion loops walk an index where the
  original walks a pointer. The fix is the class iterator below.
- Some game comparators have internal linkage (an anonymous namespace). Then every
  instantiation is internal, and `std::__sort` inlines its one call to `__insertion_sort_3`. This
  made `fn_7100338620`'s `__sort` **O** with plain `std::__sort`, without `lib/libcxx/sort.h`.
  - Map the instantiations with `extern "C"` aliases: a declaration that uses the internal type
    is itself internal and gets dropped.
  - Write each alias as `void fn_X()` on its own line inside an `extern "C" { }` block, so the
    `NON_MATCHING` annotation attaches. `check.py` doesn't read `extern "C" void fn_X()`, so
    those functions show up as **W**.
- When an internal instantiation's comparator is empty, the game's helpers take no comparator
  argument (dead-argument elimination). An alias counts as taking the address, which blocks that.
  `fn_7100d4de00.cpp` therefore gets the instantiations emitted from an unmapped anchor function
  that calls `std::sort`, and names them with `asm(".set ...")`. That made its
  `__insertion_sort_incomplete` **O**.

Three more families: `fn_7100354e10` and `fn_710081ccd0` (0x48-byte entries, nearest first with
z taken as 0) and `fn_71010dd1d0` (entry pointers by depth, then by tile group). The insertion
sorts of the first two and all of `fn_71010dd1d0` are **O**; the rest are **m**.
- The distance with z taken as 0 matched when each position is copied into a vector with z = 0
  and a comparator member takes the full 3D distance to it. Writing out the 2D terms plus `z * z`
  gives the right values in the wrong load order.
- Clang adds the inline hint only for an explicit `inline`: a member defined in its class gets the
  default threshold (250 instead of 325). `fn_71010dd1d0`'s comparator is inlined everywhere, as
  in the game, only when `operator()` is declared `inline`.
- `fn_71010dd1d0`'s helpers had their comparator dropped (internal, empty), so it uses the
  anchor and `.set` names. Its `__sort3` and `__sort5` stay in `src/game_hi/`: `check.py` masks
  call targets, so they don't need names here.

`fn_7100abbd40` is the same code as `fn_7100354e10`'s family. `fn_71001e2e30` sorts object pointers
by a float in a copied info block, with an internal comparator: all five **O**.

### Sort through a class iterator

Many of the game's pointer containers have their own iterator
class, and libc++ then instantiates the helpers on it rather than on a raw pointer.
`src/game_s7/UnkIter.h` is a minimal random-access iterator over a plain array. Sorting through it
fixes three differences at once:
- the insertion loops walk a pointer, not an index;
- `__sort4`/`__sort5` inline the way the game's do;
- the halving in `__sort` is unsigned, even with plain `std::__sort`.

With it, `fn_710072a110`, `fn_71010dd1d0` and `fn_71001e2e30` became fully **O**. So did the clone
families `fn_71000a0660`/`fn_71000a5e00` (by name, `strcmp`) and
`fn_71002c5c00`/`fn_71007f6100`/`fn_71007f6b90`/`fn_71007f7620` (by a float; the float is in a
union, so the original reloads it after each element store). Try the iterator first on any sort
family. It also made `fn_7100213380` (0x40-byte polymorphic boxes, below) fully **O**. It doesn't help the 0x48-byte handle
families (`fn_7100566930`, `fn_71003cd6e0`, `fn_7100354e10`, `fn_710081ccd0`), `fn_710010e470` or
`fn_7100338620`.

Also claimed by stream 7 (2026-10-22, coordinator-approved), although each `__sort` is under
2 KB: the sort families `fn_7100256810`, `fn_7100742750`, `fn_7100743340`, `fn_7100744000`,
`fn_7101072a80`, `fn_710109b9f0` and `fn_71011bc0b0`, with their helpers. Stream 7 also takes any
further libc++ sort family it finds, after noting it here. Hand slices, leave these to stream 7.
Further families claimed the same way: `fn_71002aa690` (0x30-byte elements) and `fn_7100c75e40`
(0x28-byte elements), with their helpers. Both are done on plain pointers (the class iterator made
them worse), with comparator types of external linkage, since each keeps `__insertion_sort_3` out
of line:
- `fn_71002aa690`: nearest first with z taken as 0, like `fn_7100354e10`. Four **O**. The two
  insertion sorts are **m**: the held entry's store-back is addressed from another register. The
  position's x and z had to be union members; y as well made it worse.
- `fn_7100c75e40`: by a key, then nearest first in 2D. `__sort`, `__sort4` and `__sort5` are **O**.
  `__sort3` and the insertion sorts are **m**: the original loads the first entry before the
  branch that picks the swap.

All seven are done (2026-10-22), each through `UnkIter`: every function is **O** except
`fn_7100744000`'s own `__sort` (**m**, below).
- `fn_7100742750` compares with a plain function pointer. With a comparator type that isn't
  internal, `std::__sort` keeps `__insertion_sort_3` out of line (`fn_7100742ee0`), as this copy
  does; an internal comparator inlines it, as the other six do. So whether the game's `__sort`
  calls `__insertion_sort_3` tells you whether the comparator was internal.
- `fn_7100256810` (an s32 field), `fn_71011bc0b0` (an s32 key with a fallback field) and
  `fn_7100744000` (a float key, then an id) needed the compared field to be a union member, so
  that it is read again after each element store. `fn_7100744000` also needed a swap that assigns
  the two members in turn, and its `__sort` is still one instruction off (the original advances
  the pointer and then loads; ours loads with a pre-indexed address).
- `fn_7101072a80`, `fn_710109b9f0` and `fn_71011bc0b0` have empty comparators, so they use the
  anchor and `.set` names.

More instantiations found by their recursion (each 2 KB, or with 2 KB helpers):
- `fn_7100134020`: all five **m**. 0x160-byte entries (three pooled string ids and a string with
  its own storage) by name. One difference is left: the original reuses the compared id's pointer
  when a swap moves the entry. That needs the alias analysis to know the swap's temporary isn't the
  entry, which it does only for pointer arguments, but sorting raw pointers pairs stores the
  original keeps apart. See the file's note.
- `fn_7100213380`: done, all five **O**. 0x40-byte polymorphic boxes by the centre on an axis. A
  copy rebuilds the box from its corners and stores them again as whole tenths. Three details
  mattered: one inline virtual keeps the vtable out of the file (the copy's calls stay indirect),
  the assignment is user-written (so the tenths and the last word are copied separately), and the
  elements are sorted through `UnkIter`.
- `fn_7100212a80` (not a sort: it builds a box tree and calls `fn_7100213380`)
- `fn_7100248e50`
- `fn_710030ed9c`
- `fn_7100336ca0`: all five **m** (weak handles assigned by `fn_71003384e0`, nearest first).
  The class iterator fixed the handle reload and the sizes. The comparisons still load the target
  and the positions in another order, whatever the distance's shape.

Copies of slice 2's `fn_71002dacc0` (a pair of empty string ids) outside slice 2's range,
`fn_7100a8e07c` and `fn_7100ef1970`, are stream 7's (coordinator's call, 2026-10-22): both **O**
in `src/game_s7/fn_7100a8e07c.cpp`.

### Copy-assignment of property records

(stream 7, 2026-10-22; `fn_710043439c` and `fn_71004c8f58`
are under 2 KB in slice 2's range but share the types, so stream 7 took them too, with no open PR
touching them). `fn_7100442780`, `fn_710043439c`, `fn_710048b740`, `fn_7100510548` and
`fn_71004c8f58` are all **O**: records holding 0x70-byte entries (a property with an any-like
value assigned by `fn_710009efb8`, then a 0x40-byte block). Types are in
`src/game_s7/UnkAnyPropsS7.h`. Each operator= is defaulted out of line and aliased to the address,
because a wrapper calling it doesn't inline it. With a defaulted operator=, the layout alone decides
the code:
- runs of trivially copyable fields become one memcpy (loads, then stores);
- fields copied one by one, interleaved, are wrapper types with a user operator= (`UnkVal<T>`,
  or a struct with its own operator=; field order inside it can even be reversed);
- an 8-byte gap that is never copied is a polymorphic member's vptr;
- the weak handle's operator= needs `always_inline`.
The memcpy of the last array entry is scheduled with whatever plain fields follow it, so the
load order there tells you whether the next field is plain or wrapped (`fn_7100510548`).
`fn_7100fbdfd0` (slice 6's range) also calls `fn_710009efb8` once; it is not part of this family.
The records' copy constructors are **O** in the same files: `fn_7100441cfc`, `fn_710048acac`,
`fn_710050f824`, and in slice 2's range `fn_7100433c5c` and `fn_71004c8ad4` (stream 7's, same
terms). Each is defaulted and aliased like the operator=. What the constructors needed:
- the entries are separate members (an array is copied by a loop) and derive from the property,
  each level storing its vptr;
- the records derive from a base (vtable 0x7101cd1b38) and sometimes a middle class, whose vptr
  store survives only if an untyped copy follows it (`fn_710048acac`'s two-u32 field);
- the block, `UnkPropRef`'s values, `UnkVal` and `UnkTail` have user copy constructors, so they
  are copied one by one instead of joining a memcpy run.

### Collection-type registration

(stream 7): `fn_71000e2800` (14 KB, **O**, no callers in code)
adds 105 type getters' results to their one-byte lists (`fn_710009ec00`), builds 60 `CClass`
globals from their names (`CollectionTypeMapper<...>` value types, `CRntVector<...>` and so on;
the constructors take four or six arguments), registers their destructors and 45 conversions.
Every global and callback is reached through the GOT, so all are plain `extern`s. The source was
written mechanically from the call sequence, with the helpers in `UnkStaticInitS7.h`.

### Exact copies of matched functions

(stream 7, claimed 2026-10-22): a whole-binary scan for `U`
functions whose instructions equal an `O` function's (local branches normalised, calls and page
addresses identical) found 115 (27 KB); 114 are written as copies of their originals' source in
`src/game_s7/` (one file per original file, named after its first copy; each lists its copies),
and `fn_71004a84c8` stays with stream 2. Several were "tried and left" by the clone rounds above
(0x7100287de8, 0x71000df388, 0x7100020898, 0x71000d1fec, 0x71002444f4); the hand-matched originals
now cover them. Rerun the scan after each batch of new matches: new `O` functions bring new copies.
A looser pass (calls may target different functions with the same position in each body, and
data page addresses may differ) found 35 more `U` copies of 17 originals, also claimed by stream 7
on 2026-10-22. They are written the same way, with each copy's own callees and data declared; the
largest family is `fn_7100d94830`'s erase-by-key (9 copies).
A third pass (also masking the offsets of loads and stores whose base came from a page address)
found 56 more `U` copies of 30 originals, claimed by stream 7 on 2026-10-22; the largest family is
`fn_7100d2ac00` (8 copies, in `fn_7100a15260.cpp`). Copy files keep the original's names, strings
and comments (asmdiff masks data), and their first line says so.
The two copies of `fn_71002430e8` (a generated clone, `refl_StrIdHashErase`) that release their
keys through CFilePathStrId's pool, `fn_71000741d4` and `fn_71011aa6a0`, are stream 7's (**O**).
Stream 7 probed the id table's destructor `fn_71000407f0` (96 KB, see "Giant functions") on
2026-10-22 and left it: built as a defaulted destructor of the generated table it reloads the pool
at the same ids as the original (every 99), but the pool's register is swapped in each stretch. The
original's first stretch keeps the pool in x8 (its first load still reads `this` from x0, before
the copy to x19); ours copies `this` first and keeps the pool in x0, and a 3-id table is all x0.
Ten more shapes of the id destructor (an early return, no local pool, a cached empty pointer,
`__builtin_expect` on each test, `always_inline`) don't change that.

### Slice 1 taken over by stream 7

(2026-10-22, coordinator's call): the hand slice below
0x71002a4814 (`U` game functions of 128 B-2 KB, `src/game_s1/`) had no active stream. Stream 7
starts from the shared headers and patterns found since (`src/game_s7/UnkIter.h`,
`include/game_s2/UnkWeakHandle.h`, `include/game_s2/UnkOwnedList.h`) and from near copies of
matched functions, not from the stragglers slice 1 left.

### Slice 1 near copies, batch 12

(stream 7, 2026-10-22): 80 functions **O**, mostly slice 1's
same-shape copies of matched templates with other element sizes, offsets, pools or callees
(`src/game_s1/`, one file per original, each header naming the original). Families: the
owned-list map Reserve (`fn_71001358e0`, 15 copies binary-wide; `Reserve` must be
`always_inline` or the extra copies become 4-byte tail calls; slice 5's `m` `fn_7100b48944` has
the same code), id-list owner destructors, out-of-line element arrays (move-assign, End, bound
refs), slot-array SetAt, id-record pair and map assigns, string-id map Clear/Begin/End, variant
resets and assigns, pair assigns and equality. Also `src/game_s7/fn_7100f798fc.cpp`: the five
string-id map End iterators after the begins of `fn_7100f79844.cpp` (slice 6, no active stream).
Left: `fn_71000d56ac`, `fn_71000e00a0`, `fn_710111410c` (array equality through an out-of-line
element compare, 94%: one register). The TU static initialisers `fn_7100205820` and
`fn_71001f2650` left here are **O** in batch 13.

### Slice 1 static initialisers, batch 13

(stream 7, 2026-10-22): 16 of slice 1's 33 per-unit
static initialisers **O** (about 12 KB; `src/game_s1/`, with game/UnkTUInit.h's helpers). New
shapes, kept local to the files: header ids built the other way round ("sources" first, the word
stored last as in `fn_71001258b0`, or not at all), sometimes followed by the three function
records (`fn_71000f3150`, `fn_71000113b0`); a class kept right after the header (registered
after its getter is called); enum converters whose functions are local to the unit (declared
`visibility("hidden")`, `fn_7100261530`, `fn_71001fa720`); resource types added to a registry
(`fn_7100005700` for the animation and curve resources, `fn_71000113b0` for blend spaces).
`fn_71001545d0` (fonts) is **m**: two identity matrices whose stores merge differently. Left:
the rest have one-off layouts (sub-allocators `fn_7100224c70`, whose header ids merge with the
statics; game tables `fn_710015ef10`; larger units `fn_7100140400`, `fn_710001f460`,
`fn_71001b2e60`, `fn_71001e0820`, `fn_7100158130`), 0-60% as drafted.

### Slice 1 near copies, batch 14

(stream 7, 2026-10-22): 22 functions **O** (`src/game_s1/`),
copies of generated or hand-matched functions whose code differs only in element types, offsets
or callees. Each copy names its own callees and data. They cover a vector Free inlined into its
owner at an offset, cached iterator value equality (`UnkIterValue` with 0x18- and 0x30-byte
elements compared out of line, or 64-bit keys, which only match through a key struct with its own
`operator!=`), End and Insert for 1- and 2-byte elements, by-value equality with 64-bit indices,
deferred callback calls with bool and u16 arguments, a pair equality, a record destructor and a
reflected-value handler. Wrappers around a template that the compiler won't inline (`Insert`,
`refl_DeferredInvoke`) use `__attribute__((flatten))`. Left, all 60-88% when drafted:
`fn_7100124084` (scheduling), `fn_710020d4b8` (calls swapped, which changes the block layout), the
string-id keyed copies `fn_71000de3b0`/`fn_71000de8e8` (float elements), `fn_71000de574` and
`fn_71000de6b0` (4-byte elements), and reflection type getters whose frames differ
(`fn_71000d5e74`, `fn_71000d5f4c`, `fn_71002475b0`).

### Slice 1 near copies, batch 15

(stream 7, 2026-10-22): 38 functions **O** (`src/game_s1/`).
Most are construction and assignment of the reflection collections' cached iterator values and
value pairs, for other element and key types (`fn_71000cd580.cpp`, `fn_71000e0f70.cpp`). A
`refl_IterValueMove` instantiation wrapped in a `flatten` function reproduces each move
constructor. Elements that the original copies member by member need a user-written move
constructor; with an implicit one the copy merges into wider loads. The copy constructor (state 1
copied, state 2 read through the reference) is local. Pair equality over float vectors compares
within a tolerance (0.001, or the global `g_unk_7101647774` read at each use). Also: string-id
arrays (clears, guarded clears, moves; some in CFilePathStrId's pool), a string-id move-assign,
a string-id pair assign, and a transform node angle setter that also keeps the negated z angle
at 0x150 (negate the stored angle, not the argument, to get the integer `eor`). Left at 0-48%:
copy- and move-assignment of 8-byte iterator values (`fn_71001f1d38`, `fn_71001f1db4`), a
push-back that the copy file won't inline (`fn_71002a1778`, `fn_7100279ca0`), and drafts whose
originals are generated (`mkcopy2` only finds hand-written originals).

### Slice 1 near copies, batch 16

(stream 7, 2026-10-22): 28 functions **O** (`src/game_s1/`).
More of the cached iterator values: equality with 8-byte keys (`fn_71000d4d50.cpp`), moves whose
elements are a string id and a file path id, a reflected value, a counted reference, or a file
path id with a counted resource (`fn_71000cd580.cpp`), and resets and move-assigns of values
owning string ids (`fn_71000d0378.cpp`). Reflected-value and reference elements only inline as a
placement-new move in a `flatten` function. A counted reference copies as "null, then assign"
(the original reloads the slot after the count update, so the count sits in a union, as in
`UnkWeakHandle.h`). In a move-assign, a string id re-reads its pool for the other's empty string
while a file path id keeps the pool read for the release. The relocation check also caught three
batch 15 functions that named the wrong string pool (`fn_71000cdbbc`, `fn_71000ce0a4` use the
default one, `fn_710000cc44` CFilePathStrId's); they now name the original's pool. Left: `fn_710001dc08` (an array
erase, not the move-assign its size suggested).

### Slice 1 near copies, batch 17

(stream 7, 2026-10-22): 5 functions **O**, 2 **m**
(`src/game_s1/`). One is the `refl_InitClass` of an unnamed class whose getter is inlined into
`fn_7100103ef0` (`fn_71000f56d4.cpp`). It is written as a hand-written trait specialization plus
an explicit instantiation of the shared `CClass` constructor, because `gen_reflection.py` only
emits classes that have a getter. The others are a move-assign of an id and file path record
(`fn_710001d15c`, `fn_710001cd80`), equality of holders of an id and weak handles compared by
target (`fn_7100244aac.cpp`), and file-path iterator copy-assigns at 73.6% that differ only in
registers (`fn_71000cfe14`, `fn_71000cd658`, **m**).

### Slice 1 near copies, batch 18

(stream 7, 2026-10-22): 6 functions **O** (`src/game_s1/`):
- two destructors of a reflection class that free an array of 16-byte id records, then call the base destructor (`fn_7100099468.cpp`);
- clears of a string-id table keyed by string ids (`fn_71000d99d0`, `fn_71000db40c`);
- a destructor of three ids and a string, cleared first (`fn_7100133be4.cpp`);
- the erase of a hash set laid out as the string pool set, with its link at 0x10 (`fn_71000866dc.cpp`).

Not attempted: the callback-set iterator copies `fn_71002475b0` and `fn_71002476ec`. They need the compiler-generated vtables of a copied class, which the copy files can't name yet.

### Slice 1 near copies, batch 19

(stream 7, 2026-10-23): 8 functions **O** (`src/game_s1/`):
- a deleting destructor that frees an array of 16-byte id records (`fn_710016a7f8`);
- keyed value array destructors, one with its own vtable and values freed by `fn_7100020164`, two with polymorphic values (`fn_710001fa30.cpp`);
- type-descriptor push pairs (`fn_7100263ea8.cpp`);
- `End` for 12-byte elements (`fn_71000cc60c`, `fn_71001205d0`).

The copies call the shared clone templates through `flatten` wrappers, with traits of their own.

### Slice 1 near copies, batch 20

(stream 7, 2026-10-23): 6 functions **O** (`src/game_s1/`):
- destructors of a file path id and a counted resource, with or without a leading word (`fn_7100269170`, `fn_710000f514`);
- an insert into a string-id hash map keyed by the name an object reports through its vtable (`fn_71000875d0.cpp`);
- `End` for 0x18-byte out-of-line elements (`fn_71000d5f4c`) and for a word table keyed by string ids (`fn_71000df938`);
- a bound-pair assignment for 0x18-byte records (`fn_71000d5e74.cpp`).

Also fixed `fn_71000db40c` from batch 18: its values are file path ids. It matched byte for byte, but it named the default pool, which the relocation check caught.

### Slice 1 near copies, batch 21

(stream 7, 2026-10-23): 7 functions **O** (`src/game_s1/fn_7100099468.cpp`):
- the deleting destructor of the class that frees its id records at 0xf0 (`fn_7100099550`), and the complete destructor of the class with id records at 0x10 (`fn_710016a714`);
- `Free` of arrays of bare string ids from either pool (`fn_71001b15b4`, `fn_71000d076c`, `fn_71000cdfb0`);
- `Begin` over the same arrays (`fn_71000d0f1c`, `fn_71000ce8b8`).

The matching `End` copies (`fn_71000d0ff4`, `fn_71000ce990`) are left at 96%: the original loads the count before the data pointer.

### Slice 1 near copies, batch 22

(stream 7, 2026-10-23): 5 functions **O** (`src/game_s1/`):
- `PushBack` of a copy for records of a word and a 0x100-byte object (`fn_71002a1778.cpp`, two copies of `fn_7100b1a860`);
- the complete and deleting destructors of a class with a string id and a string (`fn_710002ddc0.cpp`). The deleting one releases the id without resetting it, so it is written out;
- a flag lookup in a `CRntVector`-bucketed hash map (`fn_710014bdec.cpp`, like `fn_710019e100`).

Left: `fn_71001f1db4` (an iterator value move-assign with 64-bit keys whose stores alias the state; 23%).

### Slice 1 near copies, batch 23

(stream 7, 2026-10-23): 3 functions **O**, 1 **m** (`src/game_s1/`):
- a hash-set key move (`fn_710014239c.cpp`, a copy of `fn_710019e100` that appends before erasing);
- the Clear of a string-id keyed table of counted resources (`fn_71001d7444` in `fn_71000d0378.cpp`; the value loop only matches as a do-while);
- a byte array copy constructor (`fn_71000e1848` in `fn_71000d1f3c.cpp`).

Also `fn_7100272bbc.cpp`, a pair assignment copy of `fn_7100c0837c`, is minor: the original copies the record's 0x15-byte tail last 8 bytes first. Left: the set erase `fn_7100142504`/`fn_710019e268` (15%; the list links only alias as an array, but the bucket-chain loop still differs).

### Slice 1 near copies, batch 24

(stream 7, 2026-10-23): 6 functions **O** (`src/game_s1/`):
- complete and deleting destructors of two classes that hold a string id and an array of plain values (`fn_71001f9c3c`, `fn_71001f9cd4`, `fn_710026cd30`, `fn_710026cdcc` in `fn_710002ddc0.cpp`);
- a hash-set key lookup (`fn_710019e4fc` in `fn_710014bdec.cpp`);
- an iterator value move-assign whose file path id re-reads its pool for the other's empty string (`fn_7100267684` in `fn_71000d0378.cpp`; `UnkReleasedStrIdT` now takes a `Reread` flag).

These were found by re-running the near-copy search against the functions matched in batches 20 to 23.

### Slice 1 near copies, batch 25

(stream 7, 2026-10-23): 21 small functions **O** (`src/game_s1/`). They came from widening the near-copy search to functions of 64 bytes and up:
- getters building reflected values (`fn_71002678c8.cpp`);
- calls of a virtual slot on each child of an array with a float or bool argument (`fn_710014be78.cpp`);
- iterator value moves with 64-bit indices and out-of-line element moves (`fn_71000dbc34.cpp`);
- find-by-id-and-forward wrappers (`fn_71001fcd4c.cpp`, copies of `fn_71001fd8a0`);
- array equality for words and floats, and a keyed set with u64 keys (`fn_71000d25f0.cpp`).

### Slice 1 near copies, batch 26

(stream 7, 2026-10-23): 33 small functions **O** (`src/game_s1/`), from the near-copy search at 36 bytes and up:
- 13 more find-and-forward wrappers (`fn_71001fcd4c.cpp`);
- destructors: an id plus an allocator-owned block (`fn_710002ddc0.cpp`), chained-set holders (`fn_7100022518.cpp`), an array holder (`fn_710014a86c.cpp`), a node list (`fn_7100218a2c.cpp`);
- reflection invokers: a constant-true getter (`fn_7100254728.cpp`), a two-float setter (`fn_71001109c4.cpp`), in-place array construction (`fn_710025d844.cpp`), iterator increments over refcounted pointers (`fn_7100236ff4.cpp`);
- a wide-string search (`fn_7100067460.cpp`), a translation matrix (`fn_7100078c9c.cpp`), a timer reset whose division by 1000 is written out the way the original computes it (`fn_7100080c40.cpp`), and a sub-allocator free hook (`fn_71001039b4.cpp`).
Dropped after the time box: `fn_71000876d8` (hash lookup; load scheduling) and `fn_71001f1d38` (iterator value copy with 64-bit keys).

### Slice 1 near copies, batch 27

(stream 7, 2026-10-23): 35 small functions **O** (`src/game_s1/`), mostly groups of identical copies:
- string id assignment from a string, in both pools (`fn_71000cc800.cpp`); destructors of an id and path pair (`fn_710001cbd8.cpp`);
- keyed-array lookups returning null when absent (`fn_71002519a8.cpp`), and a byte-array equality (`fn_71000d25f0.cpp`);
- node list destructors with node destructors, and a list clear (`fn_7100219134.cpp`); linked-list iterator increments (`fn_710023644c.cpp`);
- per-child virtual calls and calls over element arrays (`fn_710014be78.cpp`), one more find-and-forward variant (`fn_71001fcd4c.cpp`), a float-sum reflection getter (`fn_71001109c4.cpp`);
- destructors: pooled objects (`fn_710019bfb8.cpp`), a three-member class (`fn_7100086d7c.cpp`), a counted reference (`fn_71000139c0.cpp`) and a vibration player derived from `nn::hid::VibrationPlayer` (`fn_71000707dc.cpp`, declaring the two SDK destructors it calls);
- a matrix composition (`fn_7100078c9c.cpp`).

### Slice 1 near copies, batch 28

(stream 7, 2026-10-23): 20 small functions **O** (`src/game_s1/`):
- destructors: a counted id (`fn_710002ddc0.cpp`), a dead-marked object (`fn_71001f3438.cpp`), a plain object with blocks and an array (`fn_710019506c.cpp`), counted and id members (`fn_71001bd92c.cpp`), an active flag and an owned mutex (`fn_710008e8cc.cpp`), owned-object arrays (`fn_7100243aac.cpp`);
- constructors (`fn_7100138090.cpp`, `fn_71001a9f84.cpp`), record copy and copy-assignment (`fn_71001b3a3c.cpp`, `fn_71000d0378.cpp`);
- a folded-hash set lookup (`fn_710014bdec.cpp`), a keyed presence test (`fn_71002519a8.cpp`), a table end (`fn_71000d0378.cpp`), a lazily created part's id setter (`fn_71001c3504.cpp`), an interpolated value setter (`fn_7100105f1c.cpp`);
- two millisecond clocks (`fn_7100080c40.cpp`, calling `nn::os` and `nn::oe` by their SDK names).
Dropped after the time box: `fn_71000de3b0`/`fn_71000de8e8` (iterator value equality over floats; register order).

### Slice 1 hubs, batch 1

(stream 7, 2026-10-23): work order switched to hub functions (most
unmatched callers first). **O**: `fn_710009d50c` (reflected is-kind-of walk over `CType::m_parent`,
1778 unmatched callers), `fn_71000a18d4` (builds an `UnkReflValue`; the last argument is a u32
mode, 1 = by reference, so `UnkReflValue.h` now declares it `u32 by_ref`; the type is stored as a
plain pointer so the copy function reloads), `fn_710002614c` (transform matrix getter, 545
callers), `fn_7100076cec` (transform scale). `fn_7100079b18` (transform angles) is
`NON_MATCHING(minor)` at 91.5% (b's x/y swap registers). `include/game/UnkXformNode.h` now has the
full `UnkXform` layout (4x4 matrix, cached angle 0x40, scale 0x4c, flags 0x58) with
`GetTranslation`/`GetAngle`/`GetScale`, and `UnkVec3f` gained a 3-float constructor (the matrix
getter's translation needs it). Unmatched local re-declarations of `fn_71000a18d4` elsewhere still
use their own types.

### Slice 1 hubs, batch 2

(stream 7, 2026-10-23): 27 unmatched callers of the transform hubs
**O** (`src/game_s1/`), all through `include/game/UnkXformNode.h`: angle/scale getters of objects
holding an `UnkXform` (`fn_71001413d4` and five copies), pose copies into a node (`fn_71001439d8`
and four copies), matrix setters that pass the translation on (`fn_71001f70dc` and three copies,
`fn_710020d744`, `fn_7100206294`), node angle/scale/translation setters with change checks
(`fn_710020d5b8`, `fn_710020d33c`, `fn_710020d4b8`; `fn_7100209b48` sets the angles but tests
the translation), `fn_710020aafc` (scale by a factor),
`fn_710020a44c` (pushes a dirty node to its target), `fn_710023415c`, `fn_71001bc8a4`,
`fn_710023e348` (reflected angle getter) and `fn_7100196674` (cached aspect-scaled matrix).
`UnkXform` gained `GetMatrix`, `SetAngle`, `SetScale`, `SetTranslation` and a from-matrix
constructor; `UnkMatrix44` moved to `include/base/UnkMatrix44.h`; `UnkXformNode` gained the fields
at 0xf9-0x108 (static_assert on its size; `fn_710020a6e8` and `fn_71010d3508` adjusted). Caution: `UnkXformNode`'s own fields (`angle_0x58`, `flags_0x70`, ...) and
`Xform()->...` describe the same memory with different struct paths, so type-based alias analysis
treats them as independent; within one function use one view. Folding `UnkXformNode` onto an
embedded `UnkXform` (27 files) would remove that. Dropped after a time-box: `fn_71000127c8`
(matrix product: the original reloads the zeroed rows 1-3 of the result), `fn_7100025bd0`/`fn_7100025d10`
(transform tree update: matrix copy split and a leftover flag branch).

### Slice 1 hubs, batch 3

(stream 7, 2026-10-23): 8 more transform callers **O**: bone world
matrices by index (`fn_71001c1b70`; by id `fn_71001c1694` is `NON_MATCHING(minor)`: the original
compares the not-found end pointer with itself), `fn_71001bfd5c` (matrix follower),
`fn_71001bc740` (move and face the new position; `fn_710007a7f0` takes its direction by value,
which a non-trivial copy passes by address), `fn_71001f4434`, `fn_71001eaf3c` (pool entry
placement), `fn_7100140f8c`/`fn_7100145f80` (apply a transform but keep the own scale) and
`fn_7100208a64` (target link on/off). `UnkXform` now has a word-by-word copy constructor;
`include/base/UnkMatrix44.h` has `UnkCopyMatrix` (written out, since a loop becomes a memcpy).
`UnkXformNode` is back to ending at the target pointer (0x110 bytes): the matrix at 0x110 belongs
to one subclass (`fn_710020a44c`), and another keeps flags there (`fn_7100208a64`).

### dlmalloc 2.8.4 vendored

(stream 7, 2026-10-22, owner-approved): `lib/dlmalloc/` holds
upstream `malloc.c` unmodified and `mspace.c` with the game's settings (README there). It was
fetched from a mirror pinned to a commit in `tools/fetch_sources.py` and `data/sources.json`,
because the author's site is blocked here. `tools/sigmatch.py --library dlmalloc` names the five
functions the game links (`create_mspace_with_base`, `mspace_malloc`, `mspace_free`,
`mspace_memalign`, `dlmalloc_usable_size`, 0x71000815b0-0x7100082c38), and
`tools/gen_functions_csv.py` marks them **L**. Three are byte-identical; `mspace_free` is 8 bytes
longer in the game (a near match). For the owner: `include/base/UnkDefaultAllocator.h` still
calls these "apparently dlmalloc's mspace functions" and can now name them.

### Matching does not check relocated targets

`tools/asmdiff.py` masks every relocated field, so
a function stays **O** with the wrong global, GOT slot or callee named. Two slips found this way
and fixed: `src/game_s7/fn_710104ed30.cpp` released its ids through the default pool where the
original uses CFilePathStrId's (`g_unk_7101d47200`), and vtables named 0x10 below the address
the GOT slot holds (the slot holds the vtable itself; `&Vtable[2]` is what gets stored). A
scratch check compares each `fn_`/`g_unk_` relocation of an **O** function against the original
instruction at the same offset (b/bl targets, ADRP pages, low-12 offsets, and for GOT loads a slot
holding that address); worth adding to `check.py` (owner's call). Over `src/game_s7/` and
`src/game_s1/` it now finds nothing. The older stream 7 copy files used to call the original's
callees and read its data (about 670 references in 24 files: right for matching, wrong for a
port); they now name their own callees, data and strings, and their headers say only the comments
describe the original. Where a shared helper held the name, the helper takes it as a template
argument (the string pool of `UnkReleasedStrIdT`, the vtable of `UnkIdPropertyT`, the owner
assignment of `UnkCallbackSet`). The scratch renamer that did this (each relocation's original
target, renamed inside that function only) is the same check run the other way.

### Field registrations the reflection parser stopped at

(stream 7, 2026-10-22), both **O**:
CCharClassAIComponent's member fields (`fn_7100408c20`, 33 KB, 142 fields, the giant-functions
table's "tunable registration") and CExplosionGfx's (`fn_7100ddf434`, 3.6 KB). Both are
`tools/refl_attrs.py` drafts finished by hand, in `src/game_s7/`. What the parser lacks: defaults
of a reflected struct type (four `SDeathImpactDisplacement` values, two 8-byte stores, typed by
the class getter); defaults copied from global `CVector2D`/`CVector4D` constants through the GOT,
which match only as member-by-member copies (`CVector2D{g.x, g.y}`); and an empty `CStrId`
default, which the original builds without testing the pointer after the pool check
(`UnkEmptyStrIdValue` in `fn_7100408c20.cpp`; `CStrId(UnkStrIdInline{}, "", true)` tests it).
The large one also needs `alternatives()`' typed lambda form. Still `U`: `fn_7100beaf3c` (a
`CRntString` default joined from two globals with "|"), and five small ones (`fn_710044e7fc`,
`fn_71004ded68`, `fn_7100b8bb94`, `fn_7100097378`, `fn_71002584a0`).

### Render resource types

(stream 7): `fn_71001b1dd0` (**m**, 2.5 KB) registers nine render
resource types through a record with an out-of-line constructor and destructor (`fn_71000e9a7c`,
`fn_71000e8dd0`). Findings: this TU's file-path ids decrement and test in two steps and assign as
destroy-then-move; the vector growth moves elements bitwise (`memcpy`, so `count` is reloaded) with
`src`/`dst` pointers and a `u32` counter; the file's statics are one merged block
(`g_unk_716e5543a0`). Left: the last copy loop's count stays in memory in ours (one instruction).

### Copies of stream 2's shapes

(stream 7, claimed 2026-10-22): the static-id guard copies of
`fn_7100464520` (`fn_71005b9d70`, `fn_71006e0f90`, `fn_7100898e80`, in `fn_71005b9d70.cpp`) and the
copies of the compare `fn_71004a84c8` (`fn_71005d9dec`, `fn_71005ee098`, in `fn_71005d9dec.cpp`;
each member of the compared value is inline or, in mode 2, a pointer; the loop index must be `u64`).
`fn_71004a84c8` itself stays with stream 2. Also claimed (2026-10-22): the three copies of each of
stream 2's owned-list array functions (`UnkOwnedList.h`) at 0x71005da4f8-0x71005db4c0,
0x71005e3778-0x71005e4740 and 0x71005ee7a4-0x71005ef76c (clear, compare, free, move-assign, erase,
grow-at, begin, end), in `fn_71005da4f8.cpp`.

### Inventory string and projectile models

(stream 7): `fn_7101147868` (**O**, 4.9 KB) appends a
"SamusInventory" record whose value is 26 digits, one per item definition (1 when the inventory's
map holds an amount above zero under the item's qualified id). Findings: the lazy id check must be
a pointer compare (`data == &pool->empty`; the integer compare other files use inverts the branch
here); the record is built as a temporary inside the append call; and its constructor
(`fn_7100ec4240`, declared with an `asm` label) takes the two ids by value, so the caller destroys
the copies after the append and keeps their final stores. `fn_7101144a5c` (11.8 KB, next to it)
builds a local map from the 26 area-and-item names and walks it; still `U`.
`fn_7100ed4cb8` (**O**) loads the nine beam projectile models through the manager's pool; the
model pointer must be declared before the path id (stack order).

### Hash lists by height

(stream 7, 2026-10-22): `fn_71003f2a8c` and `fn_71006c26fc` clear a
list of 64-bit name hashes and push three to five constants, chosen by the height difference of two
positions against 300. `fn_71006c26fc` is **O**; `fn_71003f2a8c` is **m** (one store's index
register). Both are in `src/game_s7/fn_71003f2a8c.cpp`, using a local copy of the `UnkVector` layout
whose PushBack copies the old elements one by one through a pointer. Two details mattered:
- The list must be a reference parameter. Only then can the step be loaded before the first push's
  capacity test (the original selects the step when the capacity is 0).
- Each branch pushes its own last hash and returns. A shared tail lets Clang sink the pushes into
  one with a phi of the constant.

### Flag lists

(stream 7): `fn_7100eed9ec` (**O**) and `fn_7100600c98` (**m**) build a local list
of `u32` flags on the stack (`src/game_s7/UnkFlagListS7.h`: a linked list with a vtable,
allocator and flags 0x13) and hand it to `fn_71008724f8`. The list's fields are union members:
the original reloads them after every node store. `alignas(16)` puts the list at sp+0x10.
`fn_7100600c98`'s remaining difference is that the first call's flag statics share the list's stack slot.

### Link groups

(stream 7): `fn_7100de7d88` (**O**) is the defaulted destructor of 13 link groups,
each three pointer arrays plus one field. It needs 13 separate members: an array of
non-trivially-destructible members becomes a loop. `fn_7100b34b44` (**m**, registers swap from the
second push) clears a pointer list and pushes 14 addresses inside the same object.
