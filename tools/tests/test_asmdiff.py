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


if __name__ == "__main__":
    unittest.main()
