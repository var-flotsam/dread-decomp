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


if __name__ == "__main__":
    unittest.main()
