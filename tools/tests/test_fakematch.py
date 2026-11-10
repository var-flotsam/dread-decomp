"""tools/fakematch.py on small inline sources (no game data)."""

import unittest

from tools.tests import _path  # noqa: F401
import fakematch


def kinds(text: str) -> list[tuple[int, str, str | None]]:
    return [(h.line, h.kind, h.function) for h in fakematch.scan_text("x.cpp", text)]


class ScanTest(unittest.TestCase):
    def test_offset_cast_in_a_function(self):
        src = "u32 fn_7100000100(void* p) {\n    return *(u32*)((char*)p + 0x10);\n}\n"
        self.assertEqual(kinds(src), [(2, "offset-cast", "fn_7100000100")])

    def test_vtable_address_point_on_a_global_is_not_counted(self):
        src = "void fn_7100000100(Foo* f) {\n    f->vt = reinterpret_cast<char*>(g_unk_7101000000) + 0x10;\n}\n"
        self.assertEqual(kinds(src), [])

    def test_volatile_but_not_asm_volatile(self):
        src = ("void fn_7100000100(Foo* f) {\n    volatile s32 k = f->kind;\n"
               "    // keeps the reload\n    asm volatile(\"\" ::: \"memory\");\n}\n")
        self.assertEqual(kinds(src), [(2, "volatile", "fn_7100000100"), (4, "barrier", "fn_7100000100")])

    def test_barrier_needs_a_comment(self):
        bare = "void fn_7100000100(u64 n) {\n    asm(\"\" : \"+r\"(n));\n}\n"
        self.assertEqual([k for _, k, _ in kinds(bare)], ["barrier", "barrier-no-comment"])
        trailing = "void fn_7100000100(u64 n) {\n    asm(\"\" : \"+r\"(n));  // keeps n live\n}\n"
        self.assertEqual([k for _, k, _ in kinds(trailing)], ["barrier"])

    def test_empty_if_but_not_empty_loop(self):
        src = ("void fn_7100000100(Foo* f) {\n    if (f->a) {\n    }\n"
               "    while (Poll(f) == 2) {\n    }\n    for (;;) {}\n}\n")
        self.assertEqual(kinds(src), [(2, "empty-block", "fn_7100000100")])

    def test_patterns_in_comments_and_strings_are_ignored(self):
        src = ("// *(u32*)((char*)p + 0x10) volatile\n"
               "void fn_7100000100() {\n    Log(\"volatile (char*)p + 4\");\n}\n")
        self.assertEqual(kinds(src), [])

    def test_member_defined_in_its_struct(self):
        src = ("struct UnkQuery {\n    u32 a;\n    void Set(u32 v) {\n"
               "        __builtin_memcpy(&a, &v, 4);\n    }\n};\n")
        self.assertEqual(kinds(src), [(4, "untyped-store", "UnkQuery::(member)")])

    def test_attribute_before_the_name(self):
        src = ("__attribute__((visibility(\"hidden\"))) void fn_7100000100(void* p) {\n"
               "    *(u8*)((u8*)p + 3) = 0;\n}\n")
        self.assertEqual(kinds(src), [(2, "offset-cast", "fn_7100000100")])


if __name__ == "__main__":
    unittest.main()
