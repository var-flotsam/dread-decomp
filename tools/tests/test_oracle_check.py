"""port_lift.Oracle.check's verdict over its inputs: a pass needs an input the original ended on
without a fault (two runs crashing alike were compared only up to the crash). No game data: the
oracle's runs are hand-made. Skipped when pyelftools or capstone is not installed (port_lift's
imports)."""

import types
import unittest

from tools.tests import _path  # noqa: F401

try:
    import port_lift
except ImportError:  # pyelftools or capstone missing
    port_lift = None

ADDR = 0x7100000000


def original(end: str, cut=None):
    return types.SimpleNamespace(visited={1}, cut=cut, noret_call=None, call_log=[], events=[(("end", end), ())],
                                 small_outs={})


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class CrashAgreements(unittest.TestCase):
    def check(self, ends, verdict="", exact=False):
        """`exact`: the lift's events equal the original's, the crash included (the pre-round-10
        standard); otherwise compare's loose both-fault agreement (a register the callee is not
        passed differs), events not equal."""
        o = port_lift.Oracle.__new__(port_lift.Oracle)
        o.preset_orig, o.dicts = {}, {ADDR: ()}
        o.originals = lambda addr: [(n, {}, original(e)) for n, e in enumerate(ends)]
        lift_events = [(("end", "fault"), ())] if exact else [(("call", 1, (), ((0, 1),)), ()), (("end", "fault"), ())]
        o.machine = lambda *a, **k: types.SimpleNamespace(run=lambda start: None, events=lift_events)
        lo = types.SimpleNamespace(functions={ADDR: (0x70C0000000, 16)}, unresolved={},
                                   text=(0x70C0000000, 0x70C0000010), guards=())
        emu = port_lift.pcode_emu
        saved = emu.compare
        emu.compare = lambda a, b, ret: verdict
        try:
            return o.check(ADDR, lo, "x32")
        finally:
            emu.compare = saved

    def test_only_crash_agreements_are_inconclusive(self):
        """Every input faults in both runs, agreeing only by compare's loose both-fault rule: the
        result and the stores after the crash were never compared."""
        v = self.check(["fault"] * 8)
        self.assertEqual(v[0], "inconclusive")
        self.assertIn("only crash agreements", v[1])

    def test_exact_crash_agreements_pass(self):
        """Every input faults in both runs with exactly the same events, the crash included: the
        standard functions passed on before round 10 (179 of lifted_all12's accepts, lost to the
        crash-only rule)."""
        v = self.check(["fault"] * 8, exact=True)
        self.assertEqual((v[0], v[2]), ("pass", 8))

    def test_one_ended_agreement_passes(self):
        v = self.check(["fault"] * 7 + ["return"])
        self.assertEqual((v[0], v[2]), ("pass", 8))

    def test_ended_agreements_pass(self):
        self.assertEqual(self.check(["return", "noreturn"])[0], "pass")


if __name__ == "__main__":
    unittest.main()
