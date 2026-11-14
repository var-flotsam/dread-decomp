"""tools/trymatch.py's main(): object placement, exit status and the RELOC check, with the compiler,
the function list and the original binary replaced by stand-ins (no game data, no toolchain).
"""

import contextlib
import io
import subprocess
import tempfile
import types
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import asmdiff
import trymatch

RET, BL = bytes.fromhex("c0035fd6"), bytes.fromhex("00000094")  # ret; bl (little-endian words)


def res(symbol, address, matching=True):
    return asmdiff.FunctionResult(symbol, address, 8, 8, [] if matching else [1])


class Main(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.src = self.root / "scratch.cpp"
        self.src.write_text("// made up\n")
        self.build = subprocess.CompletedProcess([], 0, "", "")
        self.objects = [asmdiff.ObjFunction("fn_7100000100", b"\0" * 8, {}),
                        asmdiff.ObjFunction("helper", b"\0" * 8, {})]
        self.results = [res("fn_7100000100", 0x7100000100)]
        self.reloc_mismatches = []
        self.commands = []

        def cxx_command(src, obj, extra):
            self.commands.append((src, obj, extra))
            return ["clang", str(src), "-o", str(obj)]

        def diff_with_targets(funcs, original, by_name, source=None):
            return self.results, {}, {}, None

        for p in (mock.patch.object(trymatch, "ROOT", self.root),
                  mock.patch.object(trymatch.buildcfg, "cxx_command", cxx_command),
                  mock.patch.object(trymatch.subprocess, "run", lambda *a, **k: self.build),
                  mock.patch.object(trymatch.check, "load_csv", lambda: ({}, {})),
                  mock.patch.object(trymatch.asmdiff, "read_object", lambda obj: self.objects),
                  mock.patch.object(trymatch.asmdiff, "Original",
                                    lambda v: types.SimpleNamespace(function=lambda a: RET)),
                  mock.patch.object(trymatch.check, "diff_with_targets", diff_with_targets),
                  mock.patch.object(trymatch.check, "special_mismatches", lambda *a, **k: []),
                  mock.patch.object(trymatch.asmdiff, "check_relocs",
                                    lambda r, o, t: types.SimpleNamespace(mismatches=self.reloc_mismatches)),
                  mock.patch.object(trymatch.asmdiff, "format_mismatch", lambda m, sym: f"{sym} differs")):
            p.start()
            self.addCleanup(p.stop)

    def run_main(self, *argv):
        out = io.StringIO()
        with mock.patch("sys.argv", ["trymatch.py", "--version", "9.9.9", *argv]), contextlib.redirect_stdout(out):
            rc = trymatch.main()
        return rc, out.getvalue()

    def test_all_match(self):
        rc, out = self.run_main(str(self.src))
        self.assertEqual(rc, 0)
        self.assertIn("MATCH  0x7100000100  fn_7100000100", out)
        self.assertIn("1/1 functions match (1 not mapped to an address)", out)

    def test_default_object_path(self):
        self.run_main(str(self.src))
        src, obj, extra = self.commands[0]
        self.assertEqual(src, self.src.resolve())
        self.assertEqual(obj, self.root / "build" / "trymatch" / "9.9.9" / "scratch.o")
        self.assertTrue(obj.parent.is_dir())
        self.assertEqual(extra, [])

    def test_obj_dir_and_extra_flags(self):
        objdir = self.root / "objs"
        self.run_main("--obj-dir", str(objdir), str(self.src), "--", "-fno-inline", "-O2")
        _, obj, extra = self.commands[0]
        self.assertEqual(obj, objdir / "scratch.o")
        self.assertEqual(extra, ["-fno-inline", "-O2"])

    def test_build_failure_is_exit_2(self):
        self.build = subprocess.CompletedProcess([], 1, "", "scratch.cpp:1: error: oops\n")
        rc, out = self.run_main(str(self.src))
        self.assertEqual(rc, 2)
        self.assertIn("error: oops", out)

    def test_mismatch_is_exit_1(self):
        self.results = [res("fn_7100000100", 0x7100000100), res("fn_7100000200", 0x7100000200, False)]
        self.objects.append(asmdiff.ObjFunction("fn_7100000200", BL + RET, {}))
        rc, out = self.run_main(str(self.src))
        self.assertEqual(rc, 1)
        self.assertIn("50.0%  0x7100000200", out)
        self.assertIn("KIND   calls (original 0, ours 1)", out)
        self.assertIn("1/2 functions match", out)

    def test_wrong_relocation_target_counts_as_mismatch(self):
        self.reloc_mismatches = [types.SimpleNamespace(symbol="fn_7100000300")]
        rc, out = self.run_main(str(self.src))
        self.assertEqual(rc, 1)
        self.assertIn("  RELOC  fn_7100000300 differs", out)
        self.assertIn("0/1 functions match", out)
        self.assertIn("1 with a wrong relocation target", out)

    def test_no_reloc_skips_check(self):
        self.reloc_mismatches = [types.SimpleNamespace(symbol="fn_7100000300")]
        rc, out = self.run_main("--no-reloc", str(self.src))
        self.assertEqual(rc, 0)
        self.assertNotIn("RELOC", out)

    def test_nothing_mapped_is_exit_1(self):
        self.results = []
        rc, out = self.run_main(str(self.src))
        self.assertEqual(rc, 1)
        self.assertIn("0/0 functions match (2 not mapped to an address)", out)


if __name__ == "__main__":
    unittest.main()
