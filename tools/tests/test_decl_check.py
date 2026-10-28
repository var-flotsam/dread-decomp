"""tools/decl_check.py on small hand-written sources (no game data)."""

import unittest

from tools.tests import _path  # noqa: F401
import decl_check


def sigs(text: str) -> dict[str, list[str]]:
    out: dict[str, list[str]] = {}
    for d in decl_check.scan(text, "src/x.cpp"):
        out.setdefault(d.name, []).append(d.signature)
    return out


class Normalisation(unittest.TestCase):
    def test_parameter_names_and_defaults_dropped(self):
        a = sigs("void fn_7100000010(int count, char* p = nullptr);")
        b = sigs("void fn_7100000010(int, char*);")
        self.assertEqual(a, b)

    def test_void_parameter_list(self):
        self.assertEqual(sigs("int fn_7100000020(void);"), sigs("int fn_7100000020();"))

    def test_extern_variable_counts_definition_does_not(self):
        self.assertIn("g_unk_7100000030", sigs("extern int g_unk_7100000030;"))
        self.assertNotIn("g_unk_7100000030", sigs("int g_unk_7100000030;"))

    def test_definitions_and_members_skipped(self):
        self.assertEqual(sigs("void fn_7100000040(int a) { (void)a; }"), {})
        self.assertEqual(sigs("void CFoo::fn_7100000050(int);"), {})

    def test_namespace_scope(self):
        found = decl_check.scan("namespace a { void fn_7100000060(int); }", "src/x.cpp")
        self.assertEqual([(d.name, d.scope) for d in found], [("fn_7100000060", "a")])

    def test_asm_label_names_the_symbol(self):
        self.assertIn("fn_7100000070", sigs('void Ctor(void* self) asm("fn_7100000070");'))


class Counting(unittest.TestCase):
    def test_conflict_and_header_redeclared(self):
        r = decl_check.Report({
            "include/game/A.h": "void fn_7100000080(int);",
            "src/game/b.cpp": "void fn_7100000080(long);",
            "src/game/c.cpp": "void fn_7100000090(int);",
        })
        self.assertEqual(r.conflicting, ["fn_7100000080"])
        self.assertEqual(r.header_redeclared, ["fn_7100000080"])
        self.assertEqual(r.counts()["declared"], 2)


if __name__ == "__main__":
    unittest.main()
