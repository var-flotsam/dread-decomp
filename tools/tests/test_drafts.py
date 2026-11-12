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


if __name__ == "__main__":
    unittest.main()
