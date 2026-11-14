"""tools/tu_status.py: units counted from small invented function lists, and the tracked table's shape
(no game data)."""

import csv
import json
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import progress
import tu_status

A = 0x7100000000


def unit(index: int, start: int, end: int, hint: str = "", conf: str = "high") -> dict:
    return {"index": index, "start": f"0x{A + start:x}", "end": f"0x{A + end:x}", "start_i": A + start,
            "end_i": A + end, "class_hint": hint, "hint_source": "reflection" if hint else "", "confidence": conf}


def fn(offset: int, status: str, size: int = 0x10, subsystem: str = "game") -> dict:
    return {"address": f"0x{A + offset:x}", "size": str(size), "name": f"fn_{A + offset:x}",
            "name_source": "unknown", "subsystem": subsystem, "status": status}


UNITS = [unit(0, 0x000, 0x100, "CFoo;CFoo::CParams"), unit(1, 0x100, 0x200), unit(2, 0x200, 0x300),
         unit(3, 0x300, 0x400), unit(4, 0x400, 0x500), unit(5, 0x500, 0x600)]
ROWS = [fn(0x000, "O"), fn(0x010, "O", 0x20),                               # 0: matched
        fn(0x100, "O"), fn(0x110, "m"), fn(0x120, "L"),                     # 1: done (its m is verified)
        fn(0x200, "O", 0x40, "engine?"), fn(0x210, "m"), fn(0x220, "U"), fn(0x230, "M"),   # 2: partial
        fn(0x300, "U"), fn(0x310, "U"),                                     # 3: untouched
        fn(0x400, "L"),                                                     # 4: library
        fn(0x700, "L")]                                                     # in no unit; unit 5 is empty
VERIFIED = frozenset({A + 0x110})


class Build(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.table = tu_status.build(UNITS, ROWS, VERIFIED)

    def test_states(self):
        self.assertEqual([r["state"] for r in self.table],
                         ["matched", "done", "partial", "untouched", "library", "empty"])

    def test_library_functions_are_not_counted(self):
        r = self.table[1]
        self.assertEqual((r["functions"], r["bytes"], r["L"]), (2, 0x20, 1))

    def test_an_unverified_minor_function_keeps_the_unit_partial(self):
        self.assertEqual(tu_status.build(UNITS, ROWS, frozenset())[1]["state"], "partial")

    def test_counts_and_bytes(self):
        r = self.table[2]
        self.assertEqual([r[s] for s in tu_status.STATUSES], [1, 1, 1, 0, 1, 0])
        self.assertEqual((r["functions"], r["bytes"], r["matched_bytes"], r["done_bytes"], r["verified"]),
                         (4, 0x70, 0x40, 0x40, 0))
        self.assertEqual(self.table[1]["done_bytes"], 0x20)

    def test_subsystem_is_the_largest_by_bytes_without_the_question_mark(self):
        self.assertEqual(self.table[2]["subsystem"], "engine")

    def test_names(self):
        self.assertEqual([r["name"] for r in self.table[:2]], ["CFoo", "UnkTU1"])

    def test_members_outside_every_unit(self):
        inside, outside = tu_status.members(UNITS, ROWS)
        self.assertEqual([r["address"] for r in outside], [f"0x{A + 0x700:x}"])
        self.assertEqual(sum(map(len, inside.values())), len(ROWS) - 1)

    def test_by_state_adds_up(self):
        t = tu_status.by_state(self.table)
        self.assertEqual({s: v["units"] for s, v in t.items()},
                         {"matched": 1, "done": 1, "partial": 1, "untouched": 1, "library": 1, "empty": 1})
        self.assertEqual(sum(t[s]["functions"] for s in tu_status.GAME_STATES),
                         sum(r["status"] != "L" for r in ROWS))

    def test_render_round_trips(self):
        rows = list(csv.DictReader(tu_status.render(self.table).splitlines()))
        self.assertEqual(list(rows[0]), tu_status.COLUMNS)
        self.assertEqual([(r["index"], r["state"], r["O"]) for r in rows[:2]], [("0", "matched", "2"), ("1", "done", "1")])


class Quality(unittest.TestCase):
    def test_per_unit_quality_counts(self):
        quality = {A + 0x200: ("complete", False), A + 0x210: ("unproven", False), A + 0x220: ("missing", False),
                   A + 0x230: ("unproven", True), A + 0x000: ("match_generated", False), A + 0x010: ("complete", False)}
        table = tu_status.build(UNITS, ROWS, VERIFIED, quality)
        self.assertEqual((table[2]["complete"], table[2]["unproven"], table[2]["missing"]), (1, 2, 1))
        self.assertEqual((table[0]["complete"], table[0]["match_generated"]), (1, 1))

    def test_quality_by_address_reads_the_check_cache(self):
        rows = [fn(0x000, "O"), fn(0x010, "O"), fn(0x020, "U"), fn(0x030, "O")]
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            for rel, entries in (("game/a.o.diff.json", [{"address": A, "name": "fn_7100000000"}]),
                                 ("lifted/l.o.diff.json", [{"address": A + 0x10, "name": "fn_7100000010"}])):
                p = root / "build" / "check" / "9.9.9" / rel
                p.parent.mkdir(parents=True)
                p.write_text(json.dumps({"entries": entries}), encoding="utf-8")
            with mock.patch.object(progress, "ROOT", root):
                q = progress.quality_by_address("9.9.9", rows, frozenset())
        # The last O is in no object of the cache: the cache is older than the function list.
        self.assertEqual([q[A + o][0] for o in (0, 0x10, 0x20, 0x30)],
                         ["complete", "match_generated", "missing", "unproven"])


class Resolve(unittest.TestCase):
    def test_by_index_name_and_other_class(self):
        self.assertEqual(tu_status.resolve_unit(UNITS, "3")["index"], 3)
        self.assertEqual(tu_status.resolve_unit(UNITS, "CFoo")["index"], 0)
        self.assertEqual(tu_status.resolve_unit(UNITS, "CFoo::CParams")["index"], 0)
        self.assertEqual(tu_status.resolve_unit(UNITS, "UnkTU2")["index"], 2)


class Summary(unittest.TestCase):
    def test_moved_units_only(self):
        base = tu_status.build(UNITS, ROWS, frozenset())
        now = tu_status.build(UNITS, [dict(r, status="O") if r["status"] == "U" and r["address"].endswith("300") else r
                                      for r in ROWS], VERIFIED)
        text = "\n".join(tu_status.summary(now, base, "origin/main"))
        self.assertIn("moves 2 units against `origin/main`; 1 newly matched or done.", text)
        self.assertIn("| 1 | `UnkTU1` | partial → **done** | 1 → 1 of 2 | 1 `m` |", text)
        self.assertIn("| 3 | `UnkTU3` | untouched → **partial** | 0 → 1 of 2 | 1 `U` |", text)
        self.assertNotIn("| 2 | `", text)

    def test_no_change(self):
        t = tu_status.build(UNITS, ROWS, VERIFIED)
        self.assertIn("No unit changes against `x`.", tu_status.summary(t, t, "x"))


class TrackedTable(unittest.TestCase):
    """data/tu_status.csv is refreshed by the session that merges a round, so it may be behind the function list
    on a branch: these check its shape against the unit map, never its counts."""

    @classmethod
    def setUpClass(cls):
        with tu_status.OUT.open(newline="", encoding="utf-8") as fh:
            cls.rows = list(csv.DictReader(fh))

    def test_columns(self):
        self.assertEqual(list(self.rows[0]), tu_status.COLUMNS)

    def test_one_row_per_unit_of_the_map(self):
        import tu_map
        units = tu_map.load_map()
        self.assertEqual([(r["index"], r["start"], r["end"]) for r in self.rows],
                         [(str(u["index"]), u["start"], u["end"]) for u in units])

    def test_rows_are_consistent(self):
        for r in self.rows:
            n = {k: int(r[k]) for k in tu_status.COLUMNS[7:]}
            self.assertEqual(n["functions"], n["O"] + n["m"] + n["M"] + n["W"] + n["U"], r["index"])
            self.assertEqual(r["state"], tu_status.state(n), r["index"])
            self.assertLessEqual(n["matched_bytes"], n["done_bytes"])
            self.assertLessEqual(n["done_bytes"], n["bytes"])


if __name__ == "__main__":
    unittest.main()
