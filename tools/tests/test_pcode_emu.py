"""tools/pcode_emu.py's compare() on runs cut off by steps, with a per-run step budget
(Machine.max_steps, which port_lift raises to retry a long loop). No game data: the runs are
stand-ins carrying only what compare reads. Skipped when pyelftools is not installed."""

import unittest
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401

try:
    import pcode_emu
except ImportError:  # pyelftools missing
    pcode_emu = None


def run(end: str, steps: int, max_steps: int, cut=None):
    return SimpleNamespace(events=[(("end", end), ())], steps=steps, max_steps=max_steps, cut=cut, clipped=False)


@unittest.skipIf(pcode_emu is None, "pyelftools is not installed")
class CutOff(unittest.TestCase):
    def test_two_cut_off_runs_decide_nothing(self):
        n = pcode_emu.MAX_STEPS
        d = pcode_emu.compare(run("time-out", n + 1, n, "steps"), run("time-out", n + 1, n, "steps"), None)
        self.assertTrue(d.startswith("skip: "), d)

    def test_slower_lift_judged_by_its_own_budget(self):
        n = pcode_emu.MAX_STEPS
        short = n // pcode_emu.SLOWER
        # The original ended after `short` steps: a lift cut off at the default budget is too slow.
        d = pcode_emu.compare(run("return", short, n), run("time-out", n + 1, n, "steps"), None)
        self.assertTrue(d.startswith("the lift ran out of steps"), d)
        # An original that needed more than the default budget / SLOWER is not held to it.
        d = pcode_emu.compare(run("return", short + 1, n), run("time-out", n + 1, n, "steps"), None)
        self.assertTrue(d.startswith("skip: "), d)
        # With a retry budget, the same original is held to the larger one.
        big = n * pcode_emu.RETRY_STEPS
        d = pcode_emu.compare(run("return", short + 1, big), run("time-out", big + 1, big, "steps"), None)
        self.assertTrue(d.startswith("the lift ran out of steps"), d)


if __name__ == "__main__":
    unittest.main()
