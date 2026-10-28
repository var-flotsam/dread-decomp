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


if __name__ == "__main__":
    unittest.main()
