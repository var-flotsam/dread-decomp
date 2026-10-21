# Task cards

Short, self-contained jobs for the Mac workstream (a small local model; see `AGENTS.md` and
`docs/HANDOFF.md`, "Workstreams"). Each card fits in about 2k tokens and lists the goal, the files
you may edit, exact commands, an example address, what done looks like, and a time-box.

Do them in order. Always do `00` first on a new machine or after a toolchain change.

| Card | Title | Generator | Size of the prize |
|---|---|---|---|
| `00-triage.md` | Triage: check the setup and report | none | none (gate) |
| `01-statics-strid-local.md` | Guarded statics: local static string ids | `gen_statics` | 13 functions, 1,664 B |
| `02-statics-family-456.md` | Guarded statics: the 456-byte family | `gen_statics` | 37 functions, 16,872 B |
| `03-clones-vector-copy-92.md` | Clone template: the 92-byte memcpy family | `gen_clones` | 25 functions, 2,300 B |

Numbers were measured on main at `71f7115` (2026-10-21) with `tools/shape_census.py` and
`tools/gen_lift.py --report`. They drift as other work lands; rerun the card's census command.

Rules for every card:
- Edit only the files the card lists. Generated folders are rewritten by their generator.
- `tools/verify.py` must print `RESULT: PASS` before you commit.
- Stuck or out of time: write `docs/tasks/notes/<card-name>.md` (`AGENTS.md`, "When you are
  stuck") and stop.

Writing a new card (owner or PC): copy the section layout of `01`, measure the family with
`tools/shape_census.py`, and give one verified example address.
