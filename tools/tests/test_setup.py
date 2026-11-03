"""tools/setup.py on a made-up ExeFS/RomFS in a temporary folder (no game data).

The repository paths (ROOT, ORIG, HASHES) are patched to the temporary folder, so nothing is
written to data/hashes.json, orig/ or build/ of the checkout.
"""

import contextlib
import hashlib
import io
import json
import shutil
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import setup

BUILD_ID = bytes(range(0x20))


def fake_nso(build_id=BUILD_ID) -> bytes:
    return b"NSO0" + bytes(0x3C) + build_id + b"\0" * 16


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.exefs = self.root / "dump" / "exefs"
        self.romfs = self.root / "dump" / "romfs"
        self.exefs.mkdir(parents=True)
        (self.romfs / "system").mkdir(parents=True)
        (self.exefs / "main").write_bytes(fake_nso())
        for name in ("main.npdm", "rtld", "sdk", "subsdk1", "subsdk0"):
            (self.exefs / name).write_bytes(name.encode())
        (self.romfs / "a.txt").write_bytes(b"alpha")
        (self.romfs / "system" / "b.bin").write_bytes(b"\0" * 10)
        for name, value in (("ROOT", self.root), ("ORIG", self.root / "orig"),
                            ("HASHES", self.root / "data" / "hashes.json")):
            p = mock.patch.object(setup, name, value)
            p.start()
            self.addCleanup(p.stop)

    def run_main(self, *argv):
        out, err = io.StringIO(), io.StringIO()
        with mock.patch("sys.argv", ["setup.py", *argv]), contextlib.redirect_stdout(out), \
                contextlib.redirect_stderr(err):
            rc = setup.main()
        return rc, out.getvalue()


class Pieces(Fixture):
    def test_sha256_file(self):
        self.assertEqual(setup.sha256_file(self.romfs / "a.txt"), hashlib.sha256(b"alpha").hexdigest())

    def test_nso_build_id(self):
        self.assertEqual(setup.nso_build_id(self.exefs / "main"), BUILD_ID.hex())

    def test_nso_build_id_bad_magic(self):
        with self.assertRaisesRegex(setup.SetupError, "not an NSO"):
            setup.nso_build_id(self.exefs / "rtld")

    def test_exefs_files_required_then_sorted_subsdks(self):
        self.assertEqual(setup.exefs_files(self.exefs), ["main", "main.npdm", "rtld", "sdk", "subsdk0", "subsdk1"])

    def test_exefs_files_missing(self):
        (self.exefs / "sdk").unlink()
        with self.assertRaisesRegex(setup.SetupError, "missing: sdk"):
            setup.exefs_files(self.exefs)

    def test_hash_romfs_manifest(self):
        summary, lines = setup.hash_romfs(self.romfs)
        self.assertEqual(lines, [f"{hashlib.sha256(b'alpha').hexdigest()}  a.txt",
                                 f"{hashlib.sha256(bytes(10)).hexdigest()}  system/b.bin"])
        self.assertEqual(summary["file_count"], 2)
        self.assertEqual(summary["total_bytes"], 15)
        self.assertEqual(summary["manifest_sha256"],
                         hashlib.sha256(("\n".join(lines) + "\n").encode()).hexdigest())

    def test_hash_romfs_empty_or_missing(self):
        empty = self.root / "empty"
        empty.mkdir()
        with self.assertRaisesRegex(setup.SetupError, "empty"):
            setup.hash_romfs(empty)
        with self.assertRaisesRegex(setup.SetupError, "not a directory"):
            setup.hash_romfs(self.root / "nowhere")

    def test_which_version(self):
        db = {"1.0.0": {"exefs": {"main_build_id": "aa"}}, "2.1.0": {"exefs": {"main_build_id": "bb"}}}
        self.assertEqual(setup.which_version(db, "bb"), "2.1.0")
        self.assertIsNone(setup.which_version(db, "cc"))

    def test_compare_exefs(self):
        exp = {"main_build_id": "aa", "files": {"main": "1", "sdk": "2", "rtld": "3"}}
        act = {"main_build_id": "bb", "files": {"main": "1", "sdk": "9", "subsdk0": "4"}}
        self.assertEqual(setup.compare(exp, act, "exefs"), [
            "main build ID bb != expected aa", "exefs/rtld: missing", "exefs/sdk: sha256 mismatch",
            "exefs/subsdk0: unexpected file"])

    def test_compare_romfs(self):
        exp = {"file_count": 2, "total_bytes": 15, "manifest_sha256": "x"}
        self.assertEqual(setup.compare(exp, dict(exp), "romfs"), [])
        self.assertEqual(setup.compare(exp, {**exp, "file_count": 3}, "romfs"), ["romfs file_count: 3 != expected 2"])

    def test_save_and_load_hashes(self):
        self.assertEqual(setup.load_hashes(), {})
        setup.save_hashes({"b": 1, "a": 2})
        text = setup.HASHES.read_text()
        self.assertTrue(text.index('"a"') < text.index('"b"'))
        self.assertEqual(setup.load_hashes(), {"a": 2, "b": 1})


class Main(Fixture):
    def test_first_run_records_unconfirmed_and_copies(self):
        rc, out = self.run_main("--version", "9.9.9", "--exefs", str(self.exefs), "--romfs", str(self.romfs))
        self.assertEqual(rc, 0)
        db = json.loads(setup.HASHES.read_text())
        self.assertFalse(db["9.9.9"]["confirmed"])
        self.assertEqual(db["9.9.9"]["exefs"]["main_build_id"], BUILD_ID.hex())
        self.assertTrue((self.root / "orig" / "9.9.9" / "romfs" / "system" / "b.bin").is_file())
        self.assertTrue((self.root / "build" / "9.9.9" / "romfs_manifest.txt").is_file())
        self.assertIn("First run", out)

    def test_second_run_verifies_and_confirms(self):
        args = ("--version", "9.9.9", "--exefs", str(self.exefs), "--romfs", str(self.romfs))
        self.run_main(*args)
        rc, out = self.run_main(*args, "--confirm")
        self.assertEqual(rc, 0)
        self.assertIn("All hashes match", out)
        self.assertTrue(json.loads(setup.HASHES.read_text())["9.9.9"]["confirmed"])
        rc, out = self.run_main("--version", "9.9.9", "--verify-only")
        self.assertEqual(rc, 0)

    def test_changed_file_fails_verification(self):
        args = ("--version", "9.9.9", "--exefs", str(self.exefs), "--romfs", str(self.romfs))
        self.run_main(*args)
        (self.romfs / "a.txt").write_bytes(b"beta!")
        with self.assertRaisesRegex(setup.SetupError, "manifest_sha256"):
            self.run_main(*args)

    def test_dump_of_another_version_rejected(self):
        args = ("--exefs", str(self.exefs), "--romfs", str(self.romfs))
        self.run_main("--version", "1.0.0", *args)
        with self.assertRaisesRegex(setup.SetupError, "matches version 1.0.0"):
            self.run_main("--version", "2.0.0", *args)

    def test_recorded_version_with_other_build_id_rejected(self):
        args = ("--version", "1.0.0", "--exefs", str(self.exefs), "--romfs", str(self.romfs))
        self.run_main(*args)
        (self.exefs / "main").write_bytes(fake_nso(b"\xff" * 0x20))
        with self.assertRaisesRegex(setup.SetupError, "does not match the recorded build ID"):
            self.run_main(*args)

    def test_verify_only_without_record(self):
        dest = self.root / "orig" / "3.0.0"
        shutil.copytree(self.exefs, dest / "exefs")
        shutil.copytree(self.romfs, dest / "romfs")
        with self.assertRaisesRegex(setup.SetupError, "no recorded hashes"):
            self.run_main("--version", "3.0.0", "--verify-only")

    def test_dump_paths_required(self):
        with self.assertRaises(SystemExit):
            self.run_main("--version", "1.0.0", "--exefs", str(self.exefs))


if __name__ == "__main__":
    unittest.main()
