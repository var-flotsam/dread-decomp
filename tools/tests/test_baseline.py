"""tools/baseline.py's selection, normalisation and save/restore bookkeeping (no game data, no builds)."""

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import baseline
import decl_check


class Normalise(unittest.TestCase):
    def test_warning_and_note_positions_dropped(self):
        out = "src/a.cpp:12:5: warning: unused\nsrc/a.cpp:3:1: note: here\n"
        self.assertEqual(baseline.normalise(out), "src/a.cpp: warning: unused\nsrc/a.cpp: note: here\n")

    def test_errors_keep_positions(self):
        out = "src/a.cpp:12:5: error: boom\n"
        self.assertEqual(baseline.normalise(out), out)

    def test_moved_warning_compares_equal(self):
        self.assertEqual(baseline.normalise("x.h:10:2: warning: w\n"), baseline.normalise("x.h:11:2: warning: w\n"))


class Outputs(unittest.TestCase):
    def test_broken(self):
        self.assertTrue(baseline.broken("BUILD FAILED\n[exit 2]\n\n"))
        self.assertFalse(baseline.broken("3/3 functions match\n[exit 0]\n"))
        self.assertFalse(baseline.broken("2/3 functions match\n[exit 1]\n"))

    def test_summary_line(self):
        self.assertEqual(baseline.summary_line("a\n 2/3 functions match \nRELOC x\n[exit 1]\n"), "2/3 functions match")
        self.assertEqual(baseline.summary_line("x\nsrc/a.cpp:1:1: error: boom\n[exit 2]\n"),
                         "src/a.cpp:1:1: error: boom")
        self.assertEqual(baseline.summary_line("only\nlast\n"), "last")
        self.assertEqual(baseline.summary_line("  \n"), "(no output)")


class Paths(unittest.TestCase):
    def test_rel(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d).resolve()
            with mock.patch.object(baseline, "ROOT", root):
                self.assertEqual(baseline.rel("src/game/a.cpp"), "src/game/a.cpp")
                self.assertEqual(baseline.rel(root / "include" / "x.h"), "include/x.h")
                self.assertEqual(baseline.rel("src/game/../base/b.cpp"), "src/base/b.cpp")

    def test_out_and_src_paths(self):
        with mock.patch.object(baseline, "BASE", Path("B")):
            self.assertEqual(baseline.out_path("t", "src/a.cpp"), Path("B/t/out/src/a.cpp.txt"))
            self.assertEqual(baseline.src_path("t", "src/a.cpp"), Path("B/t/src/src/a.cpp"))


class Selection(unittest.TestCase):
    """include_map() is replaced by a fixed map, so no git or tree scan is needed."""

    MAP = {
        "A.h": ["src/game/one.cpp", "include/game/B.h"],
        "B.h": ["src/game/two.cpp", "src/game/B.h"],   # a same-named local header: followed by basename
        "C.h": ["src/game/three.cpp"],
    }

    def setUp(self):
        saved = baseline._INCLUDE_CACHE
        baseline._INCLUDE_CACHE = {k: list(v) for k, v in self.MAP.items()}
        self.addCleanup(setattr, baseline, "_INCLUDE_CACHE", saved)

    def test_includers_transitive(self):
        self.assertEqual(baseline.includers("include/game/A.h"),
                         {"src/game/one.cpp", "include/game/B.h", "src/game/two.cpp", "src/game/B.h"})

    def test_includers_skip_self(self):
        self.assertEqual(baseline.includers("src/game/B.h"), {"src/game/two.cpp"})

    def test_expand_keeps_sources_only(self):
        self.assertEqual(baseline.expand({"include/game/A.h", "src/game/x.cpp"}),
                         {"src/game/one.cpp", "src/game/two.cpp", "src/game/x.cpp"})

    def test_select_files_symbols_headers(self):
        report = decl_check.Report({
            "src/game/four.cpp": "void fn_7100000010(int);",
            "src/clones/gen.cpp": "void fn_7100000010(int);",   # generated: never selected
        })
        with tempfile.TemporaryDirectory() as d:
            root = Path(d).resolve()
            lst = root / "list.txt"
            lst.write_text("src/game/five.cpp\n\n  include/game/C.h  \nlib/x.cpp\n", encoding="utf-8")
            with mock.patch.object(baseline, "ROOT", root):
                got = baseline.select(["src/game/x.cpp", "@" + str(lst)], ["fn_7100000010"], ["include/game/A.h"],
                                      report=report)
        self.assertEqual(got, ["src/game/five.cpp", "src/game/four.cpp", "src/game/one.cpp",
                               "src/game/three.cpp", "src/game/two.cpp", "src/game/x.cpp"])


class SaveRestore(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmp.cleanup)
        self.root = Path(self._tmp.name).resolve()
        for name, value in (("ROOT", self.root), ("BASE", self.root / "build" / "baseline")):
            p = mock.patch.object(baseline, name, value)
            p.start()
            self.addCleanup(p.stop)
        (self.root / "src").mkdir()
        (self.root / "include").mkdir()
        (self.root / "src" / "a.cpp").write_text("int a;\n", encoding="utf-8")
        (self.root / "include" / "h.h").write_text("int h;\n", encoding="utf-8")

    def test_save_compare_restore(self):
        outputs = {"src/a.cpp": "1/1 functions match\n[exit 0]\n"}
        with mock.patch.object(baseline, "run_all", lambda v, paths, j: {p: outputs[p] for p in paths}):
            baseline.save("t", ["src/a.cpp"], "2.1.0", 1, copies=["include/h.h"])
            self.assertEqual(baseline.saved_files("t"), ["include/h.h", "src/a.cpp"])
            self.assertEqual(baseline.out_path("t", "src/a.cpp").read_text(encoding="utf-8"), outputs["src/a.cpp"])
            self.assertEqual(baseline.compare("t", ["src/a.cpp"], "2.1.0", 1)["src/a.cpp"], ("UNCHANGED", ""))

            outputs["src/a.cpp"] = "0/1 functions match\n[exit 1]\n"
            outputs["src/new.cpp"] = "x\n"
            res = baseline.compare("t", ["src/a.cpp", "src/new.cpp"], "2.1.0", 1)
        self.assertEqual(res["src/a.cpp"][0], "CHANGED")
        self.assertIn("+0/1 functions match", res["src/a.cpp"][1])
        self.assertIn("-1/1 functions match", res["src/a.cpp"][1])
        self.assertEqual(res["src/new.cpp"], ("NEW", "x\n"))

        (self.root / "src" / "a.cpp").write_text("int changed;\n", encoding="utf-8")
        (self.root / "include" / "h.h").write_text("int changed;\n", encoding="utf-8")
        done = baseline.restore("t", ["src/a.cpp", "include/h.h", "src/never_saved.cpp"])
        self.assertEqual(done, ["src/a.cpp", "include/h.h"])
        self.assertEqual((self.root / "src" / "a.cpp").read_text(encoding="utf-8"), "int a;\n")
        self.assertEqual((self.root / "include" / "h.h").read_text(encoding="utf-8"), "int h;\n")

    def test_saved_files_empty_tag(self):
        self.assertEqual(baseline.saved_files("missing"), [])

    def test_restore_keeps_bytes(self):
        (self.root / "src" / "c.cpp").write_bytes(b"int c;\r\n")
        with mock.patch.object(baseline, "run_all", lambda v, paths, j: {p: "" for p in paths}):
            baseline.save("t", ["src/c.cpp"], "2.1.0", 1)
        (self.root / "src" / "c.cpp").write_bytes(b"x")
        baseline.restore("t", ["src/c.cpp"])
        self.assertEqual((self.root / "src" / "c.cpp").read_bytes(), b"int c;\r\n")


if __name__ == "__main__":
    unittest.main()
