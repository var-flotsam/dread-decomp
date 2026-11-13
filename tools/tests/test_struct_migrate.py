"""tools/struct_migrate.py: the text side (finding a definition, taking it out, the header). No compiler."""
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import struct_migrate

SRC = """// Unit 7: stores a typed value. Names and layouts are placeholders.
#include "game/CBlackboard.h"
#include "types.h"

struct UnkPropertyName;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

#include "late.h"
struct UnkTypedValueEx { int more; };
"""


class Span(unittest.TestCase):
    def test_the_definition_alone_by_default(self):
        self.assertEqual(struct_migrate.span(SRC, "UnkTypedValue"), (7, 10))

    def test_its_one_line_comment_only_when_asked_for_by_what_it_says(self):
        self.assertEqual(struct_migrate.span(SRC, "UnkTypedValue", "A typed value as the section setter"), (6, 10))
        self.assertEqual(struct_migrate.span(SRC, "UnkTypedValue", "something else"), (7, 10))

    def test_a_file_s_opening_comment_is_never_a_type_s(self):
        text = "// Unit 7: typed values.\n// Names are placeholders.\nstruct UnkTypedValue {\n    void* value;\n};\n"
        self.assertEqual(struct_migrate.span(text, "UnkTypedValue", "typed value"), (2, 4))   # a block, not one line

    def test_the_layout_assertions_under_it_go_with_it(self):
        text = ("struct UnkA {\n    u8 pad[0x10];\n    u32 n;\n};\n"
                'static_assert(__builtin_offsetof(UnkA, n) == 0x10, "layout");\n'
                'static_assert(sizeof(UnkB) == 8, "another type\'s");\nint f();\n')
        self.assertEqual(struct_migrate.span(text, "UnkA"), (0, 4))
        self.assertNotIn("static_assert(__builtin_offsetof(UnkA", struct_migrate.without(text, "UnkA", "game/UnkA.h"))
        self.assertIn("static_assert(sizeof(UnkB)", struct_migrate.without(text, "UnkA", "game/UnkA.h"))

    def test_a_declaration_or_a_longer_name_is_not_it(self):
        self.assertIsNone(struct_migrate.span("struct UnkTypedValue;\n", "UnkTypedValue"))
        self.assertEqual(struct_migrate.span(SRC, "UnkTypedValueEx"), (14, 14))                # on one line
        self.assertIsNone(struct_migrate.span(SRC + "struct UnkTypedValue {\n};\n", "UnkTypedValue"))   # two of them


class Without(unittest.TestCase):
    def test_the_definition_goes_and_the_include_comes_above_where_it_was(self):
        out = struct_migrate.without(SRC, "UnkTypedValue", "game/UnkTypedValue.h", "A typed value").split("\n")
        self.assertEqual(out[1:4], ['#include "game/CBlackboard.h"', '#include "types.h"', '#include "game/UnkTypedValue.h"'])
        self.assertNotIn("struct UnkTypedValue {", out)
        self.assertNotIn("// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).", out)
        self.assertIn("void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);", out)
        self.assertLess(out.index('#include "game/UnkTypedValue.h"'), out.index('#include "late.h"'))

    def test_the_comment_stays_without_the_word_for_it(self):
        out = struct_migrate.without(SRC, "UnkTypedValue", "game/UnkTypedValue.h")
        self.assertIn("// A typed value as the section setter takes it", out)

    def test_a_header_that_is_included_is_not_included_twice(self):
        text = SRC.replace('#include "types.h"\n', '#include "game/UnkTypedValue.h"\n#include "types.h"\n')
        self.assertEqual(struct_migrate.without(text, "UnkTypedValue", "game/UnkTypedValue.h").count("UnkTypedValue.h"), 1)

    def test_a_source_without_includes_gets_it_under_its_comment(self):
        text = "// Unit 7.\nstruct UnkA {\n    int a;\n};\nint f(UnkA* p) {\n    return p->a;\n}\n"
        self.assertEqual(struct_migrate.without(text, "UnkA", "game/UnkA.h").split("\n")[:2], ["// Unit 7.", '#include "game/UnkA.h"'])

    def test_no_definition_no_edit(self):
        self.assertIsNone(struct_migrate.without("int f();\n", "UnkA", "game/UnkA.h"))


class Header(unittest.TestCase):
    BLOCK = "struct UnkA {\n    int a;\n};"

    def test_a_new_header(self):
        with tempfile.TemporaryDirectory() as tmp:
            text = struct_migrate.header_text(Path(tmp) / "UnkA.h", self.BLOCK, ["base/x.h"], "UnkA")
        self.assertTrue(text.startswith("#pragma once\n"))
        self.assertIn('#include "base/x.h"\n#include "types.h"\n\nstruct UnkA {', text)

    def test_an_existing_header_gets_it_once(self):
        with tempfile.TemporaryDirectory() as tmp:
            h = Path(tmp) / "UnkA.h"
            h.write_text("#pragma once\n\nstruct UnkB {\n    int b;\n};\n", encoding="utf-8")
            once = struct_migrate.header_text(h, self.BLOCK, [], "UnkA")
            h.write_text(once, encoding="utf-8")
            twice = struct_migrate.header_text(h, self.BLOCK, [], "UnkA")
        self.assertEqual(once.count("struct UnkA {"), 1)
        self.assertEqual(once, twice)
        self.assertIn("struct UnkB {", once)


if __name__ == "__main__":
    unittest.main()
