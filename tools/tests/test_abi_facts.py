"""tools/abi_facts.py's callee-side result check (AbiFacts.defines) on hand-assembled AArch64.

No game data: each function is a few instruction words written here. Skipped when capstone is
not installed; the hosted build job has it.
"""

import struct
import unittest

from tools.tests import _path  # noqa: F401

try:
    import abi_facts
except ImportError:  # capstone missing
    abi_facts = None

RET = 0xD65F03C0
FMOV_S0_1 = 0x1E2E1000       # fmov s0, #1.0
FADD_S0 = 0x1E212800         # fadd s0, s0, s1
STR_S0_X19 = 0xBD000260      # str s0, [x19]
BLR_X8 = 0xD63F0100


def bl(at: int, target: int) -> int:
    return 0x94000000 | (((target - at) >> 2) & 0x3FFFFFF)


def b(at: int, target: int) -> int:
    return 0x14000000 | (((target - at) >> 2) & 0x3FFFFFF)


def code(at: int, *words) -> bytes:
    """Words, where a callable is given its own address (for a branch)."""
    return b"".join(struct.pack("<I", w(at + 4 * i) if callable(w) else w) for i, w in enumerate(words))


SETTER, STORE, FLOAT, CALLS_FLOAT, TAIL_STORE, INDIRECT, CALLER_SETTER, CALLER_FLOAT, SINF = (
    0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000, 0x8000, 0x9000)
FUNCTIONS = {
    # a void setter: s0 is an argument for a callee that stores it and defines nothing
    SETTER: code(SETTER, FMOV_S0_1, lambda a: bl(a, STORE), RET),
    STORE: code(STORE, STR_S0_X19, RET),
    FLOAT: code(FLOAT, FADD_S0, RET),
    CALLS_FLOAT: code(CALLS_FLOAT, lambda a: bl(a, FLOAT), RET),
    TAIL_STORE: code(TAIL_STORE, lambda a: b(a, STORE)),
    INDIRECT: code(INDIRECT, BLR_X8, RET),
    # callers that read s0 right after the call
    CALLER_SETTER: code(CALLER_SETTER, lambda a: bl(a, SETTER), STR_S0_X19, RET),
    CALLER_FLOAT: code(CALLER_FLOAT, lambda a: bl(a, FLOAT), STR_S0_X19, RET),
}


class FakeOriginal:
    def __init__(self, functions: dict[int, bytes]):
        self.code = functions
        self.sizes = {a: len(c) for a, c in functions.items()}

    def function(self, address: int) -> bytes:
        return self.code[address]

    def plt_import(self, address: int):
        return "sinf" if address == SINF else None


class FakeGraph:
    callers = {SETTER: [CALLER_SETTER], FLOAT: [CALLER_FLOAT, CALLS_FLOAT]}


@unittest.skipIf(abi_facts is None, "capstone is not installed")
class Defines(unittest.TestCase):
    def setUp(self):
        self.af = abi_facts.AbiFacts(FakeOriginal(FUNCTIONS), rows={}, graph=FakeGraph())

    def test_own_write(self):
        self.assertIs(self.af.defines(FLOAT, 100), True)

    def test_through_a_call(self):
        self.assertIs(self.af.defines(CALLS_FLOAT, 100), True)

    def test_argument_and_leftover(self):
        self.assertIs(self.af.defines(STORE, 100), False)    # its entry s0 only
        self.assertIs(self.af.defines(SETTER, 100), False)   # what a void callee left
        self.assertIs(self.af.defines(TAIL_STORE, 100), False)

    def test_unknown(self):
        self.assertIsNone(self.af.defines(INDIRECT, 100))

    def test_import(self):
        self.assertIs(self.af.defines(SINF, 100), True)

    def test_result_main_drops_a_read_of_no_result(self):
        self.assertEqual(self.af.result_main(SETTER), frozenset())
        self.assertEqual(self.af.result_main(FLOAT), frozenset({100}))


if __name__ == "__main__":
    unittest.main()
