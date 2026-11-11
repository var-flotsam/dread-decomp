"""tools/decl_plan.py: which shared declarations are planned for a unit header, and why the rest are not.

Made-up sources and units; no game data, no compiler.
"""

import unittest

from tools.tests import _path  # noqa: F401
import decl_check
import decl_plan

UNITS = [
    {"index": "7", "start": "0x7100000100", "end": "0x71000001ff", "class_hint": "game::CFoo", "hint_source": "reflection",
     "confidence": "high"},
    {"index": "8", "start": "0x7100000200", "end": "0x71000002ff", "class_hint": "CBar", "hint_source": "guess",
     "confidence": "high"},
    {"index": "9", "start": "0x7100000300", "end": "0x71000003ff", "class_hint": "base::snd::CMixer;CVoice",
     "hint_source": "names", "confidence": "high"},
]


def tree(**files: str) -> dict[str, str]:
    return {f"src/{name}.cpp" if "/" not in name else name: text for name, text in files.items()}


class Plan(unittest.TestCase):
    def plan(self, files):
        return decl_plan.plan(decl_check.Report(files), files, UNITS)

    def test_a_shared_function_moves_to_its_units_header(self):
        files = tree(a="struct UnkA;\nvoid fn_7100000110(UnkA* self, u32 n);\n",
                     b="struct UnkA { int x; };\nvoid fn_7100000110(UnkA* self, u32 n);\n")
        planned, why = self.plan(files)
        self.assertEqual([(p.symbol, p.header, p.copies, p.types) for p in planned],
                         [("fn_7100000110", "include/game/CFoo.h", 2, {"UnkA": "struct"})])
        self.assertEqual(planned[0].declaration, "void fn_7100000110(UnkA* self, u32 n);")
        self.assertEqual(why, {})

    def test_unit_folders(self):
        self.assertEqual([decl_plan.unit_dir(u) for u in UNITS], ["game/CFoo", "game/UnkTU8", "base/snd/CMixer"])

    def test_what_is_left_out(self):
        files = tree(
            a="extern int g_unk_7100900000;\nvoid fn_7100000120(int);\nvoid fn_7100000130(int);\n"
              "namespace n { void fn_7100000140(int); }\nvoid fn_7100000150(base::UnkX*);\nvoid fn_7100000160(UnkGone*);\n"
              "void fn_7100000900(int);\n",
            b="extern int g_unk_7100900000;\nvoid fn_7100000120(u32);\nvoid fn_7100000130(int);\n"
              "namespace n { void fn_7100000140(int); }\nvoid fn_7100000150(base::UnkX*);\nvoid fn_7100000160(UnkGone*);\n"
              "void fn_7100000900(int);\n",
            **{"include/x.h": "void fn_7100000130(int);\n"})
        planned, why = self.plan(files)
        self.assertEqual(planned, [])
        self.assertEqual(dict(why), {
            "a global, not a function": 1,                                       # g_unk_7100900000
            "conflicting signatures": 1,                                         # fn_7100000120
            "already in a header under include/": 1,                             # fn_7100000130
            "declared inside a namespace or struct": 1,                          # fn_7100000140
            "namespaced, template, function-pointer or asm-label types": 1,      # fn_7100000150
            "a type that is not a plain struct declared in the file": 1,         # fn_7100000160
            "in no unit": 1,                                                     # fn_7100000900
        })

    def test_header_and_chunks(self):
        files = tree(a="union UnkU;\nstruct UnkA;\nvoid fn_7100000110(UnkA*);\nint fn_7100000111(UnkU*);\nvoid fn_7100000310();\n",
                     b="union UnkU;\nstruct UnkA;\nvoid fn_7100000110(UnkA*);\nint fn_7100000111(UnkU*);\nvoid fn_7100000310();\n")
        planned, _ = self.plan(files)
        text = decl_plan.header_text("include/game/CFoo.h", [p for p in planned if p.header == "include/game/CFoo.h"])
        self.assertTrue(text.startswith("#pragma once\n\n// CFoo: functions of this translation unit"))
        self.assertTrue(text.endswith('#include "types.h"\n\nstruct UnkA;\nunion UnkU;\n'))
        # A header's symbols stay in one plan file.
        parts = decl_plan.chunks(planned, 2)
        self.assertEqual([sorted(p.symbol for p in part) for part in parts],
                         [["fn_7100000310"], ["fn_7100000110", "fn_7100000111"]])

    def test_comments_on_the_copies(self):
        files = tree(
            a="struct UnkA;\n// Sets up the two.\nvoid fn_7100000112(UnkA*);\n"
              "void fn_7100000110(UnkA*);  // the base constructor\n",
            b="struct UnkA;\n\nvoid fn_7100000110(UnkA*);  // the base constructor\nvoid fn_7100000112(UnkA*);\n",
            c="struct UnkA;\n// Not about them.\n\nvoid fn_7100000110(UnkA*);\n")
        planned, _ = self.plan(files)
        self.assertEqual(decl_plan.copy_comments(decl_check.Report(files), files, planned), [
            ("fn_7100000110", "include/game/CFoo.h", 2, "line", "the base constructor"),
            ("fn_7100000110", "include/game/CFoo.h", 1, "above", "Sets up the two."),
            ("fn_7100000112", "include/game/CFoo.h", 1, "above", "Sets up the two."),
        ])

    def test_globals_go_to_the_unit_whose_data_span_holds_them(self):
        spans = {0x7101a00010: "7", 0x7101a00020: "8", 0x7101a00030: "7", 0x7101a00040: "7"}
        both = ("struct UnkA;\nextern UnkA* g_unk_7101a00010;\n"
                '#define HIDDEN __attribute__((visibility("hidden")))\nextern char g_unk_7101a00020[] HIDDEN;\n'
                "extern UnkA g_unk_7101a00040[];\nextern int g_unk_7101a00050;\n")
        files = tree(a=both + "extern int g_unk_7101a00030;\n",
                     b=both + 'extern int g_unk_7101a00030 __attribute__((visibility("hidden")));\n')
        planned, why = decl_plan.plan(decl_check.Report(files), files, UNITS, spans.get)
        self.assertEqual([(p.symbol, p.header, p.declaration, p.types) for p in planned], [
            ("g_unk_7101a00010", "include/game/CFoo.h", "extern UnkA* g_unk_7101a00010;", {"UnkA": "struct"}),
            ("g_unk_7101a00020", "include/game/UnkTU8.h",
             'extern char g_unk_7101a00020[] __attribute__((visibility("hidden")));', {}),
        ])
        self.assertEqual(dict(why), {"a global whose copies differ in their attributes": 1,     # ..30
                                     "a global array of a struct type": 1,                      # ..40
                                     "a global in no unit's data span": 1})                     # ..50
        text = decl_plan.header_text("include/game/CFoo.h", planned[:1])
        self.assertIn("CFoo: functions and globals of this translation unit that other files use (tools/tu_map.py unit 7,",
                      text)

    def test_an_existing_unit_header_is_extended(self):
        files = tree(a="struct UnkA;\nstruct UnkB;\nvoid fn_7100000110(UnkA*, UnkB*);\nextern UnkB* g_unk_7101a00010;\n",
                     b="struct UnkA;\nstruct UnkB;\nvoid fn_7100000110(UnkA*, UnkB*);\nextern UnkB* g_unk_7101a00010;\n")
        planned, _ = decl_plan.plan(decl_check.Report(files), files, UNITS, {0x7101a00010: "7"}.get)
        old = ("#pragma once\n\n// CFoo: functions of this translation unit that other files call (tools/tu_map.py unit 7,\n"
               '// 0x7100000100-0x71000001ff).\n#include "types.h"\n\nstruct UnkA;\nvoid fn_7100000111(UnkA*);\n')
        self.assertEqual(decl_plan.extended(old, planned), (
            "#pragma once\n\n// CFoo: functions and globals of this translation unit that other files use "
            '(tools/tu_map.py unit 7,\n// 0x7100000100-0x71000001ff).\n#include "types.h"\n\nstruct UnkB;\nstruct UnkA;\n'
            "void fn_7100000111(UnkA*);\n"))
        bare = '#pragma once\n#include "types.h"\nvoid fn_7100000111();\n'
        self.assertEqual(decl_plan.extended(bare, planned),
                         '#pragma once\n#include "types.h"\n\nstruct UnkA;\nstruct UnkB;\n\nvoid fn_7100000111();\n')


if __name__ == "__main__":
    unittest.main()
