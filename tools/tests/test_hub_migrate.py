"""tools/hub_migrate.py's text edits and plan parsing on hand-written sources (no game data, no builds)."""

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import decl_check
import hub_migrate


def remove(text: str, symbol: str = "fn_7100000010") -> str:
    for d in sorted((d for d in decl_check.scan(text, "src/x.cpp") if d.name == symbol),
                    key=lambda d: d.start, reverse=True):
        text = hub_migrate.remove_statement(text, d.start, d.end)
    return text


class Spelling(unittest.TestCase):
    def test_include_and_src_roots(self):
        self.assertEqual(hub_migrate.include_spelling("include/game/CFoo.h"), "game/CFoo.h")
        self.assertEqual(hub_migrate.include_spelling("src/game/local.h"), "game/local.h")
        self.assertEqual(hub_migrate.include_spelling("other/x.h"), "other/x.h")


class RemoveStatement(unittest.TestCase):
    def test_whole_line_with_trailing_comment(self):
        text = '#include "a.h"\n\nvoid fn_7100000010(int); // old\nint x;\n'
        self.assertEqual(remove(text), '#include "a.h"\n\nint x;\n')

    def test_shared_line_cuts_statement_only(self):
        self.assertEqual(remove("int a; void fn_7100000010(int); int b;\n"), "int a;  int b;\n")

    def test_double_blank_collapsed(self):
        self.assertEqual(remove("// c\n\nvoid fn_7100000010(int);\n\nint b;\n"), "// c\n\nint b;\n")

    def test_multiline_declaration(self):
        text = "int a;\nvoid fn_7100000010(int x,\n                   char* y);\nint b;\n"
        self.assertEqual(remove(text), "int a;\nint b;\n")

    def test_trailing_blank_lines_trimmed(self):
        self.assertEqual(remove("int a;\n\nextern int g_unk_7100000020;\n", "g_unk_7100000020"), "int a;\n")

    def test_other_symbols_untouched(self):
        text = "void fn_7100000010(int);\nvoid fn_7100000030(int);\n"
        self.assertEqual(remove(text), "void fn_7100000030(int);\n")


class Includes(unittest.TestCase):
    def test_has_include_by_basename(self):
        self.assertTrue(hub_migrate.has_include('#include "../game/CFoo.h"\n', "include/game/CFoo.h"))
        self.assertFalse(hub_migrate.has_include("#include <CFoo.h>\n", "include/game/CFoo.h"))
        self.assertFalse(hub_migrate.has_include('#include "CFooBar.h"\n', "include/game/CFoo.h"))

    def test_sorted_into_first_quoted_block(self):
        text = '#include "b.h"\n#include "z.h"\n\n#include "c.h"\n'
        self.assertEqual(hub_migrate.add_include(text, "game/A.h"),
                         '#include "b.h"\n#include "game/A.h"\n#include "z.h"\n\n#include "c.h"\n')

    def test_appended_to_block_end(self):
        self.assertEqual(hub_migrate.add_include('#include "a.h"\n#include "b.h"\nint x;\n', "game/z.h"),
                         '#include "a.h"\n#include "b.h"\n#include "game/z.h"\nint x;\n')

    def test_after_system_includes(self):
        self.assertEqual(hub_migrate.add_include("#include <cstdint>\nint x;\n", "game/A.h"),
                         '#include <cstdint>\n\n#include "game/A.h"\nint x;\n')

    def test_after_pragma_once(self):
        out = hub_migrate.add_include("#pragma once\nint x;\n", "game/A.h")
        self.assertTrue(out.startswith('#pragma once\n\n#include "game/A.h"\n'))

    def test_after_leading_comment(self):
        self.assertEqual(hub_migrate.add_include("// header\nint x;\n", "game/A.h"),
                         '// header\n\n#include "game/A.h"\n\nint x;\n')

    def test_empty_preamble(self):
        self.assertEqual(hub_migrate.add_include("int x;\n", "game/A.h"), '#include "game/A.h"\n\nint x;\n')


class CanonicalHeader(unittest.TestCase):
    def report(self, files):
        return decl_check.Report(files)

    def test_one_header(self):
        r = self.report({"include/game/A.h": "void fn_7100000010(int);", "src/game/b.cpp": "void fn_7100000010(int);"})
        self.assertEqual(hub_migrate.canonical_header(r, "fn_7100000010", None)[0], "include/game/A.h")

    def test_none_or_several(self):
        r = self.report({"include/game/A.h": "void fn_7100000010(int);", "include/game/B.h": "void fn_7100000010(int);",
                         "src/game/b.cpp": "void fn_7100000020(int);"})
        self.assertIsNone(hub_migrate.canonical_header(r, "fn_7100000010", None)[0])
        self.assertIsNone(hub_migrate.canonical_header(r, "fn_7100000020", None)[0])

    def test_given_header_wins(self):
        r = self.report({"src/game/b.cpp": "void fn_7100000020(int);"})
        self.assertEqual(hub_migrate.canonical_header(r, "fn_7100000020", "include/x.h"), ("include/x.h", "given"))


class Plan(unittest.TestCase):
    def test_read_plan(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "plan.tsv"
            p.write_text("# comment\n\nfn_7100000010\tinclude/a.h\n"
                         "g_unk_7100000020\tinclude/b.h\textern int g_unk_7100000020;\n"
                         "fn_7100000030\tinclude/c.h\t  \n", encoding="utf-8")
            self.assertEqual(hub_migrate.read_plan(str(p)), [
                ("fn_7100000010", "include/a.h", None),
                ("g_unk_7100000020", "include/b.h", "extern int g_unk_7100000020;"),
                ("fn_7100000030", "include/c.h", None),
            ])

    def test_read_plan_rejects_short_line(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "plan.tsv"
            p.write_text("fn_7100000010 include/a.h\n", encoding="utf-8")
            with self.assertRaises(SystemExit):
                hub_migrate.read_plan(str(p))

    def test_first_line_prefers_error(self):
        diff = "--- a\n+++ b\n-old line\n+new line\n+x.cpp: error: boom\n"
        self.assertEqual(hub_migrate.first_line(diff), "+x.cpp: error: boom")
        self.assertEqual(hub_migrate.first_line("--- a\n+++ b\n-gone\n"), "-gone")
        self.assertEqual(hub_migrate.first_line(""), "")


class Forwards(unittest.TestCase):
    BEFORE = "#pragma once\nstruct UnkOld;\nvoid fn_7100000010(UnkOld*);\n"

    def test_unused_new_forward_is_found(self):
        now = "#pragma once\nstruct UnkGone;\nstruct UnkOld;\nstruct UnkUsed;\nvoid fn_7100000010(UnkOld*);\n" \
              "void fn_7100000020(UnkUsed*);\n"
        self.assertEqual(hub_migrate.unused_forwards(now, self.BEFORE), ["UnkGone"])

    def test_forward_the_header_had_is_kept(self):
        # unused, but not ours to remove
        now = "#pragma once\nstruct UnkOld;\n"
        self.assertEqual(hub_migrate.unused_forwards(now, self.BEFORE), [])

    def test_a_longer_name_is_not_a_use(self):
        now = "#pragma once\nstruct UnkGone;\nstruct UnkOld;\nvoid fn_7100000010(UnkOld*, UnkGone2*);\n"
        self.assertEqual(hub_migrate.unused_forwards(now, self.BEFORE), ["UnkGone"])

    def test_class_keyword_and_trailing_comment(self):
        now = "class UnkGone;  // from a plan\nstruct UnkOld;\n"
        self.assertEqual(hub_migrate.unused_forwards(now, self.BEFORE), ["UnkGone"])
        self.assertEqual(hub_migrate.drop_forwards(now, ["UnkGone"]), "struct UnkOld;\n")

    def test_drop_leaves_the_rest(self):
        now = "struct UnkA;\nstruct UnkGone;\nstruct UnkB;\nvoid fn_7100000010(UnkA*, UnkB*);\n"
        self.assertEqual(hub_migrate.drop_forwards(now, ["UnkGone"]),
                         "struct UnkA;\nstruct UnkB;\nvoid fn_7100000010(UnkA*, UnkB*);\n")


class Includes2(unittest.TestCase):
    BEFORE = '#pragma once\n#include "base/Old.h"\n#include "types.h"\nvoid fn_7100000010(UnkOld*);\n'
    TYPES = {"base/Old.h": ["UnkOld"], "base/Id.h": ["CId", "UnkIdBase"], "game/Vec.h": ["UnkVec"]}

    def test_a_new_include_nothing_uses_is_found(self):
        now = ('#pragma once\n#include "base/Id.h"\n#include "base/Old.h"\n#include "game/Vec.h"\n#include "types.h"\n'
               "void fn_7100000010(UnkOld*);\nvoid fn_7100000020(base::CId*);\n")
        self.assertEqual(hub_migrate.unused_includes(now, self.BEFORE, self.TYPES.get), ["game/Vec.h"])
        self.assertEqual(hub_migrate.drop_includes(now, ["game/Vec.h"]),
                         '#pragma once\n#include "base/Id.h"\n#include "base/Old.h"\n#include "types.h"\n'
                         "void fn_7100000010(UnkOld*);\nvoid fn_7100000020(base::CId*);\n")

    def test_an_include_the_header_had_is_kept(self):
        now = '#pragma once\n#include "base/Old.h"\n#include "types.h"\n'
        self.assertEqual(hub_migrate.unused_includes(now, self.BEFORE, self.TYPES.get), [])

    def test_an_include_that_cannot_be_read_is_kept(self):
        now = '#pragma once\n#include "base/Old.h"\n#include "nn/os.h"\n#include "types.h"\nvoid fn_7100000010(UnkOld*);\n'
        self.assertEqual(hub_migrate.unused_includes(now, self.BEFORE, self.TYPES.get), [])


class ReadWrite(unittest.TestCase):
    def test_write_crlf(self):
        with tempfile.TemporaryDirectory() as d, mock.patch.object(hub_migrate, "ROOT", Path(d)):
            hub_migrate.write("x.cpp", "int a;\nint b;\n", True)
            self.assertEqual((Path(d) / "x.cpp").read_bytes(), b"int a;\r\nint b;\r\n")
            hub_migrate.write("y.cpp", "int a;\n", False)
            self.assertEqual((Path(d) / "y.cpp").read_bytes(), b"int a;\n")

    def test_read_lf(self):
        with tempfile.TemporaryDirectory() as d, mock.patch.object(hub_migrate, "ROOT", Path(d)):
            (Path(d) / "y.cpp").write_bytes(b"int a;\n")
            self.assertEqual(hub_migrate.read("y.cpp"), ("int a;\n", False))

    def test_read_detects_crlf(self):
        with tempfile.TemporaryDirectory() as d, mock.patch.object(hub_migrate, "ROOT", Path(d)):
            (Path(d) / "x.cpp").write_bytes(b"int a;\r\nint b;\r\n")
            self.assertEqual(hub_migrate.read("x.cpp"), ("int a;\nint b;\n", True))

    def test_crlf_round_trip(self):
        with tempfile.TemporaryDirectory() as d, mock.patch.object(hub_migrate, "ROOT", Path(d)):
            (Path(d) / "x.cpp").write_bytes(b"int a;\r\nint b;\r\n")
            text, crlf = hub_migrate.read("x.cpp")
            hub_migrate.write("x.cpp", text.replace("int b;\n", ""), crlf)
            self.assertEqual((Path(d) / "x.cpp").read_bytes(), b"int a;\r\n")

if __name__ == "__main__":
    unittest.main()
