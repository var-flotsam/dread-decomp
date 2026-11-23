"""tools/drafts.py: the text side (includes for a new folder, the tier-2 note, the index). No compiler and no
game data."""
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import drafts

SRC = """// Unit 7: sets the thing up. Names and layouts are placeholders.
#include "types.h"
#include "UnkLocal.h"
#include "../../../src/game_s2/UnkShared.h"

void fn_7100000100(UnkA* self);
u32 fn_7100000200(UnkA* self);

void fn_7100000100(UnkA* self) {
    self->a = fn_7100000200(self);
}

// Counts them.
u32 fn_7100000200(UnkA* self) {
    return self->n;
}
"""


class Includes(unittest.TestCase):
    def test_an_include_relative_to_the_folder_follows_the_file(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            old, new = root / "drafts" / "game" / "CFoo", root / "src" / "game" / "CFoo"
            for d in (old, new, root / "src" / "game_s2"):
                d.mkdir(parents=True)
            (old / "UnkLocal.h").write_text("", encoding="utf-8")
            (root / "src" / "game_s2" / "UnkShared.h").write_text("", encoding="utf-8")
            out = drafts.rehome(SRC, old, new)
        self.assertIn('#include "types.h"', out)                       # found through include/: untouched
        self.assertIn('#include "../../game_s2/UnkShared.h"', out)
        self.assertIn('#include "../../../drafts/game/CFoo/UnkLocal.h"', out)   # still where it was: seen, not lost
        self.assertEqual(out.count("#include"), 3)

    def test_and_back(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            old, new = root / "src" / "game" / "CFoo", root / "drafts" / "game" / "CFoo"
            for d in (old, new, root / "src" / "game_s2"):
                d.mkdir(parents=True)
            (root / "src" / "game_s2" / "UnkShared.h").write_text("", encoding="utf-8")
            text = '#include "../../game_s2/UnkShared.h"\n'
            self.assertEqual(drafts.rehome(text, old, new), '#include "../../../src/game_s2/UnkShared.h"\n')


class Note(unittest.TestCase):
    def test_the_definition_is_found_not_the_declaration_or_the_call(self):
        lines = SRC.split("\n")
        self.assertEqual(lines[drafts.definition_line(lines, "fn_7100000100")], "void fn_7100000100(UnkA* self) {")
        self.assertEqual(lines[drafts.definition_line(lines, "fn_7100000200")], "u32 fn_7100000200(UnkA* self) {")
        self.assertIsNone(drafts.definition_line(lines, "fn_7100000300"))

    def test_a_definition_over_several_lines(self):
        lines = ["void fn_7100000100(UnkA* self,", "                   u32 n);", "", "void fn_7100000100(UnkA* self,",
                 "                   u32 n) {", "}"]
        self.assertEqual(drafts.definition_line(lines, "fn_7100000100"), 3)

    def test_the_note_goes_above_the_definition_once(self):
        text, missing = drafts.annotate(SRC, ["fn_7100000100"])
        lines = text.split("\n")
        i = lines.index("void fn_7100000100(UnkA* self) {")
        self.assertEqual(lines[i - 1], drafts.MINOR_NOTE)
        self.assertEqual(missing, [])
        self.assertEqual(drafts.annotate(text, ["fn_7100000100"])[0], text)

    def test_a_function_with_a_note_keeps_its_own(self):
        own = SRC.replace("// Counts them.", "// NON_MATCHING(minor): x9 and x10 swapped.\n// Counts them.")
        self.assertEqual(drafts.annotate(own, ["fn_7100000200"]), (own, []))

    def test_a_comment_block_is_kept_together(self):
        text, _ = drafts.annotate(SRC, ["fn_7100000200"])
        lines = text.split("\n")
        i = lines.index("u32 fn_7100000200(UnkA* self) {")
        self.assertEqual(lines[i - 2:i], ["// Counts them.", drafts.MINOR_NOTE])

    def test_a_name_with_no_definition_is_reported(self):
        self.assertEqual(drafts.annotate(SRC, ["fn_7100000300"]), (SRC, ["fn_7100000300"]))


class Header(unittest.TestCase):
    LINES = ["// Unit 7. Names and layouts are placeholders.", '#include "types.h"', "",
             "struct UnkA;", "void fn_7100000300(UnkA* self);  // resets it", "u32 fn_7100000400(UnkA* self,",
             "                  u32 n);", "extern u32 g_unk_7100000500, g_unk_7100000600;", "",
             "void fn_7100000100(UnkA* self) {", "    fn_7100000300(self);", "}"]

    def test_the_declaration_goes_and_the_header_comes(self):
        out = drafts.take_header(self.LINES, 5, "fn_7100000300", "game/UnkA.h")
        self.assertEqual(out[1:3], ['#include "types.h"', '#include "game/UnkA.h"'])
        self.assertNotIn("void fn_7100000300(UnkA* self);  // resets it", out)
        self.assertIn("    fn_7100000300(self);", out)                       # the call stays
        self.assertEqual(len(out), len(self.LINES))

    def test_a_header_that_is_included_is_not_included_twice(self):
        lines = self.LINES[:2] + ['#include "game/UnkA.h"'] + self.LINES[2:]
        out = drafts.take_header(lines, 6, "fn_7100000300", "game/UnkA.h")
        self.assertEqual(out.count('#include "game/UnkA.h"'), 1)
        self.assertEqual(len(out), len(lines) - 1)

    def test_a_file_without_includes_gets_it_under_its_comment(self):
        lines = ["// Unit 7.", "// Placeholders.", "void fn_7100000300();", "void fn_7100000100() {", "}"]
        out = drafts.take_header(lines, 3, "fn_7100000300", "game/UnkA.h")
        self.assertEqual(out[:3], ["// Unit 7.", "// Placeholders.", '#include "game/UnkA.h"'])

    def test_what_is_not_one_statement_on_one_line_is_left_for_a_person(self):
        self.assertIsNone(drafts.take_header(self.LINES, 6, "fn_7100000400", "game/UnkA.h"))    # two lines
        self.assertIsNone(drafts.take_header(self.LINES, 8, "g_unk_7100000500", "game/UnkA.h"))  # two globals
        self.assertIsNone(drafts.take_header(self.LINES, 11, "fn_7100000300", "game/UnkA.h"))    # the wrong line
        self.assertIsNone(drafts.take_header(self.LINES, 40, "fn_7100000300", "game/UnkA.h"))


class Index(unittest.TestCase):
    def test_rows_are_written_by_address_with_the_table_s_columns(self):
        rows = [{"address": "0x7100000200", "size": "8", "file": "drafts/b.cpp", "difference": "operations",
                 "similarity": "50.0", "differing_instructions": "1", "from": "t", "note": ""},
                {"address": "0x7100000100", "size": "12", "file": "drafts/a.cpp", "difference": "match",
                 "similarity": "100.0", "differing_instructions": "0", "from": "t", "note": "held: x"}]
        with tempfile.TemporaryDirectory() as tmp:
            old, drafts.INDEX = drafts.INDEX, Path(tmp) / "INDEX.csv"
            try:
                drafts.write_index(rows)
                text = drafts.INDEX.read_bytes().decode("utf-8")
                back = drafts.read_index()
            finally:
                drafts.INDEX = old
        self.assertEqual(text.split("\n")[0], ",".join(drafts.FIELDS))
        self.assertNotIn("\r", text)
        self.assertEqual([r["address"] for r in back], ["0x7100000100", "0x7100000200"])
        self.assertEqual(back[0]["note"], "held: x")

    def test_the_kinds_are_the_table_s_words(self):
        self.assertEqual(drafts.KINDS["registers"], "registers or order")
        self.assertEqual(drafts.KINDS["control"], "control flow")

    def test_a_long_reason_is_cut(self):
        self.assertEqual(drafts.short("a  b\n c"), "a b c")
        self.assertEqual(len(drafts.short("x" * 300, 100)), 100)


class Absorb(unittest.TestCase):
    """drafts.py absorb over a made-up tree: text only, nothing compiled."""
    UNIT = """// Unit 180: lookups. Names are placeholders.
#include "game/UnkTU180.h"

static u32 Count(UnkA* self);

void fn_710015dc04(UnkA* self) {
    self->n = 0;
}

u32 CLookup::Find(UnkA* self,
                  u32 key) {
    return key;
}
"""

    def tree(self, root: Path) -> list[dict]:
        (root / "src" / "game" / "UnkTU180").mkdir(parents=True)
        (root / "drafts" / "game" / "UnkTU180").mkdir(parents=True)
        (root / "drafts" / "game" / "Other").mkdir(parents=True)
        (root / "src" / "game" / "UnkTU180" / "Lookup.cpp").write_text(self.UNIT, encoding="utf-8")
        rows = []
        for addr, folder in ((0x710015dc04, "UnkTU180"), (0x710015ddc0, "UnkTU180"), (0x710015e600, "Other"),
                             (0x710015e800, "UnkTU180")):
            f = root / "drafts" / "game" / folder / f"fn_{addr:x}.cpp"
            f.write_text(f"void fn_{addr:x}() {{}}\n", encoding="utf-8")
            rows.append({"address": f"0x{addr:x}", "size": "8", "file": f.relative_to(root).as_posix(),
                         "difference": "operations", "similarity": "98.9", "differing_instructions": "1",
                         "from": "t", "note": ""})
        return rows

    def run_absorb(self, names, listed, write, into="src/game/UnkTU180/Lookup.cpp", extra=None):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp).resolve()
            rows = self.tree(root)
            for rel, text in (extra or {}).items():
                (root / rel).write_text(text, encoding="utf-8")
            old = drafts.ROOT
            drafts.ROOT = root
            try:
                done, problems = drafts.absorb([root / n for n in names], root / into, rows, listed, write)
                left = sorted(p.relative_to(root).as_posix() for p in (root / "drafts").rglob("*"))
            finally:
                drafts.ROOT = old
        return done, problems, [r["address"] for r in rows], left

    def test_a_draft_the_target_defines_by_its_placeholder_goes_with_its_row(self):
        done, problems, rows, left = self.run_absorb(["drafts/game/UnkTU180/fn_710015dc04.cpp"], {}, True)
        self.assertEqual(problems, [])
        self.assertEqual(len(done), 1)
        self.assertIn("removed  0x710015dc04", done[0])
        self.assertIn("defines it as fn_710015dc04", done[0])
        self.assertNotIn("0x710015dc04", rows)
        self.assertNotIn("drafts/game/UnkTU180/fn_710015dc04.cpp", left)

    def test_by_its_name_in_the_function_list_over_several_lines(self):
        done, problems, rows, _ = self.run_absorb(["drafts/game/UnkTU180/fn_710015ddc0.cpp"],
                                                  {0x710015ddc0: "CLookup::Find"}, True)
        self.assertEqual(problems, [])
        self.assertIn("defines it as CLookup::Find", done[0])
        self.assertNotIn("0x710015ddc0", rows)

    def test_a_function_only_declared_there_stays(self):
        done, problems, _, left = self.run_absorb(["drafts/game/Other/fn_710015e600.cpp"], {0x710015e600: "Count"},
                                                  True)
        self.assertEqual(done, [])
        self.assertEqual(len(problems), 1)
        self.assertIn("drafts/game/Other/fn_710015e600.cpp", left)

    def test_the_folder_a_draft_leaves_empty_goes_too(self):
        unit = self.UNIT + "\nstatic u32 Count(UnkA* self) {\n    return self->n;\n}\n"
        done, problems, _, left = self.run_absorb(["drafts/game/Other/fn_710015e600.cpp"], {0x710015e600: "Count"},
                                                  True, extra={"src/game/UnkTU180/Lookup.cpp": unit})
        self.assertEqual(problems, [])
        self.assertIn("defines it as Count", done[0])
        self.assertNotIn("drafts/game/Other", left)
        self.assertIn("drafts/game/UnkTU180", left)

    def test_without_write_nothing_changes(self):
        done, problems, rows, left = self.run_absorb(["drafts/game/UnkTU180/fn_710015dc04.cpp"], {}, False)
        self.assertIn("would remove  0x710015dc04", done[0])
        self.assertIn("0x710015dc04", rows)
        self.assertIn("drafts/game/UnkTU180/fn_710015dc04.cpp", left)

    def test_a_function_the_target_does_not_define_stays(self):
        done, problems, rows, left = self.run_absorb(["drafts/game/UnkTU180/fn_710015e800.cpp",
                                                      "drafts/game/UnkTU180/fn_710015dc04.cpp"],
                                                     {0x710015e800: "COther::Find"}, True)
        self.assertEqual(len(done), 1)                     # the other one still goes
        self.assertEqual(len(problems), 1)
        self.assertIn("does not define 0x710015e800 (looked for fn_710015e800 or COther::Find)", problems[0])
        self.assertIn("0x710015e800", rows)
        self.assertIn("drafts/game/UnkTU180/fn_710015e800.cpp", left)

    def test_a_file_without_a_row_and_a_target_outside_src(self):
        _, problems, _, _ = self.run_absorb(["drafts/game/UnkTU180/missing.cpp"], {}, True)
        self.assertIn("no row of drafts/INDEX.csv", problems[0])
        done, problems, rows, _ = self.run_absorb(["drafts/game/UnkTU180/fn_710015dc04.cpp"], {}, True,
                                                  into="drafts/game/UnkTU180/fn_710015dc04.cpp")
        self.assertEqual(done, [])
        self.assertIn("not a source file under src/", problems[0])
        self.assertIn("0x710015dc04", rows)

    def test_defines(self):
        text = Absorb.UNIT
        self.assertTrue(drafts.defines(text, "fn_710015dc04"))
        self.assertTrue(drafts.defines(text, "CLookup::Find"))
        self.assertTrue(drafts.defines(text, "game::CLookup::Find"))   # namespaces as a block or a prefix
        self.assertFalse(drafts.defines(text, "COther::Find"))         # another class's function of that name
        self.assertFalse(drafts.defines(text, "Count"))                # declared only
        self.assertEqual(drafts.candidate_names(0x710015dc04, "fn_710015dc04"), ["fn_710015dc04"])
        self.assertEqual(drafts.candidate_names(0x710015ddc0, "CLookup::Find"), ["fn_710015ddc0", "CLookup::Find"])


if __name__ == "__main__":
    unittest.main()
