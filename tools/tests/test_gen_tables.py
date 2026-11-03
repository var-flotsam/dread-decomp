"""tools/gen_tables.py's register simulator and literal helpers on hand-assembled code (no game data).

Code and strings are invented: a tiny fake module whose functions are a few AArch64 words built
by the encoders below, at made-up addresses.
"""

import struct
import unittest

from tools.tests import _path  # noqa: F401
import gen_tables as gt

BASE = 0x7100000000
DATA_PAGE = 0x7100002000  # where the fake strings live


# -- A minimal AArch64 encoder (only the forms the tests use) --------------------------------
class Asm:
    def __init__(self, pc: int):
        self.start = self.pc = pc
        self.words: list[int] = []

    def _emit(self, w: int) -> "Asm":
        self.words.append(w)
        self.pc += 4
        return self

    def movz(self, rd, imm):  # mov xD, #imm
        return self._emit(0xD2800000 | (imm << 5) | rd)

    def mov(self, rd, rm):  # mov xD, xM (orr xD, xzr, xM)
        return self._emit(0xAA0003E0 | (rm << 16) | rd)

    def add(self, rd, rn, imm):  # add xD, xN|sp, #imm (rn 31 = sp)
        return self._emit(0x91000000 | (imm << 10) | (rn << 5) | rd)

    def adrp(self, rd, target):
        off = ((target >> 12) - (self.pc >> 12)) & 0x1FFFFF
        return self._emit(0x90000000 | ((off & 3) << 29) | ((off >> 2) << 5) | rd)

    def addr(self, rd, target):  # adrp + add
        return self.adrp(rd, target).add(rd, rd, target & 0xFFF)

    def str_post(self, rt, rn, imm):  # str xT, [xN], #imm
        return self._emit(0xF8000400 | ((imm & 0x1FF) << 12) | (rn << 5) | rt)

    def bl(self, target):
        return self._emit(0x94000000 | (((target - self.pc) >> 2) & 0x3FFFFFF))

    def b(self, target):
        return self._emit(0x14000000 | (((target - self.pc) >> 2) & 0x3FFFFFF))

    def cbz(self, rt, delta):
        return self._emit(0xB4000000 | (((delta >> 2) & 0x7FFFF) << 5) | rt)

    def ret(self):
        return self._emit(0xD65F03C0)

    def code(self) -> bytes:
        return b"".join(struct.pack("<I", w) for w in self.words)


class FakeImage:
    def __init__(self, strings: dict[int, bytes]):
        data = bytearray(DATA_PAGE - BASE + 0x1000)
        for addr, s in strings.items():
            data[addr - BASE:addr - BASE + len(s) + 1] = s + b"\0"
        self.data = bytes(data)
        self.relocs: dict = {}


class FakeOriginal:
    def __init__(self, functions: dict[int, bytes], strings: dict[int, bytes] | None = None):
        self.functions = functions
        self.img = FakeImage(strings or {})

    def function(self, address: int) -> bytes:
        return self.functions[address]


def orig_of(asm: Asm, strings=None) -> FakeOriginal:
    return FakeOriginal({asm.start: asm.code()}, strings)


# -- Helpers ---------------------------------------------------------------------------------
class Helpers(unittest.TestCase):
    def test_imm(self):
        self.assertEqual(gt.imm("#0x10"), 16)
        self.assertEqual(gt.imm(" #-8"), -8)
        self.assertEqual(gt.imm("12"), 12)

    def test_reg_folds_w_into_x(self):
        self.assertEqual(gt.reg("w3"), "x3")
        self.assertEqual(gt.reg("x29"), "x29")
        self.assertEqual(gt.reg("sp"), "sp")
        self.assertEqual(gt.reg("wzr"), "wzr")

    def test_literal_escapes(self):
        self.assertEqual(gt.literal(b"abc"), '"abc"')
        self.assertEqual(gt.literal(b'a"b\\c'), r'"a\"b\\c"')
        self.assertEqual(gt.literal(b"\n"), r'"\012"')
        self.assertEqual(gt.literal(b"\xc3\xa9"), r'"\303\251"')

    def test_literal_escapes_question_mark(self):
        # no trigraph can form
        self.assertEqual(gt.literal(b"??="), r'"\077\077="')

    def test_cstring(self):
        orig = FakeOriginal({}, {DATA_PAGE + 4: b"hello"})
        self.assertEqual(gt.cstring(orig, DATA_PAGE + 4), b"hello")
        self.assertEqual(gt.cstring(orig, DATA_PAGE + 4 + 5), b"")

    def test_rush_float_shortest(self):
        bits = lambda f: struct.unpack("<I", struct.pack("<f", f))[0]  # noqa: E731
        self.assertEqual(gt.rush_float(bits(1.0)), "1.0f")
        self.assertEqual(gt.rush_float(bits(0.1)), "0.1f")
        self.assertEqual(gt.rush_float(bits(2.5)), "2.5f")
        self.assertEqual(gt.rush_float(bits(-100.0)), "-1e+02f")  # %g's shortest form, still a float literal
        self.assertEqual(gt.rush_float(bits(1e20)), "1e+20f")

    def test_rush_float_round_trips(self):
        for f in (0.3, 1.0 / 3.0, 123456.789, 7e-5):
            b = struct.unpack("<I", struct.pack("<f", f))[0]
            text = gt.rush_float(b)
            self.assertEqual(struct.unpack("<I", struct.pack("<f", float(text[:-1])))[0], b, text)


# -- The register simulator ------------------------------------------------------------------
class SimulateCalls(unittest.TestCase):
    def test_arguments_are_symbolic(self):
        a = Asm(0x7100000100)
        a.addr(1, DATA_PAGE + 0x20).movz(2, 5).add(0, 0, 8).bl(0x7100001570).ret()
        calls = gt.simulate_calls(orig_of(a), a.start)
        self.assertEqual(calls, [(0x7100001570,
                                  [("arg0", 8), ("abs", DATA_PAGE + 0x20), ("imm", 5), ("arg3", 0), ("arg4", 0)])])

    def test_call_clobbers_and_returns(self):
        a = Asm(0x7100000100)
        a.mov(19, 0).bl(0x7100000010).mov(1, 0).add(0, 19, 0x10).b(0x7100000020)
        calls = gt.simulate_calls(orig_of(a), a.start)
        self.assertEqual([c for c, _ in calls], [0x7100000010, 0x7100000020])  # the tail call counts
        args = calls[1][1]
        self.assertEqual(args[0], ("arg0", 0x10))  # x19 survives the call
        self.assertEqual(args[1], ("ret", 0))
        self.assertIsNone(args[2])  # x2 clobbered by the first call

    def test_stack_and_post_index(self):
        a = Asm(0x7100000100)
        a.add(0, 31, 0x30).str_post(2, 1, 8).mov(1, 1).bl(0x7100000010).ret()
        (_, args), = gt.simulate_calls(orig_of(a), a.start)
        self.assertEqual(args[0], ("sp", 0x30))
        self.assertEqual(args[1], ("arg1", 8))

    def test_conditional_branch_stops(self):
        a = Asm(0x7100000100)
        a.cbz(0, 8).bl(0x7100000010).ret()
        with self.assertRaises(gt.TableError):
            gt.simulate_calls(orig_of(a), a.start)

    def test_unconditional_branch_before_the_end_stops(self):
        a = Asm(0x7100000100)
        a.b(0x7100000010).bl(0x7100000020).ret()
        with self.assertRaises(gt.TableError):
            gt.simulate_calls(orig_of(a), a.start)


# -- strid_table on a two-entry fake ---------------------------------------------------------
EMPTY, ALPHA, BETA = DATA_PAGE, DATA_PAGE + 0x10, DATA_PAGE + 0x20
STRINGS = {EMPTY: b"", ALPHA: b"alpha", BETA: b"be?ta"}


def table_ctor(texts: list[tuple[int, bytes]], flag: int = 1) -> Asm:
    a = Asm(gt.TABLE_CTOR)
    a.mov(19, 0)
    for k, (addr, text) in enumerate(texts):
        a.addr(0, addr).movz(1, len(text)).bl(gt.CRC64)
        a.mov(3, 0).addr(1, addr).movz(2, len(text)).movz(4, flag).add(0, 19, 8 * (k + 1)).bl(gt.ID_CTOR)
    return a.ret()


def names_fn(texts: list[tuple[int, bytes]]) -> Asm:
    a = Asm(gt.NAMES_FN)
    n = len(texts)
    for k, (addr, _) in enumerate(texts):
        a.add(0, 31, 0x10 + 0x30 * k).addr(1, EMPTY).bl(gt.TMP_CTOR)
        a.addr(0, addr).add(1, 31, 0x10 + 0x30 * k).bl(gt.NAME_SINK)
    for k in range(n):
        a.add(0, 31, 0x10 + 0x30 * (n - 1 - k)).bl(gt.TMP_DTOR)
    return a.ret()


def strid_original(ctor: Asm, names: Asm) -> FakeOriginal:
    return FakeOriginal({ctor.start: ctor.code(), names.start: names.code()}, STRINGS)


class StridTable(unittest.TestCase):
    TEXTS = [(ALPHA, b"alpha"), (BETA, b"be?ta")]

    def test_entries(self):
        orig = strid_original(table_ctor(self.TEXTS), names_fn(self.TEXTS))
        self.assertEqual(gt.strid_entries(orig), [b"alpha", b"be?ta"])

    def test_generated_source(self):
        orig = strid_original(table_ctor(self.TEXTS), names_fn(self.TEXTS))
        text = gt.generate_strid_table(orig)
        self.assertIn('    UnkStrIdH id_0x8 = UNK_STRID("alpha");\n', text)
        self.assertIn('    UnkStrIdH id_0x10 = UNK_STRID("be\\077ta");\n', text)
        self.assertIn('    UnkStrValue v1("");\n    fn_7100065b14("be\\077ta", v1);\n', text)
        self.assertTrue(text.endswith("}\n"))

    def test_wrong_flag_stops(self):
        orig = strid_original(table_ctor(self.TEXTS, flag=0), names_fn(self.TEXTS))
        with self.assertRaises(gt.TableError):
            gt.strid_entries(orig)

    def test_names_differing_from_table_stop(self):
        orig = strid_original(table_ctor(self.TEXTS), names_fn(list(reversed(self.TEXTS))))
        with self.assertRaises(gt.TableError):
            gt.strid_entries(orig)


if __name__ == "__main__":
    unittest.main()
