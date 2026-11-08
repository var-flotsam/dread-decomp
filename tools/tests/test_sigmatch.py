"""tools/sigmatch.py's position-independent signature and near-match pairing on hand-assembled
AArch64 words (no game data, no compiler)."""

import struct
import unittest

from tools.tests import _path  # noqa: F401
import sigmatch

RET = 0xD65F03C0
NOP = 0xD503201F


def code(*words):
    return struct.pack(f"<{len(words)}I", *words)


def adrp(rd, imm=0):
    return 0x90000000 | ((imm & 3) << 29) | (((imm >> 2) & 0x7FFFF) << 5) | rd


def adr(rd, imm=0):
    return 0x10000000 | ((imm & 3) << 29) | (((imm >> 2) & 0x7FFFF) << 5) | rd


def add_imm(rd, rn, imm):
    return 0x91000000 | (imm << 10) | (rn << 5) | rd


def ldr_x(rt, rn, off):
    return 0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt


def str_x(rt, rn, off):
    return 0xF9000000 | ((off // 8) << 10) | (rn << 5) | rt


def movz(rd, imm):
    return 0xD2800000 | (imm << 5) | rd


def bl(imm):
    return 0x94000000 | (imm & 0x3FFFFFF)


def b(imm):
    return 0x14000000 | (imm & 0x3FFFFFF)


def add_reg(rd, rn, rm):
    return 0x8B000000 | (rm << 16) | (rn << 5) | rd


sig = sigmatch.signature


class Signature(unittest.TestCase):
    def test_branch_targets_masked(self):
        self.assertEqual(sig(code(bl(0), b(0), RET)), sig(code(bl(0x1234), b(0x40), RET)))
        self.assertNotEqual(sig(code(bl(0))), sig(code(b(0))))

    def test_adrp_and_adr_pages_masked(self):
        self.assertEqual(sig(code(adrp(8, 1), RET)), sig(code(adrp(8, 0x7777), RET)))
        self.assertEqual(sig(code(adr(3, 1))), sig(code(adr(3, 0x400))))
        self.assertNotEqual(sig(code(adrp(8))), sig(code(adrp(9))))

    def test_lo12_masked_only_from_an_adrp_base(self):
        self.assertEqual(sig(code(adrp(8), add_imm(0, 8, 0x10))), sig(code(adrp(8, 5), add_imm(0, 8, 0x678))))
        self.assertEqual(sig(code(adrp(8), ldr_x(0, 8, 0x10))), sig(code(adrp(8), ldr_x(0, 8, 0x400))))
        # without the adrp the offset is a struct offset and stays
        self.assertNotEqual(sig(code(add_imm(0, 8, 0x10))), sig(code(add_imm(0, 8, 0x20))))
        self.assertNotEqual(sig(code(ldr_x(0, 8, 0x10))), sig(code(ldr_x(0, 8, 0x18))))

    def test_page_ends_when_register_overwritten(self):
        a = sig(code(adrp(8), movz(8, 1), add_imm(0, 8, 0x10)))
        z = sig(code(adrp(8), movz(8, 1), add_imm(0, 8, 0x20)))
        self.assertNotEqual(a, z)
        a = sig(code(adrp(8), add_reg(8, 1, 2), ldr_x(0, 8, 0x10)))
        z = sig(code(adrp(8), add_reg(8, 1, 2), ldr_x(0, 8, 0x18)))
        self.assertNotEqual(a, z)

    def test_add_into_same_register_ends_the_page(self):
        # add x8, x8, :lo12: makes x8 a full address; a later load from it is a struct offset
        a = sig(code(adrp(8), add_imm(8, 8, 0x10), ldr_x(0, 8, 0x8)))
        z = sig(code(adrp(8), add_imm(8, 8, 0x30), ldr_x(0, 8, 0x10)))
        self.assertNotEqual(a, z)
        self.assertEqual(sig(code(adrp(8), add_imm(8, 8, 0x10), ldr_x(0, 8, 0x8))),
                         sig(code(adrp(8), add_imm(8, 8, 0x30), ldr_x(0, 8, 0x8))))

    def test_store_keeps_the_page(self):
        # str x8, [x8, #lo12] does not write x8, so the next access is still page-relative
        a = sig(code(adrp(8), str_x(8, 8, 0x10), ldr_x(0, 8, 0x20)))
        z = sig(code(adrp(8), str_x(8, 8, 0x40), ldr_x(0, 8, 0x80)))
        self.assertEqual(a, z)

    def test_load_into_base_ends_the_page(self):
        a = sig(code(adrp(8), ldr_x(8, 8, 0x10), ldr_x(0, 8, 0x20)))
        z = sig(code(adrp(8), ldr_x(8, 8, 0x10), ldr_x(0, 8, 0x28)))
        self.assertNotEqual(a, z)

    def test_other_page_registers_survive(self):
        a = sig(code(adrp(8), adrp(9), movz(9, 0), add_imm(0, 8, 0x10)))
        z = sig(code(adrp(8, 3), adrp(9, 4), movz(9, 0), add_imm(0, 8, 0x50)))
        self.assertEqual(a, z)

    def test_hex_digest(self):
        s = sig(code(RET))
        self.assertEqual(len(s), 40)
        int(s, 16)

    def test_call_does_not_end_a_callee_saved_page(self):
        a = sig(code(adrp(19), bl(0), add_imm(0, 19, 0x10)))
        z = sig(code(adrp(19), bl(19), add_imm(0, 19, 0x20)))
        self.assertEqual(a, z)

    def test_bl_ends_a_link_register_page(self):
        # BL writes x30, so an add from x30 after it is no longer an adrp page offset.
        a = sig(code(adrp(30), bl(0), add_imm(0, 30, 0x10)))
        z = sig(code(adrp(30), bl(0), add_imm(0, 30, 0x20)))
        self.assertNotEqual(a, z)


class Mnemonics(unittest.TestCase):
    def test_mnemonics(self):
        self.assertEqual(sigmatch.mnemonics(code(movz(0, 1), adrp(8), add_imm(0, 8, 4), bl(3), RET)),
                         ["mov", "adrp", "add", "bl", "ret"])


def body(n, seed):
    """n instructions of a fixed mnemonic pattern; `seed` changes only registers/immediates."""
    pattern = [lambda i: movz(i % 8, seed), lambda i: add_imm(1, 2, (i + seed) & 0xFFF),
               lambda i: ldr_x(3, 4, 8 * (i % 16)), lambda i: str_x(5, 6, 8 * ((i + seed) % 16))]
    return [pattern[i % 4](i) for i in range(n)]


class NearMatches(unittest.TestCase):
    A1, A2, A3 = 0x7100030000, 0x7100031000, 0x7100032000

    def test_mutual_best_pair_accepted(self):
        compiled = code(*body(40, 1), RET)
        binary = code(*body(38, 2), NOP, NOP, RET)  # two words differ in mnemonic
        unrelated = code(*([NOP] * 40), RET)
        got = sigmatch.near_matches([("luaV_x", compiled)], {self.A1: binary, self.A2: unrelated})
        self.assertEqual(got, {"luaV_x": self.A1})

    def test_small_functions_ignored(self):
        small = code(*body(20, 1), RET)  # under FUZZY_MIN_SIZE bytes
        self.assertLess(len(small), sigmatch.FUZZY_MIN_SIZE)
        self.assertEqual(sigmatch.near_matches([("f", small)], {self.A1: small}), {})

    def test_below_ratio_rejected(self):
        compiled = code(*body(40, 1), RET)
        binary = code(*body(20, 1), *([NOP] * 20), RET)  # half the mnemonics differ
        self.assertEqual(sigmatch.near_matches([("f", compiled)], {self.A1: binary}), {})

    def test_length_difference_skipped(self):
        compiled = code(*body(40, 1), RET)
        binary = code(*body(60, 1), RET)  # more than a fifth longer
        self.assertEqual(sigmatch.near_matches([("f", compiled)], {self.A1: binary}), {})

    def test_not_mutual_rejected(self):
        # g matches A1 exactly; f's best is also A1, but A1's best is g, so f gets nothing
        f = code(*body(38, 1), NOP, NOP, RET)
        g = code(*body(40, 1), RET)
        got = sigmatch.near_matches([("f", f), ("g", g)], {self.A1: code(*body(40, 2), RET)})
        self.assertEqual(got, {"g": self.A1})


class FakeOriginal:
    def __init__(self, funcs):
        self.funcs = funcs
        self.sizes = {a: len(c) for a, c in funcs.items()}

    def function(self, a):
        return self.funcs[a]


class BinaryIndex(unittest.TestCase):
    def test_groups_by_size_and_signature(self):
        x = code(adrp(8, 1), add_imm(0, 8, 0x10), RET)
        y = code(adrp(8, 9), add_imm(0, 8, 0x80), RET)  # same after masking
        z = code(movz(0, 1), RET)
        idx = sigmatch.binary_index(FakeOriginal({1: x, 2: y, 3: z}))
        self.assertEqual(idx[(12, sig(x))], [1, 2])
        self.assertEqual(idx[(8, sig(z))], [3])
        self.assertEqual(len(idx), 2)


if __name__ == "__main__":
    unittest.main()


class BinaryStrings(unittest.TestCase):
    """The strings an ADRP + ADD pair points at, which tell look-alikes such as luaopen_table and
    luaopen_debug apart."""

    class FakeOriginal:
        def __init__(self, data, code):
            self.img = type("Img", (), {"data": data})()
            self._code = code

        def function(self, addr):
            return self._code

    def test_adrp_add_string(self):
        data = bytearray(0x3000)
        data[0x2010:0x2016] = b"table\0"
        add = 0x91000000 | (0x10 << 10) | (1 << 5) | 1  # add x1, x1, #0x10
        fake = self.FakeOriginal(bytes(data), code(adrp(1, 2), add, RET))
        self.assertEqual(sigmatch.binary_strings(fake, 0x7100000000), frozenset({"table"}))

    def test_non_text_is_not_a_string(self):
        data = bytearray(0x3000)
        data[0x2010:0x2013] = b"\x01\x02\0"
        add = 0x91000000 | (0x10 << 10) | (1 << 5) | 1
        fake = self.FakeOriginal(bytes(data), code(adrp(1, 2), add, RET))
        self.assertEqual(sigmatch.binary_strings(fake, 0x7100000000), frozenset())
