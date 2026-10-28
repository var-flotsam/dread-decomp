"""tools/policy_check.py's content rules. Samples are assembled at run time so this file itself
passes the check."""

import unittest

from tools.tests import _path  # noqa: F401
import policy_check


class Rules(unittest.TestCase):
    def test_listing(self):
        lines = "".join(f"71{i:08x}: " + "ld" + "r x0, [x1]\n" for i in range(4))
        self.assertTrue(policy_check.LISTING.search(lines))
        self.assertFalse(policy_check.LISTING.search(lines.split("\n", 1)[0] + "\n"))

    def test_keys(self):
        k = "".join(f"key_{i} = " + "ab" * 16 + "\n" for i in range(2))
        self.assertTrue(policy_check.KEYS.search(k))

    def test_home_paths(self):
        self.assertTrue(policy_check.HOME_PATH.search("C:" + "\\Users\\" + "someone\\x"))
        self.assertTrue(policy_check.HOME_PATH.search("/" + "home/" + "someone/x"))
        self.assertFalse(policy_check.HOME_PATH.search("%USERPROFILE%\\x and $HOME/x"))

    def test_email(self):
        self.assertTrue(policy_check.EMAIL.search("someone" + "@" + "mail.test.org"))
        self.assertFalse(policy_check.EMAIL.search("123+someone" + "@" + "users.noreply.github.com"))

    def test_local_dirs_and_types(self):
        self.assertTrue(policy_check.problems("orig/2.1.0/x"))
        self.assertTrue(policy_check.problems("src/x.nso"))


if __name__ == "__main__":
    unittest.main()
