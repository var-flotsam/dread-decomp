#!/usr/bin/env python3
"""The README's status card: a slim version of the project dashboard, from tracked files only.

The card follows the project's two goals. Decompilation: the share of code bytes matching (`O`) and
usable (`O`+`m`) and the functions left, for the whole binary (data/functions/, counted like
tools/progress.py) and the boot path (the boot row of docs/HANDOFF.md's generated progress table,
since the boot slice needs the Ghidra export). PC port: the port rows of HANDOFF's "Where things
stand" (tools/dread/handoff.py); in the image, a bar for each that reads "N of M" (the semantic lifts).
It needs no game data, so GitHub's runners can produce it.

  (no option)   print the card's markdown
  --write       rewrite README.md between the status:begin and status:end markers, and docs/status.svg
  --check       exit 1 when README.md's card or docs/status.svg differ from what --write would write
  --summary F   append the card to F as markdown with text bars (for $GITHUB_STEP_SUMMARY), with what
                the change does to the whole binary when --base is given

The pull request check (.github/workflows/policy.yml) writes the card to the job summary with
--base, so each pull request shows its effect. README.md is refreshed with --write by the session
that merges a round, like docs/HANDOFF.md's table: a card committed by every branch would conflict.

Usage: python tools/status_card.py [--write | --check] [--summary FILE] [--base REF]
"""

from __future__ import annotations

import argparse
import csv
import io
import re
import subprocess
import sys
from html import escape
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import progress  # noqa: E402
from dread import functions, handoff  # noqa: E402
from dread.paths import ROOT  # noqa: E402

README = ROOT / "README.md"
SVG = ROOT / "docs" / "status.svg"
BEGIN, END = "<!-- status:begin -->", "<!-- status:end -->"
OF = re.compile(r"^([\d,]+) of ([\d,]+)\b")


def whole(rows: list[dict]) -> dict:
    t = progress.tally([r for r in rows if r["status"] != "L"])
    return {"functions": t["functions"], "o_pct": t["matching_bytes_pct"], "om_pct": t["usable_bytes_pct"],
            "u_functions": t["by_status"]["U"]["functions"], "by_status": t["by_status"]}


def rows_at(ref: str) -> list[dict]:
    """data/functions/main_*.csv as committed at `ref`."""
    git = lambda *a: subprocess.run(["git", *a], cwd=ROOT, capture_output=True, check=True,
                                    encoding="utf-8").stdout
    out: list[dict] = []
    for name in git("ls-tree", "--name-only", ref, "data/functions/").split():
        if name.endswith(".csv"):
            out.extend(csv.DictReader(io.StringIO(git("show", f"{ref}:{name}"))))
    return out


def area(a: dict) -> str:
    """The area's name without a parenthesised path: "Semantic lifts, boot slice (build/port/lifted)"."""
    return re.sub(r" \([^)]*/[^)]*\)", "", a["area"])


def card() -> dict:
    md = handoff.text()
    prog = handoff.progress(md)
    port = [a for a in handoff.areas(md) if a["goal"] == "port"]
    bars = []
    for a in port:
        m = OF.match(a["state"])
        if m:
            n, total = (int(g.replace(",", "")) for g in m.groups())
            bars.append({"label": area(a).replace("Semantic lifts", "Lifts"), "n": n, "total": total})
        else:
            bars.append({"label": area(a), "state": a["state"]})
    return {"whole": whole(functions.rows()), "boot": prog.get("boot"), "port": port, "bars": bars}


def code(s: str) -> str:
    """Function names and paths in a state as code spans, the way HANDOFF writes them."""
    return re.sub(r"\b(fn_[0-9a-f]+|tools/\S+\.py)\b", r"`\1`", s)


def markdown(c: dict, image: bool) -> list[str]:
    w, b = c["whole"], c["boot"]
    out = []
    if image:
        alt = (f"Matching: {w['o_pct']:.2f}% of the whole binary"
               + (f", {b['o_pct']:.2f}% of the boot path" if b else "")
               + "".join(f"; {x['label']}: " + (x["state"] if "state" in x else f"{x['n']:,} of {x['total']:,}")
                         for x in c["bars"]))
        return [f'<p align="center"><img src="docs/status.svg" alt="{escape(alt)}" width="760"></p>', "",
                "Matching is by code bytes of the game's own code (libraries excluded); usable adds the "
                "minor non-matching functions, which behave the same. The boot path is everything `nnMain` "
                "reaches by direct calls. Lifts are machine translations the port runs until matching code "
                "replaces them."]
    out += ["**Decompilation**: C++ that compiles to the original bytes (game code; libraries excluded).", "",
            "| Scope | Matching | Usable (`O`+`m`) | Functions left |", "|---|---|---|---|",
            f"| Whole binary | **{w['o_pct']:.2f}%** | {w['om_pct']:.2f}% | {w['u_functions']:,} of {w['functions']:,} |"]
    if b:
        out.append(f"| Boot path (from `nnMain`) | **{b['o_pct']:.2f}%** | {b['om_pct']:.2f}% "
                   f"| {b['u_functions']:,} of {b['functions']:,} |")
    out += ["", "**PC port**: one program on Linux first, lifted code standing in until matching replaces it.", "",
            "| Area | State |", "|---|---|"]
    out += [f"| {area(a)} | {code(a['state'])} |" for a in c["port"]]
    return out


def readme_block(c: dict) -> str:
    return "\n".join([BEGIN, "<!-- Generated by tools/status_card.py --write; rerun it rather than edit. -->",
                      *markdown(c, image=True), END])


def svg(c: dict) -> str:
    """Bars for matching (whole binary, boot path) and each "N of M" port row, the other port rows'
    states as text; light and dark."""
    w, b = c["whole"], c["boot"]
    rows = [("Decompilation", "Whole binary", w["o_pct"], w["om_pct"],
             f"{w['o_pct']:.1f}% matching · {w['om_pct']:.1f}% usable")]
    if b:
        rows.append(("", "Boot path", b["o_pct"], b["om_pct"], f"{b['o_pct']:.1f}% matching · {b['om_pct']:.1f}% usable"))
    for i, x in enumerate(c["bars"]):
        section = "PC port" if i == 0 else ""
        if "state" in x:
            rows.append((section, x["label"], None, None, x["state"]))
            continue
        pct = 100 * x["n"] / x["total"]
        rows.append((section, x["label"], pct, pct, f"{x['n']:,} of {x['total']:,} accepted"))
    W, x0, bw, rh = 760, 180, 340, 30
    y, body = 8, []
    for section, label, o, om, note in rows:
        if section:
            y += 22 if y > 8 else 14
            body.append(f'<text class="h" x="0" y="{y}">{escape(section)}</text>')
            y += 12
        cy = y + rh / 2
        body.append(f'<text class="l" x="0" y="{cy:.0f}" dy="0.35em">{escape(label)}</text>')
        if o is None:  # a state with no count: the text where the bar would be
            body.append(f'<text class="s" x="{x0}" y="{cy:.0f}" dy="0.35em">{escape(note)}</text>')
            y += rh
            continue
        body.append(f'<rect class="t" x="{x0}" y="{cy - 6:.0f}" width="{bw}" height="12" rx="6"/>')
        if om > o:
            body.append(f'<rect class="m" x="{x0}" y="{cy - 6:.0f}" width="{bw * om / 100:.1f}" height="12" rx="6"/>')
        body.append(f'<rect class="o" x="{x0}" y="{cy - 6:.0f}" width="{bw * o / 100:.1f}" height="12" rx="6"/>')
        body.append(f'<text class="n" x="{x0 + bw + 14}" y="{cy:.0f}" dy="0.35em">{escape(note)}</text>')
        y += rh
    H = y + 8
    style = (".h{font:600 13px system-ui,-apple-system,'Segoe UI',sans-serif;fill:#1f2328}"
             ".l,.n,.s{font:13px system-ui,-apple-system,'Segoe UI',sans-serif;fill:#1f2328}.n{fill:#59636e}.s{fill:#1f2328;font-weight:500}"
             ".t{fill:#e6e8eb}.m{fill:#9fd8b0}.o{fill:#2f9e5a}"
             "@media (prefers-color-scheme:dark){.h,.l,.s{fill:#e6edf3}.n{fill:#9198a1}.t{fill:#2b3138}"
             ".m{fill:#2c6e45}.o{fill:#4cc27a}}")
    return "\n".join([f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
                      f'role="img"><style>{style}</style>', *body, "</svg>", ""])


def summary(c: dict, base: str | None) -> list[str]:
    bar = lambda p: "█" * round(p / 5) + "░" * (20 - round(p / 5))
    w = c["whole"]
    out = ["## Status card", "", f"`{bar(w['o_pct'])}` **{w['o_pct']:.2f}%** of the whole binary matches", ""]
    if base:
        then = whole(rows_at(base))
        d = w["o_pct"] - then["o_pct"]
        moved = {s: w["by_status"][s]["functions"] - then["by_status"][s]["functions"] for s in progress.STATUSES}
        changed = ", ".join(f"`{s}` {n:+,}" for s, n in moved.items() if n)
        out += [f"This change against `{base}`: matching {d:+.2f} points"
                + (f"; functions by status {changed}" if changed else "; no status changes"), ""]
    out += markdown(c, image=False)
    out += ["", "Boot path and port rows come from `docs/HANDOFF.md` as of this commit; "
            "the README's card is refreshed by the session that merges a round."]
    return out


def write_readme(text: str, block: str) -> str:
    if BEGIN not in text or END not in text:
        raise SystemExit(f"README.md: no {BEGIN} ... {END} block")
    head, rest = text.split(BEGIN, 1)
    return head + block + rest.split(END, 1)[1]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    mode = ap.add_mutually_exclusive_group()
    mode.add_argument("--write", action="store_true", help="rewrite README.md's card and docs/status.svg")
    mode.add_argument("--check", action="store_true", help="exit 1 when they are stale")
    ap.add_argument("--summary", type=Path, help="append the card as markdown to this file")
    ap.add_argument("--base", help="with --summary: the ref to compare the whole binary against")
    args = ap.parse_args()

    c = card()
    readme = write_readme(README.read_text(encoding="utf-8"), readme_block(c))
    image = svg(c)
    if args.write:
        README.write_text(readme, encoding="utf-8", newline="\n")
        SVG.write_text(image, encoding="utf-8", newline="\n")
        print("README.md and docs/status.svg rewritten")
    elif args.check:
        stale = [p.relative_to(ROOT).as_posix() for p, want in ((README, readme), (SVG, image))
                 if not p.exists() or p.read_text(encoding="utf-8") != want]
        if stale:
            print(f"stale: {', '.join(stale)}; run python tools/status_card.py --write")
            return 1
        print("status card current")
    if args.summary:
        with args.summary.open("a", encoding="utf-8") as fh:
            fh.write("\n".join(summary(c, args.base)) + "\n")
    if not (args.write or args.check or args.summary):
        print("\n".join(markdown(c, image=False)))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
