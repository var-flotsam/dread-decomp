"""tools/name_check.py: which names point to more than one address. Made-up references; no game data."""

import struct
import unittest

from tools.tests import _path  # noqa: F401
import name_check


class Inconsistent(unittest.TestCase):
    def test_a_name_with_one_address_is_fine(self):
        uses = {("", "_ZN1AD1Ev"): [(0x7100001000, "src/a.cpp", 0x7100002000), (0x7100001000, "src/b.cpp", 0x7100003000)]}
        self.assertEqual(name_check.inconsistent(uses), [])

    def test_the_references_off_the_usual_address_are_reported(self):
        uses = {
            ("", "_ZN1AD1Ev"): [(0x7100001000, "src/a.cpp", 0x7100002000), (0x7100001000, "src/b.cpp", 0x7100003000),
                                (0x7100009000, "src/c.cpp", 0x7100004000)],
            ("", "_ZTV1B"): [(0x7101a00010, "src/a.cpp", 0x7100002000), (0x7101a00090, "src/d.cpp", 0x7100005000),
                             (0x7101a00050, "src/c.cpp", 0x7100004000)],
        }
        self.assertEqual(name_check.inconsistent(uses), [
            # a tie goes to the lowest address; the names with the most stray references come first
            ("_ZTV1B", 0x7101a00010, [(0x7101a00050, "src/c.cpp", 0x7100004000), (0x7101a00090, "src/d.cpp", 0x7100005000)]),
            ("_ZN1AD1Ev", 0x7100001000, [(0x7100009000, "src/c.cpp", 0x7100004000)]),
        ])

    def test_a_local_static_is_compared_within_its_source(self):
        self.assertTrue(name_check.is_local("_ZZ13fn_7100000100vE6holder"))
        self.assertTrue(name_check.is_local("_ZGVZ13fn_7100000100vE6holder"))
        self.assertFalse(name_check.is_local("_ZTV1B"))
        uses = {("src/a.cpp", "_ZZ1fvE1s"): [(0x7101d00000, "src/a.cpp", 0x7100002000)],
                ("src/b.cpp", "_ZZ1fvE1s"): [(0x7101d00040, "src/b.cpp", 0x7100003000)]}
        self.assertEqual(name_check.inconsistent(uses), [])

    def test_a_thunk_counts_as_its_target(self):
        words = {0x7100001000: 0x14000004, 0x7100001010: 0xD65F03C0, 0x7100002000: 0x17FFFC00}
        sizes = {0x7100001000: 4, 0x7100001010: 4, 0x7100002000: 12}
        self.assertEqual(name_check.through_thunks(0x7100001000, sizes.get, words.get), 0x7100001010)   # b +0x10
        self.assertEqual(name_check.through_thunks(0x7100001010, sizes.get, words.get), 0x7100001010)   # a ret
        self.assertEqual(name_check.through_thunks(0x7100002000, sizes.get, words.get), 0x7100002000)   # 12 bytes

    def test_copies_of_the_same_code_are_one_meaning(self):
        # bl +8; adrp x8; add x8, x8, #0x10; ldr x9, [x8, #8]; ret   and the same at other places with other targets
        a = struct.pack("<5I", 0x94000002, 0x90000008, 0x91004108, 0xF9400509, 0xD65F03C0)
        b = struct.pack("<5I", 0x94000400, 0xB0000048, 0x91012108, 0xF9400509, 0xD65F03C0)
        c = struct.pack("<5I", 0x94000002, 0x90000008, 0x91004108, 0xF9400909, 0xD65F03C0)   # another field offset
        self.assertEqual(name_check.masked(a), name_check.masked(b))
        self.assertNotEqual(name_check.masked(a), name_check.masked(c))
        code = {0x7100001000: name_check.masked(a), 0x7100002000: name_check.masked(b), 0x7100003000: name_check.masked(c)}
        uses = {("", "f"): [(0x7100001000, "src/a.cpp", 1), (0x7100002000, "src/b.cpp", 2)],
                ("", "g"): [(0x7100001000, "src/a.cpp", 1), (0x7100003000, "src/b.cpp", 2)],
                ("", "v"): [(0x7101a00000, "src/a.cpp", 1), (0x7101a00040, "src/b.cpp", 2)]}
        self.assertEqual([s for s, _a, _r in name_check.inconsistent(uses, code.get)], ["g", "v"])
        self.assertEqual(len(name_check.inconsistent(uses)), 3)


if __name__ == "__main__":
    unittest.main()
