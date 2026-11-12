"""tools/sample_review.py: which functions are new, and that a seed gives the same sample (no game data)."""
import unittest

from tools.tests import _path  # noqa: F401
import sample_review


def row(address, status, name=None, size="100"):
    return {"address": f"0x{address:x}", "size": size, "name": name or f"fn_{address:x}", "status": status}


NOW = [row(0x7100000100, "O"), row(0x7100000200, "m"), row(0x7100000300, "U"), row(0x7100000400, "O"),
       row(0x7100000500, "M"), row(0x7100000600, "L"), row(0x7100000700, "O")]
BEFORE = {0x7100000100: "U", 0x7100000200: "U", 0x7100000300: "U", 0x7100000400: "O", 0x7100000500: "m",
          0x7100000600: "L"}


class Newly(unittest.TestCase):
    def test_only_what_was_not_decompiled_before_counts(self):
        new = [r["address"] for r in sample_review.newly_done(BEFORE, NOW)]
        # 0x400 was O already, 0x500 was m (a status change, not a new function), 0x300 is still U, 0x600 is library
        self.assertEqual(new, ["0x7100000100", "0x7100000200", "0x7100000700"])   # 0x700 was not in the list then


class Pick(unittest.TestCase):
    ROWS = [row(0x7100000000 + 0x10 * i, "O") for i in range(200)]

    def test_a_seed_gives_the_same_sample_whatever_the_order_of_the_candidates(self):
        a = sample_review.pick(self.ROWS, 20, "2026-W46")
        b = sample_review.pick(list(reversed(self.ROWS)), 20, "2026-W46")
        self.assertEqual(a, b)
        self.assertEqual(len({r["address"] for r in a}), 20)
        self.assertEqual(a, sorted(a, key=lambda r: int(r["address"], 16)))

    def test_another_week_another_sample(self):
        self.assertNotEqual(sample_review.pick(self.ROWS, 20, "2026-W46"), sample_review.pick(self.ROWS, 20, "2026-W47"))

    def test_fewer_candidates_than_asked_for_gives_them_all(self):
        self.assertEqual(len(sample_review.pick(self.ROWS[:5], 20, "x")), 5)


class Where(unittest.TestCase):
    TEXT = ("// Unit 7.\nvoid fn_7100000100(UnkA* self);\n\nvoid fn_7100000200() {\n    fn_7100000100(nullptr);\n}\n\n"
            "void fn_7100000100(UnkA* self) {\n}\n\nbool CFoo::IsReady() const {\n    return true;\n}\n")

    def test_the_first_line_at_the_margin_that_names_it(self):
        self.assertEqual(sample_review.definition_line(self.TEXT, "fn_7100000200"), 4)
        self.assertEqual(sample_review.definition_line(self.TEXT, "game::CFoo::IsReady"), 11)
        self.assertIsNone(sample_review.definition_line(self.TEXT, "fn_7100000900"))

    def test_the_questions_cover_what_no_gate_checks(self):
        text = " ".join(sample_review.QUESTIONS)
        for word in ("comment", "names", "layouts", "steer the compiler", "decompiler output", "declare"):
            self.assertIn(word, text)


if __name__ == "__main__":
    unittest.main()
