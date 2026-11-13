# Handoff: dread-decomp

The current state in one screen, for the next developer, human or AI. Rules and conventions are in
`CLAUDE.md`; the route to a running PC build in `docs/PORT_PLAN.md`; stable facts (repository map,
rebuilding from scratch, key facts about the binary, decisions made, gotchas) in `docs/reference.md`;
history and the index of matching findings in `docs/JOURNAL.md`.

**Who edits this page:** only the session that merges a round of pull requests, after the merge.
Feature branches add a journal entry instead and leave this page alone, so it never conflicts.
Keep it under about 100 lines: anything longer goes to the journal with a link from here.

Target: **Metroid Dread 2.1.0**, `main` build ID `646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`.

## Start here

- **Open work:** `gh pr list` is the source of truth for what is waiting to merge.
- **What to match next:** `tools/hubs.py --version 2.1.0 --boot`, then `--ready --boot`; procedure in
  `.claude/skills/dread-match` (one function) and `.claude/skills/dread-hubs` (a batch).
- **Port:** `tools/port_link.py --version 2.1.0` rebuilds `build/port/link/boot_missing.csv`; the Linux
  executable and the boot run are in `port/README.md`, "Linux in Docker".
- Every number here names the command that produced it; rerun the command rather than trust the page.

## Where things stand

| Area | State |
|---|---|
| Phases 1-3 (dump, recon, compiler) | **Done.** `docs/recon.md`, `docs/compiler.md` |
| Generators | **In maintenance.** `src/lifted/` 46,443 functions; the lifter's byte mode is at its limit |
| Semantic lifts, every `U` function | **13,643 of 13,691 accepted** (lifted_all14, lifter round 10, #310; the lifter is frozen here, see Direction; the worklist is 2,450 smaller since those are now hand-matched in `src/`; `port_lift.py --slice all --jobs 6`, 109 min, 300 s oracle budget per function, `<out>/timing.tsv`). Left: 58, 36 of them rejected or inconclusive; round 10 bisects 6 new losses and tries `--small-out-counts` and `--junk-scratch` |
| Hand matching (Phase 4.4) | **In progress.** Hub-first along the boot path, then near copies of matched functions (`tools/nearcopy.py` over the whole binary); no conflicting signature (the last 20 were settled on 2026-11-12), 1,243 extra copies of declarations and 1 definition that is called differently than a declaration says are left, and 984 definitions name a type differently than their declaration (`--definition-types`, counted since 2026-11-12) (`tools/decl_check.py`, `--definitions`, 2026-11-12; 9,308 copies before the unit headers of #360 and #362). The port's hand-written boot functions were compared instruction by instruction in #264-#266: 11 `O`, the rest minor with the differences in their comments (journal, "port functions round 3") |
| Hand-typed, readable code (the completion metric) | **3.38 MB `O` (18.0% of the game's code), 3.80 MB `O`+`m`: 29.2% of matched bytes** (`tools/progress.py --origin`, 2026-11-11, from the check cache; 2.20 MB on 2026-11-05, 0.76 MB on 2026-10-24). 1.93 MB of it is in agent-batch files (`*_ov*`, `*_b<NN>*`, `*_l<addr>`). Generated code is the other 70.8%: it verifies, it does not count |
| Lifted code still in the port (scaffolding; must shrink to zero) | **13,643 semantic lifts, 7.39 MB** linked whole-binary (`build/port/lifted_all14/manifest.json`, lifter round 10, #310); 2,266 lifts, 1.21 MB on the boot slice (`build/port/lifted/manifest.json`); 46,443 byte-identical `src/lifted/` functions beside them. Shrinks only by hand matching |
| Port M0, M1 steps 1-4 | **Done.** Compile sweep, whole-program link, semantic lifts with the P-code oracle, runtime stubs, data loader |
| Semantic lifts, boot slice (`build/port/lifted`) | **2,171 of 2,220 accepted** (1.20 MB of 1.26 MB; oracle median coverage 87%); 49 left (21 inconclusive, 17 rejected, 7 indirect calls with stack arguments, 4 other); 95 more override `src/lifted` copies that lose arguments |
| Semantic lifts, real boot to the title screen | **Every function the boot enters before the title screen has an accepted lift**: trace11 accepts 5,497 of 5,522 (`build/port/trace2_list.txt`, the recomp trace without status `L`), 23 in `src/`; the 2 left are never entered before the title screen. The last ones pass on real entry states and recorded calls from the recomp (`recomp_trace.py --snapshot`) |
| Port, Linux executable | **Boots into gameplay** (journal, 2026-11-09: title, prologue and visor movies, then the first room with the HUD, scripted input, on lavapipe; not rerun for this page). The nightly gate is the earlier milestone, the frame loop with all 10 boot milestones (`tools/boot_distance.py --nvn`, journal round 8); it needs Docker Desktop running on the runner and failed on that on 2026-11-11. Rounds 3-8: the full and boot-trace lift sets, one heap, statics bound to the image, Clang 9's ABI rules, a silent `nn::atk`/`nn::audio`, the NVN shim on lavapipe, `--no-relax`. Left (journal, 2026-11-09): `fn_7100212a80`, `fn_7100eb9b90`, and a movie-worker crash in one boot of three |
| Port M2 (graphics) | **Shim complete for the start-up.** All 1,342 shader stages to SPIR-V; all 146 NVN functions the game calls; all 671 programs drawn on lavapipe |
| Windows | **Compile only.** All sources compile with llvm-mingw; Linux stays the one running target until the title screen |

## Progress

<!-- progress:begin -->
Generated by `tools/progress.py --slice boot --handoff` on 2026-11-12 from `data/functions/`; rerun it rather than edit.

| Scope | Functions | Bytes | `O` matching (bytes) | `O`+`m` usable | `U` left |
|---|---|---|---|---|---|
| Whole binary (`L` excluded) | 101,374 | 18.77 MB | **66.90%** (94,714) | 69.45% | 6,194 |
| Boot slice (from `nnMain`, direct calls) | 4,091 | 1.71 MB | **44.14%** (3,055) | 54.58% | 836 |

Done, whole binary (`O`, or `m` that `tools/equiv_test.py` verified equivalent): **66.93%**; 28 of the 452 `m` functions are verified (5,692 bytes).
<!-- progress:end -->

Of the 12.55 MB matched, 9.16 MB is generated (reflection 4.56, lifted 2.18, clones 1.29, reflected types,
statics and tables 1.14) and 3.38 MB is hand-written (`tools/progress.py --origin`, 2026-11-11). Of the 5.72 MB
still `U`, 4.74 MB is in functions over 600 bytes.

## Direction (owner, 2026-10-24: clean, readable, playable in a few months)

**Purpose (owner, 2026-11-05, `CLAUDE.md`):** a full, human-readable source port that anyone builds from
their own files, with no translation layer in the product; a tool for the community's understanding of a game
they already own. Hand-typed code is the completion metric; lifted code is scaffolding that must shrink to zero
(both rows above).

**Owner decisions, 2026-11-11** (after the independent maintenance review; `CLAUDE.md`, "Purpose"): the repository is
public, to get outside help. Decompilation is complete when every function is hand-written, readable and
proven equivalent: a byte match, or a registers-only difference that a machine test confirms (three tiers,
`CLAUDE.md`); readability outranks a match. Generated code, `src/lifted` included, is a stepping stone. A template family (the reflection boilerplate) counts as hand-written once its template
reads as source. `nn::atk` is decompiled as its own library (`docs/reference.md`). **Bulk agent runs are on hold**
until class and declaration headers are generated for workers to share (`check.py` compares string and constant
contents since #355; unit headers hold the one declaration of 2,025 shared symbols since #360 and #362, and
`data/reflection_fields.csv` lists every reflected member since #365; the 40-function test of 2026-11-12 showed
no gain from them, so what is left is a worker brief that adds declarations to headers, open decision 4); `nn::atk` is the exception. The owner allowed one measured pilot on 2026-11-12: 50 ready
boot-path functions gave 8 matches at about 736,000 tokens each, and the declaration counts held only with
hand work after the mechanical step (journal, "Pilot batch of 50 functions"); widening is the owner's call.
The kit (`tools/batch/lean/`), its worker brief and the two skills follow the tier rules since 2026-11-12:
a registers-only result goes to `tools/permute.py` and `tools/equiv_test.py`, every other non-match to
`drafts/` (`tools/drafts.py`), and the coordinator moves copied declarations to unit headers. The rewritten
brief has not been run by a worker yet. The coordinator of a piece of work merges it; the owner merges
sweeping revisions of `main`.

Route B (`docs/PORT_PLAN.md`): one program now, lifted code as a bridge that matching replaces. Hubs
first along the boot slice; one declaration per shared symbol in `include/`; files by unit, not by
address; relocation targets are part of a match; generators in maintenance except the lifter's
semantic mode; pull requests of a hub cluster or about 30 functions, merged on a green check.

## Workstreams and next steps

One session per stream; each edits only its files. Changes to `include/` say so in the PR title.
The full per-stream lists as of 2026-10-27 are in the journal, "Handoff next-steps snapshot".

| Stream | Files | Branches | Next |
|---|---|---|---|
| A. Boot slice and hubs | `src/<subsystem>/`, migrated `src/game_*/`, `include/` | `claude/boot-<topic>` | The resource registry's lookups (`fn_71000ea448`, `fn_71000ead80`; base/UnkResKey.h has their types), `fn_7100ee1f34` and the rest of `hubs.py --boot`; near copies below 90% (`tools/nearcopy.py`); the low-scoring minors of #264-#266 |
| B. Core engine types | `src/base/`, `include/base/` | `claude/core-<topic>` | Reflected function array growth and ctor/dtor; allocator functions by `operator new`; string pool's `m` functions; fix the `runtime?` inference that marks matched functions `L` |
| C. Gameplay slice | `src/game/`, `include/game/` | `claude/game-<topic>` | After the title screen |
| D. Port layer | `port/` | `claude/port-<topic>` | M1/M2: the frame loop's traps (`fn_710014d1a0`), the 26 unbindable function-local statics (journal round 8), drop the 11 byte-mode overrides once src/lifted is regenerated; M2: alpha test, Vulkan surface, texel upload, render-target formats 0x29/0x32 |
| E. Tooling | `tools/`, `.github/` | `claude/tools-<topic>` | Boot trace from Eden's GDB stub; subsystem inference from `tu_map.csv`; the 504 `src/game_*` files with no known unit and their 15 local headers (to `include/`) |
| M. Generators | `tools/gen_*.py`, generated `src/` directories | `claude/gen-<topic>`, `claude/lifter-<topic>` | Indirect calls with stack arguments (count the outgoing slots, compare them in the oracle), inputs that crash or loop on every seed, the oracle's rejections |

Merging: merge `main` into the branch, take `main`'s `data/functions/*.csv`, rerun only the generator you
own, then `tools/check.py --update` (`tools/verify.py --regen <generator>` does both). Before the PR:
no `REGRESSION`, `DUPLICATE` or hand-written `RELOC` lines; `decl_check.py --base origin/main` PASS; a
journal entry.

## Open decisions for the owner

None open. Decided on 2026-11-12:
1. `tools/decl_check.py --strict` runs in CI: one more identical local copy of a declaration fails (#373).
2. Semantic lifts stay generated and untracked (`docs/PORT_PLAN.md`, decision 2).
3. A labelled member is its symbol's declaration, and the free prototype for that symbol goes. The 94 labels of
   `claude/project-thread-3x0lf8` predate the unit headers; they are to be redone on current `main` with the class
   headers, and the branch goes then.
4. The 40-function test of shared headers ran on 2026-11-12 and showed no gain (7 of 20 matched on the current
   recipe, 8 of 20 with the shared material, at the same cost; journal). Struct headers are not generated. Workers
   still copy a declaration (parallel workers must not edit shared headers) and the coordinator's step moves the
   copies (`tools/batch/lean/COORDINATOR.md`, step 10). Before a bulk run the hubs
   without a header need one (`fn_7100353be8`, `UnkGameManagerIdTable`; `fn_7100dec788` and `fn_7100d78378` have
   theirs since the third declarations pass).
