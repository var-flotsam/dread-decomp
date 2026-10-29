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


def run(end: str, steps: int, scale: int = 1, cut=None, events=None, undef=()):
    return SimpleNamespace(events=events or [(("end", end), ())], steps=steps, scale=scale, cut=cut, clipped=False,
                           undef_mem=set(undef), calls_room=0)


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


@unittest.skipIf(pcode_emu is None, "pyelftools is not installed")
class Changes(unittest.TestCase):
    """Events carry the bytes changed since the one before; the memory at an event is all of them."""

    def test_written_follows_the_changes(self):
        events = [(("call", 1, (), ()), ((0x10, 1), (0x11, 2))), (("call", 2, (), ()), ((0x11, None), (0x12, 3))),
                  (("end", "return"), ((0x10, 4),))]
        self.assertEqual(pcode_emu.written(events, 0), {0x10, 0x11})
        self.assertEqual(pcode_emu.written(events, 2), {0x10, 0x12})

    def test_a_byte_written_back_before_the_next_event_is_no_change(self):
        m = SimpleNamespace(dirty={0x10: 5}, _ckpt={0x10: 5}, _touched={0x10}, events=[], snapshot_bytes=0,
                            calls_room=0, scale=1, cut=None, steps=0)
        m.max_events = lambda: 99
        pcode_emu.Machine.checkpoint(m, ("call", 1, (), ()))
        m.dirty[0x11] = 7
        m._touched = {0x11}
        pcode_emu.Machine.checkpoint(m, ("end", "return"))
        self.assertEqual([e[1] for e in m.events], [(), ((0x11, 7),)])


@unittest.skipIf(pcode_emu is None, "pyelftools is not installed")
class UndefinedBytes(unittest.TestCase):
    """A struct copy's padding, copied from stack the original never wrote, is not compared."""

    def events(self, mem):
        return [(("call", 0x7100001570, (), ()), tuple(sorted(mem.items()))), (("end", "return"), tuple(sorted(mem.items())))]

    def test_padding_copied_by_the_original_only(self):
        orig = run("return", 10, events=self.events({0x1000: 1, 0x1001: 0, 0x1002: 0}), undef={0x1001, 0x1002})
        lift = run("return", 10, events=self.events({0x1000: 1}))
        self.assertEqual(pcode_emu.compare(orig, lift, None), "")

    def test_a_missing_defined_byte_still_differs(self):
        orig = run("return", 10, events=self.events({0x1000: 1, 0x1001: 0}), undef={0x1002})
        lift = run("return", 10, events=self.events({0x1000: 1}))
        self.assertTrue(pcode_emu.compare(orig, lift, None).startswith("memory at event 0"))


@unittest.skipIf(pcode_emu is None, "pyelftools is not installed")
class FloatToInt(unittest.TestCase):
    """Scalar fcvt*s/fcvt*u run directly: unsigned forms saturate to 0 and all ones."""

    def convert(self, w: int, value: float, double: bool = False) -> int:
        import struct
        m = pcode_emu.Machine.__new__(pcode_emu.Machine)
        m.regs, m.undef_vn, m.state, m.lift = bytearray(0x10000), set(), {}, None
        q1 = pcode_emu.Q0 + pcode_emu.QSTRIDE
        m.regs[q1:q1 + 8] = struct.pack("<d", value) if double else struct.pack("<f", value) + bytes(4)
        m.set_reg(pcode_emu.X0, 8, 0xDEADBEEFDEADBEEF)
        self.assertEqual(m.step(0x7100000000, w, *range(6)), ("next", 0))
        return m.x(0)

    def test_saturation_and_rounding(self):
        base = 0x1E200020  # w0 <- s1
        fcvtzu, fcvtzs, fcvtmu, fcvtps, fcvtas, fcvtns = (base | 3 << 19 | 1 << 16, base | 3 << 19, base | 2 << 19 | 1 << 16,
                                                          base | 1 << 19, base | 4 << 16, base)
        self.assertEqual(self.convert(fcvtzu, 5e9), 0xFFFFFFFF)  # (Ghidra's TRUNC: 0x7fffffff)
        self.assertEqual(self.convert(fcvtzu, -3.0), 0)
        self.assertEqual(self.convert(fcvtzu, float("nan")), 0)
        self.assertEqual(self.convert(fcvtzs, -1.5), 0xFFFFFFFF)
        self.assertEqual(self.convert(fcvtzs, 5e9), 0x7FFFFFFF)
        self.assertEqual(self.convert(fcvtzs, float("-inf")), 0x80000000)
        self.assertEqual(self.convert(fcvtmu, 2.7), 2)
        self.assertEqual(self.convert(fcvtps, -2.5), 0xFFFFFFFE)
        self.assertEqual(self.convert(fcvtas, 2.5), 3)
        self.assertEqual(self.convert(fcvtns, 2.5), 2)
        self.assertEqual(self.convert(fcvtzu | 1 << 31 | 1 << 22, 1e20, True), 0xFFFFFFFFFFFFFFFF)  # fcvtzu x0, d1

    def test_rounding_stays_float(self):
        import struct
        m = pcode_emu.Machine.__new__(pcode_emu.Machine)
        m.regs, m.undef_vn, m.state, m.lift = bytearray(0x10000), set(), {}, None
        q1, q2 = pcode_emu.Q0 + pcode_emu.QSTRIDE, pcode_emu.Q0 + 2 * pcode_emu.QSTRIDE
        for rmode, value, want in ((2, 649.7, 649.0), (1, -0.5, -0.0), (0, 2.5, 2.0), (4, 2.5, 3.0), (3, -1.7, -1.0)):
            m.regs[q1:q1 + 4] = struct.pack("<f", value)
            self.assertEqual(m.step(0x7100000000, 0x1E244022 | rmode << 15, *range(6)), ("next", 0))  # frint* s2, s1
            got = struct.unpack("<f", bytes(m.regs[q2:q2 + 4]))[0]
            self.assertEqual((got, str(got)), (want, str(want)), (rmode, value))



@unittest.skipIf(pcode_emu is None, "pyelftools is not installed")
class Snapshots(unittest.TestCase):
    def test_module_addresses_move_to_ours(self):
        import json
        import os
        import tempfile
        import zlib
        page = bytearray(pcode_emu.PAGE)
        page[8:16] = (pcode_emu.RECOMP_MAIN + 0x1230).to_bytes(8, "little")  # a pointer into the module
        page[16:24] = (0x123456789).to_bytes(8, "little")  # a heap pointer stays
        head = {"x": [hex(pcode_emu.RECOMP_MAIN + 0x40)] + ["0x0"] * 30, "q": ["0x0"] * 32,
                "sp": "0x1000", "stack": "0102", "pages": 2}
        raw = pcode_emu.SNAP_MAGIC + json.dumps(head).encode() + b"\n"
        raw += (pcode_emu.RECOMP_MAIN + 0x2000).to_bytes(8, "little") + bytes(page)  # a module page
        raw += (0x200000000).to_bytes(8, "little") + bytes(page)
        fd, path = tempfile.mkstemp(suffix=".snap")
        os.write(fd, zlib.compress(raw))
        os.close(fd)
        try:
            s = pcode_emu.load_snapshot(path, 0x100000)
        finally:
            os.unlink(path)
        B = pcode_emu.BASE
        self.assertEqual(s.x[0], B + 0x40)
        self.assertEqual(sorted(s.pages), [0x200000000 // pcode_emu.PAGE, (B + 0x2000) // pcode_emu.PAGE])
        p = s.pages[0x200000000 // pcode_emu.PAGE]
        self.assertEqual(int.from_bytes(p[8:16], "little"), B + 0x1230)
        self.assertEqual(int.from_bytes(p[16:24], "little"), 0x123456789)
        self.assertEqual(s.stack, b"\x01\x02")

    def test_calls_beside_the_snapshot(self):
        import json
        import os
        import tempfile
        from types import SimpleNamespace
        R, B = pcode_emu.RECOMP_MAIN, pcode_emu.BASE
        head = {"x": ["0x0"] * 31, "q": ["0x0"] * 32, "sp": "0x7fff0000", "stack": "", "pages": 0}
        vtable = (R + 0x5000).to_bytes(8, "little").hex()
        calls = [{"target": hex(R + 0x100), "x": ["0x5", hex(R + 0x40)], "q": ["0x1"] * 4,
                  "frame": [[-0x34, "0700000000000000"]], "mem": [["0x200000010", vtable]]},
                 {"target": hex(R + 0x200), "x": ["0x0", "0x0"], "q": ["0x0"] * 4, "frame": [], "mem": []}]
        d = tempfile.mkdtemp()
        snap = os.path.join(d, "fn_7100000000.snap")
        with open(snap, "wb") as f:
            f.write(pcode_emu.SNAP_MAGIC + json.dumps(head).encode() + b"\n")
        with open(snap[:-5] + ".calls", "wb") as f:  # (the second line cut short: the game closed)
            f.write(pcode_emu.CALLS_MAGIC + b"\n".join(json.dumps(c).encode() for c in calls)[:-10])
        try:
            s = pcode_emu.load_snapshot(snap, 0x100000)
        finally:
            for n in os.listdir(d):
                os.unlink(os.path.join(d, n))
            os.rmdir(d)
        self.assertEqual(len(s.calls), 1)
        c = s.calls[0]
        self.assertEqual((c.target, c.x), (B + 0x100, [5, B + 0x40]))
        self.assertEqual(c.frame, [(-0x34, bytes([7]) + bytes(7))])
        self.assertEqual(c.mem, [(0x200000010, (B + 0x5000).to_bytes(8, "little"))])
        # Replayed in order while the targets agree; a call elsewhere ends it, a modelled one waits
        m = SimpleNamespace(snap=s, replay_i=0, replay_end=None, calls=0, canonical=lambda a: a)
        rec = pcode_emu.Machine.recorded
        self.assertIsNone(rec(m, B + 0x900, ("import", "memcpy"), True))
        self.assertIsNone(m.replay_end)
        self.assertIs(rec(m, B + 0x100, B + 0x100, False), c)
        self.assertIsNone(rec(m, B + 0x100, B + 0x100, False))
        m.replay_i = 0
        self.assertIsNone(rec(m, B + 0x300, B + 0x300, False))
        self.assertIn("the real run's", m.replay_end)
