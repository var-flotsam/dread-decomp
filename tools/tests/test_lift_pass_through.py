"""tools/gen_lift.py's pass-through in byte mode: a call names the argument registers its callee
reads (abi_facts) that the function still holds. Two registers it must not name:

- s0 right after a call, for a variadic callee (one whose facts read all of v0-v7, as its
  prologue saves them all): naming it would read the earlier call's result as a float and
  retype that call (fn_7100504720's Lua call after fn_710054b93c);
- in FlatLifter, a caller-saved register a call left garbage in: its local still holds the value
  from before the call (fn_710085b76c's x1 at a virtual call), on the straight path and after a
  label that another path reaches from after the call (fn_7100547fe4).

No game data: a few hand-encoded A64 words. Skipped when pyelftools or capstone is not installed."""

import struct
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401

try:
    import abi_facts
    import gen_lift
except ImportError:  # pyelftools or capstone missing
    gen_lift = None

ADDR = 0x7100001000
F, V, G = ADDR + 0x1000, ADDR + 0x1100, ADDR + 0x1200  # a plain callee, a variadic one, one reading x0-x1
S = ADDR + 0x1300  # a callee that builds its result at x8
STP, MOV_FP, LDP, RET = 0xA9BF7BFD, 0x910003FD, 0xA8C17BFD, 0xD65F03C0
MOV_X19_X0 = 0xAA0003F3  # mov x19, x0
MOV_X0_X19 = 0xAA1303E0  # mov x0, x19
MOV_X20_X1 = 0xAA0103F4  # mov x20, x1 (the function reads its x1: X1 has a value)


def bl(at: int, target: int) -> int:
    return 0x94000000 | (((target - (ADDR + at)) // 4) & 0x3FFFFFF)


def cbz_x0(at: int, target: int) -> int:
    return 0xB4000000 | ((((target - at) // 4) & 0x7FFFF) << 5)


@unittest.skipIf(gen_lift is None, "pyelftools or capstone is not installed")
class PassThrough(unittest.TestCase):
    def setUp(self):
        saved = gen_lift.Lifter.facts, gen_lift.Lifter.shape, gen_lift.Lifter.main
        self.addCleanup(lambda: setattr(gen_lift.Lifter, "facts", saved[0]) or
                        setattr(gen_lift.Lifter, "shape", saved[1]) or setattr(gen_lift.Lifter, "main", saved[2]))
        gen_lift.Lifter.shape, gen_lift.Lifter.main = {}.get, {}.get
        every = frozenset(range(8)) | frozenset(range(100, 108))
        gen_lift.Lifter.facts = {
            ADDR: abi_facts.Facts(args=frozenset({0, 1}), stack=False),
            F: abi_facts.Facts(args=frozenset({0}), stack=False),
            V: abi_facts.Facts(args=every, stack=False, wide=every),
            G: abi_facts.Facts(args=frozenset({0, 1}), stack=False, wide=frozenset({0, 1})),
            S: abi_facts.Facts(args=frozenset({0, 8}), stack=False, wide=frozenset({0, 8})),
        }.get

    def lifts(self, words, cls, plain=False, expect=True):
        code = b"".join(struct.pack("<I", w) for w in words)
        orig = SimpleNamespace(function=lambda a: code, sizes={ADDR: len(code)}, plt_import=lambda a: None,
                               img=None, got_slot=lambda *a: None, version="test", module="main")
        out = []
        for returns in ("value", "void"):
            lifter = cls(orig, ADDR, 0, returns)
            lifter.opts = frozenset()
            lifter.plain = plain
            r = lifter.lift()
            if r:
                out.append(r[0])
        if expect:
            self.assertTrue(out, "nothing lifted")
        return out

    def call_line(self, text, fn):
        return next(ln.strip() for ln in text.split("\n") if f"fn_{fn:x}(" in ln and "namespace" not in ln)

    def test_plain_lift_refuses_a_callee_with_an_indirect_result(self):
        # b S (S reads x8): a plain lift would call S as an ordinary function, which on x86-64
        # makes its first argument the result slot (fn_7100de9c30 -> fn_71011a2fbc); it must not lift
        words = [0x14000000 | ((S - ADDR) // 4)]
        for cls in (gen_lift.Lifter, gen_lift.FlatLifter):
            self.assertEqual(self.lifts(words, cls, plain=True, expect=False), [])
            for text in self.lifts(words, cls):
                self.assertIn(f"sret::fn_{S:x}(", text)

    def test_variadic_callee_gets_no_earlier_result(self):
        # stp; mov; bl F; bl V; ldp; ret: F passes x0 on, V's s0 is F's leftover, not a float result
        words = [STP, MOV_FP, bl(8, F), bl(12, V), LDP, RET]
        for cls in (gen_lift.Lifter, gen_lift.FlatLifter):
            for text in self.lifts(words, cls):
                self.assertNotIn("fret::", text)
                self.assertNotIn("dret::", text)
                self.assertIn("p0" if cls is gen_lift.Lifter else "X0", self.call_line(text, F))

    def test_flat_skips_a_register_a_call_clobbered(self):
        # stp; mov; mov x20, x1; mov x19, x0; bl F; mov x0, x19; bl G; ldp; ret: G reads x0 and x1,
        # but x1 is garbage after F (X1 still holds the incoming x1): the call names x0 only
        words = [STP, MOV_FP, MOV_X20_X1, MOV_X19_X0, bl(16, F), MOV_X0_X19, bl(24, G), LDP, RET]
        for text in self.lifts(words, gen_lift.FlatLifter):
            line = self.call_line(text, G)
            self.assertIn("X0", line)
            self.assertNotIn("X1", line)

    def test_flat_names_an_untouched_register(self):
        # stp; mov; bl G; ldp; ret: x0 and x1 reach G untouched, so both are named
        words = [STP, MOV_FP, bl(8, G), LDP, RET]
        for text in self.lifts(words, gen_lift.FlatLifter):
            line = self.call_line(text, G)
            self.assertIn("X0", line)
            self.assertIn("X1", line)

    def test_flat_label_after_a_call(self):
        # stp; mov; mov x20, x1; mov x19, x0; bl F; cbz x0, L; mov x9, x19; L: mov x0, x19; bl G; ldp;
        # ret: both paths reach the label from after F, so x1 is still F's garbage there
        MOV_X9_X19 = 0xAA1303E9
        words = [STP, MOV_FP, MOV_X20_X1, MOV_X19_X0, bl(16, F), cbz_x0(ADDR + 20, ADDR + 28), MOV_X9_X19,
                 MOV_X0_X19, bl(32, G), LDP, RET]
        for text in self.lifts(words, gen_lift.FlatLifter):
            self.assertNotIn("X1", self.call_line(text, G))


    def test_flat_label_on_a_path_without_the_call(self):
        # stp; mov; mov x20, x1; cbz x0, L; bl F; b END; L: bl G; END: ldp; ret: F comes first in
        # address order but not on G's path, so x1 still holds the incoming value at G
        # (abi_facts' untouched, given here) and is named
        words = [STP, MOV_FP, MOV_X20_X1, cbz_x0(ADDR + 12, ADDR + 24), bl(16, F), 0x14000002, bl(24, G), LDP, RET]
        table = gen_lift.Lifter.facts

        class Facts:
            def facts(self, a):
                return table(a)

            def untouched(self, a):
                return {ADDR + 16: (F, frozenset({0, 1})), ADDR + 24: (G, frozenset({0, 1}))}

        gen_lift.Lifter.facts = Facts().facts
        for text in self.lifts(words, gen_lift.FlatLifter):
            self.assertIn("X1", self.call_line(text, G))


@unittest.skipIf(gen_lift is None, "pyelftools or capstone is not installed")
class Rounds(unittest.TestCase):
    """lift_rounds: a function matched only by a plain lift (odd attempt index) gets the ROUND2
    rewrites of its pass-through lifts, and a match there replaces the plain one; a pass-through
    match in the first round stands. lift_all and build_all are stubbed: attempts are strings."""

    def run_rounds(self, first, later):
        saved = gen_lift.lift_all, gen_lift.build_all, gen_lift.Lifter.facts
        self.addCleanup(lambda: (setattr(gen_lift, "lift_all", saved[0]), setattr(gen_lift, "build_all", saved[1]),
                                 setattr(gen_lift.Lifter, "facts", saved[2])))
        gen_lift.Lifter.facts = {}.get  # (byte mode with facts: attempts alternate pass-through, plain)
        rounds = []

        def lift_all(orig, cands, why=None, opts=frozenset()):
            r = len(rounds)
            rounds.append(sorted(cands))
            return [{a: (f"r{r}k{k}", set()) for a in cands} for k in range(4)]

        def build_all(orig, lifted, work, best=None, reloc_bad=None, which=None):
            r = len(rounds) - 1
            match = first if r == 0 else later
            chosen = {}
            for k, d in enumerate(lifted):
                for a in d:
                    if k in match.get(a, ()) and a not in chosen:
                        chosen[a] = d[a]
                        if which is not None:
                            which[a] = k
            return chosen, {a for d in lifted for a in d}

        gen_lift.lift_all, gen_lift.build_all = lift_all, build_all
        chosen, _, _ = gen_lift.lift_rounds(None, [1, 2], Path("unused"))
        return {a: c[0] for a, c in chosen.items()}, rounds

    def test_plain_match_replaced_by_a_later_pass_through_one(self):
        # 1 matches only as plain (k=1) in the first round, as pass-through (k=0) in the next
        chosen, rounds = self.run_rounds({1: {1}, 2: {0}}, {1: {0, 1}})
        self.assertEqual(chosen, {1: "r1k0", 2: "r0k0"})
        self.assertEqual(rounds[1], [1])  # (2 matched with pass-through: not retried)

    def test_plain_match_kept_when_nothing_better(self):
        chosen, _ = self.run_rounds({1: {1}}, {1: {1}})  # (later rounds: plain only, ignored)
        self.assertEqual(chosen, {1: "r0k1"})


@unittest.skipIf(gen_lift is None, "pyelftools or capstone is not installed")
class Definitions(unittest.TestCase):
    """defined_outside's line match: leading attributes, nested parentheses included, are skipped,
    so a hand-written `__attribute__((visibility("hidden"))) void fn_X() {` counts as defining fn_X
    (src/engine/UnkStartupCallbacks.cpp's fn_7100001714 was lifted a second time)."""

    def name(self, line):
        m = gen_lift.NAME_RE.match(gen_lift.ATTR_RE.sub("", line))
        return m and m.group(1)

    def test_attribute_with_nested_parentheses(self):
        self.assertEqual(self.name('__attribute__((visibility("hidden"))) void fn_7100001714() {'), "7100001714")

    def test_several_attributes(self):
        self.assertEqual(self.name('__attribute__((noinline)) __attribute__((visibility("hidden"))) u32 fn_7100001718(int x) {'),
                         "7100001718")


if __name__ == "__main__":
    unittest.main()
