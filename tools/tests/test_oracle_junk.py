"""pcode_emu's junk tracking (Machine.junk_scratch, port_lift --junk-scratch): after a call, x9-x18,
v16-v31 and the upper halves of v8-v15 are undefined (AAPCS64); a compared argument, store, branch or
result the original computes from them makes the input a skip, never an agreement or a rejection;
the lift's run compares as without tracking. Off, nothing is tracked. No game data: hand-built Machines and P-code ops."""

import types
import unittest

from tools import pcode_emu
from tools.pcode_emu import BASE, ENTRY_SP, Q0, QSTRIDE, X0

HEAP = 0x7200000000  # outside the image and the stack: a compared write
LOCAL = ENTRY_SP - 0x40  # in the function's own frame
CALLEE = 0x7100001000

# P-code opcodes for the tests' exec calls (the real table comes from the P-code pickle)
OPS = {9001: "STORE", 9002: "LOAD", 9003: "COPY", 9004: "INT_ADD", 9005: "SUBPIECE", 9006: "INT_ZEXT",
       9007: "PIECE", 9008: "INT_OR"}
STORE, LOAD, COPY, ADD, SUBPIECE, ZEXT, PIECE, OR = 9001, 9002, 9003, 9004, 9005, 9006, 9007, 9008


def xr(n: int, size: int = 8):
    return (2, X0 + 8 * n, size)


def vr(n: int, size: int = 16, at: int = 0):
    return (2, Q0 + QSTRIDE * n + at, size)


def const(v: int, size: int = 8):
    return (0, v, size)


class _opcodes:
    def __enter__(self):
        pcode_emu.OPNAMES.update(OPS)

    def __exit__(self, *exc):
        for k in OPS:
            pcode_emu.OPNAMES.pop(k, None)


def machine(junk: bool = True, lift=None, args=frozenset({0}), imports=None) -> pcode_emu.Machine:
    """A real Machine (no P-code database, no image): every callee reads `args` (abi_facts)."""
    db = {"ops": {}, "userops": {}, "opcodes": {}}
    image = types.SimpleNamespace(end=BASE, bss_end=BASE, text=(BASE, BASE), data=b"")
    facts = types.SimpleNamespace(args=args, wide=args, stack_slots=0)
    m = pcode_emu.Machine(db, image, 2, lambda a: False, lambda a: 0, lift, imports or (lambda t: None),
                          lambda t: facts, {})
    m.setup()
    if junk:
        m.junk_scratch = True  # (the instance's own: port_lift sets the class attribute)
    return m


def run(m, *ops):
    with _opcodes():
        for code, out, ins in ops:
            m.exec(code, out, ins)


def end(m):
    m.finish("return")
    return m


class Marking(unittest.TestCase):
    def test_off_tracks_nothing(self):
        m = machine(junk=False)
        m.call(CALLEE)
        run(m, (COPY, xr(0), [xr(14)]), (STORE, None, [const(0), const(HEAP), xr(0)]))
        m.call(CALLEE)
        self.assertEqual((m.junk_vn, m.junk_mem, m.junk_at), (set(), set(), None))

    def test_a_call_leaves_scratch_undefined(self):
        m = machine()
        self.assertFalse(m.junk_reg(X0 + 8 * 14, 8))  # (the entry value is the caller's: defined)
        m.call(CALLEE)
        for n in range(9, 19):
            self.assertTrue(m.junk_reg(X0 + 8 * n, 8), n)
        for n in list(range(0, 9)) + list(range(19, 31)):
            self.assertFalse(m.junk_reg(X0 + 8 * n, 8), n)
        self.assertTrue(m.junk_reg(Q0 + QSTRIDE * 16, 16))
        self.assertTrue(m.junk_reg(Q0 + QSTRIDE * 31, 16))
        self.assertFalse(m.junk_reg(Q0, 16))  # (v0-v7: a callee's results)
        self.assertFalse(m.junk_reg(Q0 + QSTRIDE * 8, 8))  # d8 is callee-saved
        self.assertTrue(m.junk_reg(Q0 + QSTRIDE * 8 + 8, 8))  # its upper half is not
        self.assertIsNone(m.junk_at)  # (undefined, but nothing used it)

    def test_a_write_defines_the_register(self):
        m = machine()
        m.call(CALLEE)
        run(m, (COPY, xr(14), [const(5)]))
        self.assertFalse(m.junk_reg(X0 + 8 * 14, 8))
        run(m, (COPY, xr(0), [xr(14)]))
        m.call(CALLEE)
        self.assertIsNone(m.junk_at)

    def test_a_w_write_defines_the_whole_register(self):
        m = machine()
        m.call(CALLEE)
        run(m, (COPY, xr(14, 4), [const(5, 4)]), (ZEXT, xr(14), [xr(14, 4)]))
        self.assertFalse(m.junk_reg(X0 + 8 * 14, 8))

    def test_modelled_memcpy_leaves_its_scratch_undefined(self):
        m = machine(imports=lambda t: "memcpy" if t == CALLEE else None)
        m.set_reg(X0, 8, LOCAL)
        m.set_reg(X0 + 8, 8, LOCAL - 0x20)
        m.set_reg(X0 + 16, 8, 8)
        m.call(CALLEE)
        self.assertFalse(m.junk_reg(X0, 8))  # (its result: the destination)
        self.assertTrue(m.junk_reg(X0 + 8, 8))
        self.assertTrue(m.junk_reg(X0 + 8 * 14, 8))
        self.assertTrue(m.junk_reg(Q0, 16))
        self.assertIsNone(m.junk_at)

    def test_modelled_math_keeps_its_result(self):
        m = machine(imports=lambda t: "sqrtf" if t == CALLEE else None)
        m.call(CALLEE)
        self.assertFalse(m.junk_reg(Q0, 4))
        self.assertTrue(m.junk_reg(Q0 + 4, 4))
        self.assertTrue(m.junk_reg(X0, 8))
        self.assertTrue(m.junk_reg(X0 + 8 * 9, 8))


class Carrying(unittest.TestCase):
    def setUp(self):
        self.m = machine()
        self.m.call(CALLEE)

    def test_arithmetic_carries(self):
        run(self.m, (ADD, (1, 0x100, 8), [xr(14), const(1)]), (COPY, xr(0), [(1, 0x100, 8)]))
        self.assertTrue(self.m.junk_reg(X0, 8))

    def test_parts_carry_byte_by_byte(self):
        # the low half of v8 (d8) is defined after a call, its upper half is not
        run(self.m, (SUBPIECE, xr(1), [vr(8), const(0, 4)]), (SUBPIECE, xr(2), [vr(8), const(8, 4)]))
        self.assertFalse(self.m.junk_reg(X0 + 8, 8))
        self.assertTrue(self.m.junk_reg(X0 + 16, 8))
        run(self.m, (PIECE, vr(0), [vr(8, 8, 8), vr(8, 8)]))  # (hi: v8's upper half, lo: d8)
        self.assertFalse(self.m.junk_reg(Q0, 8))
        self.assertTrue(self.m.junk_reg(Q0 + 8, 8))

    def test_bitwise_ops_carry_byte_by_byte(self):
        # mov v0.16b, v8.16b (an orr of v8 with itself): d0 defined, the upper half not
        run(self.m, (OR, vr(0), [vr(8), vr(8)]))
        self.assertFalse(self.m.junk_reg(Q0, 8))
        self.assertTrue(self.m.junk_reg(Q0 + 8, 8))

    def test_memcmp_of_undefined_bytes(self):
        m = machine(imports=lambda t: "memcmp" if t == 0x7100002000 else None)
        m.call(CALLEE)
        run(m, (STORE, None, [const(0), const(LOCAL), xr(14)]))
        m.set_reg(X0, 8, LOCAL)
        m.set_reg(X0 + 8, 8, LOCAL - 0x20)
        m.set_reg(X0 + 16, 8, 8)
        m.call(0x7100002000)
        self.assertEqual(m.junk_at, 1)

    def test_through_the_frame(self):
        run(self.m, (STORE, None, [const(0), const(LOCAL), xr(14)]))
        self.assertIsNone(self.m.junk_at)  # (a local: not compared)
        run(self.m, (LOAD, xr(3), [const(0), const(LOCAL)]))
        self.assertTrue(self.m.junk_reg(X0 + 24, 8))
        run(self.m, (STORE, None, [const(0), const(LOCAL), const(7)]), (LOAD, xr(3), [const(0), const(LOCAL)]))
        self.assertFalse(self.m.junk_reg(X0 + 24, 8))


class Uses(unittest.TestCase):
    def test_argument(self):
        m = machine()
        m.call(CALLEE)
        run(m, (COPY, xr(0), [xr(14)]))
        m.call(CALLEE)
        self.assertEqual(m.junk_at, 1)  # (the second call's event)
        self.assertIn("argument x0", m.junk_why)

    def test_unread_register_is_no_use(self):
        m = machine()
        m.call(CALLEE)
        run(m, (COPY, xr(1), [xr(14)]))  # (the callee reads x0 only)
        m.call(CALLEE)
        self.assertIsNone(m.junk_at)

    def test_heap_store(self):
        m = machine()
        m.call(CALLEE)
        run(m, (STORE, None, [const(0), const(HEAP), xr(14, 4)]))
        self.assertEqual(m.junk_at, 1)

    def test_store_address(self):
        m = machine()
        m.call(CALLEE)
        run(m, (STORE, None, [const(0), xr(14), const(0, 4)]))
        self.assertEqual(m.junk_at, 1)

    def test_branch(self):
        m = machine()
        m.call(CALLEE)
        op = {"BRANCH": 1, "CBRANCH": 2, "BRANCHIND": 3, "CALL": 4, "CALLIND": 5, "RETURN": 6}
        m.ops_db = {0x12345678: [(2, None, [const(8), xr(14, 1)])]}  # (a cbnz on w14's low byte)
        m.step(0x7100000000, 0x12345678, *op.values())
        self.assertEqual(m.junk_at, 1)
        self.assertIn("branch", m.junk_why)

    def test_branch_on_a_defined_value(self):
        m = machine()
        m.call(CALLEE)
        op = {"BRANCH": 1, "CBRANCH": 2, "BRANCHIND": 3, "CALL": 4, "CALLIND": 5, "RETURN": 6}
        m.ops_db = {0x12345678: [(2, None, [const(8), xr(19, 1)])]}
        m.step(0x7100000000, 0x12345678, *op.values())
        self.assertIsNone(m.junk_at)

    def test_memcpy_of_undefined_bytes_to_the_heap(self):
        m = machine(imports=lambda t: "memcpy" if t == 0x7100002000 else None)
        m.call(CALLEE)
        run(m, (STORE, None, [const(0), const(LOCAL), xr(14)]))
        m.set_reg(X0, 8, HEAP)
        m.set_reg(X0 + 8, 8, LOCAL)
        m.set_reg(X0 + 16, 8, 8)
        m.call(0x7100002000)
        self.assertEqual(m.junk_at, 1)


def pair(orig_ops, lift_ops, ret=None, junk=True):
    """Two runs: a call, then each side's ops, a second call, the end; compare()'s verdict."""
    runs = []
    for ops in (orig_ops, lift_ops):
        m = machine(junk=junk)
        m.call(CALLEE)
        run(m, *ops)
        m.call(CALLEE)
        runs.append(end(m))
    return pcode_emu.compare(runs[0], runs[1], ret)


class Compare(unittest.TestCase):
    # The original stores the stale x14 to the heap; the lift stores its own (another) value there.
    STALE = [(STORE, None, [const(0), const(HEAP), xr(14)])]
    OWN = [(COPY, xr(14), [const(5)]), (STORE, None, [const(0), const(HEAP), xr(14)])]

    def test_stale_store_is_a_skip(self):
        self.assertTrue(pair(self.STALE, self.OWN).startswith("skip: "), pair(self.STALE, self.OWN))

    def test_lift_only_stale_store_compares_as_before(self):
        """The lift relies on a stale register where the original does not: no skip, a difference."""
        self.assertEqual(pair(self.OWN, self.STALE), pair(self.OWN, self.STALE, junk=False))
        self.assertIn("memory at event 1", pair(self.OWN, self.STALE))

    def test_off_rejects_as_before(self):
        self.assertIn("memory at event 1", pair(self.STALE, self.OWN, junk=False))

    def test_both_stale_is_a_skip_not_an_agreement(self):
        self.assertTrue(pair(self.STALE, self.STALE).startswith("skip: "))
        self.assertEqual(pair(self.STALE, self.STALE, junk=False), "")

    def test_a_difference_before_the_use_still_rejects(self):
        orig, lift = machine(), machine()
        for m, v in ((orig, 1), (lift, 2)):
            run(m, (STORE, None, [const(0), const(HEAP + 8), const(v)]))
            m.call(CALLEE)
            run(m, *self.STALE)
            m.call(CALLEE)
            end(m)
        self.assertIn("memory at event 0", pcode_emu.compare(orig, lift, None))

    def test_defined_values_still_compare(self):
        own2 = [(COPY, xr(14), [const(6)]), (STORE, None, [const(0), const(HEAP), xr(14)])]
        self.assertIn("memory at event 1", pair(self.OWN, own2))
        self.assertEqual(pair(self.OWN, self.OWN), "")

    def test_stale_argument_is_a_skip(self):
        self.assertTrue(pair([(COPY, xr(0), [xr(14)])], [(COPY, xr(0), [const(3)])]).startswith("skip: "))

    def test_stale_result_is_a_skip(self):
        orig, lift = machine(), machine()
        for m in (orig, lift):
            m.call(CALLEE)
        run(orig, (COPY, xr(0), [xr(14)]))
        run(lift, (COPY, xr(0), [const(3)]))
        self.assertTrue(pcode_emu.compare(end(orig), end(lift), "x64").startswith("skip: "))
        off_o, off_l = machine(junk=False), machine(junk=False)
        for m in (off_o, off_l):
            m.call(CALLEE)
        run(off_o, (COPY, xr(0), [xr(14)]))
        run(off_l, (COPY, xr(0), [const(3)]))
        self.assertEqual(pcode_emu.compare(end(off_o), end(off_l), "x64"), "result x0")

    def test_lift_only_stale_result_compares_as_before(self):
        orig, lift = machine(), machine()
        for m in (orig, lift):
            m.call(CALLEE)
        run(orig, (COPY, xr(0), [const(3)]))
        run(lift, (COPY, xr(0), [xr(14)]))
        self.assertEqual(pcode_emu.compare(end(orig), end(lift), "x64"), "result x0")

    def test_bare_runs_compare_as_before(self):
        """compare() on runs without junk state (the tests' SimpleNamespace runs)."""
        def ns(v):
            return types.SimpleNamespace(events=[(("end", "return"), ((HEAP, v),))], undef_mem=set(),
                                         clipped=False, cut=None, steps=1, scale=1, arg_slots=0)
        self.assertEqual(pcode_emu.compare(ns(1), ns(1), None), "")
        self.assertIn("memory at event 0", pcode_emu.compare(ns(1), ns(2), None))


if __name__ == "__main__":
    unittest.main()
