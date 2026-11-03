"""tools/gen_lift.py's FlatLifter, semantic mode: a float register used at both widths (the
"dual" set) keeps its S and D variables in step, also when a call's float result is typed later
(retype_call). No game data: the function is a few hand-encoded A64 words and the Original is a
stand-in. Skipped when pyelftools or capstone is not installed."""

import struct
import unittest

from tools.tests import _path  # noqa: F401

try:
    import gen_lift
except ImportError:  # pyelftools or capstone missing
    gen_lift = None

ADDR = 0x7100001000
CALLEE = ADDR + 8 + 0x1000


class _Img:
    data, relocs, symbols = b"", {}, []


class _Original:
    """What the lifters read of asmdiff.Original: one function and its callee."""
    version, module = "test", "main"

    def __init__(self, words: list[int]):
        self.code = b"".join(struct.pack("<I", w) for w in words)
        self.sizes = {ADDR: len(self.code), CALLEE: 4}
        self.img = _Img()

    def function(self, address: int) -> bytes:
        return self.code if address == ADDR else struct.pack("<I", 0xD65F03C0)

    def got_slot(self, slot: int):
        return None, None

    def plt_import(self, address: int):
        return None


def lift(words: list[int], returns: str):
    lifter = gen_lift.FlatLifter(_Original(words), ADDR, 0, returns)
    lifter.opts = frozenset({"semantic"})
    return lifter, lifter.lift()


# stp x29, x30, [sp, #-16]! ; mov x29, sp ; bl CALLEE ; <body> ; ldp x29, x30, [sp], #16 ; ret
PRO, EPI = [0xA9BF7BFD, 0x910003FD, 0x94000400], [0xA8C17BFD, 0xD65F03C0]
FADD_S1_S0_S0 = 0x1E202801  # reads s0: the call returns an f32
FCVT_D0_S1 = 0x1E22C020     # writes d0: s0/d0 is used at both widths
FADD_D1_D0_D0 = 0x1E602801  # reads d0: the call returns an f64
FCVT_S0_D1 = 0x1E624020     # writes s0


@unittest.skipIf(gen_lift is None, "pyelftools or capstone is not installed")
class DualCallResult(unittest.TestCase):
    def check(self, words: list[int], t: str, other: str):
        lifter, r = None, None
        for returns in ("void", "value"):
            lifter, r = lift(words, returns)
            if r:
                break
        self.assertIsNotNone(r, lifter.reason)
        self.assertIn(100, lifter.dual)
        lines = [ln.strip() for ln in r[0].split("\n")]
        call = next(i for i, ln in enumerate(lines) if ln.startswith(f"{t}0 = "))
        self.assertIn("(", lines[call])  # the call line, retyped
        # The other width is set from it on the very next line, as write() would.
        want = gen_lift.reinterpret_float(f"{t}0", "f32" if t == "S" else "f64")
        self.assertEqual(lines[call + 1], f"{other}0 = {want};")

    def test_f32_result_sets_d0(self):
        self.check(PRO + [FADD_S1_S0_S0, FCVT_D0_S1] + EPI, "S", "D")

    def test_f64_result_sets_s0(self):
        self.check(PRO + [FADD_D1_D0_D0, FCVT_S0_D1] + EPI, "D", "S")

    def test_sync_line_precedes_later_code(self):
        # Inserted right after the call, not appended: the fadd that read s0 comes after it.
        lifter, r = lift(PRO + [FADD_S1_S0_S0, FCVT_D0_S1] + EPI, "void")
        self.assertIsNotNone(r, lifter.reason)
        lines = [ln.strip() for ln in r[0].split("\n")]
        sync = lines.index(f"D0 = {gen_lift.reinterpret_float('S0', 'f32')};")
        self.assertTrue(lines[sync - 1].startswith("S0 = "))
        self.assertTrue(any(ln.startswith("S1 = ") for ln in lines[sync + 1:]))

    def test_single_width_gets_no_sync(self):
        # s0 only (no d0 anywhere): no dual set, no extra line.
        fmov_s1_s0 = 0x1E204001
        lifter, r = lift(PRO + [fmov_s1_s0] + EPI, "void")
        self.assertIsNotNone(r, lifter.reason)
        self.assertNotIn(100, lifter.dual)
        self.assertNotIn("    D0 = lift_", r[0])


if __name__ == "__main__":
    unittest.main()
