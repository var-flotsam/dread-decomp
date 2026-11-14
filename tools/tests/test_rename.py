"""tools/rename.py and tools/dread/names.py: the name map's rules, its application, the rewrite of the code and the
digest's spelling of a renamed callee. No compiler and no game data."""
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401
import equiv_test
import rename
from dread import names


def row(address="0x7100219588", name="ProjectToScreen", source="inferred", reason="projects a point"):
    return {"address": address, "name": name, "name_source": source, "reason": reason}


class NameRules(unittest.TestCase):
    def test_good_names(self):
        for n in ("ProjectToScreen", "UnkRenderManager::ProjectToScreen", "game::CFoo::Update"):
            self.assertIsNone(names.name_problem(n), n)

    def test_bad_names(self):
        for n in ("fn_7100219588", "UnkFoo::fn_7100219588", "CFoo::vf_0x10", "CFoo::refl_GetClass", "Foo(int)",
                  "Foo::", "class", "__Foo", "_Bar", "1Foo"):
            self.assertIsNotNone(names.name_problem(n), n)

    def test_rows(self):
        self.assertEqual(names.row_problems([row()]), [])
        bad = names.row_problems([row(), row(name="Other"), row(address="0x7100000100"),
                                  row(address="0x7100000200", name="X", source="rtti", reason=" ")])
        text = "\n".join(bad)
        self.assertIn("named again", text)
        self.assertIn("names 0x7100219588 too", text)
        self.assertIn("name_source 'rtti'", text)
        self.assertIn("needs its reason", text)

    def test_apply(self):
        frows = [{"address": "0x7100219588", "name": "fn_7100219588", "name_source": "unknown"},
                 {"address": "0x7100000100", "name": "CFoo::refl_GetClass", "name_source": "reflection"}]
        problems = names.apply(frows, [row(), row(address="0x7100000100", name="Mine"),
                                       row(address="0x7100000300", name="Gone")])
        self.assertEqual(frows[0]["name"], "ProjectToScreen")
        self.assertEqual(frows[0]["name_source"], "inferred")
        self.assertEqual(frows[1]["name"], "CFoo::refl_GetClass")
        self.assertEqual(len(problems), 2)


class Symbols(unittest.TestCase):
    def test_source_names_are_read_whole(self):
        self.assertEqual([n for *_, n in names.source_names("_ZN7Bar3Foo3BazEv")], ["Bar3Foo", "Baz"])
        self.assertEqual([n for *_, n in names.source_names("_Z13fn_7100219588P16UnkRenderManagerPKvjj")],
                         ["fn_7100219588", "UnkRenderManager"])

    def test_normalise(self):
        norm = names.SymbolNormaliser([row(), row(address="0x7100000100", name="UnkFoo::Update")])
        self.assertEqual(norm("_Z15ProjectToScreenP16UnkRenderManagerPKvjj"),
                         "_Z13fn_7100219588P16UnkRenderManagerPKvjj")
        self.assertEqual(norm("_ZN6UnkFoo6UpdateEv"), "_ZN6UnkFoo13fn_7100000100Ev")
        self.assertEqual(norm("_ZNK6UnkFoo6UpdateEv"), "_ZNK6UnkFoo13fn_7100000100Ev")
        # Another class's Update, and a name inside a longer identifier, are someone else's.
        self.assertEqual(norm("_ZN6UnkBar6UpdateEv"), "_ZN6UnkBar6UpdateEv")
        self.assertEqual(norm("_ZN7Bar3Foo6UpdateEv"), "_ZN7Bar3Foo6UpdateEv")
        self.assertEqual(norm("memcpy"), "memcpy")

    def test_empty_map_changes_nothing(self):
        self.assertEqual(names.SymbolNormaliser([])("_Z15ProjectToScreenv"), "_Z15ProjectToScreenv")

    def test_digest_survives_a_rename(self):
        before = {4: SimpleNamespace(type=283, symbol="_Z13fn_7100219588P16UnkRenderManagerPKvjj", addend=0,
                                     section=None, data=None)}
        after = {4: SimpleNamespace(type=283, symbol="_Z15ProjectToScreenP16UnkRenderManagerPKvjj", addend=0,
                                    section=None, data=None)}
        old = names._normaliser
        try:
            names._normaliser = names.SymbolNormaliser([row()])
            self.assertEqual(equiv_test.digest(b"\x00" * 8, before), equiv_test.digest(b"\x00" * 8, after))
            names._normaliser = names.SymbolNormaliser([])
            self.assertNotEqual(equiv_test.digest(b"\x00" * 8, before), equiv_test.digest(b"\x00" * 8, after))
        finally:
            names._normaliser = old


SRC = '''// Calls fn_7100219588 (kept: comments name callees by address).
#include "types.h"
namespace game {
struct UnkFoo {
    void fn_7100000100();
};
void UnkFoo::fn_7100000100() {
    fn_7100219588(nullptr, "fn_7100219588");
}
}
bool fn_7100219588(void* self, const char* s) {
    return self != nullptr;
}
'''


class Rewrite(unittest.TestCase):
    def test_code_only(self):
        out, n, literal = rename.replace_in_code(SRC, "fn_7100219588", "ProjectToScreen")
        self.assertEqual(n, 2)
        self.assertEqual(literal, 1)
        self.assertIn("// Calls fn_7100219588", out)
        self.assertIn('ProjectToScreen(nullptr, "fn_7100219588")', out)
        self.assertIn("bool ProjectToScreen(void* self", out)

    def test_no_partial_identifiers(self):
        out, n, _ = rename.replace_in_code("UnkRefl_fn_7100219588 x; UnkStruct_7100219588_Xform y;",
                                           "fn_7100219588", "P")
        self.assertEqual(n, 0)

    def test_definition_scopes(self):
        self.assertEqual(rename.definition_scopes(SRC, "fn_7100219588"), [""])
        self.assertEqual(rename.definition_scopes(SRC, "fn_7100000100"), ["game::UnkFoo"])
        inline = "struct UnkFoo {\n    void fn_7100000100() {}\n};\nvoid fn_7100000100();\n"
        self.assertEqual(rename.definition_scopes(inline, "fn_7100000100"), [])

    def test_map_file_round_trip(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "names.csv"
            names.write([row(address="0x7100000200", name="B"), row()], p)
            self.assertEqual([r["address"] for r in names.read(p)], ["0x7100000200", "0x7100219588"])


class Tree(unittest.TestCase):
    def test_tracked_map_is_clean(self):
        self.assertEqual(rename.check(), [])


if __name__ == "__main__":
    unittest.main()
