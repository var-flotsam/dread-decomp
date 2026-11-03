"""tools/compiler_matrix.py: the configuration sets, and run_config/main with the compiler and the
diff against the original replaced by stand-ins (no game data, no toolchain).

ROOT is patched to a temporary folder holding made-up sources, so build/ of the checkout is untouched.
"""

import contextlib
import io
import json
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import asmdiff
import compiler_matrix


def result(match: bool, size=16, diff=()):
    return asmdiff.FunctionResult("fn_7100000100", 0x7100000100, size, size, list(diff))


class Sets(unittest.TestCase):
    def test_each_set_has_sources_opts_and_extras(self):
        self.assertEqual(set(compiler_matrix.SETS), {"leaf", "nonleaf", "atk"})
        for name, spec in compiler_matrix.SETS.items():
            self.assertTrue(spec["opt"], name)
            self.assertTrue(spec["extra"], name)
            for opt in spec["opt"]:
                self.assertRegex(opt, r"^-O[0-3s]$")

    def test_atk_reuses_nonleaf_axes(self):
        atk, nonleaf = compiler_matrix.SETS["atk"], compiler_matrix.SETS["nonleaf"]
        self.assertEqual(atk["sources"], "atk_id")
        self.assertEqual(atk["opt"], nonleaf["opt"])
        self.assertEqual(atk["extra"], nonleaf["extra"])

    def test_nonleaf_extras_all_use_cortex_a57(self):
        for key, flags in compiler_matrix.SETS["nonleaf"]["extra"].items():
            self.assertIn("-mcpu=cortex-a57", flags, key)

    def test_base_flags_compile_only_for_aarch64(self):
        self.assertIn("--target=aarch64-none-elf", compiler_matrix.BASE_FLAGS)
        self.assertEqual(compiler_matrix.BASE_FLAGS[-1], "-c")


class RunConfig(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        srcdir = self.root / "src" / "compiler_id"
        srcdir.mkdir(parents=True)
        for name in ("fn_a", "fn_b", "fn_c"):
            (srcdir / f"{name}.cpp").write_text("// made up\n")
        self.calls = []
        for p in (mock.patch.object(compiler_matrix, "ROOT", self.root),
                  mock.patch.object(compiler_matrix.fetch_toolchains, "clang_path",
                                    lambda v: Path(f"/fake/clang-{v}")),
                  mock.patch.object(compiler_matrix.subprocess, "run", self.fake_run),
                  mock.patch.object(compiler_matrix.asmdiff, "diff_object", self.fake_diff)):
            p.start()
            self.addCleanup(p.stop)

    def fake_run(self, cmd, **kw):
        self.calls.append(cmd)
        src = Path(cmd[-3])
        if src.stem == "fn_c":
            return subprocess.CompletedProcess(cmd, 1, "", "warning\nerror: no good\n")
        return subprocess.CompletedProcess(cmd, 0, "", "")

    def fake_diff(self, obj, original):
        # fn_a matches; fn_b differs in one of four instructions.
        return [result(True)] if obj.stem == "fn_a" else [result(False, diff=[2])]

    def test_scores_and_errors(self):
        r = compiler_matrix.run_config("9.9.9", "leaf", "9.0.1", "-O3", "a57+fast", None)
        self.assertEqual(r["config"], "clang-9.0.1 -O3 a57+fast")
        self.assertEqual((r["matched"], r["total"]), (1, 3))
        self.assertEqual(r["mean_score"], round((1.0 + 0.75) / 3, 4))
        self.assertEqual(r["functions"]["fn_c"], {"error": "error: no good"})
        self.assertEqual(r["functions"]["fn_b"]["diff_insns"], 1)
        self.assertTrue(r["functions"]["fn_a"]["match"])

    def test_command_line(self):
        compiler_matrix.run_config("9.9.9", "leaf", "9.0.1", "-O2", "a57", None)
        cmd = self.calls[0]
        self.assertEqual(cmd[0], "/fake/clang-9.0.1")
        self.assertEqual(cmd[1:1 + len(compiler_matrix.BASE_FLAGS)], compiler_matrix.BASE_FLAGS)
        self.assertEqual(cmd[-5:-2], ["-O2", "-mcpu=cortex-a57", str(self.root / "src/compiler_id/fn_a.cpp")])
        self.assertEqual(cmd[-2], "-o")

    def test_output_folder_name_sanitised(self):
        compiler_matrix.run_config("9.9.9", "leaf", "9.0.1", "-O3", "a57+fast", None)
        out = self.root / "build/compiler_matrix/9.9.9/leaf/clang-9.0.1_-O3_a57_fast"
        self.assertTrue(out.is_dir())
        self.assertEqual(Path(self.calls[0][-1]), out / "fn_a.o")

    def test_empty_extra_has_no_trailing_space(self):
        r = compiler_matrix.run_config("9.9.9", "leaf", "9.0.1", "-O1", "", None)
        self.assertEqual(r["config"], "clang-9.0.1 -O1")

    def test_main_ranks_and_writes_results(self):
        out = io.StringIO()
        with mock.patch.object(compiler_matrix.asmdiff, "Original", lambda v: None), \
                mock.patch("sys.argv", ["compiler_matrix.py", "--version", "9.9.9", "--only", "9.0.1,10.0.0",
                                        "-j", "2"]), contextlib.redirect_stdout(out):
            self.assertEqual(compiler_matrix.main(), 0)
        results = json.loads((self.root / "build/compiler_matrix/9.9.9/leaf/results.json").read_text())
        spec = compiler_matrix.SETS["leaf"]
        self.assertEqual(len(results), 2 * len(spec["opt"]) * len(spec["extra"]))
        keys = [(-r["matched"], -r["mean_score"], r["config"]) for r in results]
        self.assertEqual(keys, sorted(keys))
        self.assertIn(f"{len(results)} configs", out.getvalue())


if __name__ == "__main__":
    unittest.main()
