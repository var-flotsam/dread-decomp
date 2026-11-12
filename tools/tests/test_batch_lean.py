"""tools/batch/lean/coord.py: which job a function gets next, and what its worker is told (no agents, no game
data)."""
import importlib.util
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401

KIT = Path(__file__).resolve().parents[1] / "batch" / "lean"
spec = importlib.util.spec_from_file_location("lean_coord", KIT / "coord.py")
coord = importlib.util.module_from_spec(spec)
spec.loader.exec_module(coord)


def entry(tier="small", prior=None, attempts=()):
    return {"addr": "7100000100", "size": 200, "tier": tier, "prior": prior, "attempts": list(attempts),
            "file": "src/game/CFoo/CFoo_<What>_l000100.cpp", "status": "queued"}


def attempt(outcome, best=0, model="sonnet"):
    return {"outcome": outcome, "best": best, "model": model, "blocker": "x9 and x10 swapped"}


class Plan(unittest.TestCase):
    def test_first_pass_by_size(self):
        self.assertEqual(coord.plan(entry(), {}), ("sonnet", 2))
        self.assertEqual(coord.plan(entry("large"), {}), ("opus", 3))

    def test_a_match_is_finished(self):
        self.assertIsNone(coord.plan(entry(attempts=[attempt("match", 100)]), {}))

    def test_a_close_non_match_gets_one_retry(self):
        self.assertEqual(coord.plan(entry(attempts=[attempt("differs", 80)]), {}), ("opus", 2))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("differs", 60)]), {}))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("differs", 80), attempt("differs", 90, "opus")]), {}))

    def test_a_registers_only_result_gets_no_model_retry(self):
        # CLAUDE.md, tier 2: a permuter and the machine test take it from there, however close it is
        self.assertIsNone(coord.plan(entry(attempts=[attempt("registers", 98)]), {}))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("minor", 98)]), {}))

    def test_one_model_means_one_job(self):
        cfg = {"model": "sonnet", "budget": 6}
        self.assertEqual(coord.plan(entry("large"), cfg), ("sonnet", 6))
        self.assertIsNone(coord.plan(entry(attempts=[attempt("differs", 90)]), cfg))


class Brief(unittest.TestCase):
    def test_the_worker_is_sent_to_the_tracked_brief(self):
        text = coord.prompt(entry(), "sonnet", 6, {})
        self.assertIn("Follow tools/batch/lean/worker.md exactly", text)
        self.assertIn("Address: 7100000100 (200 bytes); context build/lean/ctx/7100000100.txt", text)
        self.assertIn("Attempt budget: 6.", text)
        self.assertNotIn("NON_MATCHING", text)       # a worker never leaves a non-match in src/
        self.assertNotIn("Ghidra", text)

    def test_ghidra_s_view_is_for_reading(self):
        text = coord.prompt(entry(), "sonnet", 6, {"ghidra": True})
        self.assertIn("build/lean/ghidra/7100000100.txt", text)
        self.assertIn("Never copy its text", text)

    def test_an_earlier_run_s_result_is_passed_on(self):
        text = coord.prompt(entry(prior={"best": 71, "blocker": "loop form"}), "sonnet", 2, {})
        self.assertIn("An earlier run reached 71% (blocker: loop form).", text)

    def test_the_brief_keeps_non_matches_out_of_src_and_names_the_forms(self):
        brief = (KIT / "worker.md").read_text(encoding="utf-8")
        for needle in ('"match"|"registers"|"differs"|"skipped"', "build/lean/drafts/<addr>.cpp", "flatten",
                       "Readable first", "tools/batch/lean/try.py"):
            self.assertIn(needle, brief)


if __name__ == "__main__":
    unittest.main()
