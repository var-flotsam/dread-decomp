"""tools/verify_dump.py's NSO and main.npdm checks on synthetic files (no game data).

verify_dump imports mercury-engine-data-structures at module level for the RomFS check, which the
CI venv does not install; when it is missing, small stand-in modules are put in sys.modules for
the import only. check_romfs is not tested here: it needs a real files.toc.
"""

import contextlib
import io
import struct
import sys
import tempfile
import types
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
from tools.tests.test_nso2elf import build_nso


def _import_verify_dump():
    try:
        import verify_dump
        return verify_dump
    except ImportError:
        pass
    names = {
        "mercury_engine_data_structures": {},
        "mercury_engine_data_structures.base_resource": {"resolve_asset_id": lambda *a: 0},
        "mercury_engine_data_structures.formats": {},
        "mercury_engine_data_structures.formats.pkg": {"Pkg": object},
        "mercury_engine_data_structures.formats.toc": {"Toc": object},
        "mercury_engine_data_structures.game_check": {"Game": types.SimpleNamespace(DREAD="dread")},
    }
    saved = {n: sys.modules.get(n) for n in names}
    try:
        for n, attrs in names.items():
            sys.modules[n] = types.SimpleNamespace(**attrs)
        import verify_dump
        return verify_dump
    finally:
        for n, old in saved.items():
            if old is None:
                sys.modules.pop(n, None)
            else:
                sys.modules[n] = old


verify_dump = _import_verify_dump()


def npdm(program_id: int, name=b"Fixture", magic=b"META", aci_magic=b"ACI0") -> bytes:
    raw = bytearray(0x200)
    raw[0:4] = magic
    raw[0x20:0x20 + len(name)] = name
    struct.pack_into("<II", raw, 0x70, 0x100, 0x40)
    raw[0x100:0x104] = aci_magic
    struct.pack_into("<Q", raw, 0x110, program_id)
    return bytes(raw)


def quiet(fn, *args):
    with contextlib.redirect_stdout(io.StringIO()):
        return fn(*args)


class Npdm(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name) / "main.npdm"

    def check(self, raw):
        self.path.write_bytes(raw)
        return quiet(verify_dump.check_npdm, self.path)

    def test_dread_title_id_passes(self):
        self.assertEqual(self.check(npdm(verify_dump.DREAD_TITLE_ID)), [])

    def test_other_title_id_fails(self):
        errs = self.check(npdm(0x0100000000001000))
        self.assertEqual(len(errs), 1)
        self.assertIn("not Metroid Dread", errs[0])

    def test_bad_magic(self):
        self.assertIn("bad magic", self.check(npdm(verify_dump.DREAD_TITLE_ID, magic=b"ATEM"))[0])

    def test_missing_aci0(self):
        self.assertIn("ACI0", self.check(npdm(verify_dump.DREAD_TITLE_ID, aci_magic=b"XXXX"))[0])


class Nsos(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.exefs = Path(self.tmp.name)
        self.segs = [(0, b"\x01" * 0x40), (0x40, b"\x02" * 0x20), (0x60, b"\x03" * 0x20)]

    def test_no_files_is_no_errors(self):
        self.assertEqual(quiet(verify_dump.check_nsos, self.exefs), [])

    def test_good_hashes_pass(self):
        for name in ("main", "sdk"):
            (self.exefs / name).write_bytes(build_nso(self.segs))
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            self.assertEqual(verify_dump.check_nsos(self.exefs), [])
        self.assertIn("text, rodata, data", out.getvalue())
        self.assertIn("ABABAB", out.getvalue())  # build ID prefix

    def test_bad_hash_reported(self):
        (self.exefs / "main").write_bytes(build_nso(self.segs, bad_hash=1))
        errs = quiet(verify_dump.check_nsos, self.exefs)
        self.assertEqual(len(errs), 1)
        self.assertIn("rodata sha256", errs[0])

    def test_unchecked_segment_not_verified(self):
        (self.exefs / "main").write_bytes(build_nso(self.segs, bad_hash=2, check_hash=(True, True, False)))
        self.assertEqual(quiet(verify_dump.check_nsos, self.exefs), [])

    def test_only_known_names_checked(self):
        (self.exefs / "other").write_bytes(b"not an nso")
        self.assertEqual(quiet(verify_dump.check_nsos, self.exefs), [])


if __name__ == "__main__":
    unittest.main()
