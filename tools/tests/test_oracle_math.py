"""tools/pcode_emu.py's libm model: Machine.math and nextafterf, on a hand-built Machine (no game
data). Skipped when pyelftools is not installed."""

import math
import struct
import unittest

from tools.tests import _path  # noqa: F401

try:
    import pcode_emu
except ImportError:  # pyelftools missing
    pcode_emu = None


def pack(v: float, size: int) -> bytes:
    return struct.pack("<f" if size == 4 else "<d", v)


@unittest.skipIf(pcode_emu is None, "pyelftools not installed")
class MathCall(unittest.TestCase):
    """Machine.math: arguments in s0/s1 (d0/d1), the result in q0."""

    def machine(self, size: int, a: float, b: float = 0.0):
        m = pcode_emu.Machine.__new__(pcode_emu.Machine)
        m.regs, m.state, m.undef_vn = bytearray(pcode_emu.REG_SIZE), {}, set()
        q0, q1 = pcode_emu.Q0, pcode_emu.Q0 + pcode_emu.QSTRIDE
        m.regs[q0:q0 + size] = pack(a, size)
        m.regs[q1:q1 + size] = pack(b, size)
        return m

    def call(self, name: str, a: float, b: float = 0.0) -> float:
        size = 4 if name.endswith("f") else 8
        m = self.machine(size, a, b)
        m.math(name)
        return pcode_emu.f_from(m.reg(pcode_emu.Q0, size), size)

    def bits(self, name: str, a_bits: int, size: int) -> int:
        m = self.machine(size, 0.0)
        m.regs[pcode_emu.Q0:pcode_emu.Q0 + size] = a_bits.to_bytes(size, "little")
        m.math(name)
        return m.reg(pcode_emu.Q0, size)

    def test_double_variant(self):
        """sqrt reads d0 and writes d0."""
        self.assertEqual(self.call("sqrt", 9.0), 3.0)

    def test_float_variant_rounds_to_single(self):
        """sinf's result is the single-precision value of the double result."""
        want = struct.unpack("<f", struct.pack("<f", math.sin(1.0)))[0]
        self.assertEqual(self.call("sinf", 1.0), want)

    def test_two_arguments(self):
        """pow takes d0 and d1."""
        self.assertEqual(self.call("pow", 2.0, 10.0), 1024.0)
        self.assertEqual(self.call("atan2f", 1.0, 1.0), struct.unpack("<f", struct.pack("<f", math.pi / 4))[0])

    def test_result_fills_q0(self):
        """The whole 16-byte q0 is written: the bytes above the result are zero."""
        m = self.machine(4, 4.0)
        m.regs[pcode_emu.Q0 + 4:pcode_emu.Q0 + 16] = b"\xff" * 12
        m.math("sqrtf")
        self.assertEqual(m.reg(pcode_emu.Q0, 16), struct.unpack("<I", pack(2.0, 4))[0])

    def test_caller_saved_cleared(self):
        """x0-x18 are zeroed and the argument registers' states become junk."""
        m = self.machine(8, 1.0)
        for i in pcode_emu.CALLER_SAVED_X:
            m.set_reg(pcode_emu.X0 + 8 * i, 8, 0x1234 + i)
        m.math("cos")
        self.assertTrue(all(m.x(i) == 0 for i in pcode_emu.CALLER_SAVED_X))
        self.assertTrue(all(m.state[i] == "junk" for i in range(9)))
        self.assertTrue(all(m.state[100 + i] == "junk" for i in range(8)))

    def test_nan_argument_keeps_payload_quieted(self):
        """A signalling NaN comes back quiet with its payload (fn_7100118274)."""
        self.assertEqual(self.bits("sqrtf", 0x7F800123, 4), 0x7FC00123)
        self.assertEqual(self.bits("sqrt", 0x7FF0000000000042, 8), 0x7FF8000000000042)

    def test_nan_second_argument(self):
        """A NaN in d1 of a two-argument function is the one returned."""
        m = self.machine(4, 2.0)
        q1 = pcode_emu.Q0 + pcode_emu.QSTRIDE
        m.regs[q1:q1 + 4] = (0x7F800007).to_bytes(4, "little")
        m.math("powf")
        self.assertEqual(m.reg(pcode_emu.Q0, 4), 0x7FC00007)

    def test_rounding_functions_keep_infinity(self):
        """floor, ceil, trunc and round return +-inf unchanged."""
        for name in ("floor", "ceil", "trunc", "round", "floorf", "ceilf", "truncf", "roundf"):
            self.assertEqual(self.call(name, math.inf), math.inf, name)
            self.assertEqual(self.call(name, -math.inf), -math.inf, name)

    def test_rounding_values(self):
        """round goes half away from zero; floor and ceil as in C."""
        self.assertEqual(self.call("round", 2.5), 3.0)
        self.assertEqual(self.call("roundf", -2.5), -3.0)
        self.assertEqual(self.call("floor", -1.5), -2.0)
        self.assertEqual(self.call("ceilf", 1.25), 2.0)

    def test_log_of_zero(self):
        """log, log2 and log10 of 0 are -inf."""
        for name in ("log", "log2", "log10", "logf"):
            self.assertEqual(self.call(name, 0.0), -math.inf, name)

    def test_domain_error_is_nan(self):
        """sqrt of a negative number and log of one are NaN."""
        self.assertTrue(math.isnan(self.call("sqrt", -1.0)))
        self.assertTrue(math.isnan(self.call("logf", -1.0)))

    def test_overflow_is_inf(self):
        """exp beyond the double range is +inf; expf beyond the single range too."""
        self.assertEqual(self.call("exp", 1000.0), math.inf)
        self.assertEqual(self.call("expf", 100.0), math.inf)

    def test_nextafter_double(self):
        """nextafter uses math.nextafter."""
        self.assertEqual(self.call("nextafter", 1.0, 2.0), math.nextafter(1.0, 2.0))


@unittest.skipIf(pcode_emu is None, "pyelftools not installed")
class NextAfterF(unittest.TestCase):
    """nextafterf: one single-precision step from a towards b."""

    def step(self, a_bits: int, b: float) -> int:
        return pcode_emu.f_to(pcode_emu.nextafterf(pcode_emu.f_from(a_bits, 4), b), 4)

    def test_towards_larger(self):
        """A positive value steps up; a negative one steps towards zero."""
        self.assertEqual(self.step(0x3F800000, 2.0), 0x3F800001)
        self.assertEqual(self.step(0xBF800000, 0.0), 0xBF7FFFFF)

    def test_towards_smaller(self):
        """A positive value steps towards zero; a negative one away from it."""
        self.assertEqual(self.step(0x3F800000, 0.0), 0x3F7FFFFF)
        self.assertEqual(self.step(0xBF800000, -2.0), 0xBF800001)

    def test_from_zero(self):
        """0 steps to the smallest subnormal of b's sign."""
        self.assertEqual(self.step(0, 1.0), 1)
        self.assertEqual(self.step(0, -1.0), 0x80000001)

    def test_equal_returns_b(self):
        """a == b returns b (so nextafterf(0, -0) is -0)."""
        self.assertEqual(self.step(0x40400000, 3.0), 0x40400000)
        self.assertEqual(self.step(0, -0.0), 0x80000000)

    def test_largest_steps_to_inf(self):
        """The largest finite single steps to +inf."""
        self.assertEqual(self.step(0x7F7FFFFF, math.inf), 0x7F800000)

    def test_nan(self):
        """A NaN in either argument gives NaN."""
        self.assertTrue(math.isnan(pcode_emu.nextafterf(math.nan, 1.0)))
        self.assertTrue(math.isnan(pcode_emu.nextafterf(1.0, math.nan)))


@unittest.skipIf(pcode_emu is None, "pyelftools not installed")
class PureMath(unittest.TestCase):
    """PURE_MATH: the libm imports run as Machine.math."""

    def test_nextafter_listed(self):
        """nextafter and nextafterf are pure."""
        self.assertIn("nextafter", pcode_emu.PURE_MATH)
        self.assertIn("nextafterf", pcode_emu.PURE_MATH)

    def test_every_name_has_a_model(self):
        """Every PURE_MATH name runs through Machine.math without a KeyError."""
        for name in sorted(pcode_emu.PURE_MATH):
            size = 4 if name.endswith("f") else 8
            m = pcode_emu.Machine.__new__(pcode_emu.Machine)
            m.regs, m.state, m.undef_vn = bytearray(pcode_emu.REG_SIZE), {}, set()
            m.regs[pcode_emu.Q0:pcode_emu.Q0 + size] = pack(0.5, size)
            m.regs[pcode_emu.Q0 + pcode_emu.QSTRIDE:pcode_emu.Q0 + pcode_emu.QSTRIDE + size] = pack(0.25, size)
            m.math(name)


if __name__ == "__main__":
    unittest.main()
