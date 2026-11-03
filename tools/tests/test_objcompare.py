"""tools/objcompare.py on two small trees of hand-built objects (no game data, no compiler).

The objects are written with objdiff_gen.build_target from made-up instruction words.
"""

import contextlib
import io
import struct
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import objcompare
from objdiff_gen import Reloc, TargetFunc, build_target

NOP, RET, BL = 0xD503201F, 0xD65F03C0, 0x94000000


def code(*words: int) -> bytes:
    return struct.pack(f"<{len(words)}I", *words)


def fa(sym="fn_7100000200"):
    return TargetFunc("fn_7100000100", 1, code(BL, RET), [Reloc(0, 283, sym, 0)])


def fb(body=(NOP, RET)):
    return TargetFunc("fn_7100000300", 1, code(*body))


class Trees(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.before = Path(self.tmp.name) / "before"
        self.after = Path(self.tmp.name) / "after"

    def obj(self, tree: Path, rel: str, funcs):
        p = tree / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(build_target(funcs))

    def run_main(self):
        out = io.StringIO()
        with mock.patch("sys.argv", ["objcompare.py", str(self.before), str(self.after)]), \
                contextlib.redirect_stdout(out):
            rc = objcompare.main()
        return rc, out.getvalue()

    def test_functions_keyed_by_object_name_and_symbol(self):
        self.obj(self.before, "a/x.o", [fa(), fb()])
        funcs, clashes = objcompare.functions(self.before)
        self.assertEqual(clashes, [])
        self.assertEqual(sorted(funcs), [("x.o", "fn_7100000100"), ("x.o", "fn_7100000300")])
        code_, relocs = funcs[("x.o", "fn_7100000100")]
        self.assertEqual(code_, code(BL, RET))
        self.assertEqual(relocs, ((0, 283, "fn_7100000200", 0, False),))

    def test_moved_file_is_identical(self):
        self.obj(self.before, "old/x.o", [fa(), fb()])
        self.obj(self.after, "new/dir/x.o", [fa(), fb()])
        rc, out = self.run_main()
        self.assertEqual(rc, 0)
        self.assertIn("0 missing, 0 added, 0 changed", out)

    def test_changed_code(self):
        self.obj(self.before, "x.o", [fa(), fb()])
        self.obj(self.after, "x.o", [fa(), fb((RET,))])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("CHANGED  x.o  fn_7100000300", out)

    def test_changed_relocation_target(self):
        self.obj(self.before, "x.o", [fa()])
        self.obj(self.after, "x.o", [fa("fn_7100000999")])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("CHANGED  x.o  fn_7100000100", out)

    def test_missing_and_added(self):
        self.obj(self.before, "x.o", [fa()])
        self.obj(self.after, "x.o", [fb()])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("MISSING  x.o  fn_7100000100", out)
        self.assertIn("ADDED  x.o  fn_7100000300", out)
        self.assertIn("1 missing, 1 added, 0 changed", out)

    def test_same_name_objects_that_differ_are_ambiguous(self):
        self.obj(self.before, "a/x.o", [fb()])
        self.obj(self.before, "b/x.o", [fb((RET,))])
        self.obj(self.after, "x.o", [fb()])
        _, clashes = objcompare.functions(self.before)
        self.assertEqual(clashes, ["x.o fn_7100000300"])
        rc, out = self.run_main()
        self.assertEqual(rc, 1)
        self.assertIn("AMBIGUOUS  x.o fn_7100000300", out)

    def test_same_name_objects_that_agree_are_fine(self):
        self.obj(self.before, "a/x.o", [fb()])
        self.obj(self.before, "b/x.o", [fb()])
        self.assertEqual(objcompare.functions(self.before)[1], [])

    def test_empty_trees(self):
        self.before.mkdir()
        self.after.mkdir()
        rc, out = self.run_main()
        self.assertEqual(rc, 0)
        self.assertIn("0 functions before, 0 after", out)


if __name__ == "__main__":
    unittest.main()
