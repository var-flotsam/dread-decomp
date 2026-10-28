"""Invariants of the tracked metadata in data/ (no game data needed)."""

import unittest

from tools.tests import _path  # noqa: F401
from dread import functions

STATUSES = set("OmMWUL")
NAME_SOURCES = {"symbol", "library", "rtti", "reflection", "string", "unknown"}


class FunctionList(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.rows = functions.rows()

    def test_columns(self):
        for f in functions.files():
            with f.open(encoding="utf-8") as fh:
                self.assertEqual(fh.readline().strip().split(","), functions.COLUMNS, f.name)

    def test_addresses_unique_and_sorted(self):
        addrs = [int(r["address"], 16) for r in self.rows]
        self.assertEqual(addrs, sorted(addrs))
        self.assertEqual(len(addrs), len(set(addrs)))

    def test_values(self):
        bad = [r["address"] for r in self.rows
               if r["status"] not in STATUSES or r["name_source"] not in NAME_SOURCES or int(r["size"]) <= 0]
        self.assertEqual(bad[:10], [])

    def test_one_file_per_mib(self):
        for f in functions.files():
            mib = int(f.stem.split("_")[1], 16)
            for r in functions.read(f):
                self.assertEqual((int(r["address"], 16) - 0x7100000000) >> 20, mib, f"{f.name} {r['address']}")


if __name__ == "__main__":
    unittest.main()
