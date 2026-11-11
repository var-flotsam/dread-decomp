"""tools/worktree_setup.py: which inputs a worktree gets, and that nothing existing is replaced.

Everything happens between two temporary folders standing in for the checkouts; no git, no dump.
"""

import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import worktree_setup as ws


class Provide(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.src, self.dst = Path(self.tmp.name) / "main", Path(self.tmp.name) / "tree"
        (self.src / "orig" / "2.1.0").mkdir(parents=True)
        (self.src / "orig" / "2.1.0" / "main.elf").write_text("dump")
        (self.src / "build" / "recon" / "2.1.0").mkdir(parents=True)
        (self.src / "build" / "recon" / "2.1.0" / "main_eh_functions.json").write_text("[]")
        (self.src / "build" / "port").mkdir()
        for name in ("abi_facts_v9.pickle", "abi_facts_v15.pickle", "pcode_cache.pickle"):
            (self.src / "build" / "port" / name).write_text(name)
        self.dst.mkdir()

    def states(self, oracle=False, dry=False) -> dict[str, str]:
        return {i.path: ws.provide(i, self.src, self.dst, dry) for i in ws.items(self.src, "2.1.0", oracle)}

    def test_links_and_copies(self):
        s = self.states()
        self.assertEqual(s["orig"], "linked")
        self.assertEqual(s["build/recon/2.1.0"], "copied")
        # The link reads the source's files; the copy is a file of its own.
        self.assertEqual((self.dst / "orig" / "2.1.0" / "main.elf").read_text(), "dump")
        copy = self.dst / "build" / "recon" / "2.1.0" / "main_eh_functions.json"
        copy.write_text("changed")
        self.assertEqual((self.src / "build" / "recon" / "2.1.0" / "main_eh_functions.json").read_text(), "[]")

    def test_missing_sources_are_reported(self):
        s = self.states()
        self.assertEqual(s[".venv"], "no source")                # required: main() fails on it
        self.assertEqual(s["build/ghidra_export"], "no source")  # optional
        required = {i.path: i.required for i in ws.items(self.src, "2.1.0", False)}
        self.assertTrue(required[".venv"])
        self.assertFalse(required["build/ghidra_export"])

    def test_existing_paths_are_kept(self):
        (self.dst / "orig").mkdir()
        (self.dst / "orig" / "mine").write_text("keep")
        self.assertEqual(self.states()["orig"], "present")
        self.assertEqual((self.dst / "orig" / "mine").read_text(), "keep")
        self.assertEqual(self.states()["build/recon/2.1.0"], "present")  # a second run changes nothing

    def test_check_changes_nothing(self):
        s = self.states(dry=True)
        self.assertEqual((s["orig"], s["build/recon/2.1.0"]), ("missing", "missing"))
        self.assertEqual(list(self.dst.iterdir()), [])

    def test_oracle_takes_the_newest_facts(self):
        s = self.states(oracle=True)
        self.assertEqual(s["build/port/abi_facts_v15.pickle"], "copied")
        self.assertNotIn("build/port/abi_facts_v9.pickle", s)
        self.assertEqual((self.dst / "build" / "port" / "pcode_cache.pickle").read_text(), "pcode_cache.pickle")

    def test_oracle_without_facts(self):
        for p in (self.src / "build" / "port").glob("abi_facts_*"):
            p.unlink()
        self.assertEqual(self.states(oracle=True)["build/port/abi_facts_v*.pickle"], "no source")


if __name__ == "__main__":
    unittest.main()
