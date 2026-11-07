"""tools/forwarded_args.py: declared argument registers from demangled names, the evidence tiers on
hand-assembled AArch64, and vtable siblings. No game data: each function is a few instruction
words written here. The liveness tests are skipped when capstone is not installed."""

import struct
import unittest

from tools.tests import _path  # noqa: F401

import forwarded_args as fa

try:
    import abi_facts
except ImportError:  # capstone missing
    abi_facts = None

RET = 0xD65F03C0
LDR_X8_X0 = 0xF9400008      # ldr x8, [x0]
BLR_X8 = 0xD63F0100         # blr x8
STR_X2_X0 = 0xF9000002      # str x2, [x0]
STR_X3_X0 = 0xF9000003      # str x3, [x0]
MOV_X5_X0 = 0xAA0003E5      # mov x5, x0
MOV_X4_X0 = 0xAA0003E4      # mov x4, x0


def branch(op: int):
    return lambda at, target: op | (((target - at) >> 2) & 0x3FFFFFF)


bl, b = branch(0x94000000), branch(0x14000000)


def code(at: int, *words) -> bytes:
    return b"".join(struct.pack("<I", w(at + 4 * i) if callable(w) else w) for i, w in enumerate(words))


READS, TAIL, CALLEE, IND, CALLER, OPEN, SIB = (0x1000 * n for n in range(1, 8))
FUNCTIONS = {
    READS: code(READS, STR_X2_X0, RET),                          # reads x2 itself
    CALLEE: code(CALLEE, STR_X3_X0, RET),
    TAIL: code(TAIL, lambda a: b(a, CALLEE)),                    # hands x3 to its callee untouched
    IND: code(IND, LDR_X8_X0, BLR_X8, RET),                      # a virtual call; its caller sets x5
    CALLER: code(CALLER, MOV_X5_X0, lambda a: bl(a, IND), RET),
    OPEN: code(OPEN, LDR_X8_X0, BLR_X8, RET),                    # a virtual call, no direct caller
    SIB: code(SIB, MOV_X4_X0, LDR_X8_X0, BLR_X8, RET),           # writes x4 before its virtual call
}


class FakeOriginal:
    def __init__(self, functions: dict[int, bytes]):
        self.code = functions
        self.sizes = {a: len(c) for a, c in functions.items()}

    def function(self, address: int) -> bytes:
        return self.code[address]

    def plt_import(self, address: int):
        return None


class FakeGraph:
    callers = {CALLEE: [TAIL], IND: [CALLER]}


class Declared(unittest.TestCase):
    def test_free_function(self):
        d = fa.declared("fn_7100087ecc(UnkRegistry_A1b72*, UnkRegistryKey_A1b72 const*, void*, bool)")
        self.assertEqual((d.ints, d.floats, d.aggregate), (4, 0, False))

    def test_member_counts_this(self):
        d = fa.declared("CActor::fn_7100000010(float, int) const")
        self.assertEqual((d.ints, d.floats), (2, 1))

    def test_aggregate_marked(self):
        d = fa.declared("fn_7100000020(base::UnkPair, double)")
        self.assertEqual((d.ints, d.floats, d.aggregate), (1, 1, True))

    def test_hfa_takes_float_registers(self):
        d = fa.declared("fn_7100000040(base::math::CVector4D, base::math::CVector3D const, UnkFoo*)")
        self.assertEqual((d.ints, d.floats, d.aggregate), (1, 7, False))

    def test_variadic_skipped(self):
        self.assertIsNone(fa.declared("fn_7100000030(char const*, ...)"))

    def test_beyond(self):
        d = fa.Declared(2, 1)
        self.assertEqual(fa.beyond({0, 1, 2, 7, 8, 100, 101}, d), [2, 7, 101])


class Siblings(unittest.TestCase):
    def test_related_by_shared_entry(self):
        tables = [[0x10, 0x20, 0x30], [0x10, 0x21, 0x31], [0x99, 0x22]]
        s = fa.vtable_siblings(tables)
        self.assertEqual(s[0x20], {0x21})          # table 2 shares nothing with table 0 at an index
        self.assertEqual(s[0x31], {0x30})
        self.assertNotIn(0x22, s)

    def test_common_entry_does_not_relate(self):
        stub = 0x5
        tables = [[stub, 0x20 + n] for n in range(fa.SHARED_MAX + 1)]
        self.assertEqual(fa.vtable_siblings(tables), {})


@unittest.skipIf(abi_facts is None, "capstone is not installed")
class Tiers(unittest.TestCase):
    def setUp(self):
        self.af = abi_facts.AbiFacts(FakeOriginal(FUNCTIONS), rows={}, graph=FakeGraph())

    def tiers(self, addr):
        return {fa.reg_name(k): fa.TIERS[t] for k, t in fa.tiered_live_in(self.af, addr).items()}

    def test_read(self):
        self.assertEqual(self.tiers(READS), {"x0": "read", "x2": "read"})

    def test_direct_call(self):
        self.assertEqual(self.tiers(TAIL), {"x0": "call", "x3": "call"})

    def test_indirect_set_by_caller(self):
        self.assertEqual(self.tiers(IND), {"x0": "read", "x5": "indirect"})

    def test_open(self):
        t = self.tiers(OPEN)
        self.assertEqual(t.pop("x0"), "read")
        self.assertEqual(t, {f"x{k}": "open" for k in range(1, 8)})

    def test_open_stops_at_a_write(self):
        self.assertNotIn("x4", self.tiers(SIB))


if __name__ == "__main__":
    unittest.main()
