"""tools/asmdiff.py on an object compiled from a small fixture with the project compiler.

The "original" is the fixture's own code, so no game data is involved. Skipped when the project
compiler (tools/fetch_toolchains.py 9.0.1) or pyelftools is not installed; the hosted build job
has both.
"""

import struct
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import buildcfg
import fetch_toolchains

try:
    import asmdiff
except ImportError:  # pyelftools missing
    asmdiff = None

FIXTURE = """
extern "C" int fn_7100000200(int);
extern int g_unk_7100100000;
extern "C" int fn_7100000100(int x) { return fn_7100000200(x + g_unk_7100100000) * 3; }
extern "C" int fn_7100000200(int x) { return x ^ 0x55; }
"""


class FakeOriginal:
    """What diff_functions reads from asmdiff.Original: sizes and each function's bytes."""

    def __init__(self, code: dict[int, bytes]):
        self.code = code
        self.sizes = {a: len(c) for a, c in code.items()}

    def function(self, address: int) -> bytes:
        return self.code[address]


@unittest.skipUnless(asmdiff and fetch_toolchains.clang_path(buildcfg.COMPILER).is_file(),
                     "needs the project compiler and pyelftools")
class SelfDiff(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import subprocess
        d = Path(tempfile.mkdtemp())
        src, obj = d / "fixture.cpp", d / "fixture.o"
        src.write_text(FIXTURE, encoding="utf-8")
        subprocess.run(buildcfg.cxx_command(src, obj), check=True)
        cls.functions = {f.symbol: f for f in asmdiff.read_object(obj)}

    def original(self, patch=None) -> FakeOriginal:
        code = {int(s[3:], 16): bytearray(f.code) for s, f in self.functions.items()}
        if patch:
            patch(code)
        return FakeOriginal({a: bytes(c) for a, c in code.items()})

    def test_object_read(self):
        self.assertEqual(sorted(self.functions), ["fn_7100000100", "fn_7100000200"])
        caller = self.functions["fn_7100000100"]
        self.assertTrue(caller.relocs, "the call and the global are relocated")

    def test_identical_code_matches(self):
        res = asmdiff.diff_functions(list(self.functions.values()), self.original())
        self.assertTrue(all(r.bytes_match for r in res))

    def test_relocated_fields_are_masked(self):
        caller = self.functions["fn_7100000100"]
        off = next(o for o, r in caller.relocs.items() if r.type in asmdiff.MASKS)
        mask = asmdiff.MASKS[caller.relocs[off].type]

        def patch(code):
            insn, = struct.unpack_from("<I", code[0x7100000100], off)
            struct.pack_into("<I", code[0x7100000100], off, insn | (~mask & 0xFFFFFFFF & 0x00ABCDEF))
        res = {r.symbol: r for r in asmdiff.diff_functions([caller], self.original(patch))}
        self.assertTrue(res["fn_7100000100"].bytes_match)

    def test_changed_instruction_differs(self):
        callee = self.functions["fn_7100000200"]

        def patch(code):
            code[0x7100000200][0] ^= 0x01
        res = asmdiff.diff_functions([callee], self.original(patch))
        self.assertFalse(res[0].bytes_match)
        self.assertEqual(res[0].diff_insns, [0])



def _code(*words: int) -> bytes:
    return b"".join(struct.pack("<I", w) for w in words)


RET, BL, NOP, CBZ, BEQ = 0xD65F03C0, 0x94000000, 0xD503201F, 0xB4000040, 0x54000040
LDR_W0, LDRB_W0, LDR_W0_4 = 0xB9400000, 0x39400000, 0xB9400400
ADD_W0, ADD_W1 = 0x11000400, 0x11000421


@unittest.skipIf(asmdiff is None, "pyelftools not installed")
class ClassifyTest(unittest.TestCase):
    """asmdiff.classify on hand-assembled instruction words (no game data, no compiler)."""

    def kind(self, orig, ours) -> str:
        return asmdiff.classify(_code(*orig), _code(*ours)).split(" ")[0]

    def test_same_bytes(self):
        self.assertEqual(self.kind([ADD_W0, RET], [ADD_W0, RET]), "match")

    def test_call_count_first(self):
        # A missing call outranks the branch difference that comes with it.
        self.assertEqual(self.kind([BL, CBZ, RET], [RET]), "calls")

    def test_control_flow(self):
        self.assertEqual(self.kind([CBZ, LDR_W0, RET], [LDR_W0, RET]), "control")
        self.assertEqual(self.kind([BEQ, RET, RET], [BEQ, NOP, RET]), "control")

    def test_conditions(self):
        tbz = 0x36000040  # tbz w0, #0, +8
        self.assertEqual(self.kind([tbz, LDR_W0, RET], [CBZ, LDR_W0, RET]), "conditions")
        bne = 0x54000041  # b.ne +8
        self.assertEqual(self.kind([BEQ, LDR_W0, RET], [bne, LDR_W0, RET]), "conditions")

    def test_operations(self):
        self.assertEqual(self.kind([LDR_W0, RET], [LDRB_W0, RET]), "operations")  # width
        self.assertEqual(self.kind([LDR_W0, RET], [LDR_W0_4, RET]), "operations")  # field offset

    def test_registers_and_order(self):
        self.assertEqual(self.kind([ADD_W0, RET], [ADD_W1, RET]), "registers")
        self.assertEqual(self.kind([ADD_W0, LDR_W0, RET], [LDR_W0, ADD_W0, RET]), "registers")

    def test_page_offsets_are_relocations(self):
        # adrp x8; add x8, x8, #lo12: our object has 0 where the original has the address's low bits.
        adrp, add_lo, add_0 = 0x90000008, 0x91004108, 0x91000108
        self.assertEqual(self.kind([adrp, add_lo, ADD_W0, RET], [adrp, add_0, ADD_W1, RET]), "registers")
        # Once the full address is formed, an offset from it is a real field offset again.
        ldr_x8_8, ldr_x8_10 = 0xF9400508, 0xF9400908  # ldr x8, [x8, #8] / [x8, #16]
        self.assertEqual(self.kind([adrp, add_lo, ldr_x8_8, RET], [adrp, add_0, ldr_x8_10, RET]), "operations")


class MemoryOriginal:
    """An original whose function loads an address (adrp + lo12) and whose image holds `memory`."""

    def __init__(self, code: bytes, memory: dict[int, bytes], readable: bool = True):
        self.code, self.memory = code, memory
        if readable:
            self.data_at = self._data_at

    def function(self, address: int) -> bytes:
        return self.code

    def _data_at(self, address: int, size: int):
        for start, blob in self.memory.items():
            if start <= address and address + size <= start + len(blob):
                return blob[address - start:address - start + size]
        return None


@unittest.skipIf(asmdiff is None, "pyelftools not installed")
class ContentTest(unittest.TestCase):
    """check_relocs on a string literal and a constant-pool entry: the bytes our object points at must
    be the bytes at the address the original's adrp + lo12 pair encodes (hand-assembled, no compiler)."""

    ADDRESS = 0x7100001000
    WHERE = 0x7100003120  # two pages on, +0x120
    ADRP_0, ADRP_2 = 0x90000000, 0xD0000000  # adrp x0, <this page> / <two pages on>
    ADD_0, ADD_120 = 0x91000000, 0x91048000  # add x0, x0, #0 / #0x120
    LDR_0, LDR_120 = 0xFD400000, 0xFD409000  # ldr d0, [x0] / [x0, #0x120]

    def report(self, section, data, memory, lo12=(277, ADD_0, ADD_120), readable=True, pair=True):
        rtype, ours, theirs = lo12
        relocs = {0: asmdiff.ObjReloc(275, section, 0, section=True, data=data)}
        if pair:
            relocs[4] = asmdiff.ObjReloc(rtype, section, 0, section=True, data=data)
        res = asmdiff.FunctionResult("fn_7100001000", self.ADDRESS, 12, 12, relocs=relocs)
        original = MemoryOriginal(_code(self.ADRP_2, theirs, RET), {self.WHERE: memory}, readable)
        self.assertEqual(len(_code(self.ADRP_0, ours, RET)), 12)
        return asmdiff.check_relocs(res, original)

    def test_same_string(self):
        rep = self.report(".rodata.str1.1", b"abc\0", b"abc\0")
        self.assertEqual((dict(rep.checked), rep.mismatches, dict(rep.unchecked)), ({"string": 1}, [], {}))

    def test_other_string(self):
        rep = self.report(".rodata.str1.1", b"abc\0", b"abd\0")
        self.assertEqual([(m.kind, m.actual, m.offset) for m in rep.mismatches], [("string", self.WHERE, 0)])
        self.assertEqual(asmdiff.format_mismatch(rep.mismatches[0]),
                         '"abc" -> original "abd"  at 0x7100003120  +0x0  string')

    def test_longer_original_string(self):
        # The terminator is part of the comparison: ".bcpt" is not ".bcptl".
        rep = self.report(".rodata.str1.1", b"x.bcpt\0", b"x.bcptl\0")
        self.assertEqual(len(rep.mismatches), 1)
        self.assertIn('"x.bcpt" -> original "x.bcptl"...', asmdiff.format_mismatch(rep.mismatches[0]))

    def test_constant(self):
        ldr = (286, self.LDR_0, self.LDR_120)
        one, two = struct.pack("<d", 1.0), struct.pack("<d", 2.0)
        self.assertEqual(dict(self.report(".rodata.cst8", one, one, ldr).checked), {"const": 1})
        rep = self.report(".rodata.cst8", one, two, ldr)
        self.assertEqual([m.kind for m in rep.mismatches], ["const"])
        self.assertIn(f"{one.hex()} -> original {two.hex()}", asmdiff.format_mismatch(rep.mismatches[0]))

    def test_outside_the_image(self):
        original = self.report(".rodata.str1.1", b"abc\0", b"")
        self.assertIn("nothing (outside the image)", asmdiff.format_mismatch(original.mismatches[0]))

    def test_unchecked_without_content_image_or_pair(self):
        # A reference with no captured bytes (a jump table, a wide string), an original that cannot
        # be read (the generators' test stand-ins), and a page load with no lo12 half.
        for kwargs in ({"data": None}, {"readable": False}, {"pair": False}):
            args = {"section": ".rodata.str1.1", "data": b"abc\0", "memory": b"abd\0", **kwargs}
            rep = self.report(**args)
            self.assertEqual((dict(rep.checked), rep.mismatches, dict(rep.unchecked)), ({}, [], {"string": 1}), kwargs)
        rep = self.report(".rodata", None, b"")
        self.assertEqual(dict(rep.unchecked), {"section": 1})


CONTENT_FIXTURE = """
extern "C" const char* fn_7100000300() { return "hello"; }
extern "C" int fn_7100000400(int x) {
    switch (x) { case 0: return 7; case 1: return 5; case 2: return 1; case 3: return 6; }
    return 0;
}
"""


@unittest.skipUnless(asmdiff and fetch_toolchains.clang_path(buildcfg.COMPILER).is_file(),
                     "needs the project compiler and pyelftools")
class ObjectContent(unittest.TestCase):
    """read_object records the bytes a string or constant-pool reference points at."""

    def test_pointed_at_bytes(self):
        import subprocess
        d = Path(tempfile.mkdtemp())
        src, obj = d / "content.cpp", d / "content.o"
        src.write_text(CONTENT_FIXTURE, encoding="utf-8")
        subprocess.run(buildcfg.cxx_command(src, obj), check=True)
        fns = {f.symbol: f for f in asmdiff.read_object(obj)}
        string = {r.data for r in fns["fn_7100000300"].relocs.values()}
        self.assertEqual(string, {b"hello\0"})
        # A four-entry switch becomes a 16-byte lookup table in the constant pool.
        table = {r.data for r in fns["fn_7100000400"].relocs.values()}
        self.assertEqual(table, {struct.pack("<4i", 7, 5, 1, 6)})


if __name__ == "__main__":
    unittest.main()
