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


class Definitions(unittest.TestCase):
    def mismatches(self, files):
        return {s: v[0] for s, v in decl_check.definition_mismatches(files, decl_check.Report(files)).items()}

    def test_call_shape(self):
        self.assertEqual(decl_check.call_shape("u32 fn_7100000100(UnkA*, const UnkB&, bool, f32, UnkC, ...)", "fn_7100000100"),
                         ("i32", ("pointer", "pointer", "bool", "f32", "by value", "...")))
        self.assertEqual(decl_check.call_shape("void fn_7100000100()", "fn_7100000100"), ("void", ()))
        self.assertIsNone(decl_check.call_shape('void Ctor(void*) asm("fn_7100000100")', "fn_7100000100"))

    def test_a_definition_is_compared_with_the_declarations_of_other_files(self):
        files = {
            "include/game/A.h": "struct UnkA;\nu32 fn_7100000110(UnkA* self, u32 kind);\nvoid fn_7100000120(UnkA*);\n"
                                "void fn_7100000130(UnkA*, u32);\nvoid fn_7100000140(void*, u32);\n"
                                "void fn_7100000150(int);\nvoid fn_7100000180(f32, UnkA*, u32);\n"
                                "u64 fn_7100000190(UnkA*, void*);\n",
            "src/game/a.cpp": "struct UnkLocal;\n"
                              "void fn_7100000110(UnkLocal* self, u32 kind) {\n}\n"          # void, declared u32
                              "void fn_7100000120(UnkLocal* self) {\n}\n"                    # another struct name: same call
                              "void fn_7100000130(UnkLocal* self) {\n}\n"                    # a parameter short
                              "void fn_7100000140(void* self, void* context) {\n}\n"         # a pointer for a u32
                              "static void fn_7100000150(long a) {\n}\n"                     # internal linkage: not compared
                              "void fn_7100000160(int) {\n}\n"                               # nobody declares it
                              "void fn_7100000180(UnkLocal* self, u32 n, f32 t) {\n}\n"    # the float moved
                              "void* fn_7100000190(u64 key, s64 n) {\n    return 0;\n}\n",   # 64-bit integers for pointers: one call
        }
        self.assertEqual(self.mismatches(files), {
            "fn_7100000110": "one returns a value, the other is void",
            "fn_7100000130": "the number of parameters",
            "fn_7100000140": "a parameter's class",
            "fn_7100000180": "the order of float and other parameters (one call on AArch64; not on Windows)",
        })

    def test_a_definition_whose_types_are_no_declaration_s_is_reported(self):
        files = {
            "include/game/A.h": "struct UnkA;\nnamespace base { struct CStrId; }\n"
                                "void fn_7100000210(UnkA* self);\nUnkA* fn_7100000220(u32 n);\n"
                                "void fn_7100000230(const base::CStrId& id);\nvoid fn_7100000240(UnkA* self, u32 n);\n"
                                "void fn_7100000250(UnkA* self);\n",
            "src/game/b.cpp": "void fn_7100000250(UnkOther* self);\n",                       # a second declaration
            "src/game/a.cpp": "struct UnkLocal;\nusing base::CStrId;\n"
                              "void fn_7100000210(UnkLocal* self) {\n}\n"                    # another struct name
                              "const char* fn_7100000220(u32 n) {\n    return 0;\n}\n"       # another result
                              "void fn_7100000230(const CStrId& id) {\n}\n"                  # the namespace left out
                              "void fn_7100000240(UnkA* self, s32 n) {\n}\n"                 # another parameter
                              "void fn_7100000250(UnkOther* self) {\n}\n"                    # one declaration's text
                              "void fn_7100000260(UnkLocal* self) {\n}\n",                   # nobody declares it
        }
        found = decl_check.definition_type_differences(files, decl_check.Report(files))
        self.assertEqual({s: v[0] for s, v in found.items()}, {
            "fn_7100000210": "the name of a pointed-to type",
            "fn_7100000220": "the result's type",
            "fn_7100000230": "a namespace written out",
            "fn_7100000240": "a parameter's type",
        })
        self.assertEqual(found["fn_7100000210"][1:], ("src/game/a.cpp", "void fn_7100000210(UnkLocal*)",
                                                      "void fn_7100000210(UnkA*)"))

    def test_a_declaration_in_the_defining_file_does_not_count(self):
        files = {"src/game/a.cpp": "struct UnkA;\nvoid fn_7100000270(void* self);\nvoid fn_7100000270(UnkA* self) {\n}\n"}
        self.assertEqual(decl_check.definition_type_differences(files, decl_check.Report(files)), {})

    def test_only_a_new_type_difference_fails_and_a_namespace_never_does(self):
        def found(*kinds):
            return {f"fn_71000003{n:02x}": (what, "src/a.cpp", "", "") for n, what in kinds}
        base = found((0, "the name of a pointed-to type"), (1, decl_check.NAMESPACE_ONLY))
        cur = found((0, "the result's type"),                      # was there already, under another kind
                    (1, "a parameter's type"),                     # was the same type before: new
                    (2, decl_check.NAMESPACE_ONLY),                # the same type: never counts
                    (3, "the name of a pointed-to type"))          # new
        self.assertEqual(decl_check.new_type_differences(cur, base), ["fn_7100000301", "fn_7100000303"])
        self.assertEqual(decl_check.new_type_differences(base, base), [])
        self.assertEqual(decl_check.beyond_namespace(cur), 3)

    def test_the_definition_gets_its_line(self):
        found = decl_check.definitions("// a comment\n\nbool fn_7100000170(const char* name,\n    u32 n) {\n    return n;\n}\n", "src/x.cpp")
        self.assertEqual([(d.name, d.signature, d.line) for d in found], [("fn_7100000170", "bool fn_7100000170(const char*, u32)", 3)])


if __name__ == "__main__":
    unittest.main()
