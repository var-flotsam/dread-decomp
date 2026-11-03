"""tools/port_lift.py's pointed_to (and gen_lift's semantic lift of an x8 wrapper) on hand-assembled AArch64: the functions a function reaches
through the addresses it takes (a vtable after an adrp/add, a function pointer in a GOT slot after
an adrp/ldr). No game data: the image is a few words and relocations written here. Skipped when
port_lift's imports (capstone) are missing; the hosted build job has them.
"""

import struct
import unittest
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401

try:
    import abi_facts
    import asmdiff
    import gen_lift
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


def br(rn: int) -> int:
    return 0xD61F0000 | (rn << 5)


def mov(rd: int, rm: int) -> int:
    return 0xAA0003E0 | (rm << 16) | rd


@unittest.skipIf(port_lift is None, "capstone is not installed")
class IndirectResultWrapper(unittest.TestCase):
    """Semantic mode: a virtual-call wrapper (ldr x9, [x0]; ldr x1, [x9, #0x48]; br x1) whose
    callers point x8 at their frame (abi_facts' fwd and args hold 8) passes x8 on: its indirect
    tail call builds the result where x8 points, and a lift that drops x8 is refused."""

    A = asmdiff.BASE + 0x1000 if port_lift is not None else 0

    def setUp(self):
        saved = gen_lift.Lifter.facts, gen_lift.Lifter.shape, gen_lift.Lifter.main
        self.addCleanup(lambda: setattr(gen_lift.Lifter, "facts", saved[0]) or
                        setattr(gen_lift.Lifter, "shape", saved[1]) or setattr(gen_lift.Lifter, "main", saved[2]))
        gen_lift.Lifter.shape, gen_lift.Lifter.main = {}.get, {}.get

    def lift(self, words, fwd, args=None, cls=None):
        code = b"".join(struct.pack("<I", w) for w in words)
        orig = SimpleNamespace(function=lambda a: code, sizes={self.A: len(code)}, plt_import=lambda a: None,
                               img=None, got_slot=lambda *a: None)
        args = frozenset({0}) | (fwd & {8}) if args is None else args
        gen_lift.Lifter.facts = {self.A: abi_facts.Facts(args=args, stack=False, fwd=fwd)}.get
        if cls is not None:
            lifter = cls(orig, self.A, 0, "value")
            lifter.opts = frozenset({"semantic"})
            r = lifter.lift()
            return (r[0] if r else None), lifter.reason
        why = {}
        for d in gen_lift.lift_serial(orig, [self.A], why, frozenset({"semantic"})):
            if self.A in d:
                return d[self.A][0], ""
        return None, why[self.A][1]

    WRAPPER = (ldr(9, 0, 0), ldr(1, 9, 0x48), br(1))

    def test_without_x8_a_plain_tail_call(self):
        text, _ = self.lift(self.WRAPPER, frozenset({0}))
        self.assertIn("return ((char* (*)(char*))(r1_1))(p0);", text)
        self.assertNotIn("UnkResult", text)

    def test_x8_forwarded_is_built_in_place(self):
        text, _ = self.lift(self.WRAPPER, frozenset({0, 8}))
        self.assertTrue(text.startswith("UnkResult fn_"), text)
        self.assertIn("new ((void*)((char*)&ret), lift_at_t{}) UnkResult(((UnkResult (*)(char*))", text)

    def test_flat_lifter_x8_forwarded(self):
        text, why = self.lift(self.WRAPPER, frozenset({0, 8}), cls=gen_lift.FlatLifter)
        self.assertIsNotNone(text, why)
        self.assertTrue(text.startswith("UnkResult fn_"), text)
        self.assertIn("u64 X8 = (u64)&ret;", text)
        self.assertIn("new ((void*)(((char*)X8)), lift_at_t{}) UnkResult(", text)

    def test_x8_overwritten_lift_refused(self):
        """The facts give the function an indirect result, but x8 holds another value at the call:
        nothing passes the result address on, so no lift is accepted."""
        words = (ldr(9, 0, 0), ldr(1, 9, 0x48), mov(8, 2), br(1))
        text, why = self.lift(words, frozenset({0, 8}), args=frozenset({0, 2, 8}))
        self.assertIsNone(text)
        for cls in (gen_lift.Lifter, gen_lift.FlatLifter):
            text, why = self.lift(words, frozenset({0, 8}), args=frozenset({0, 2, 8}), cls=cls)
            self.assertIsNone(text, cls)


if __name__ == "__main__":
    unittest.main()
