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
    m.target_reg = None
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

    def test_d_write_clears(self):
        m = machine()
        m.set_reg(Q0 + QSTRIDE, 4, 1)
        m.set_reg(Q0 + QSTRIDE, 8, 1)
        self.assertFalse(m.vnarrow[1])

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


class X8Window(unittest.TestCase):
    """call: an x8 result's window past the guide's extent is undefined only up to this frame's
    next object (taken address, save slot, frame record, another argument's out-parameter)."""

    OBJ = ENTRY_SP - 0x80  # x8's result; the original's frame put another local at +8 (the guide's extent)

    def machine(self, taken=(), saves=(), frame_record=(), out_at=None):
        m = machine()
        page = pcode_emu.PAGE
        m.pages = {n: bytearray(page) for n in range(self.OBJ // page, ENTRY_SP // page + 1)}  # (the frame's)
        m.dirty, m.track, m.arg_slots, m.entry_saved = {}, False, None, set()
        m.stack_written, m.taken = set(range(self.OBJ, self.OBJ + 0x40)), {self.OBJ, *taken}
        m.saves, m.frame_record, m.out_at = set(saves), set(frame_record), dict(out_at or {})
        m.seed, m.overrides, m.dictionary, m.calls, m.call_log, m.out_words = 0, {}, None, 0, [], []
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


if __name__ == "__main__":
    unittest.main()
