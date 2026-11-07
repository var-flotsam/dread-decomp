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


STR_X1_X0 = 0xF9000001      # str x1, [x0]
MOV_X1_X0 = 0xAA0003E1      # mov x1, x0
CBZ_X0_8 = 0xB4000040       # cbz x0, .+8

# Calls: BASE reads x1; FWD tail-branches to it with x1 untouched; CALLS calls it and returns;
# HALF writes x1 on one path only (not untouched); SETS writes x1 before the call.
BASE, FWD, CALLS, HALF, SETS = (0x10000 + 0x100 * n for n in range(5))
CALL_FUNCTIONS = {
    BASE: code(BASE, STR_X1_X0, RET),
    FWD: code(FWD, lambda a: b(a, BASE)),
    CALLS: code(CALLS, lambda a: bl(a, BASE), RET),
    HALF: code(HALF, CBZ_X0_8, MOV_X1_X0, lambda a: b(a, BASE)),
    SETS: code(SETS, MOV_X1_X0, lambda a: b(a, BASE)),
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


# A switch on w1 at an address in the image, with Clang's compressed jump table after the code: each
# case writes w2 before the join reads it, so x2 is no argument (the table's targets, not every block
# after the br).
SWITCH = 0x7100010000
SWITCH_CODE = code(SWITCH,
                   0x7100043F,               # cmp w1, #1
                   0x54000188,               # b.hi default (+0x30)
                   0x90000008,               # adrp x8, (this page)
                   0x91010108,               # add x8, x8, #0x40 (the table)
                   0x10000089,               # adr x9, case0 (+0x10)
                   0x3861690A,               # ldrb w10, [x8, x1]
                   0x8B0A0929,               # add x9, x9, x10, lsl #2
                   0xD61F0120,               # br x9
                   0x52800022,               # case0: mov w2, #1
                   lambda a: b(a, SWITCH + 0x2C),
                   0x52800042,               # case1: mov w2, #2
                   0xB9000002,               # join: str w2, [x0]
                   RET,
                   RET)                      # default


class FakeImage:
    def __init__(self):
        import asmdiff
        self.data = bytearray(SWITCH - asmdiff.BASE + 0x48)
        at = SWITCH - asmdiff.BASE
        self.data[at:at + len(SWITCH_CODE)] = SWITCH_CODE
        self.data[at + 0x40:at + 0x42] = bytes([0, 2])  # case0 +0, case1 +8


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

    def test_initializer_list_takes_two(self):
        d = fa.declared("fn_7100000050(UnkTipList*, std::__1::initializer_list<unsigned int>, float)")
        self.assertEqual((d.ints, d.floats, d.aggregate), (3, 1, False))

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

    def test_switch_follows_its_table(self):
        orig = FakeOriginal({SWITCH: SWITCH_CODE})
        orig.img = FakeImage()
        af = abi_facts.AbiFacts(orig, rows={}, graph=FakeGraph())
        tiers = {fa.reg_name(k): fa.TIERS[t] for k, t in fa.tiered_live_in(af, SWITCH).items()}
        self.assertEqual(tiers, {"x0": "read", "x1": "read"})


class Calls(unittest.TestCase):
    def test_declared_regs(self):
        self.assertEqual(fa.declared_regs(fa.Declared(2, 1)), frozenset({0, 1, 100}))

    def test_dropped(self):
        held = frozenset({0, 1, 2, 100})
        # the caller declares this and an int; the callee reads x0 and x1 but declares only this
        self.assertEqual(fa.dropped(held, fa.Declared(2, 0), {0, 1}, fa.Declared(1, 0)), {1})
        # a register the caller does not declare is not its to pass on
        self.assertEqual(fa.dropped(held, fa.Declared(1, 0), {0, 1}, fa.Declared(1, 0)), set())
        # a float the callee reads beyond its declaration
        self.assertEqual(fa.dropped(held, fa.Declared(1, 1), {0, 100}, fa.Declared(1, 0)), {100})
        # not held untouched: the caller computes it (the call passes something else)
        self.assertEqual(fa.dropped(frozenset({0}), fa.Declared(2, 0), {0, 1}, fa.Declared(1, 0)), set())

    def test_casts_callee(self):
        text = "reinterpret_cast<void (*)(UnkFoo*, float)>(fn_7100001234)(self, dt);"
        self.assertTrue(fa.casts_callee(text, "fn_7100001234"))
        self.assertTrue(fa.casts_callee("reinterpret_cast<F>(&ns::fn_7100001234)(a);", "fn_7100001234"))
        self.assertFalse(fa.casts_callee(text, "fn_7100004321"))
        self.assertFalse(fa.casts_callee("fn_7100001234(self);", "fn_7100001234"))

    def test_bare_name(self):
        self.assertEqual(fa.bare_name("CInfester::fn_71005cb0b8(float) const"), "fn_71005cb0b8")


@unittest.skipIf(abi_facts is None, "capstone is not installed")
class CallSites(unittest.TestCase):
    def setUp(self):
        self.af = abi_facts.AbiFacts(FakeOriginal(CALL_FUNCTIONS), rows={}, graph=FakeGraph())

    def test_untouched_tail(self):
        self.assertEqual(self.af.untouched(FWD), {FWD: (BASE, abi_facts.ARGS - {8})})

    def test_untouched_call(self):
        t, regs = self.af.untouched(CALLS)[CALLS]
        self.assertEqual(t, BASE)
        self.assertIn(1, regs)

    def test_one_path_writes(self):
        t, regs = self.af.untouched(HALF)[HALF + 8]
        self.assertNotIn(1, regs)
        self.assertIn(0, regs)

    def test_callee_reads(self):
        self.assertEqual(fa.Analyzer(self.af).callee_args(BASE, fa.CALL, 0), {0, 1})

    def test_call_sites(self):
        import asmdiff
        relocs = {0: asmdiff.ObjReloc(asmdiff.JUMP26, "_Z13fn_7100010000P6UnkFoo", 0)}
        sites = fa.call_sites(self.af, FWD, relocs)
        self.assertEqual([(o, t, s) for o, t, s, _ in sites], [(0, BASE, "_Z13fn_7100010000P6UnkFoo")])
        held = sites[0][3]
        self.assertEqual(fa.dropped(held, fa.Declared(2, 0), {0, 1}, fa.Declared(1, 0)), {1})
        sets = fa.call_sites(self.af, SETS, {4: relocs[0]})
        self.assertEqual(fa.dropped(sets[0][3], fa.Declared(2, 0), {0, 1}, fa.Declared(1, 0)), set())

    def test_data_relocs_ignored(self):
        import asmdiff
        relocs = {0: asmdiff.ObjReloc(asmdiff.GOT_PAGE, "_ZN3Foo3barE", 0)}
        self.assertEqual(fa.call_sites(self.af, FWD, relocs), [])


if __name__ == "__main__":
    unittest.main()
