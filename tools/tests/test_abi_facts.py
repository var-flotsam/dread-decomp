"""tools/abi_facts.py's result_main on hand-assembled AArch64: which evidence of a caller reading
x0 or s0 after a call counts. No game data: each function is a few instruction words written
here, and the next callee's facts are given. Skipped when capstone is not installed; the hosted
build job has it.
"""

import struct
import unittest

from tools.tests import _path  # noqa: F401

try:
    import abi_facts
except ImportError:  # capstone missing
    abi_facts = None

RET = 0xD65F03C0
STR_S0_X19 = 0xBD000260      # str s0, [x19]


def bl(at: int, target: int) -> int:
    return 0x94000000 | (((target - at) >> 2) & 0x3FFFFFF)


def code(at: int, *words) -> bytes:
    """Words, where a callable is given its own address (for a branch)."""
    return b"".join(struct.pack("<I", w(at + 4 * i) if callable(w) else w) for i, w in enumerate(words))


VOID, NEXT, CALLER, READER, FLOAT = 0x1000, 0x2000, 0x3000, 0x4000, 0x5000
FUNCTIONS = {
    VOID: code(VOID, RET),
    NEXT: code(NEXT, RET),
    FLOAT: code(FLOAT, RET),
    CALLER: code(CALLER, lambda a: bl(a, VOID), lambda a: bl(a, NEXT), RET),   # passes x0, s0 on
    READER: code(READER, lambda a: bl(a, FLOAT), STR_S0_X19, RET),             # reads s0 itself
}


class FakeOriginal:
    def __init__(self, functions: dict[int, bytes]):
        self.code = functions
        self.sizes = {a: len(c) for a, c in functions.items()}

    def function(self, address: int) -> bytes:
        return self.code[address]

    def plt_import(self, address: int):
        return None


class FakeGraph:
    callers = {VOID: [CALLER], FLOAT: [READER]}


@unittest.skipIf(abi_facts is None, "capstone is not installed")
class ResultMain(unittest.TestCase):
    def facts(self, args: set, evidence: set):
        af = abi_facts.AbiFacts(FakeOriginal(FUNCTIONS), rows={}, graph=FakeGraph())
        af._facts[NEXT] = abi_facts.Facts(frozenset(args), False)
        af._evidence[NEXT] = frozenset(evidence)
        return af

    def test_passed_on_float(self):
        af = self.facts({0, 100}, {0, 100})
        self.assertEqual(af.result_main(VOID), frozenset({0, 100}))

    def test_next_callee_no_caller_sets_s0(self):
        af = self.facts({0, 100}, {0})
        self.assertEqual(af.result_main(VOID), frozenset({0}))

    def test_next_callee_variadic(self):
        af = self.facts({0} | set(abi_facts.FLOAT_ARGS), {0, 100})
        self.assertEqual(af.result_main(VOID), frozenset({0}))

    def test_direct_read(self):
        af = self.facts(set(), set())
        self.assertEqual(af.result_main(FLOAT), frozenset({100}))


if __name__ == "__main__":
    unittest.main()
