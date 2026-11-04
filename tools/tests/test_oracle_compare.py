"""pcode_emu's argument comparison: s<n> arguments compared on 32 bits, caller-frame addresses."""

import types
import unittest

from tools import pcode_emu
from tools.pcode_emu import ENTRY_SP, Q0, QSTRIDE, STACK_TOP, same_arg


class _Stop(Exception):
    pass


def machine() -> pcode_emu.Machine:
    """A Machine with only the state set_reg and call's argument selection touch."""
    m = pcode_emu.Machine.__new__(pcode_emu.Machine)
    m.regs, m.undef_vn, m.state, m.lift, m.vnarrow = bytearray(0x10000), set(), {}, None, {}
    m.vnarrow_at, m.steps = {}, 0
    m.target_reg = None
    m._loaded, m._all_loads = None, set()  # (no call event yet: load)
    m.entry_regs, m.save_slots, m.save_src = {}, {}, {}
    return m


def selection(m: pcode_emu.Machine, facts) -> dict:
    """The (register -> bits) selection call() records for a callee with these facts."""
    m.canonical = lambda target: target
    m.recorded = lambda target, t, modeled: None
    m.facts = lambda target: facts

    def checkpoint(event):
        raise _Stop(event)

    m.checkpoint = checkpoint
    try:
        m.call(0x7100001000)
    except _Stop as stop:
        return dict(stop.args[0][2])
    raise AssertionError("call() did not reach its checkpoint")


# A callee that reads v0 whole (d0 or wider), as the indirect-call copy does
WIDE_V0 = types.SimpleNamespace(args=frozenset({100}), wide=frozenset({100}), stack_slots=0)


class VNarrow(unittest.TestCase):
    """set_reg records whether v<n>'s last write was s<n> (4 bytes at its start)."""

    def test_s_write_sets(self):
        m = machine()
        m.set_reg(Q0 + QSTRIDE * 2, 4, 0x3F800000)
        self.assertTrue(m.vnarrow[2])

    def test_upper_zeroing_keeps(self):
        """The zeroing of the upper part after an s<n> write does not clear it."""
        m = machine()
        m.set_reg(Q0, 4, 0x3F800000)
        m.set_reg(Q0 + 4, 12, 0)
        self.assertTrue(m.vnarrow[0])

    def test_upper_zeroing_in_pieces_keeps(self):
        """Ghidra's P-code for `fadd s0, ...` zeroes the rest in four pieces: 4 bytes at +4, then 8 at
        +8, +16 and +24 (the SVE part of z<n>)."""
        m = machine()
        m.set_reg(Q0, 4, 0x3F800000)
        for rel, size in ((4, 4), (8, 8), (16, 8), (24, 8)):
            m.set_reg(Q0 + rel, size, 0)
        self.assertTrue(m.vnarrow[0])

    def test_d_write_clears(self):
        m = machine()
        m.set_reg(Q0 + QSTRIDE, 4, 1)
        m.set_reg(Q0 + QSTRIDE, 8, 1)
        self.assertFalse(m.vnarrow[1])

    def test_lane_insert_clears(self):
        """A lane insert after an s<n> write (ld1 {v0.s}[1], ins v0.d[1], x8) makes v<n> wide."""
        for rel, size in ((4, 4), (8, 8), (4, 12)):
            m = machine()
            m.set_reg(Q0 + QSTRIDE, 4, 1)
            m.set_reg(Q0 + QSTRIDE + 4, 12, 0)
            m.steps += 1
            m.set_reg(Q0 + QSTRIDE + rel, size, 5)
            self.assertFalse(m.vnarrow[1], (rel, size))
            self.assertEqual(selection(m, types.SimpleNamespace(args=frozenset({101}), wide=frozenset({101}),
                                                               stack_slots=0))[101], 64)

    def test_later_zero_insert_clears(self):
        """Only the s<n> write's own instruction may zero the upper part and keep v<n> narrow: a later
        instruction writing 0 there (ins v1.s[1], wzr) is a lane insert, and bits 32-63 are compared."""
        for rel, size in ((4, 4), (8, 8), (16, 8), (24, 8), (4, 12)):
            m = machine()
            m.set_reg(Q0 + QSTRIDE, 4, 1)
            for r, s in ((4, 4), (8, 8), (16, 8), (24, 8)):
                m.set_reg(Q0 + QSTRIDE + r, s, 0)
            self.assertTrue(m.vnarrow[1])
            m.steps += 1
            m.set_reg(Q0 + QSTRIDE + rel, size, 0)
            self.assertFalse(m.vnarrow[1], (rel, size))
            self.assertEqual(selection(m, types.SimpleNamespace(args=frozenset({101}), wide=frozenset({101}),
                                                               stack_slots=0))[101], 64)

    def test_d_upper_zeroing_keeps_wide(self):
        m = machine()
        m.set_reg(Q0, 8, 1)
        m.set_reg(Q0 + 8, 8, 0)
        self.assertFalse(m.vnarrow[0])

    def test_q_write_clears(self):
        m = machine()
        m.set_reg(Q0 + QSTRIDE, 4, 1)
        m.set_reg(Q0 + QSTRIDE, 16, 1)
        self.assertFalse(m.vnarrow[1])


class FloatSelection(unittest.TestCase):
    """call() compares a float argument last written as s<n> on 32 bits (7100656f3c)."""

    def test_s_written_is_32_bits(self):
        """Even when the callee reads the register whole (7100656f3c)."""
        m = machine()
        m.set_reg(Q0, 4, 0x3F800000)
        self.assertEqual(selection(m, WIDE_V0)[100], 32)

    def test_d_written_is_64_bits(self):
        m = machine()
        m.set_reg(Q0, 8, 0x3FF0000000000000)
        self.assertEqual(selection(m, WIDE_V0)[100], 64)

    def test_narrow_callee_read_is_32_bits(self):
        """A callee reading only s0 gets 32 bits whatever the last write was."""
        m = machine()
        m.set_reg(Q0, 8, 0x3FF0000000000000)
        facts = types.SimpleNamespace(args=frozenset({100}), wide=frozenset(), stack_slots=0)
        self.assertEqual(selection(m, facts)[100], 32)

    D0 = 0x40468D23FD1BC240

    @staticmethod
    def vcopy(m, d: int, n: int, word=None) -> None:
        """m runs `mov v<d>.16b, v<n>.16b` (an orr, as Ghidra's P-code has it, then the SVE part zeroed)."""
        w = word if word is not None else 0x4EA01C00 | n << 16 | n << 5 | d
        m.ops_db = {w: [(9101, (2, Q0 + QSTRIDE * d, 16), [(2, Q0 + QSTRIDE * n, 16), (2, Q0 + QSTRIDE * n, 16)]),
                        (9102, (2, Q0 + QSTRIDE * d + 16, 16), [(0, 0, 16)])]}
        m.uniq = {}
        with _opcodes({9101: "INT_OR", 9102: "COPY"}):
            m.step(0x1000, w, 1, 2, 3, 4, 5, 6)

    def entry(self, own_args, d0=None):
        """A Machine at the entry with d0 as v0 (a double's or an f32's bits), own_args its arguments."""
        d0 = self.D0 if d0 is None else d0
        m = machine()
        m.own_facts = types.SimpleNamespace(args=frozenset(own_args), wide=frozenset(own_args))
        m.sreads, m.vwide, m.vtag, m.entry_v = set(), set(), {i: i for i in range(8)}, {0: d0}
        m.regs[Q0:Q0 + 16] = d0.to_bytes(16, "little")
        return m

    def entry_copy(self, own_args, read_as_s: bool) -> int:
        """The bits selected for v0 written whole with the entry's d0 (`mov v0.16b, v8.16b` of a
        `mov v8.16b, v0.16b`), the function's own arguments being own_args."""
        m = self.entry(own_args)
        self.vcopy(m, 8, 0)
        if read_as_s:
            m.read((2, Q0 + QSTRIDE * 8, 4))  # (fmul s1, s1, s8)
        self.vcopy(m, 0, 8)
        return selection(m, WIDE_V0)[100]

    def test_entry_s0_read_then_copied(self):
        """s0 read where it came in (fcmp s0), then passed on through v8: an f32 too."""
        m = self.entry({100})
        m.read((2, Q0, 4))
        self.vcopy(m, 8, 0)
        self.vcopy(m, 0, 8)
        self.assertEqual(selection(m, WIDE_V0)[100], 32)

    def test_fmov_d_copy_carries_the_argument(self):
        m = self.entry({100})
        m.read((2, Q0, 4))
        self.vcopy(m, 8, 0, word=0x1E604000 | 0 << 5 | 8)  # fmov d8, d0
        self.vcopy(m, 0, 8, word=0x1E604000 | 8 << 5 | 0)
        self.assertEqual(selection(m, WIDE_V0)[100], 32)

    def test_double_with_an_unrelated_s_read_of_the_same_low_word_is_64_bits(self):
        """A double 1.0 (low word 0) passed on whole to a callee reading d0, after an s read of an
        unrelated zero register: only the argument's own s read counts, not a value."""
        m = self.entry({100}, d0=0x3FF0000000000000)
        self.vcopy(m, 8, 0)
        m.read((2, Q0 + QSTRIDE, 4))  # (fcmp s1, #0.0 on a zeroed v1)
        self.vcopy(m, 0, 8)
        self.assertEqual(selection(m, WIDE_V0)[100], 64)

    def test_vector_lane_read_is_64_bits(self):
        """A float32x2 d0: lane 0 read as s0, lane 1 read too (mov s1, v0.s[1]): not an f32."""
        m = self.entry({100})
        m.read((2, Q0, 4))
        m.read((2, Q0 + 4, 4))
        self.vcopy(m, 8, 0)
        self.vcopy(m, 0, 8)
        self.assertEqual(selection(m, WIDE_V0)[100], 64)

    def test_d_read_is_64_bits(self):
        """Read as s0 and as d0 (fmov x1, d0; a double's use): compared whole."""
        m = self.entry({100})
        self.vcopy(m, 8, 0)
        m.read((2, Q0 + QSTRIDE * 8, 4))
        m.read((2, Q0 + QSTRIDE * 8, 8))
        self.vcopy(m, 0, 8)
        self.assertEqual(selection(m, WIDE_V0)[100], 64)

    def test_other_write_ends_the_copy(self):
        """v8 rewritten with the same bits by other than a whole copy (a load, an arithmetic op): no
        longer the argument's copy."""
        m = self.entry({100})
        m.read((2, Q0, 4))
        m.set_reg(Q0 + QSTRIDE * 8, 16, self.D0)
        self.vcopy(m, 0, 8)
        self.assertEqual(selection(m, WIDE_V0)[100], 64)

    def test_whole_copy_of_an_f32_argument_is_32_bits(self):
        """The function's own s0, read as s8 and passed on whole through v8: the caller's bits above it
        are unspecified (71010df1fc)."""
        self.assertEqual(self.entry_copy({0, 100}, True), 32)

    def test_whole_copy_never_read_as_s_is_64_bits(self):
        """Passed on whole without being read as an f32: it may be a double, compared whole."""
        self.assertEqual(self.entry_copy({0, 100}, False), 64)

    def test_whole_copy_of_a_non_argument_is_64_bits(self):
        """v0 at the entry when it is not one of the function's arguments: compared whole."""
        self.assertEqual(self.entry_copy({0}, True), 64)


class BothFault(unittest.TestCase):
    """compare: two runs that crash after the same calls and writes agree, whatever the registers a
    callee is not passed hold (71004720d8)."""

    @staticmethod
    def made(v0: int, x0: int = 1, end=("end", "fault"), fault=0x10, access=("load", 8), tail=(), calls=True):
        call = ("call", 0x7100001000, ((0, 64),), ((0, x0), (100, v0)))
        head = [(call, ((0x7200000000, 5),))] if calls else []
        return types.SimpleNamespace(events=head + [(end, tail)], undef_mem=set(), clipped=False, arg_slots=0,
                                     fault=fault if end == ("end", "fault") else None,
                                     fault_access=access if end == ("end", "fault") else None)

    def test_unpassed_register_differs(self):
        self.assertEqual(pcode_emu.compare(self.made(0x3F800000), self.made(0), None), "")

    def test_passed_register_differs(self):
        self.assertEqual(pcode_emu.compare(self.made(0, x0=1), self.made(0, x0=2), None),
                         "event 0 before the original's fault")

    def test_lift_returns(self):
        self.assertTrue(pcode_emu.compare(self.made(0x3F800000), self.made(0, end=("end", "return")),
                                          None).startswith("skip"))

    def test_different_fault_address_decides_nothing(self):
        """The lift crashing on another access after the same calls never reached the original's
        stores and result past it: not an agreement."""
        self.assertTrue(pcode_emu.compare(self.made(0), self.made(0, fault=0xFF00), "x32").startswith("skip"))
        self.assertTrue(pcode_emu.compare(self.made(0x3F800000), self.made(0, fault=0xFF00), "x32").startswith("skip"))

    def test_no_calls_any_crash_is_not_any_crash(self):
        a, b = self.made(0, calls=False), self.made(0, calls=False, fault=0x18)
        self.assertTrue(pcode_emu.compare(a, b, "x32").startswith("skip"))

    def test_different_access_decides_nothing(self):
        a, b = self.made(0), self.made(0, access=("store", 8))
        self.assertTrue(pcode_emu.compare(a, b, None).startswith("skip"))
        b = self.made(0, access=("load", 4))
        self.assertTrue(pcode_emu.compare(a, b, None).startswith("skip"))

    def test_writes_after_the_last_call_are_compared(self):
        """*p = 5 after the call, then the crash: a lift storing 6 there, or nothing, does not agree."""
        a = self.made(0, tail=((0x7200000010, 5),))
        self.assertEqual(pcode_emu.compare(a, self.made(0, tail=((0x7200000010, 5),)), None), "")
        self.assertTrue(pcode_emu.compare(a, self.made(0, tail=((0x7200000010, 6),)), None).startswith("skip"))
        self.assertTrue(pcode_emu.compare(a, self.made(0), None).startswith("skip"))


class FaultEvent(unittest.TestCase):
    """Machine.run: a fault's end event holds the bytes written since the last call, and the run
    records the faulting access."""

    HEAP = 0x7200000000
    STORE5, LOADQ = 0x8B000001, 0x8B000002  # (words for the hand-made P-code below)

    def test_fault_records_the_writes_and_the_access(self):
        codes = {"BRANCH": 1, "CBRANCH": 2, "BRANCHIND": 3, "CALL": 4, "CALLIND": 5, "RETURN": 6,
                 "STORE": 9001, "LOAD": 9002}
        ops = {self.STORE5: [(9001, None, [(0, 0, 8), (0, self.HEAP, 8), (0, 5, 4)])],
               self.LOADQ: [(9002, (2, pcode_emu.X0, 4), [(0, 0, 8), (0, 0x18, 8)])]}
        db = {"ops": ops, "userops": {}, "opcodes": codes}
        image = types.SimpleNamespace(end=pcode_emu.BASE, bss_end=pcode_emu.BASE,
                                      text=(pcode_emu.BASE, pcode_emu.BASE), data=b"")
        words = {0x1000: self.STORE5, 0x1004: self.LOADQ}
        pcode_emu.set_opcodes(codes)
        try:
            m = pcode_emu.Machine(db, image, 2, lambda a: a in words, lambda a: words[a], None,
                                  lambda t: None, lambda t: None, {})
            m.setup()
            m.run(0x1000)
        finally:
            pcode_emu.OPNAMES.clear()
        self.assertEqual(m.events[-1][0], ("end", "fault"))
        self.assertEqual(dict(m.events[-1][1]), {self.HEAP + i: b for i, b in enumerate((5, 0, 0, 0))})
        self.assertEqual((m.fault, m.fault_access), (0x18, ("load", 4)))
        self.assertIsNone(m.cut)


class CallerFrame(unittest.TestCase):
    """same_arg: two addresses above the entry sp agree (71002842e4)."""

    def test_two_caller_frame_addresses_agree(self):
        """Different values, both in the caller's frame (71002842e4)."""
        self.assertTrue(same_arg(ENTRY_SP + 8, ENTRY_SP + 0x40))

    def test_one_below_entry_sp_differs(self):
        self.assertFalse(same_arg(ENTRY_SP - 8, ENTRY_SP + 8))

    def test_stack_top_is_outside(self):
        self.assertFalse(same_arg(STACK_TOP, ENTRY_SP + 8))

    def test_different_plain_values_differ(self):
        self.assertFalse(same_arg(1, 2))
        self.assertTrue(same_arg(7, 7))

    def test_stack_argument_slots_compare_exactly(self):
        """&stack_arg10 for &stack_arg9 is a different argument, not two caller-frame addresses."""
        frame = ENTRY_SP + 8 * 2
        self.assertFalse(same_arg(ENTRY_SP, ENTRY_SP + 8, frame))
        self.assertFalse(same_arg(ENTRY_SP + 8, ENTRY_SP + 0x40, frame))
        self.assertTrue(same_arg(ENTRY_SP + 8, ENTRY_SP + 8, frame))
        self.assertTrue(same_arg(frame, ENTRY_SP + 0x40, frame))

    def test_compare_bounds_by_the_larger_arg_slots(self):
        """compare passes ENTRY_SP + 8 * max(arg_slots): a call given the next stack slot differs."""
        def run(x0, slots):
            call = ("call", 0x7100001570, ((0, 64),), ((0, x0),))
            return types.SimpleNamespace(events=[(call, ()), (("end", "return"), ())], undef_mem=set(),
                                         clipped=False, cut=None, steps=1, scale=1, arg_slots=slots)
        self.assertTrue(pcode_emu.compare(run(ENTRY_SP, 2), run(ENTRY_SP + 8, 0), None).startswith("call 0"))
        self.assertEqual(pcode_emu.compare(run(ENTRY_SP + 0x10, 2), run(ENTRY_SP + 0x40, 0), None), "")
        self.assertEqual(pcode_emu.compare(run(ENTRY_SP, None), run(ENTRY_SP + 8, None), None), "")


class X8Window(unittest.TestCase):
    """call: an x8 result's window past the guide's extent is undefined only up to this frame's
    next object (taken address, save slot, frame record, another argument's out-parameter)."""

    OBJ = ENTRY_SP - 0x80  # x8's result; the original's frame put another local at +8 (the guide's extent)

    def machine(self, taken=(), saves=(), frame_record=(), out_at=None, own=()):
        m = machine()
        page = pcode_emu.PAGE
        m.pages = {n: bytearray(page) for n in range(self.OBJ // page, ENTRY_SP // page + 1)}  # (the frame's)
        m.dirty, m.track, m.arg_slots, m.entry_saved = {}, False, None, set()
        m.stack_written, m.taken = set(range(self.OBJ, self.OBJ + 0x40)), {self.OBJ, *taken}
        m.saves, m.frame_record, m.out_at = set(saves), set(frame_record), dict(out_at or {})
        m.own_stack = set(own)
        m.seed, m.overrides, m.dictionary, m.calls, m.call_log, m.out_words = 0, {}, None, 0, [], []
        m.results = []
        m.guide = [(frozenset({8}), {8: 8}, {})]  # (the lift's run: the original passed x8, extent 8)
        m.canonical = lambda target: target
        m.recorded = lambda target, t, modeled: None
        m.facts = lambda target: None
        m.checkpoint = lambda event: None
        m.set_reg(pcode_emu.X0 + 64, 8, self.OBJ)
        m.call(0x7100001000)
        return m

    def kept(self, m, lo, hi):
        return all(self.OBJ + i in m.stack_written for i in range(lo, hi))

    def cleared(self, m, lo, hi):
        return not any(self.OBJ + i in m.stack_written for i in range(lo, hi))

    def test_untaken_window_is_cleared(self):
        """Nothing else here: past the extent to the window's end is undefined (7100128810)."""
        m = self.machine()
        self.assertTrue(self.cleared(m, 8, 0x40))

    def test_taken_local_is_kept(self):
        """A local whose address this run took at +0x10 keeps what it holds: a lift's different
        store to it, copied out later, is compared."""
        m = self.machine(taken={self.OBJ + 0x10})
        self.assertTrue(self.cleared(m, 8, 0x10))
        self.assertTrue(self.kept(m, 0x10, 0x40))

    def test_save_slot_and_frame_record_are_kept(self):
        self.assertTrue(self.kept(self.machine(saves={self.OBJ + 0x18}), 0x18, 0x40))
        self.assertTrue(self.kept(self.machine(frame_record={self.OBJ + 0x20}), 0x20, 0x40))

    def test_another_arguments_out_parameter_is_kept(self):
        """Bytes an earlier call filled through another argument belong to that object."""
        m = self.machine(out_at={self.OBJ + 0x18 + i: self.OBJ + 0x18 for i in range(8)})
        self.assertTrue(self.cleared(m, 8, 0x18))
        self.assertTrue(self.kept(m, 0x18, 0x20))

    def test_own_store_is_kept(self):
        """A local this run's code stored at +0x18 before the call, its address taken only after
        (710082a500's float, passed by pointer to the next callee), keeps what it holds."""
        m = self.machine(own={self.OBJ + 0x18 + i for i in range(4)})
        self.assertTrue(self.cleared(m, 8, 0x18))
        self.assertTrue(self.kept(m, 0x18, 0x40))

    def test_own_store_under_this_results_earlier_fill_is_kept(self):
        """A byte an earlier call filled through x8 and the code then overwrote is its own."""
        m = machine()
        m.out_at = {self.OBJ + 0x10 + i: self.OBJ for i in range(8)}
        m.own_stack, m.uniq, m.res_read, m._touched = set(), {}, None, set()
        m.pages = {n: bytearray(pcode_emu.PAGE) for n in range(self.OBJ // pcode_emu.PAGE, ENTRY_SP // pcode_emu.PAGE + 1)}
        m.dirty, m.track, m.arg_slots, m.stack_written, m.saves, m.frame_record = {}, False, None, set(), set(), set()
        with _opcodes({9001: "STORE"}):
            m.exec(9001, None, [(0, 0, 8), (0, self.OBJ + 0x10, 8), (0, 5, 4)])
        self.assertNotIn(self.OBJ + 0x10, m.out_at)
        self.assertIn(self.OBJ + 0x10, m.own_stack)


class _opcodes:
    """OPNAMES entries for a test's exec calls (the table is filled from the P-code pickle)."""

    def __init__(self, names):
        self.names = names

    def __enter__(self):
        pcode_emu.OPNAMES.update(self.names)

    def __exit__(self, *exc):
        for k in self.names:
            pcode_emu.OPNAMES.pop(k, None)


class SaveSlotLoad(unittest.TestCase):
    """exec LOAD: bytes of a save slot still holding a callee-saved register's entry value are
    undefined data (7100128810: a made-up count copies an entry past a callee-filled table, over
    the original's saves, out to memory; the lift's frame holds its own locals there)."""

    SLOT, DST, ENTRY = ENTRY_SP - 0x20, ENTRY_SP - 0x60, 0x534EC1007DEC8026

    X19 = pcode_emu.X0 + 8 * 19

    def fresh(self, entry=None):
        m = machine()
        page = pcode_emu.PAGE
        entry = self.ENTRY if entry is None else entry
        m.pages = {n: bytearray(page) for n in range(self.DST // page, ENTRY_SP // page + 1)}
        m.dirty, m.track, m.arg_slots, m.uniq, m.res_read, m._touched = {}, False, None, {}, None, set()
        m.stack_written, m.frame_record, m.out_at, m.own_stack = set(), set(), {}, set()
        m.entry_saved, m.saves, m.undef_mem = {entry}, set(), set()
        m.entry_regs, m.save_slots, m.save_src = {self.X19: entry}, {}, {}
        m.regs[self.X19:self.X19 + 8] = entry.to_bytes(8, "little")
        return m

    def copy_out(self, m, src):
        """LOAD the word at src, STORE it to DST."""
        with _opcodes({9001: "STORE", 9002: "LOAD"}):
            m.exec(9002, (1, 0x100, 8), [(0, 0, 8), (0, src, 8)])
            m.exec(9001, None, [(0, 0, 8), (0, self.DST, 8), (1, 0x100, 8)])
        return m

    def run_load(self, held, via_unique=False):
        m = self.fresh()
        with _opcodes({9001: "STORE", 9003: "COPY"}):  # (the prologue's save of x19)
            if via_unique:
                m.exec(9003, (1, 0x200, 8), [(2, self.X19, 8)])
                m.exec(9001, None, [(0, 0, 8), (0, self.SLOT, 8), (1, 0x200, 8)])
            else:
                m.exec(9001, None, [(0, 0, 8), (0, self.SLOT, 8), (2, self.X19, 8)])
        m.store(self.SLOT, 8, held)
        return self.copy_out(m, self.SLOT)

    def test_entry_value_is_undefined(self):
        m = self.run_load(self.ENTRY)
        self.assertEqual(m.undef_mem, set(range(self.DST, self.DST + 8)))

    def test_save_through_a_unique_is_a_slot(self):
        """Ghidra's stp may copy the register to a unique first: still the register's save."""
        m = self.run_load(self.ENTRY, via_unique=True)
        self.assertEqual(m.undef_mem, set(range(self.DST, self.DST + 8)))

    def test_overwritten_slot_is_data(self):
        """The slot reused for a local after the restore: what it holds now is compared."""
        self.assertEqual(self.run_load(7).undef_mem, set())

    def test_local_equal_to_an_entry_value_is_data(self):
        """A snapshot run's x19 may be 0 at entry: a zeroed local (`T tmp{}`, a constant store, not
        a save of x19) copied out is compared, not taken for a save slot."""
        local = ENTRY_SP - 0x40
        m = self.fresh(entry=0)
        with _opcodes({9001: "STORE"}):
            m.exec(9001, None, [(0, 0, 8), (0, local, 8), (0, 0, 8)])
        self.assertEqual(self.copy_out(m, local).undef_mem, set())

    def test_lift_reading_its_own_save_is_data(self):
        """The lift's run: a load from its own save slot is the lift's frame bug, not the
        original's undefined index; its store is compared (compare() skips only the original's)."""
        m = self.fresh()
        m.lift = object()
        with _opcodes({9001: "STORE"}):
            m.exec(9001, None, [(0, 0, 8), (0, self.SLOT, 8), (2, self.X19, 8)])
        self.assertEqual(self.copy_out(m, self.SLOT).undef_mem, set())


class WrapperX8(unittest.TestCase):
    """call: an indirect callee (no facts) is passed x8 as the function was given it when the
    function's own callers set x8 (its Facts.fwd holds 8, a virtual-call wrapper's indirect result):
    selected to be compared, never written through or taken as the call's result kind."""

    X8 = pcode_emu.ARENA + 0x900100  # (Machine.setup's x8 object: not in the stack)

    def machine(self, fwd, x8_state="entry"):
        m = machine()
        m.own_facts = types.SimpleNamespace(fwd=frozenset(fwd), args=frozenset({0} | (set(fwd) & {8})))
        m.set_reg(pcode_emu.X0 + 64, 8, self.X8)
        m.state = {0: "entry", 8: x8_state}  # (after set_reg, which marks a write)
        return m

    def test_fwd_x8_entry_is_selected(self):
        self.assertEqual(selection(self.machine({0, 8}), None).get(8), 64)

    def test_without_fwd_x8_entry_is_not(self):
        self.assertNotIn(8, selection(self.machine({0}), None))

    def test_no_own_facts_is_not(self):
        m = self.machine({0, 8})
        m.own_facts = None
        self.assertNotIn(8, selection(m, None))

    def test_x8_after_a_call_is_not(self):
        """x8 a call left (junk) is not the function's incoming result address."""
        self.assertNotIn(8, selection(self.machine({0, 8}, "junk"), None))

    def test_known_callee_keeps_its_args(self):
        """A callee with facts: exactly the registers it reads, fwd or not."""
        facts = types.SimpleNamespace(args=frozenset({0}), wide=frozenset({0}), stack_slots=0)
        self.assertNotIn(8, selection(self.machine({0, 8}), facts))

    def test_compared_only(self):
        """The call's log (the lift's guide), out-parameters and res_sret stay as without x8."""
        logs = []
        for fwd in ({0, 8}, {0}):
            m = self.machine(fwd)
            m.seed, m.overrides, m.dictionary, m.calls, m.call_log, m.out_words = 0, {}, None, 0, [], []
            m.results, m.guide, m.frame_record, m.out_at = [], None, set(), {}
            m.canonical = lambda target: target
            m.recorded = lambda target, t, modeled: None
            m.facts = lambda target: None
            events = []
            m.checkpoint = events.append
            m.stored = {}
            m.store = lambda a, size, v, track=True, m=m: m.stored.__setitem__(a, v)
            m.call(0x7100001000)
            self.assertFalse(m.res_sret)
            self.assertEqual(m.stored, {})
            logs.append(m.call_log)
            self.assertEqual(8 in dict(events[0][2]), 8 in fwd)
        self.assertEqual(logs[0], logs[1])

    def test_lift_losing_x8_fails(self):
        """The original passed x8 on; a lift whose x8 differs at the call is a different program."""
        def run(x8, sel):
            call = ("call", 0x7100001570, sel, ((0, 1), (8, x8)))
            return types.SimpleNamespace(events=[(call, ()), (("end", "return"), ())], undef_mem=set(),
                                         clipped=False, cut=None, steps=1, scale=1, arg_slots=0)
        orig = run(self.X8, ((0, 64), (8, 64)))
        self.assertIn("argument x8", pcode_emu.compare(orig, run(0x1234, ((0, 64),)), None))
        self.assertEqual(pcode_emu.compare(orig, run(self.X8, ((0, 64),)), None), "")


if __name__ == "__main__":
    unittest.main()
