"""tools/port_lift.py's per-function wall-clock budget (ORACLE_BUDGET, Oracle.out_of_time, _check)
on a fake clock. No game data: the oracle and its checks are stand-ins.
Skipped when pyelftools or capstone is not installed."""

import types
import unittest

from tools.tests import _path  # noqa: F401

try:
    import port_lift
except ImportError:  # pyelftools or capstone missing
    port_lift = None


class Clock:
    def __init__(self):
        self.t = 1000.0

    def __call__(self):
        return self.t


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class Budget(unittest.TestCase):
    def setUp(self):
        self.clock = Clock()
        self.saved = port_lift._clock, port_lift._lift_object, dict(port_lift._CTX)
        port_lift._clock = self.clock
        port_lift._lift_object = lambda k: None

    def tearDown(self):
        port_lift._clock, port_lift._lift_object = self.saved[0], self.saved[1]
        port_lift._CTX.clear()
        port_lift._CTX.update(self.saved[2])

    def oracle(self, seconds_per_check: float, verdict: str = "inconclusive"):
        """An Oracle whose check() spends seconds_per_check on the fake clock; it records the attempts."""
        o = port_lift.Oracle.__new__(port_lift.Oracle)
        o.orig_runs, o.retry_runs, o.checked = {}, {}, []
        o.orig = types.SimpleNamespace(sizes={0x100: 16})

        def check(addr, lo, ret):
            o.checked.append(ret)
            self.clock.t += seconds_per_check
            return verdict, "why", 0, set()
        o.check = check
        port_lift._CTX.update(oracle=o, debug=False, budget=10.0)
        return o

    def test_out_of_time(self):
        o = port_lift.Oracle.__new__(port_lift.Oracle)
        o.deadline, o.over = self.clock.t + 10, None
        self.assertFalse(o.out_of_time("seeds"))
        self.clock.t += 11
        self.assertTrue(o.out_of_time("repairs"))
        self.assertTrue(o.out_of_time("fuzz"))
        self.assertEqual(o.over, "repairs")  # (the first phase to find it spent is kept)

    def test_no_deadline_never_runs_out(self):
        o = port_lift.Oracle.__new__(port_lift.Oracle)
        self.clock.t += 1e9
        self.assertFalse(o.out_of_time("seeds"))

    def test_attempts_stop_past_the_budget(self):
        o = self.oracle(6.0)
        a, k, info, fail, debug, (secs, over) = port_lift._check((0x100, [(0, "a"), (1, "b"), (2, "c"), (3, "d")]))
        self.assertIsNone(k)
        self.assertEqual(o.checked, ["a", "b"])  # (6 s, 12 s: the third is not started)
        self.assertEqual(over, "attempts")
        self.assertIn("oracle budget 10 s reached in attempts", fail)
        self.assertAlmostEqual(secs, 12.0)

    def test_within_the_budget_all_attempts_run(self):
        o = self.oracle(1.0)
        a, k, info, fail, debug, (secs, over) = port_lift._check((0x100, [(0, "a"), (1, "b"), (2, "c")]))
        self.assertEqual(o.checked, ["a", "b", "c"])
        self.assertIsNone(over)
        self.assertNotIn("budget", fail)

    def test_a_pass_is_kept(self):
        self.oracle(20.0, verdict="pass")
        a, k, info, fail, debug, (secs, over) = port_lift._check((0x100, [(0, "a"), (1, "b")]))
        self.assertEqual(k, 0)
        self.assertEqual(fail, "")


if __name__ == "__main__":
    unittest.main()
