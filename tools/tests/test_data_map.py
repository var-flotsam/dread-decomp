"""tools/data_map.py: the addresses code names, and the units' data spans built from them.

Hand-assembled instructions and made-up addresses; no game data.
"""

import struct
import unittest

from tools.tests import _path  # noqa: F401
import data_map


def adrp(rd: int, pc: int, target_page: int) -> int:
    imm = ((target_page - (pc & ~0xFFF)) >> 12) & 0x1FFFFF
    return 0x90000000 | (imm & 3) << 29 | (imm >> 2) << 5 | rd


def add(rd: int, rn: int, imm: int) -> int:
    return 0x91000000 | imm << 10 | rn << 5 | rd


def ldr64(rt: int, rn: int, offset: int) -> int:
    return 0xF9400000 | (offset // 8) << 10 | rn << 5 | rt


def str32(rt: int, rn: int, offset: int) -> int:
    return 0xB9000000 | (offset // 4) << 10 | rn << 5 | rt


BL = 0x94000010
MOV_X8_X0 = 0xAA0003E8
NOP = 0xD503201F
PC = 0x7100001000


def code(*words: int) -> bytes:
    return struct.pack(f"<{len(words)}I", *words)


class NamedAddresses(unittest.TestCase):
    def named(self, *words, got=None):
        return data_map.named_addresses(code(*words), PC, (got or {}).get)

    def test_adrp_with_add_and_with_a_store(self):
        self.assertEqual(self.named(adrp(8, PC, 0x7101a20000), add(8, 8, 0x1b8)), [0x7101a201b8])
        self.assertEqual(self.named(adrp(9, PC, 0x7101d50000), NOP, str32(1, 9, 0x24)), [0x7101d50024])

    def test_a_got_slot_is_followed(self):
        words = (adrp(8, PC, 0x7101cb9000), ldr64(8, 8, 0x40))
        self.assertEqual(self.named(*words, got={0x7101cb9040: 0x7172700008}), [0x7172700008])
        self.assertEqual(self.named(*words), [0x7101cb9040])            # not a slot: the load's own address

    def test_a_page_is_forgotten(self):
        # overwritten, or across a call for a caller-saved register; kept across a call in x19
        self.assertEqual(self.named(adrp(8, PC, 0x7101a20000), MOV_X8_X0, add(9, 8, 0x10)), [])
        self.assertEqual(self.named(adrp(8, PC, 0x7101a20000), BL, add(9, 8, 0x10)), [])
        self.assertEqual(self.named(adrp(19, PC, 0x7101a20000), BL, add(0, 19, 0x10)), [0x7101a20010])
        # once the full address is formed, later uses of the register are not page uses
        self.assertEqual(self.named(adrp(8, PC, 0x7101a20000), add(8, 8, 0x10), add(9, 8, 0x20)), [0x7101a20010])


class Spans(unittest.TestCase):
    SECTIONS = {".data": (0x1000, 0x2000), ".bss": (0x2000, 0x3000)}

    def spans(self, named):
        found, evidence = data_map.spans({a: set(u) for a, u in named.items()}, self.SECTIONS)
        return [(s.section, s.start, s.end, s.unit, s.anchors) for s in found], evidence

    def test_spans_follow_the_anchors_in_unit_order(self):
        found, evidence = self.spans({0x1000: [3], 0x1010: [3], 0x1020: [5], 0x1030: [5],
                                      0x1018: [9],                 # breaks the order: dropped
                                      0x2000: [3], 0x2040: [3, 5], 0x2080: [5]})
        self.assertEqual(found, [(".data", 0x1000, 0x1010, 3, 2), (".data", 0x1020, 0x1030, 5, 2),
                                 (".bss", 0x2000, 0x2000, 3, 1), (".bss", 0x2080, 0x2080, 5, 1)])   # 0x2040: both name it
        self.assertEqual(evidence, {".data": (5, 4), ".bss": (2, 2)})

    def test_between_two_spans(self):
        # 0x1010 is named by unit 3 and by unit 8 (not between 3 and 5): unit 3's. 0x1018 by both: nobody's.
        # 0x1020 by unit 5 and unit 1: unit 5's, from the right.
        found, _ = self.spans({0x1000: [3], 0x1010: [3, 8], 0x1018: [3, 5], 0x1020: [1, 5], 0x1030: [5]})
        self.assertEqual(found, [(".data", 0x1000, 0x1010, 3, 1), (".data", 0x1020, 0x1030, 5, 1)])
        find = data_map.finder([data_map.Span(*f) for f in found])
        self.assertEqual([find(a) for a in (0x1000, 0x1008, 0x1010, 0x1018, 0x1020, 0x1034, 0xfff)],
                         [3, 3, 3, None, 5, None, None])

    def test_an_address_of_the_other_unit_stops_the_extension(self):
        # unit 5 names 0x1010 before unit 3 names 0x1018: neither extends past the other
        found, _ = self.spans({0x1000: [3], 0x1010: [5, 9], 0x1018: [3, 9], 0x1030: [5]})
        self.assertEqual(found, [(".data", 0x1000, 0x1000, 3, 1), (".data", 0x1030, 0x1030, 5, 1)])


if __name__ == "__main__":
    unittest.main()
