"""tools/struct_check.py on small hand-written sources (no game data)."""
import unittest

from tools.tests import _path  # noqa: F401
import struct_check

A = """// Unit 7.
#include "types.h"
struct UnkA;                      // a declaration, not a definition
struct UnkA {
    u32 a;   // 0x0
    u32 b;
};
namespace game {
struct UnkB : UnkBase {
    void* p;
    ~UnkB() { Release(p); }
};
}  // namespace game
void fn_7100000100(UnkA* self) {
    struct Local { int x; };      // inside a function: part of that body
    self->a = 1;
}
template <typename T> struct UnkBox { T value; };
template <> struct UnkBox<int> { int value; int extra; };
"""
B = """struct UnkA {
    u32 a;
    u32 b;                        // the same members, other comments and spacing
};
namespace game {
struct UnkB : UnkBase {
    void* p;
    ~UnkB() { delete p; }
};
}
union UnkBits { u32 word; f32 real; };
"""
C = """struct UnkA {
    u64 a;
};
"""


def names(text):
    return [(d.name, d.line, d.code) for d in struct_check.definitions(text, "src/x.cpp")]


class Definitions(unittest.TestCase):
    def test_what_counts_as_a_definition(self):
        self.assertEqual(names(A), [("UnkA", 4, False), ("game::UnkB", 9, True), ("template UnkBox", 18, False),
                                    ("template UnkBox<int>", 19, False)])
        self.assertEqual(names(B), [("UnkA", 1, False), ("game::UnkB", 6, True), ("UnkBits", 11, False)])

    def test_a_function_that_returns_a_struct_is_not_one(self):
        self.assertEqual(names("struct UnkA fn_7100000100(int a) {\n    return {};\n}\n"), [])

    def test_a_name_with_its_namespace_written_out(self):
        self.assertEqual(names("struct base::UnkVec : base::UnkBase {\n    int n;\n};\n"), [("base::UnkVec", 1, False)])

    def test_comments_and_spacing_do_not_make_two_texts(self):
        a = next(d for d in struct_check.definitions(A, "a.cpp") if d.name == "UnkA")
        b = next(d for d in struct_check.definitions(B, "b.cpp") if d.name == "UnkA")
        self.assertEqual(a.text, b.text)


class Kinds(unittest.TestCase):
    def by(self, **files):
        return struct_check.collect({f"src/{k}.cpp": v for k, v in files.items()})

    def test_one_file_is_not_a_finding(self):
        by = self.by(a=A)
        self.assertTrue(all(struct_check.kind(ds) is None for ds in by.values()))

    def test_same_other_and_code(self):
        by = self.by(a=A, b=B, c=C)
        self.assertEqual(struct_check.kind(by["UnkA"]), "other")          # c.cpp has another body
        self.assertEqual(struct_check.kind(self.by(a=A, b=B)["UnkA"]), "same")
        self.assertEqual(struct_check.kind(by["game::UnkB"]), "code")      # two destructors, one symbol
        self.assertIsNone(struct_check.kind(by["UnkBits"]))

    def test_the_summary_counts_names_and_extra_definitions(self):
        s = struct_check.summary(self.by(a=A, b=B, c=C))
        self.assertEqual((s["several"], s["copies"], s["same"], s["other"], s["code"]), (2, 3, 0, 1, 1))

    def test_the_ratchet_sees_a_new_body_and_not_a_new_copy(self):
        before = self.by(a=A, b=B)
        self.assertEqual(struct_check.worse(self.by(a=A, b=B, d=B), before), [])           # a copy of b's text
        found = struct_check.worse(self.by(a=A, b=B, c=C), before)
        self.assertEqual(len(found), 1)
        self.assertIn("UnkA: 1 -> 2 different definitions", found[0])
        self.assertEqual(struct_check.worse(before, before), [])                            # game::UnkB was there


if __name__ == "__main__":
    unittest.main()
