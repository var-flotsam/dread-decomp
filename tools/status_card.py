#!/usr/bin/env python3
"""The README's status card: a slim version of the project dashboard, from tracked files only.

The card leads with the project's two headline figures for the whole binary: matched (`O` code bytes,
counted like tools/progress.py) and finished (byte-matching and readable, from the quality line under
docs/HANDOFF.md's table, written by tools/progress.py --handoff), with untouched (`U`) bytes. Below them,
the boot path's matched share (HANDOFF's generated table, since the boot slice needs the Ghidra export)
and three port rows from HANDOFF's "Where things stand" (tools/dread/handoff.py) under short labels:
the Linux build, the lifted code still to replace, Windows. It needs no game data, so GitHub's runners
can produce it.

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
QUALITY = re.compile(r"byte-matching and readable \*\*([0-9.]+)%\*\* \(([0-9,]+) functions\)")
LIFTED = re.compile(r"^([\d,]+) semantic lifts, ([\d.]+ [KMG]B)$")
PORT_ROWS = [("Linux build", re.compile(r"linux executable", re.I)),
             ("Lifted code", re.compile(r"^lifted code still in the port", re.I)),
             ("Windows build", re.compile(r"^windows\b", re.I))]


def whole(rows: list[dict]) -> dict:
    t = progress.tally([r for r in rows if r["status"] != "L"])
    u = t["by_status"]["U"]
    return {"functions": t["functions"], "o_pct": t["matching_bytes_pct"], "om_pct": t["usable_bytes_pct"],
            "u_functions": u["functions"], "u_bytes": u["bytes"],
            "u_pct": 100 * u["bytes"] / t["bytes"] if t["bytes"] else 0.0, "by_status": t["by_status"]}


def rows_at(ref: str) -> list[dict]:
    """data/functions/main_*.csv as committed at `ref`."""
    git = lambda *a: subprocess.run(["git", *a], cwd=ROOT, capture_output=True, check=True,
                                    encoding="utf-8").stdout
    out: list[dict] = []
    for name in git("ls-tree", "--name-only", ref, "data/functions/").split():
        if name.endswith(".csv"):
            out.extend(csv.DictReader(io.StringIO(git("show", f"{ref}:{name}"))))
    return out


def finished(md: str) -> dict | None:
    """The quality line under HANDOFF's progress table: byte-matching and readable, percent and functions."""
    m = QUALITY.search(md)
    return {"pct": float(m.group(1)), "functions": int(m.group(2).replace(",", ""))} if m else None


def port_rows(areas: list[dict]) -> list[dict]:
    """The port's headline rows under short labels: where the Linux build gets to, the lifted code still
    standing in (it must shrink to zero), and Windows. The other port areas stay in HANDOFF."""
    out = []
    for label, pick in PORT_ROWS:
        a = next((a for a in areas if pick.search(a["area"])), None)
        if not a:
            continue
        state = a["state"]
        m = LIFTED.match(state)
        if m:
            state = f"{m.group(1)} functions ({m.group(2)}) still to replace with matching code"
        out.append({"label": label, "state": state if len(state) <= 72 else state[:71] + "…"})
    return out


def card() -> dict:
    md = handoff.text()
    return {"whole": whole(functions.rows()), "finished": finished(md), "boot": handoff.progress(md).get("boot"),
            "port": port_rows(handoff.areas(md))}


def code(s: str) -> str:
    """Function names and paths in a state as code spans, the way HANDOFF writes them."""
    return re.sub(r"\b(fn_[0-9a-f]+|tools/\S+\.py)\b", r"`\1`", s)


def mb(n: int) -> str:
    return f"{n / 1e6:.2f} MB"


def headline(c: dict) -> str:
    w, f = c["whole"], c["finished"]
    return (f"{w['o_pct']:.2f}% matched" + (f", {f['pct']:.2f}% finished" if f else "")
            + f", {mb(w['u_bytes'])} untouched")


def markdown(c: dict, image: bool) -> list[str]:
    w, f, b = c["whole"], c["finished"], c["boot"]
    if image:
        alt = (f"Decompilation: {headline(c)}" + (f"; boot path {b['o_pct']:.2f}% matched" if b else "")
               + "".join(f"; {x['label']}: {x['state']}" for x in c["port"]))
        return [f'<p align="center"><img src="docs/status.svg" alt="{escape(alt)}" width="760"></p>', "",
                "**Matched**: code bytes that compile to the original's (the game's own code; libraries excluded). "
                "**Finished**: matched and readable, hand-typed code, the project's real metric; generated code "
                "verifies but does not count. **Untouched**: not decompiled at all. The boot path is everything "
                "`nnMain` reaches by direct calls. Lifted code is machine translation the port runs until matching "
                "code replaces it."]
    fin = f"**{f['pct']:.2f}%**" if f else "n/a"
    out = ["**Decompilation** (code bytes of the game's own code; libraries excluded)", "",
           "| Scope | Matched | Finished | Untouched |", "|---|---|---|---|",
           f"| Whole binary | **{w['o_pct']:.2f}%** | {fin} | {mb(w['u_bytes'])} ({w['u_functions']:,} functions) |"]
    if b:
        out.append(f"| Boot path (from `nnMain`) | **{b['o_pct']:.2f}%** | | {b['u_functions']:,} functions |")
    out += ["", "**PC port**", "", "| | State |", "|---|---|"]
    out += [f"| {x['label']} | {code(x['state'])} |" for x in c["port"]]
    return out


def readme_block(c: dict) -> str:
    return "\n".join([BEGIN, "<!-- Generated by tools/status_card.py --write; rerun it rather than edit. -->",
                      *markdown(c, image=True), END])


def svg(c: dict) -> str:
    """Three headline figures, the whole binary as one bar (finished inside matched, untouched at the
    right end), the boot path's matched share, and the port's rows as text; light and dark."""
    w, f, b = c["whole"], c["finished"], c["boot"]
    W, x0, bw = 760, 150, 420
    font = "system-ui,-apple-system,'Segoe UI',sans-serif"
    body = []
    stats = [("matched", f"{w['o_pct']:.1f}%", "o")]
    if f:
        stats.append(("finished (matched and readable)", f"{f['pct']:.1f}%", "f"))
    stats.append(("untouched", mb(w["u_bytes"]), "u"))
    for i, (label, value, cls) in enumerate(stats):
        x = i * 250
        body.append(f'<rect class="k{cls}" x="{x}" y="9" width="10" height="10" rx="2"/>')
        body.append(f'<text class="n" x="{x + 16}" y="18">{escape(label)}</text>')
        body.append(f'<text class="big" x="{x}" y="52">{escape(value)}</text>')
    y = 66
    body.append(f'<rect class="t" x="0" y="{y}" width="{W}" height="16" rx="4"/>')
    body.append(f'<rect class="o" x="0" y="{y}" width="{W * w["o_pct"] / 100:.1f}" height="16" rx="4"/>')
    if f:
        body.append(f'<rect class="f" x="0" y="{y}" width="{W * f["pct"] / 100:.1f}" height="16" rx="4"/>')
    u_w = W * w["u_pct"] / 100
    body.append(f'<rect class="u" x="{W - u_w:.1f}" y="{y}" width="{u_w:.1f}" height="16" rx="4"/>')
    body.append(f'<text class="n" x="0" y="{y + 34}">Whole binary: {w["functions"]:,} functions of game code, '
                f'{w["u_functions"]:,} untouched</text>')
    y += 46
    if b:
        cy = y + 13
        body.append(f'<text class="l" x="0" y="{cy}" dy="0.35em">Boot path</text>')
        body.append(f'<rect class="t" x="{x0}" y="{cy - 5}" width="{bw}" height="10" rx="5"/>')
        body.append(f'<rect class="o" x="{x0}" y="{cy - 5}" width="{bw * b["o_pct"] / 100:.1f}" height="10" rx="5"/>')
        body.append(f'<text class="n" x="{x0 + bw + 14}" y="{cy}" dy="0.35em">{b["o_pct"]:.1f}% matched</text>')
        y += 26
    if c["port"]:
        body.append(f'<text class="h" x="0" y="{y + 24}">PC port</text>')
        y += 32
    for x in c["port"]:
        cy = y + 13
        body.append(f'<text class="l" x="0" y="{cy}" dy="0.35em">{escape(x["label"])}</text>')
        body.append(f'<text class="s" x="{x0}" y="{cy}" dy="0.35em">{escape(x["state"])}</text>')
        y += 26
    H = y + 8
    style = (f".big{{font:600 30px {font};fill:#1f2328}}.h{{font:600 13px {font};fill:#1f2328}}"
             f".l,.n,.s{{font:13px {font};fill:#1f2328}}.n{{fill:#59636e}}"
             ".t{fill:#e6e8eb}.u,.ku{fill:#c4c9cf}.o,.ko{fill:#9fd8b0}.f,.kf{fill:#2f9e5a}"
             "@media (prefers-color-scheme:dark){.big,.h,.l,.s{fill:#e6edf3}.n{fill:#9198a1}.t{fill:#2b3138}"
             ".u,.ku{fill:#4a525b}.o,.ko{fill:#2c6e45}.f,.kf{fill:#4cc27a}}")
    return "\n".join([f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{H}" viewBox="0 0 {W} {H}" '
                      f'role="img"><style>{style}</style>', *body, "</svg>", ""])


def summary(c: dict, base: str | None) -> list[str]:
    bar = lambda p: "█" * round(p / 5) + "░" * (20 - round(p / 5))
    w = c["whole"]
    out = ["## Status card", "", f"`{bar(w['o_pct'])}` {headline(c)} (whole binary)", ""]
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
