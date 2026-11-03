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

    def test_lane_insert_clears(self):
        """A lane insert after an s<n> write (ld1 {v0.s}[1], ins v0.d[1], x8) makes v<n> wide."""
        for rel, size in ((4, 4), (8, 8), (4, 12)):
            m = machine()
            m.set_reg(Q0 + QSTRIDE, 4, 1)
            m.set_reg(Q0 + QSTRIDE + 4, 12, 0)
            m.set_reg(Q0 + QSTRIDE + rel, size, 5)
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


if __name__ == "__main__":
    unittest.main()
