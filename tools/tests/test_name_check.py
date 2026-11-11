"""tools/name_check.py: which names point to more than one address. Made-up references; no game data."""

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


if __name__ == "__main__":
    unittest.main()
