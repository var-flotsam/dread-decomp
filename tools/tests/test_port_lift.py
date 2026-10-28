"""tools/port_lift.py's pointed_to on hand-assembled AArch64: the functions a function reaches
through the addresses it takes (a vtable after an adrp/add, a function pointer in a GOT slot after
an adrp/ldr). No game data: the image is a few words and relocations written here. Skipped when
port_lift's imports (capstone) are missing; the hosted build job has them.
"""

import struct
import unittest
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401

try:
    import asmdiff
    import port_lift
except ImportError:  # capstone missing
    port_lift = None

RET = 0xD65F03C0


def adrp(rd: int, pc: int, target: int) -> int:
    imm = (target >> 12) - (pc >> 12)
    return 0x90000000 | ((imm & 3) << 29) | (((imm >> 2) & 0x7FFFF) << 5) | rd


def add(rd: int, rn: int, imm: int) -> int:
    return 0x91000000 | (imm << 10) | (rn << 5) | rd


def ldr(rt: int, rn: int, off: int) -> int:
    return 0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt


@unittest.skipIf(port_lift is None, "capstone is not installed")
class PointedTo(unittest.TestCase):
    def setUp(self):
        b = asmdiff.BASE
        self.f, self.a, self.b, self.c, self.other = b + 0x1000, b + 0x2000, b + 0x2100, b + 0x2200, b + 0x2300
        vtable, got = b + 0x5010, b + 0x6008  # (the vtable's address points past offset-to-top and type info)
        rel = asmdiff.R_AARCH64_RELATIVE
        relocs = {vtable - b: SimpleNamespace(type=rel, addend=self.a - b),
                  vtable + 8 - b: SimpleNamespace(type=rel, addend=self.b - b),
                  vtable + 16 - b: SimpleNamespace(type=rel, addend=0x9999),   # not a function: the table ends
                  vtable + 24 - b: SimpleNamespace(type=rel, addend=self.other - b),
                  got - b: SimpleNamespace(type=rel, addend=self.c - b)}
        f = self.f
        words = [adrp(8, f, vtable), add(8, 8, vtable & 0xFFF), adrp(9, f + 8, got), ldr(9, 9, got & 0xFFF), RET]
        code = b"".join(struct.pack("<I", w) for w in words)
        self.orig = SimpleNamespace(img=SimpleNamespace(relocs=relocs), function=lambda a: code)
        self.funcs = {a: {} for a in (self.f, self.a, self.b, self.c, self.other)}

    def test_vtable_and_got(self):
        self.assertEqual(port_lift.pointed_to(self.orig, self.funcs, self.f), {self.a, self.b, self.c})


if __name__ == "__main__":
    unittest.main()
