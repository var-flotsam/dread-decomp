"""The function list, data/functions/main_XX.csv: one loader for every tool.

Columns: address,size,name,name_source,subsystem,status (CLAUDE.md, "Conventions"). Rows come back
as the csv module reads them (all values strings) in file and row order, so callers that convert
`address` or `size` keep doing it themselves and their output is unchanged.
"""

from __future__ import annotations

import csv
from pathlib import Path

from dread.paths import FUNCTIONS

COLUMNS = ["address", "size", "name", "name_source", "subsystem", "status"]


def files() -> list[Path]:
    """The per-MiB CSV files in address order."""
    return sorted(FUNCTIONS.glob("main_*.csv"))


def read(path: Path) -> list[dict]:
    with path.open(newline="", encoding="utf-8") as fh:
        return list(csv.DictReader(fh))


def rows() -> list[dict]:
    """Every row of every file, in address order."""
    out: list[dict] = []
    for f in files():
        out.extend(read(f))
    return out


def by_file() -> dict[str, list[dict]]:
    """File name -> its rows, for tools that write statuses back (tools/check.py --update)."""
    return {f.name: read(f) for f in files()}


def write(name: str, frows: list[dict]) -> None:
    """Write one file back with the columns it was read with."""
    with (FUNCTIONS / name).open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=list(frows[0].keys()), lineterminator="\n")
        w.writeheader()
        w.writerows(frows)


def by_name(rows_: list[dict] | None = None) -> dict[str, list[int]]:
    """Name -> the addresses carrying it."""
    out: dict[str, list[int]] = {}
    for r in rows() if rows_ is None else rows_:
        out.setdefault(r["name"], []).append(int(r["address"], 16))
    return out
