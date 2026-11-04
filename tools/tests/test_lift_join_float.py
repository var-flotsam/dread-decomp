"""tools/gen_lift.py's structured Lifter, semantic mode: a call's unread s0 at a join where the
function reads s0 next. abi_facts.result_main scans forward from each `bl F` only up to the first
branch target, so when the call falls straight into the join (`fmov s0, #1.0; cbz w0, L; bl F;
L: fadd s0, s0, s0`) this caller gives no evidence; F's result_main without s0 is then no proof
that F is void, and the lift must not keep the other path's s0 in its place. No game data: a few
hand-encoded A64 words. Skipped when pyelftools or capstone is not installed."""

import struct
import unittest

from tools.tests import _path  # noqa: F401

try:
    import gen_lift
except ImportError:  # pyelftools or capstone missing
    gen_lift = None

ADDR = 0x7100001000
CALLEE = ADDR + 0x1008
STP, MOV_FP, LDP, RET = 0xA9BF7BFD, 0x910003FD, 0xA8C17BFD, 0xD65F03C0
FMOV_S0_1, FMOV_S0_2, FADD_S0 = 0x1E2E1000, 0x1E201000, 0x1E202800
FMOV_S1_1, FADD_S0_S1 = 0x1E2E1001, 0x1E212820  # (s1 = 1.0; s0 = s1 + s1: s0 written at the join)


def bl(at: int) -> int:
    return 0x94000000 | ((CALLEE - (ADDR + at)) // 4)


# if form: stp; mov; fmov s0, #1.0; cbz w0, L; bl F; L: fadd s0, s0, s0; ldp; ret
IF_FORM = [STP, MOV_FP, FMOV_S0_1, 0x34000040, bl(16), FADD_S0, LDP, RET]
# else form: stp; mov; cbz w0, L1; bl F; b L2; L1: fmov s0, #2.0; L2: fadd s0, s0, s0; ldp; ret
ELSE_FORM = [STP, MOV_FP, 0x34000060, bl(12), 0x14000002, FMOV_S0_2, FADD_S0, LDP, RET]
# the void call before the branch, s0 written on the other path (fn_7100900c44): stp; mov; bl F;
# cbz w0, L; fmov s0, #2.0; L: fadd s0, s0, s0; ldp; ret
CALL_FIRST = [STP, MOV_FP, bl(8), 0x34000040, FMOV_S0_2, FADD_S0, LDP, RET]
# s0 not read at the join (written first): the other path's s0 may stand
IF_WRITTEN = [STP, MOV_FP, FMOV_S0_1, 0x34000040, bl(16), FMOV_S1_1, FADD_S0_S1, LDP, RET]


class _Img:
    data, relocs, symbols = b"", {}, []


class _Original:
    version, module = "test", "main"

    def __init__(self, words):
        self.code = b"".join(struct.pack("<I", w) for w in words)
        self.sizes = {ADDR: len(self.code), CALLEE: 4}
        self.img = _Img()

    def function(self, address):
        return self.code if address == ADDR else struct.pack("<I", RET)

    def got_slot(self, slot):
        return None, None

    def plt_import(self, address):
        return None


@unittest.skipIf(gen_lift is None, "pyelftools or capstone is not installed")
class JoinReadsS0(unittest.TestCase):
    def setUp(self):
        saved = gen_lift.Lifter.main
        self.addCleanup(lambda: setattr(gen_lift.Lifter, "main", saved))
        gen_lift.Lifter.main = staticmethod(lambda a: frozenset())  # (result_main: no caller reads s0)

    def lifts(self, words):
        out = []
        for returns in ("void", "value"):
            lifter = gen_lift.Lifter(_Original(words), ADDR, 0, returns)
            lifter.opts = frozenset({"semantic"})
            r = lifter.lift()
            if r:
                out.append(r[0])
        return out

    def assert_no_dropped_result(self, words, wrong):
        for text in self.lifts(words):
            body = [ln.strip() for ln in text.split("\n")]
            call = [ln for ln in body if "fn_7100002008" in ln and "(" in ln and not ln.startswith("namespace")]
            self.assertTrue(call, text)
            # (refused, or the call's own float result read at the join: never the other path's)
            self.assertFalse(any(ln.endswith(wrong) for ln in body), text)
            self.assertTrue(any("f32" in ln and "fn_7100002008" in ln for ln in call), text)

    def test_if_form(self):
        self.assert_no_dropped_result(IF_FORM, "= r0_100 + r0_100;")

    def test_else_form(self):
        self.assert_no_dropped_result(ELSE_FORM, "= (f32)0;")

    def test_call_before_the_branch_keeps_the_other_path(self):
        """result_main's scan from this call runs past the cbz into the write of s0: its answer
        stands, and the float computed on one path is the phi (fn_7100900c44)."""
        texts = self.lifts(CALL_FIRST)
        self.assertTrue(texts)
        self.assertTrue(all("f32 m" in t and "2.0f" in t for t in texts), texts)

    def test_s0_written_at_the_join_keeps_the_other_path(self):
        """Nothing reads the call's s0: result_main's answer stands and the lift goes through."""
        self.assertTrue(self.lifts(IF_WRITTEN))


if __name__ == "__main__":
    unittest.main()
