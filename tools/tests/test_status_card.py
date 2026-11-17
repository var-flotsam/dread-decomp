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
| Lifted code still in the port (scaffolding) | **9 semantic lifts, 0.01 MB** |
| Windows | **Compile only.** |

## Progress

<!-- progress:begin -->
Generated.

| Scope | Functions | Bytes | `O` matching (bytes) | `O`+`m` usable | `U` left |
|---|---|---|---|---|---|
| Whole binary (`L` excluded) | 1,000 | 1.00 MB | **50.00%** (500) | 60.00% | 300 |
| Boot slice (from `nnMain`, direct calls) | 100 | 0.10 MB | **25.50%** (20) | 30.25% | 70 |
<!-- progress:end -->

Completeness, whole binary (`tools/progress.py --quality`): byte-matching and readable **12.50%** (40 functions); the rest.

## Workstreams and next steps

| Stream | Files | Branches | Next |
|---|---|---|---|
| A. Boot | `src/x/` | `claude/boot-<topic>` | Round 3 (`fn_7100000020`) |
"""


class Handoff(unittest.TestCase):
    def test_areas_goal_state_detail(self):
        a = handoff.areas(MD)
        self.assertEqual([x["goal"] for x in a], ["decomp", "port", "port", "port", "port"])
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
    def test_subsystems_most_untouched_first(self):
        rows = [{"subsystem": "gui", "size": "100", "status": "O"}, {"subsystem": "gui?", "size": "100", "status": "U"},
                {"subsystem": "audio", "size": "300", "status": "U"}, {"subsystem": "lib", "size": "999", "status": "L"}]
        self.assertEqual(de.subsystems(rows), [["audio", 1, 0.0, 0.0, 0.0], ["gui", 2, 0.0, 50.0, 0.0]])

    def test_daily_rows_skip_before_and_empty(self):
        base = {"matched_pct": 50.0, "finished_pct": None, "matched_functions": 5, "untouched_bytes": 2_500_000,
                "drafts": 3, "total_bytes": 10}
        rows = [{**base, "day": "before"}, {**base, "day": "2026-11-01", "total_bytes": 0},
                {**base, "day": "2026-11-02"}, {**base, "day": "2026-11-03", "finished_pct": 12.345}]
        self.assertEqual(de.daily_rows(rows), [["2026-11-02", 50.0, "", 5, 2.5, 3],
                                               ["2026-11-03", 50.0, 12.35, 5, 2.5, 3]])

    def test_port_rows(self):
        self.assertEqual(de.port(MD)[0], [2, "Semantic lifts, boot slice", "9 of 10 accepted"])


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
        self.assertAlmostEqual(c["whole"]["u_pct"], 20.0)
        self.assertEqual(c["finished"], {"pct": 12.5, "functions": 40})
        self.assertEqual(c["boot"]["o_pct"], 25.5)
        self.assertEqual(c["port"], [
            {"label": "Linux build", "state": "Boots past fn_7100000010"},
            {"label": "Lifted code", "state": "9 functions (0.01 MB) still to replace with matching code"},
            {"label": "Windows build", "state": "Compile only"}])
        md = "\n".join(sc.markdown(c, image=False))
        self.assertIn("| Whole binary | **60.00%** | **12.50%** | 0.00 MB (1 functions) |", md)
        self.assertIn("| Linux build | Boots past `fn_7100000010` |", md)
        self.assertIn("12.5%", sc.svg(c))

    def test_readme_block_replaced(self):
        out = sc.write_readme(f"a\n{sc.BEGIN}\nold\n{sc.END}\nb\n", f"{sc.BEGIN}\nnew\n{sc.END}")
        self.assertEqual(out, f"a\n{sc.BEGIN}\nnew\n{sc.END}\nb\n")


if __name__ == "__main__":
    unittest.main()
