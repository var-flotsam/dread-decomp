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


class BarrierRemoval(unittest.TestCase):
    LINES = ["void fn_7100000100(Foo* f, u64 n) {", "    u32 a = f->a;", "    // keeps n live across the call",
             "    // (the original reloads it)", '    asm("" : "+r"(n));', "    Use(a);",
             '    asm volatile("" ::: "memory");  // reads f->b again', "    f->b = n;",
             '    if (a) asm("" : "+r"(a));', "}"]

    def test_a_bare_barrier_goes_alone(self):
        out = fakematch.without_barrier(self.LINES, 5, commented=False)
        self.assertEqual(len(out), len(self.LINES) - 1)
        self.assertIn("    // (the original reloads it)", out)

    def test_a_commented_barrier_takes_the_comment_block_above_it(self):
        out = fakematch.without_barrier(self.LINES, 5, commented=True)
        self.assertEqual(out[:3], self.LINES[:2] + ["    Use(a);"])

    def test_a_comment_on_the_barrier_s_line_goes_with_the_line_only(self):
        out = fakematch.without_barrier(self.LINES, 7, commented=True)
        self.assertEqual(out, self.LINES[:6] + self.LINES[7:])

    def test_a_barrier_beside_other_code_or_in_a_macro_is_not_touched(self):
        self.assertIsNone(fakematch.without_barrier(self.LINES, 9, commented=False))
        self.assertIsNone(fakematch.without_barrier(['    asm("" : "+r"(x)); \\'], 1, commented=False))
        self.assertIsNone(fakematch.without_barrier(self.LINES, 2, commented=False))     # not a barrier
        self.assertIsNone(fakematch.without_barrier(self.LINES, 40, commented=False))

    def test_inline_asm_with_an_instruction_is_not_a_barrier(self):
        self.assertIsNone(fakematch.without_barrier(['    asm("nop");'], 1, commented=False))


class Unset(unittest.TestCase):
    STDERR = (
        "C:\\repo\\src\\game\\a.cpp:14:31: warning: variable 'dead' is uninitialized when used here [-Wuninitialized]\n"
        "    return fn_71005fd134(dead, a, b);\n"
        "                         ^~~~\n"
        "C:\\repo\\src\\game\\a.cpp:13:15: note: initialize the variable 'dead' to silence this warning\n"
        "/repo/src/game/a.cpp:33:9: warning: variable 'result' is used uninitialized whenever 'if' condition is false "
        "[-Wsometimes-uninitialized]\n"
        "/repo/src/game/a.cpp:40:5: warning: unused variable 'k' [-Wunused-variable]\n")

    def test_only_reads_of_an_unset_variable_are_taken(self):
        self.assertEqual(fakematch.unset_warnings(self.STDERR),
                         [(14, "variable 'dead' is uninitialized when used here"),
                          (33, "variable 'result' is used uninitialized whenever 'if' condition is false")])

    def test_a_clean_compile_has_none(self):
        self.assertEqual(fakematch.unset_warnings(""), [])


if __name__ == "__main__":
    unittest.main()
