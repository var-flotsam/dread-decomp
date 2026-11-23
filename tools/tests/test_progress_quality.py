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
        self.assertIn("matching and clean in form **16.67%** (10 functions; byte-matching, hand-written, no steering "
                      "form 16.67%, plus generated, read and passed 0.00%)", line)
        self.assertIn("byte-matching but not clean in form yet 33.33%", line)
        self.assertIn("identical behaviour with other bytes 16.67%", line)
        self.assertNotIn("readable", line)
        self.assertEqual(set(q), set(progress.QUALITIES))

    def test_the_headline_adds_generated_read_and_passed(self):
        q = {k: {"functions": 10, "bytes": 100} for k in progress.QUALITIES}
        n = {"read_passed": {"functions": 5, "bytes": 50}}
        self.assertEqual(progress.headline(q, n), {"functions": 15, "bytes": 150})
        self.assertEqual(progress.headline(q, None), {"functions": 10, "bytes": 100})
        line = progress.quality_line(q, {"bytes": 600}, n)
        self.assertIn("matching and clean in form **25.00%** (15 functions; byte-matching, hand-written, no steering "
                      "form 16.67%, plus generated, read and passed 8.33%)", line)
        self.assertIn("byte-matching but not clean in form yet 25.00% (generated source not yet read 8.33%", line)

    def test_the_card_and_the_daily_record_read_the_new_line_and_the_old(self):
        import daily
        import status_card
        q = {k: {"functions": 1_000, "bytes": 100} for k in progress.QUALITIES}
        new = progress.quality_line(q, {"bytes": 800}, {"read_passed": {"functions": 234, "bytes": 0}})
        old = "Completeness ...: byte-matching and readable **17.55%** (11,264 functions); byte-matching but"
        self.assertEqual(daily.finished_from_handoff(new), (12.5, 1234))
        self.assertEqual(status_card.finished(new), {"pct": 12.5, "functions": 1234})
        self.assertEqual(daily.finished_from_handoff(old), (17.55, 11264))
        self.assertEqual(status_card.finished(old), {"pct": 17.55, "functions": 11264})

    def test_the_steering_forms_are_fakematch_s(self):
        import fakematch
        self.assertTrue(set(progress.STEERING) <= set(fakematch.KINDS))
        self.assertNotIn("untyped-store", progress.STEERING)     # block copies and bit casts are ordinary source


if __name__ == "__main__":
    unittest.main()
