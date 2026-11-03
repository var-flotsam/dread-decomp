"""tools/verify.py: output parsing and the gate's decisions, with every child tool replaced by
canned output (no game data, no toolchain, nothing run).

ROOT is patched to a temporary folder, so the baseline under build/verify/ of the checkout is
untouched.
"""

import contextlib
import io
import json
import tempfile
import types
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import verify

DECL = {"declared": 5, "files": 3, "multi_file": 1, "extra_files": 1, "conflicting": 0,
        "extra_signatures": 0, "header_redeclared": 0}


def status(*addrs, code="O"):
    return [f"{code}  {a}  fn_{a[2:]}" for a in addrs]


class Parsing(unittest.TestCase):
    def test_short(self):
        self.assertEqual(verify.short("abc", 10), "abc")
        self.assertEqual(verify.short("x" * 20, 10), "xxxxxxx...")
        self.assertEqual(len(verify.short("y" * 200)), 100)

    def test_status_line(self):
        m = verify.STATUS_LINE.match("m  0x7100000100  CFoo::Bar")
        self.assertEqual(m.groups(), ("m", "0x7100000100"))
        self.assertIsNone(verify.STATUS_LINE.match("U  0x7100000100  x"))
        self.assertIsNone(verify.STATUS_LINE.match("O 0x7100000100 x"))

    def test_summary_and_progress_lines(self):
        self.assertEqual(verify.SUMMARY_LINE.match("12 functions in src/: O 10, m 2").group(1), "O 10, m 2")
        m = verify.PROGRESS_LINE.match("matching (O): 1234 functions, 5.67% of code (whole binary)")
        self.assertEqual(m.groups(), ("1234", "5.67"))

    def test_generators_in_documented_order(self):
        self.assertEqual(verify.GENERATORS[0], "gen_class_getters")
        self.assertEqual(verify.GENERATORS[-1], "gen_lift")

    def test_check_parses_output(self):
        lines = [*status("0x7100000100", "0x7100000200"), *status("0x7100000300", code="m"),
                 "BUILD FAILED src/a/b.cpp", "3 functions in src/: O 2, m 1"]
        calls = []
        with mock.patch.object(verify, "run", lambda *a: calls.append(a) or (0, lines, 1.0)):
            rc, o, failed, summary, all_lines = verify.check("9.9.9", update=True)
        self.assertEqual(calls, [("tools/check.py", "--version", "9.9.9", "--update")])
        self.assertEqual(o, {"0x7100000100", "0x7100000200"})
        self.assertEqual(failed, ["src/a/b.cpp"])
        self.assertEqual(summary, "O 2, m 1")
        self.assertEqual(all_lines, lines)


class Main(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.baseline_file = self.root / "build" / "verify" / "9.9.9" / "baseline.txt"
        self.outputs = {
            ("tools/check.py", False): (0, status("0x7100000100") + ["1 functions in src/: O 1"]),
            ("tools/check.py", True): (0, status("0x7100000100", "0x7100000200") + ["2 functions in src/: O 2"]),
            "tools/progress.py": (0, ["matching (O): 2 functions, 0.01% of code"]),
            "tools/decl_check.py": (0, json.dumps(DECL).splitlines()),
        }
        self.calls = []
        self.problems = {}
        for p in (mock.patch.object(verify, "ROOT", self.root),
                  mock.patch.object(verify, "run", self.fake_run),
                  mock.patch.object(verify.policy_check, "tracked", lambda base: ["a.txt"]),
                  mock.patch.object(verify.policy_check, "problems", lambda f: self.problems.get(f, [])),
                  mock.patch.object(verify, "subprocess", types.SimpleNamespace(
                      run=lambda *a, **k: types.SimpleNamespace(stdout="b.txt\n")))):
            p.start()
            self.addCleanup(p.stop)

    def fake_run(self, script, *args):
        self.calls.append((script, *args))
        if script == "tools/check.py":
            rc, lines = self.outputs[(script, "--update" in args)]
        else:
            rc, lines = self.outputs.get(script, (0, [f"{script} done"]))
        return rc, lines, 0.0

    def run_main(self, *argv):
        out = io.StringIO()
        with mock.patch("sys.argv", ["verify.py", "--version", "9.9.9", *argv]), contextlib.redirect_stdout(out):
            rc = verify.main()
        return rc, out.getvalue()

    def test_unknown_generator(self):
        rc, out = self.run_main("--regen", "gen_nothing")
        self.assertEqual(rc, 2)
        self.assertIn("unknown gen_nothing", out)
        self.assertEqual(self.calls, [])

    def test_first_run_builds_baseline_then_passes(self):
        rc, out = self.run_main()
        self.assertEqual(rc, 0, out)
        self.assertIn("NOTE baseline: none saved", out)
        self.assertIn("O 1 -> 2 (+1)", out)
        self.assertIn("PASS policy: 2 files, 0 problems", out)
        self.assertIn("PASS decl_check (info): 5 fn_/g_unk_", out)
        self.assertIn("RESULT: PASS", out)
        self.assertEqual(self.baseline_file.read_text().split(), ["0x7100000100", "0x7100000200"])
        self.assertIn(("tools/gen_lift.py", "--prune"), self.calls)

    def test_regen_runs_in_generator_order(self):
        self.run_main("--regen", "gen_lift.py, gen_statics,gen_class_getters")
        gens = [c for c in self.calls if c[0].startswith("tools/gen_")]
        self.assertEqual(gens, [("tools/gen_class_getters.py", "--version", "9.9.9"),
                                ("tools/gen_statics.py", "--version", "9.9.9"),
                                ("tools/gen_lift.py", "--version", "9.9.9")])

    def test_regression_against_saved_baseline(self):
        self.baseline_file.parent.mkdir(parents=True)
        self.baseline_file.write_text("0x7100000100\n0x7100000999\n")
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("FAIL regressions: 1 functions no longer O: 0x7100000999", out)
        self.assertEqual(self.baseline_file.read_text(), "0x7100000100\n0x7100000999\n")  # not overwritten
        self.assertNotIn(("tools/check.py", "--version", "9.9.9"), self.calls)

    def test_flagged_regression_line_counts(self):
        self.outputs[("tools/check.py", True)] = (1, status("0x7100000100") + [
            "m  0x7100000200  fn_7100000200  REGRESSION (was O)"])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("no longer O: 0x7100000200", out)

    def test_duplicates_fail(self):
        self.outputs[("tools/check.py", True)] = (0, status("0x7100000100") + ["DUPLICATE 0x7100000100 a.cpp b.cpp"])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("FAIL duplicates: 1 functions defined in two files: 0x7100000100", out)

    def test_baseline_build_failure_stops(self):
        self.outputs[("tools/check.py", False)] = (1, ["BUILD FAILED src/x.cpp"])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("FAIL baseline: 1 sources do not build", out)
        self.assertEqual(len(self.calls), 1)

    def test_generator_failure_skips_check(self):
        self.outputs["tools/gen_statics.py"] = (3, ["Traceback", "ValueError: boom"])
        rc, out = self.run_main("--regen", "gen_statics,gen_clones")
        self.assertEqual(rc, 1)
        self.assertIn("FAIL gen_statics (exit 3): ValueError: boom", out)
        self.assertNotIn(("tools/gen_clones.py", "--version", "9.9.9"), self.calls)
        self.assertNotIn(("tools/check.py", "--version", "9.9.9", "--update"), self.calls)
        self.assertIn("NEXT: run `python tools/gen_statics.py --version 9.9.9` alone", out)

    def test_policy_problem_fails(self):
        self.problems["b.txt"] = ["game data"]
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("FAIL policy: 1 problems, first: b.txt: game data", out)

    def test_progress_failure_and_bad_decl_json(self):
        self.outputs["tools/progress.py"] = (1, ["oops"])
        self.outputs["tools/decl_check.py"] = (1, ["not json"])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("FAIL progress.py (exit 1)", out)
        self.assertIn("NOTE decl_check: no counts (exit 1): not json", out)


if __name__ == "__main__":
    unittest.main()
