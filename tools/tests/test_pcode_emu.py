"""tools/pcode_emu.py's compare() on runs cut off by steps, with a per-run step budget
(Machine.scale, which port_lift raises to retry a long loop). No game data: the runs are
stand-ins carrying only what compare reads. Skipped when pyelftools is not installed."""

import unittest
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401

try:
    import pcode_emu
except ImportError:  # pyelftools missing
    pcode_emu = None


def run(end: str, steps: int, scale: int = 1, cut=None):
    return SimpleNamespace(events=[(("end", end), ())], steps=steps, scale=scale, cut=cut, clipped=False)


@unittest.skipIf(pcode_emu is None, "pyelftools is not installed")
class CutOff(unittest.TestCase):
    def test_two_cut_off_runs_decide_nothing(self):
        n = pcode_emu.MAX_STEPS
        d = pcode_emu.compare(run("time-out", n + 1, cut="steps"), run("time-out", n + 1, cut="events"), None)
        self.assertTrue(d.startswith("skip: "), d)

    def test_slower_lift_judged_by_its_own_budget(self):
        n = pcode_emu.MAX_STEPS
        short = n // pcode_emu.SLOWER
        # The original ended after `short` steps: a lift cut off at the default budget is too slow.
        d = pcode_emu.compare(run("return", short), run("time-out", n + 1, cut="steps"), None)
        self.assertTrue(d.startswith("the lift ran out of steps"), d)
        # An original that needed more than the default budget / SLOWER is not held to it.
        d = pcode_emu.compare(run("return", short + 1), run("time-out", n + 1, cut="steps"), None)
        self.assertTrue(d.startswith("skip: "), d)
        # With a retry budget, the same original is held to the larger one.
        k = pcode_emu.RETRY_SCALE
        d = pcode_emu.compare(run("return", short + 1), run("time-out", n * k + 1, k, "steps"), None)
        self.assertTrue(d.startswith("the lift ran out of steps"), d)


if __name__ == "__main__":
    unittest.main()
