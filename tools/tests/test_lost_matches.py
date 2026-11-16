"""tools/lost_matches.py: matched functions a change loses against its base. Made-up rows; no game data."""
import unittest

from tools.tests import _path  # noqa: F401
import lost_matches

BASE = {0x7100000100: ("fn_7100000100", "O"), 0x7100000200: ("fn_7100000200", "O"),
        0x7100000300: ("fn_7100000300", "U"), 0x7100000400: ("fn_7100000400", "O")}


class Lost(unittest.TestCase):
    def test_nothing_lost_when_statuses_hold_or_rise(self):
        head = {**BASE, 0x7100000300: ("fn_7100000300", "O")}
        self.assertEqual(lost_matches.lost(BASE, head), [])

    def test_a_demoted_or_missing_match_is_reported(self):
        head = {**BASE, 0x7100000200: ("fn_7100000200", "U")}
        del head[0x7100000400]
        self.assertEqual(lost_matches.lost(BASE, head),
                         [(0x7100000200, "fn_7100000200", "U"), (0x7100000400, "fn_7100000400", "missing")])

    def test_a_non_matching_function_may_change_freely(self):
        head = {**BASE, 0x7100000300: ("fn_7100000300", "M")}
        self.assertEqual(lost_matches.lost(BASE, head), [])


class Judge(unittest.TestCase):
    HEAD = {**BASE, 0x7100000100: ("fn_7100000100", "m"), 0x7100000200: ("fn_7100000200", "U")}

    def test_a_row_the_change_adds_allows_that_demotion_only(self):
        ok, bad = lost_matches.judge(BASE, self.HEAD, set(), {0x7100000100})
        self.assertEqual([a for a, _, _ in ok], [0x7100000100])
        self.assertEqual([a for a, _, _ in bad], [0x7100000200])

    def test_a_row_already_in_the_base_allows_nothing(self):
        ok, bad = lost_matches.judge(BASE, self.HEAD, {0x7100000100}, {0x7100000100})
        self.assertEqual(ok, [])
        self.assertEqual(len(bad), 2)


if __name__ == "__main__":
    unittest.main()
