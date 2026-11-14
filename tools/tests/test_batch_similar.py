"""tools/batch/lean/similar.py: which matched functions a worker is shown as references, on hand-assembled
AArch64 words and a made-up source tree (no game data)."""
import importlib.util
import struct
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
from tools.tests.test_nearcopy import RET, add_imm, adrp, bl, ldr_w, ldr_x, movz, str_x

KIT = Path(__file__).resolve().parents[1] / "batch" / "lean"
spec = importlib.util.spec_from_file_location("lean_similar", KIT / "similar.py")
similar = importlib.util.module_from_spec(spec)
spec.loader.exec_module(similar)

BASE = 0x7100000000
HELPER, OTHER, GLOBAL = BASE + 0x9000, BASE + 0x9100, BASE + 0x20010
STP = 0xA9BF7BFD   # stp x29, x30, [sp, #-16]!
LDP = 0xA8C17BFD   # ldp x29, x30, [sp], #16


def words(*w):
    return struct.pack(f"<{len(w)}I", *w)


def getter(pc, reg, off, callee):
    """Load a field, store it into a global's field, call a helper: 9 instructions."""
    return words(STP, ldr_x(reg, 0, off), adrp(9, pc + 8, GLOBAL), add_imm(9, 9, GLOBAL & 0xFFF), str_x(reg, 9, 8),
                 bl(pc + 20, callee), movz(0, 1), LDP, RET)


def counter(pc):
    """Nothing like getter: loads and constants only."""
    return words(ldr_w(1, 0, 4), ldr_w(2, 0, 8), ldr_w(3, 0, 12), movz(4, 7), movz(5, 9), ldr_w(6, 0, 16),
                 movz(0, 0), ldr_w(7, 0, 20), RET)


class Tree:
    """A source tree and its functions: add(addr, code, file, text) defines one."""

    def __init__(self, tmp):
        self.root, self.code, self.funcs = Path(tmp), {}, {}

    def add(self, addr, code, file=None, text=None, status="O"):
        self.code[addr] = code
        self.funcs[addr] = (len(code), f"fn_{addr:x}", status)
        if file:
            p = self.root / file
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(text or f"// A unit.\n#include \"types.h\"\n\nvoid fn_{addr:x}(void* self) {{\n}}\n",
                         encoding="utf-8")

    def index(self):
        return similar.Index(self.funcs, similar.definitions(self.root), lambda a, n: self.code[a][:n], self.root)


class Tokens(unittest.TestCase):
    def test_registers_and_constants_are_taken_out(self):
        a = similar.tokens(getter(BASE, 8, 0x10, HELPER), BASE)
        b = similar.tokens(getter(BASE + 0x400, 10, 0x48, OTHER), BASE + 0x400)
        self.assertEqual(a, b)
        self.assertEqual(a[1], "ldr x, [x, #]")
        self.assertEqual(a[0], "stp fp, lr, [sp, #]!")

    def test_the_callee_is_a_feature_of_its_own(self):
        code = getter(BASE, 8, 0x10, HELPER)
        f = similar.features(similar.tokens(code, BASE), similar.nearcopy.refs(BASE, code))
        self.assertIn(f"ref {HELPER:x}", f)
        self.assertIn(f"ref {GLOBAL:x}", f)
        self.assertIn("3 stp fp, lr, [sp, #]! | ldr x, [x, #] | adrp x, #", f)


class Definitions(unittest.TestCase):
    def test_a_definition_in_a_hand_written_source_and_nothing_else(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src/game/CFoo").mkdir(parents=True)
            (root / "src/lifted").mkdir(parents=True)
            (root / "src/game/CFoo/CFoo.cpp").write_text(
                "#include \"types.h\"\n"
                "void fn_7100000300(int);            // a declaration\n"
                "extern \"C\" void fn_7100000400(int);\n"
                "struct UnkFoo { int a; };\n"
                "\n"
                "// Resets it.\n"
                "void fn_7100000100(UnkFoo* self) {\n"
                "    fn_7100000300(self->a);\n"
                "}\n"
                "\n"
                "s32 fn_7100000200(UnkFoo* self,\n"
                "                  int b)\n"
                "{\n"
                "    return self->a + b;\n"
                "}\n", encoding="utf-8")
            (root / "src/lifted/l.cpp").write_text("void fn_7100000500() {\n}\n", encoding="utf-8")
            self.assertEqual(similar.definitions(root), {0x7100000100: ("src/game/CFoo/CFoo.cpp", 7),
                                                         0x7100000200: ("src/game/CFoo/CFoo.cpp", 11)})


class Nearest(unittest.TestCase):
    def tree(self, tmp):
        t = Tree(tmp)
        t.add(BASE + 0x100, getter(BASE + 0x100, 8, 0x10, HELPER), "src/game/CFoo/CFoo_Get_l000100.cpp")
        t.add(BASE + 0x200, counter(BASE + 0x200), "src/game/CFoo/CFoo_Count_l000200.cpp")
        for i in range(8):   # other matched functions, so that what the first two have is not common to all
            a = BASE + 0x1000 + 0x100 * i
            t.add(a, words(*[movz(k % 8, k + i) for k in range(6 + i)], RET), f"src/game/CBar/CBar_l{i}.cpp")
        t.add(BASE + 0x800, getter(BASE + 0x800, 10, 0x48, HELPER), status="U")   # the function to write
        return t

    def test_the_function_of_the_same_shape_and_callee_comes_first(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = self.tree(tmp)
            refs = t.index().nearest(BASE + 0x800, 36)
            self.assertEqual(refs[0]["addr"], BASE + 0x100)
            self.assertEqual(refs[0]["file"], "src/game/CFoo/CFoo_Get_l000100.cpp")
            self.assertEqual((refs[0]["kind"], refs[0]["lined"], refs[0]["same"]), ("alike", 1.0, 1.0))
            self.assertEqual((refs[0]["shared"], refs[0]["of"], refs[0]["named"]), ([HELPER, GLOBAL], 2, 1.0))
            self.assertNotIn(BASE + 0x200, [r["addr"] for r in refs])   # nothing alike: under MIN_LINED

    def test_the_function_that_names_the_same_callees_comes_second_however_unlike(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = self.tree(tmp)
            pc = BASE + 0x300   # other code, three times the size, the same helper and global
            caller = words(*[movz(k % 5, k) for k in range(14)], adrp(9, pc + 56, GLOBAL),
                           add_imm(9, 9, GLOBAL & 0xFFF), bl(pc + 64, HELPER), *[movz(1, k) for k in range(8)], RET)
            t.add(pc, caller, "src/game/CFoo/CFoo_Call_l000300.cpp")
            refs = t.index().nearest(BASE + 0x800, 36)
            self.assertEqual([(r["addr"], r["kind"]) for r in refs], [(BASE + 0x100, "alike"), (pc, "names")])
            self.assertEqual((refs[1]["named"], refs[1]["shared"]), (1.0, [HELPER, GLOBAL]))
            self.assertLess(refs[1]["lined"], similar.MIN_LINED)

    def test_a_function_that_does_not_match_or_is_generated_is_no_reference(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = self.tree(tmp)
            t.add(BASE + 0x300, getter(BASE + 0x300, 8, 0x10, HELPER), "src/game/CFoo/CFoo_m.cpp", status="m")
            t.add(BASE + 0x400, getter(BASE + 0x400, 8, 0x10, HELPER), "src/lifted/l.cpp")
            self.assertEqual([r["addr"] for r in t.index().nearest(BASE + 0x800, 36)], [BASE + 0x100])

    def test_a_file_with_a_form_the_brief_rules_out_is_not_shown(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = self.tree(tmp)
            t.add(BASE + 0x100, getter(BASE + 0x100, 8, 0x10, HELPER), "src/game/CFoo/CFoo_Get_l000100.cpp",
                  "void fn_7100000100(void* self) {\n    asm volatile(\"\" ::: \"memory\");\n}\n")
            t.add(BASE + 0x300, getter(BASE + 0x300, 8, 0x10, OTHER), "src/game/CFoo/CFoo_Get_l000300.cpp",
                  "__attribute__((always_inline)) inline void helper() {}\nvoid fn_7100000300(void* self) {\n}\n")
            self.assertEqual(t.index().nearest(BASE + 0x800, 36), [])

    def test_a_second_copy_of_a_reference_is_left_out(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = self.tree(tmp)
            t.add(BASE + 0x300, getter(BASE + 0x300, 11, 0x20, OTHER), "src/game/CFoo/CFoo_Get_l000300.cpp")
            self.assertEqual([r["addr"] for r in t.index().nearest(BASE + 0x800, 36)], [BASE + 0x100])

    def test_an_alike_reference_is_between_half_and_twice_the_size(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = self.tree(tmp)
            big = getter(BASE + 0x100, 8, 0x10, HELPER)[:-4] + words(*[movz(1, k) for k in range(12)], RET)
            t.add(BASE + 0x100, big, "src/game/CFoo/CFoo_Get_l000100.cpp")
            # 84 bytes against 36: too big to be alike, small enough to show what both name
            self.assertEqual([(r["addr"], r["kind"]) for r in t.index().nearest(BASE + 0x800, 36)],
                             [(BASE + 0x100, "names")])
            huge = big[:-4] + words(*[movz(2, k) for k in range(12)], RET)   # 132 bytes: over three times
            t.add(BASE + 0x100, huge, "src/game/CFoo/CFoo_Get_l000100.cpp")
            self.assertEqual(t.index().nearest(BASE + 0x800, 36), [])


class Render(unittest.TestCase):
    def test_the_section_shows_the_source_and_pairs_the_first_with_its_disassembly(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src").mkdir()
            (root / "src/a.cpp").write_text("// A unit.\nvoid fn_7100000100(void* self) {\n}\n", encoding="utf-8")
            long = ["// filler"] * similar.WHOLE_FILE + ["", "// Counts.", "// Twice.", "void fn_7100000200() {",
                                                         "    int a = 0;", "}", "", "void later() {", "}"]
            (root / "src/b.cpp").write_text("\n".join(long) + "\n", encoding="utf-8")
            refs = [{"addr": 0x7100000100, "size": 36, "kind": "alike", "lined": 0.91, "named": 0.5, "same": 1.0,
                     "file": "src/a.cpp", "line": 2, "shared": [0x7100009000], "of": 2},
                    {"addr": 0x7100000200, "size": 40, "kind": "names", "lined": 0.3, "named": 0.0, "same": None,
                     "file": "src/b.cpp", "line": similar.WHOLE_FILE + 4, "shared": [], "of": 2}]
            text = "\n".join(similar.render(refs, unit_of=lambda a: 7 if a == 0x7100000100 else 8, unit=7,
                                            listing=lambda a, n: f"<listing of {a:x}>", root=root))
            self.assertIn("[1] fn_7100000100, alike in code (36 bytes, 91% of its instructions line up with yours, "
                          "100% equal place for place, names 1 of your 2 callees and globals, same unit): "
                          "src/a.cpp line 2", text)
            self.assertIn("    both name: fn_7100009000", text)
            self.assertIn("// A unit.\nvoid fn_7100000100(void* self) {\n}", text)
            self.assertEqual(text.count("<listing of"), 1)
            self.assertIn("<listing of 7100000100>", text)
            self.assertIn("[2] fn_7100000200, names what you name (40 bytes, 30% of its instructions line up with "
                          "yours): src/b.cpp", text)
            self.assertIn("// Counts.\n// Twice.\nvoid fn_7100000200() {\n    int a = 0;\n}", text)
            self.assertNotIn("later", text)
            self.assertNotIn("filler", text)

    def test_no_reference_is_said(self):
        self.assertIn("there is no reference", similar.render([])[0])


if __name__ == "__main__":
    unittest.main()
