"""The tables of docs/HANDOFF.md, read for the dashboard (tools/dashboard_export.py) and the README's
status card (tools/status_card.py). Cells come back without markdown emphasis or code spans."""

from __future__ import annotations

import re

from dread.paths import ROOT

HANDOFF = ROOT / "docs" / "HANDOFF.md"
# Areas of "Where things stand" that belong to the PC port: its milestones, platforms, and the
# semantic lifts that bridge it until matching replaces them (docs/PORT_PLAN.md, Route B).
PORT_AREAS = re.compile(r"\b(port|semantic lifts|windows|linux)\b", re.I)


def text() -> str:
    return HANDOFF.read_text(encoding="utf-8")


def plain(s: str) -> str:
    return re.sub(r"\*\*|`", "", s).strip()


def table(md: str, heading: str) -> list[list[str]]:
    """Body rows of the first markdown table after `heading`, cells as written (markdown kept)."""
    if heading not in md:
        raise SystemExit(f"docs/HANDOFF.md: no heading {heading!r}")
    block: list[str] = []
    for line in md.split(heading, 1)[1].splitlines()[1:]:
        if line.startswith("|"):
            block.append(line)
        elif block:
            break
    if len(block) < 3:
        raise SystemExit(f"docs/HANDOFF.md: no table under {heading!r}")
    return [[c.strip() for c in line.strip("|").split("|")] for line in block[2:]]


def goal(area: str) -> str:
    """decomp or port."""
    return "port" if PORT_AREAS.search(area) else "decomp"


def areas(md: str) -> list[dict]:
    """"Where things stand": order, goal, area, state (the leading bold run), detail."""
    out = []
    for i, (area, state) in enumerate(table(md, "## Where things stand"), 1):
        # "**Done.** Dump imported ...", "**15,499 of 15,996 accepted** (...)"
        m = re.match(r"\*\*(.+?)\*\*\s*(.*)", state)
        st, detail = (m.group(1).rstrip("."), m.group(2)) if m else (state, "")
        out.append({"order": i, "goal": goal(plain(area)), "area": plain(area), "state": plain(st),
                    "detail": plain(detail)})
    return out


def workstreams(md: str) -> list[dict]:
    """"Workstreams and next steps": stream, files, branches, next."""
    return [dict(zip(("stream", "files", "branches", "next"), (plain(c) for c in row[:4])))
            for row in table(md, "## Workstreams")]


def progress(md: str) -> dict[str, dict]:
    """The generated progress table (tools/progress.py --handoff), by scope: whole and boot, with
    functions, matching and usable shares (percent) and U functions left."""
    out = {}
    for row in table(md, "<!-- progress:begin -->"):
        scope = "boot" if row[0].lower().startswith("boot") else "whole"
        num = lambda s: float(re.search(r"[\d,.]+", plain(s)).group(0).replace(",", ""))
        out[scope] = {"functions": int(num(row[1])), "o_pct": num(row[3]), "om_pct": num(row[4]),
                      "u_functions": int(num(row[5]))}
    return out
