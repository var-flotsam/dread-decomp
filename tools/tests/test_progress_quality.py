"""tools/progress.py: which row of the completeness table a function is in (no game data)."""
import unittest

from tools.tests import _path  # noqa: F401
import progress


class Quality(unittest.TestCase):
    def test_complete_needs_both_qualities(self):
        self.assertEqual(progress.quality("O", hand=True, steered=False, verified=False), "complete")
        self.assertEqual(progress.quality("O", hand=True, steered=True, verified=False), "match_steered")
        self.assertEqual(progress.quality("O", hand=False, steered=False, verified=False), "match_generated")

    def test_identical_behaviour_with_other_bytes_is_its_own_row(self):
        self.assertEqual(progress.quality("m", hand=True, steered=False, verified=True), "equivalent")
        self.assertEqual(progress.quality("m", hand=True, steered=False, verified=False), "unproven")
        self.assertEqual(progress.quality("M", hand=True, steered=False, verified=True), "unproven")   # only m is judged
        self.assertEqual(progress.quality("W", hand=True, steered=False, verified=False), "unproven")

    def test_what_is_not_written_is_missing(self):
        self.assertEqual(progress.quality("U", hand=False, steered=False, verified=False), "missing")

    def test_every_row_has_a_label_and_the_line_names_both_qualities(self):
        q = {k: {"functions": 10, "bytes": 100} for k in progress.QUALITIES}
        line = progress.quality_line(q, {"bytes": 600})
        self.assertIn("byte-matching and readable **16.67%** (10 functions)", line)
        self.assertIn("byte-matching but not readable yet 33.33%", line)
        self.assertIn("identical behaviour with other bytes 16.67%", line)
        self.assertEqual(set(q), set(progress.QUALITIES))

    def test_the_steering_forms_are_fakematch_s(self):
        import fakematch
        self.assertTrue(set(progress.STEERING) <= set(fakematch.KINDS))
        self.assertNotIn("untyped-store", progress.STEERING)     # block copies and bit casts are ordinary source


if __name__ == "__main__":
    unittest.main()
