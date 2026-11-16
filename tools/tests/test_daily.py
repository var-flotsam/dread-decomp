"""tools/daily.py: the figures it reads from tracked files (no game data)."""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import daily  # noqa: E402


class Figures(unittest.TestCase):
    def test_finished_is_read_from_the_quality_line(self):
        text = "Completeness ...: byte-matching and readable **17.55%** (11,264 functions); byte-matching but"
        self.assertEqual(daily.finished_from_handoff(text), (17.55, 11264))
        self.assertEqual(daily.finished_from_handoff("no line"), (None, None))

    def test_library_functions_are_left_out(self):
        rows = [{"status": "O", "size": "30"}, {"status": "U", "size": "10"}, {"status": "L", "size": "60"}]
        t = daily.status_totals(rows)
        self.assertEqual((t["matched_bytes"], t["untouched_bytes"], t["total_bytes"]), (30, 10, 40))
        self.assertAlmostEqual(t["matched_pct"], 75.0)

    def test_drafts_count_leaves_out_the_header(self):
        self.assertEqual(daily.drafts_count("address,size\n0x1,4\n0x2,8\n"), 2)
        self.assertEqual(daily.drafts_count(""), 0)


if __name__ == "__main__":
    unittest.main()
