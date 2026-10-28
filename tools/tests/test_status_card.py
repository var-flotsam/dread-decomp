"""docs/HANDOFF.md parsing (tools/dread/handoff.py), the dashboard export's lift and boot rows, and the
README's status card, on made-up input (no dump, no build/)."""

import unittest

from tools.tests import _path  # noqa: F401
import dashboard_export as de
import status_card as sc
from dread import handoff

MD = """# Handoff

## Where things stand

| Area | State |
|---|---|
| Phases 1-3 | **Done.** `docs/recon.md` |
| Semantic lifts, boot slice (`build/port/lifted`) | **9 of 10 accepted** (1 left) |
| Port, Linux executable | **Boots past `fn_7100000010`.** traps after it |
| Windows | **Compile only.** |

## Progress

<!-- progress:begin -->
Generated.

| Scope | Functions | Bytes | `O` matching (bytes) | `O`+`m` usable | `U` left |
|---|---|---|---|---|---|
| Whole binary (`L` excluded) | 1,000 | 1.00 MB | **50.00%** (500) | 60.00% | 300 |
| Boot slice (from `nnMain`, direct calls) | 100 | 0.10 MB | **25.50%** (20) | 30.25% | 70 |
<!-- progress:end -->

## Workstreams and next steps

| Stream | Files | Branches | Next |
|---|---|---|---|
| A. Boot | `src/x/` | `claude/boot-<topic>` | Round 3 (`fn_7100000020`) |
"""


class Handoff(unittest.TestCase):
    def test_areas_goal_state_detail(self):
        a = handoff.areas(MD)
        self.assertEqual([x["goal"] for x in a], ["decomp", "port", "port", "port"])
        self.assertEqual(a[1]["state"], "9 of 10 accepted")
        self.assertEqual(a[2]["state"], "Boots past fn_7100000010")
        self.assertEqual(a[0]["detail"], "docs/recon.md")

    def test_workstreams_keep_next(self):
        w = handoff.workstreams(MD)[0]
        self.assertEqual(w["next"], "Round 3 (fn_7100000020)")
        self.assertEqual(w["branches"], "claude/boot-<topic>")

    def test_progress_rows(self):
        p = handoff.progress(MD)
        self.assertEqual(p["boot"], {"functions": 100, "o_pct": 25.5, "om_pct": 30.25, "u_functions": 70})
        self.assertEqual(p["whole"]["u_functions"], 300)


class Export(unittest.TestCase):
    def test_lifts_by_kind(self):
        man = {"functions": {"a": {"outcome": "accepted", "size": 8}, "b": {"outcome": "accepted", "size": 4},
                             "c": {"outcome": "inconclusive", "size": 100},
                             "d": {"outcome": "rejected by the oracle", "size": 2},
                             "e": {"outcome": "rejected: size cap", "size": 9000}}}
        rows = de.lifts({"boot": man})
        self.assertEqual(rows[0], ["boot", "rejected: size cap", "rejected", 1, 9000])
        self.assertIn(["boot", "accepted", "accepted", 2, 12], rows)
        self.assertEqual({r[2] for r in rows}, {"accepted", "inconclusive", "rejected"})

    def test_boot_run(self):
        res = {"furthest_label": "nnMain entered",
               "milestones": [{"key": "a", "label": "A", "reached": True, "seconds": 1.234},
                              {"key": "b", "label": "B", "reached": False, "seconds": None}],
               "traps": {"distinct": 3, "functions": 2, "imports": 1, "mismatched": 0},
               "lifted": {"linked": 10, "ran": 4}, "end": {"signal": "SIGSEGV"}, "fault": None}
        ms, run = de.boot(res)
        self.assertEqual(ms, [[1, "a", "A", "yes", 1.2], [2, "b", "B", "no", ""]])
        self.assertEqual(run, ["nnMain entered", 1, 2, 3, 2, 1, 0, 4, 10, "signal SIGSEGV"])


class Card(unittest.TestCase):
    def setUp(self):
        self._text = handoff.text
        handoff.text = lambda: MD
        rows = [{"address": "0", "size": "60", "status": "O"}, {"address": "1", "size": "20", "status": "m"},
                {"address": "2", "size": "20", "status": "U"}, {"address": "3", "size": "500", "status": "L"}]
        self._rows = sc.functions.rows
        sc.functions.rows = lambda: rows

    def tearDown(self):
        handoff.text = self._text
        sc.functions.rows = self._rows

    def test_card(self):
        c = sc.card()
        self.assertAlmostEqual(c["whole"]["o_pct"], 60.0)  # L left out
        self.assertEqual(c["boot"]["o_pct"], 25.5)
        self.assertEqual(c["bars"][0], {"label": "Lifts, boot slice", "n": 9, "total": 10})
        self.assertEqual(c["bars"][1], {"label": "Port, Linux executable", "state": "Boots past fn_7100000010"})
        md = "\n".join(sc.markdown(c, image=False))
        self.assertIn("| Whole binary | **60.00%** | 80.00% | 1 of 3 |", md)
        self.assertIn("| Port, Linux executable | Boots past `fn_7100000010` |", md)
        self.assertIn("Lifts, boot slice", sc.svg(c))

    def test_readme_block_replaced(self):
        out = sc.write_readme(f"a\n{sc.BEGIN}\nold\n{sc.END}\nb\n", f"{sc.BEGIN}\nnew\n{sc.END}")
        self.assertEqual(out, f"a\n{sc.BEGIN}\nnew\n{sc.END}\nb\n")


if __name__ == "__main__":
    unittest.main()
