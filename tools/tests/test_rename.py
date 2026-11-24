"""tools/rename.py and tools/dread/names.py: the name map's rules, its application, the rewrite of the code and the
digest's spelling of a renamed callee. No compiler and no game data."""
import contextlib
import io
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

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


REVERT_SRC = '''// Calls ProjectToScreen's callee by address.
bool ProjectToScreen(void* self);
bool Other(void* self) {
    return ProjectToScreen(self);
}
bool ProjectToScreen(void* self) {
    return self != nullptr;
}
'''


class Revert(unittest.TestCase):
    """rename.revert on a tree of its own: a map, a one-file function list and one source."""

    def setUp(self):
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        self.root = Path(tmp.name)
        (self.root / "src").mkdir()
        self.src = self.root / "src" / "a.cpp"
        self.src.write_text(REVERT_SRC, encoding="utf-8", newline="")
        self.map = self.root / "names.csv"
        names.write([row(), row(address="0x7100000200", name="Kept")], self.map)
        self.frows = {"main_02.csv": [
            {"address": "0x7100000100", "size": "8", "name": "CFoo::refl_GetClass", "name_source": "reflection",
             "subsystem": "game", "status": "O"},
            {"address": "0x7100000200", "size": "8", "name": "Kept", "name_source": "inferred",
             "subsystem": "game", "status": "O"},
            {"address": "0x7100000300", "size": "8", "name": "fn_7100000300", "name_source": "unknown",
             "subsystem": "game", "status": "U"},
            {"address": "0x7100219588", "size": "8", "name": "ProjectToScreen", "name_source": "inferred",
             "subsystem": "game", "status": "O"}]}
        self.written: dict[str, list[dict]] = {}
        patches = [
            mock.patch.object(rename, "ROOT", self.root),
            mock.patch.object(rename, "tracked_files", lambda: ["src/a.cpp"]),
            mock.patch.object(rename, "tools_mentioning", lambda ident: []),
            mock.patch.object(rename.functions, "by_file",
                              lambda: {f: [dict(r) for r in rows] for f, rows in self.frows.items()}),
            mock.patch.object(rename.functions, "write", lambda name, rows: self.written.__setitem__(name, rows)),
            mock.patch.object(rename.names, "read", lambda path=None: names_read(self.map)),
            mock.patch.object(rename.names, "write", lambda rows, path=None: names_write(rows, self.map)),
        ]
        names_read, names_write = names.read, names.write
        for p in patches:
            p.start()
            self.addCleanup(p.stop)

    def run_revert(self, address, reason="the code does not support the name", dry_run=False):
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = rename.revert(address, reason, dry_run)
        return code, out.getvalue(), err.getvalue()

    def unchanged(self):
        return (self.written == {} and self.src.read_text(encoding="utf-8") == REVERT_SRC
                and [r["address"] for r in names.read(self.map)] == ["0x7100000200", "0x7100219588"])

    def test_restores_map_list_and_code(self):
        code, out, _ = self.run_revert(0x7100219588)
        self.assertEqual(code, 0)
        self.assertEqual([r["name"] for r in names.read(self.map)], ["Kept"])
        self.assertEqual(list(self.written), ["main_02.csv"])
        by_addr = {r["address"]: r for r in self.written["main_02.csv"]}
        self.assertEqual((by_addr["0x7100219588"]["name"], by_addr["0x7100219588"]["name_source"]),
                         ("fn_7100219588", "unknown"))
        untouched = [r for r in self.written["main_02.csv"] if r["address"] != "0x7100219588"]
        self.assertEqual(untouched, self.frows["main_02.csv"][:3])
        text = self.src.read_text(encoding="utf-8")
        self.assertEqual(text, REVERT_SRC.replace("bool ProjectToScreen(", "bool fn_7100219588(")
                         .replace("return ProjectToScreen(", "return fn_7100219588("))
        self.assertIn("// Calls ProjectToScreen's callee", text)
        self.assertIn("reason: the code does not support the name", out)

    def test_is_the_inverse_of_a_rename(self):
        self.assertEqual(self.run_revert(0x7100219588)[0], 0)
        self.frows = dict(self.written)
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = rename.rename(0x7100219588, "ProjectToScreen", "projects a point", False)
        self.assertEqual(code, 0, err.getvalue())
        self.assertEqual(self.src.read_text(encoding="utf-8"), REVERT_SRC)
        self.assertEqual(names.read(self.map), [row(address="0x7100000200", name="Kept"), row()])

    def test_refuses_a_name_the_binary_carries(self):
        code, _, err = self.run_revert(0x7100000100)
        self.assertEqual(code, 1)
        self.assertIn("named by the binary", err)
        self.assertTrue(self.unchanged())

    def test_refuses_an_address_with_no_name(self):
        code, _, err = self.run_revert(0x7100000300)
        self.assertEqual(code, 1)
        self.assertIn("no name to revert", err)
        self.assertTrue(self.unchanged())
        code, _, err = self.run_revert(0x7100999999)
        self.assertEqual(code, 1)
        self.assertIn("not a function", err)

    def test_needs_a_reason(self):
        code, _, err = self.run_revert(0x7100219588, reason="  ")
        self.assertEqual(code, 1)
        self.assertIn("needs its reason", err)
        self.assertTrue(self.unchanged())

    def test_dry_run_changes_nothing(self):
        code, out, _ = self.run_revert(0x7100219588, dry_run=True)
        self.assertEqual(code, 0)
        self.assertIn("ProjectToScreen -> fn_7100219588, 3 uses in 1 files", out)
        self.assertIn("dry run", out)
        self.assertTrue(self.unchanged())


class Tree(unittest.TestCase):
    def test_tracked_map_is_clean(self):
        self.assertEqual(rename.check(), [])


if __name__ == "__main__":
    unittest.main()
