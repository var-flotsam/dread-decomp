# Task cards

Short, self-contained jobs for the Mac workstream (a small local model; see `AGENTS.md` and
`docs/HANDOFF.md`, "Workstreams"). Each card fits in about 2k tokens and lists the goal, the files
you may edit, exact commands, an example address, what done looks like, and a time-box.

Do them in order. Always do `00` first on a new machine or after a toolchain change.

| Card | Title | Generator | Size of the prize |
|---|---|---|---|
| `00-triage.md` | Triage: check the setup and report | none | none (gate) |
| `01-statics-strid-local.md` | Guarded statics: local static string ids | `gen_statics` | 13 functions, 1,664 B |
| `02-clones-owner-object-of-class-64.md` | Clone template: owner's object is of a class | `gen_clones` | 16 functions, 1,024 B |
| `03-clones-owner-object-is-a-92.md` | Clone template: act on the owner's object if it is a class | `gen_clones` | 14 functions, 1,288 B |

Numbers were measured on main at `71f7115` (2026-10-21) with `tools/shape_census.py` and
`tools/gen_lift.py --report`. They drift as other work lands; rerun the card's census command.

Rules for every card:
- Edit only the files the card lists. Generated folders are rewritten by their generator.
- `tools/verify.py` must print `RESULT: PASS` before you commit.
- Stuck or out of time: write `docs/tasks/notes/<card-name>.md` (`AGENTS.md`, "When you are
  stuck") and stop.

Writing a new card (owner or PC): copy the section layout of `01`, measure the family with
`tools/shape_census.py`, and give one verified example address. Pick small families
(<= 150 B, >= 8 copies) that `docs/HANDOFF.md` doesn't list as left over or tried, and confirm
with `tools/trymatch.py` that the card's source matches before writing it down. The example
source in each card here was checked that way.
