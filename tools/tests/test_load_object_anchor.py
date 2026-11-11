"""tools/pcode_emu.py's load_object with `anchor`: an object's own strings, statics and functions
placed at the addresses they stand for in the original, as a hand-written source needs.

A small fixture is compiled with the project compiler; no game data. Skipped when the compiler
(tools/fetch_toolchains.py 9.0.1) or pyelftools is not installed; the hosted build job has both.
"""

import struct
import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import buildcfg
import fetch_toolchains

try:
    import asmdiff
    import pcode_emu
except ImportError:  # pyelftools missing
    asmdiff = pcode_emu = None

FIXTURE = """
extern "C" void fn_7100000900(const char*, int*);
extern "C" __attribute__((noinline)) int fn_7100000200(int x) { return x ^ 0x55; }
static int s_count;
// A string whose address leaves the function, a static it writes, a call to a function this file
// defines, and a jump table (a reference into the function itself).
extern "C" int fn_7100000100(int x) {
    fn_7100000900("hello", &s_count);
    s_count += fn_7100000200(x);
    switch (x) {
    case 0: fn_7100000900("a", nullptr); break;
    case 1: fn_7100000900("b", &s_count); break;
    case 2: fn_7100000900("c", nullptr); return 5;
    case 3: fn_7100000900("d", &s_count); return 9;
    case 4: fn_7100000900("e", nullptr); return 1;
    }
    return s_count;
}
"""
STRINGS, STATIC = 0x7100500000, 0x7100600000


def resolve(name: str):
    m = asmdiff.TARGET_RE.match(asmdiff.strip_params(name))
    return int(m.group(1), 16) if m else None


@unittest.skipUnless(pcode_emu and fetch_toolchains.clang_path(buildcfg.COMPILER).is_file(),
                     "needs the project compiler and pyelftools")
class Anchor(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        d = Path(tempfile.mkdtemp())
        src, cls.obj = d / "anchor.cpp", d / "anchor.o"
        src.write_text(FIXTURE, encoding="utf-8")
        subprocess.run(buildcfg.cxx_command(src, cls.obj), check=True)

    def load(self, anchor):
        self.asked = []

        def spy(name, section, offset, kind):
            self.asked.append((name, section.name, offset, kind))
            return anchor(name, section, offset, kind)
        return pcode_emu.load_object(str(self.obj), resolve, {}, anchor=spy if anchor else None)

    def addresses(self, lo, addr) -> set:
        """What the function's adrp + add/ldr/str pairs point at, as laid out."""
        start, size = lo.functions[addr]
        out, pages = set(), {}
        for a in range(start, start + size, 4):
            w = pcode_emu._word(lo, a)
            if w & 0x9F000000 == 0x90000000:   # adrp
                imm = ((w >> 5) & 0x7FFFF) << 2 | (w >> 29) & 3
                imm -= (1 << 21) if imm & (1 << 20) else 0
                pages[w & 31] = (a & ~0xFFF) + (imm << 12)
            elif w & 0xFFC00000 == 0x91000000 and (w >> 5) & 31 in pages:   # add x, x, #imm
                out.add(pages[(w >> 5) & 31] + ((w >> 10) & 0xFFF))
            elif w & 0x3B000000 == 0x39000000 and (w >> 5) & 31 in pages:   # ldr/str [x, #imm]
                out.add(pages[(w >> 5) & 31] + (((w >> 10) & 0xFFF) << (w >> 30)))
        return out

    def test_without_anchor_everything_is_the_objects_own(self):
        lo = self.load(None)
        start, size = lo.functions[0x7100000100]
        own = lambda a: pcode_emu.LIFT_BASE <= a < pcode_emu.LIFT_BASE + 0x100000  # noqa: E731
        self.assertTrue(all(own(a) for a in self.addresses(lo, 0x7100000100)))
        local = [t for p, t in lo.branch.items() if start <= p < start + size and own(t)]
        self.assertEqual(local, [lo.functions[0x7100000200][0]])   # the call to the file's own function

    def test_strings_statics_and_local_functions_take_the_given_addresses(self):
        def anchor(name, section, offset, kind):
            if kind == "STT_FUNC":
                return resolve(name)
            if name is None and section.name.startswith(".rodata.str"):
                return STRINGS + offset
            if name == "_ZL7s_count":
                return STATIC + offset
            return None
        lo = self.load(anchor)
        start, size = lo.functions[0x7100000100]
        seen = self.addresses(lo, 0x7100000100)
        self.assertIn(STATIC, seen)
        self.assertTrue(any(STRINGS <= a < STRINGS + 0x100 for a in seen))
        targets = {t for p, t in lo.branch.items() if start <= p < start + size}
        self.assertIn(0x7100000200, targets)                        # the local function, by its original address
        self.assertNotIn(lo.functions[0x7100000200][0], targets)
        self.assertEqual(lo.unresolved, {})
        # The function's references into itself (its jump table) were never offered for anchoring.
        self.assertFalse([a for a in self.asked if a[0] == "fn_7100000100"])

    def test_unknown_address_makes_the_function_inconclusive(self):
        lo = self.load(lambda name, section, offset, kind: False if name == "_ZL7s_count" else None)
        start, size = lo.functions[0x7100000100]
        inside = [n for p, ns in lo.unresolved.items() if start <= p < start + size for n in ns]
        self.assertTrue(inside and all("no address in the original" in n for n in inside), inside)
        other, osize = lo.functions[0x7100000200]
        self.assertFalse([p for p in lo.unresolved if other <= p < other + osize])


if __name__ == "__main__":
    unittest.main()
