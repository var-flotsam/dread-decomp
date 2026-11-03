"""tools/gen_clones.py: kind table integrity and the pattern helpers on tiny instruction lists and
hand-assembled words (no game data)."""

import struct
import unittest
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401
import gen_clones as gc

BASE = 0x7100000000


class FakeOriginal:
    def __init__(self, functions=None, relocs=None):
        self.code = functions or {}
        self.sizes = {a: len(c) for a, c in self.code.items()}
        self.img = SimpleNamespace(relocs=relocs or {})

    def function(self, address):
        return self.code[address]


def relative(target: int):
    return SimpleNamespace(type=gc.R_AARCH64_RELATIVE, addend=target - BASE)


class KindTable(unittest.TestCase):
    def test_headers_exist(self):
        for name, k in gc.KINDS.items():
            with self.subTest(kind=name):
                self.assertTrue((gc.ROOT / "include" / k.header).is_file(), k.header)

    def test_word_variants_name_a_base_kind(self):
        variants = {n: k for n, k in gc.KINDS.items() if k.late}
        self.assertTrue(variants)
        for name, k in variants.items():
            with self.subTest(kind=name):
                self.assertIn(k.late, gc.KINDS)
                self.assertIsNone(gc.KINDS[k.late].late)
                self.assertIsNone(k.ref)
                self.assertFalse(k.exact)
                self.assertTrue(any(name == f"{k.late}_{el}" for el in gc.WORD_VARIANTS))

    def test_word_variant_replaces_element(self):
        name = next(n for n, k in gc.KINDS.items() if k.late and n.endswith("_u32"))
        k = gc.KINDS[name]
        base_text = gc.KINDS[k.late].instantiate("T", [("lsl", "x1, x8, #3")])
        text = k.instantiate("T", [("lsl", "x1, x8, #3")])
        self.assertIn("UnkCloneWord<T>", base_text)
        self.assertEqual(text, base_text.replace("UnkCloneWord<T>", "UnkCloneSlot32<T>"))

    def test_collection_probe_kinds(self):
        # every collection template gets a probe for owners unless it has a reference or is excluded
        for name, (_, _, refs) in gc.COLLECTIONS.items():
            with self.subTest(collection=name):
                if "owner" not in refs and (name, "owner") not in gc.NO_PROBE:
                    self.assertIsNone(gc.KINDS[f"{name}_owner"].ref)
        self.assertNotIn("iter_value_copy_owner", gc.KINDS)


class Collections(unittest.TestCase):
    def test_elem_size(self):
        self.assertEqual(gc.elem_size([("mov", "x0, x1"), ("lsl", "x1, x8, #4")]), 16)
        self.assertEqual(gc.elem_size([("lsl", "x1, x8, #0")]), 1)
        self.assertIsNone(gc.elem_size([("lsl", "x2, x8, #3")]))

    def test_collection_element_types(self):
        fmt = "template void f<{T}>();"
        self.assertEqual(gc.collection(fmt, "ptr")("T", []), "template void f<T*>();")
        self.assertEqual(gc.collection(fmt, "owner_a")("T", []), "template void f<UnkCloneOwner<T, UnkClonePolyA>>();")
        self.assertEqual(gc.collection(fmt, "elem")("T", [("lsl", "x1, x8, #5")]),
                         "template void f<UnkCloneElem<T, 32>>();")
        self.assertIsNone(gc.collection(fmt, "elem")("T", []))  # no size in the code

    def test_strid_member_and_ident(self):
        self.assertEqual(gc.strid_member(0x18, gc.STRID_POOL), "base::UnkStrIdAt<0x18>")
        self.assertEqual(gc.strid_member(0x8, gc.FILE_PATH_POOL),
                         f"base::UnkMemberAt<base::UnkStrIdMemberOf<g_unk_{gc.FILE_PATH_POOL:x}>, 0x8>")
        self.assertEqual(gc.strid_ident("T", gc.STRID_POOL), "UnkCloneStrId<T>")
        self.assertEqual(gc.strid_ident("T", gc.FILE_PATH_POOL), "UnkCloneStrIdB<T>")


class Text(unittest.TestCase):
    def test_widthless(self):
        for m, want in (("ldrb", "ldr"), ("sturh", "stur"), ("strh", "str"), ("ldr", "ldr"), ("ldrsb", "ldrsb"),
                        ("sub", "sub")):
            with self.subTest(m=m):
                self.assertEqual(gc.widthless(m), want)

    def test_calls_and_target(self):
        insns = [("bl", "#0x7100000010"), ("blr", "x8"), ("b", "#0x7100000020"), ("bl", "#0x7100000030")]
        self.assertEqual(gc.calls(insns), [0x7100000010, 0x7100000030])
        self.assertEqual(gc.target("#0x7100000020"), 0x7100000020)

    def test_wide_constant(self):
        insns = [("mov", "x8, #0x1111"), ("movk", "x8, #0x2222, lsl #16"), ("movk", "x8, #0x3333, lsl #32"),
                 ("movk", "x8, #0x4444, lsl #48")]
        self.assertEqual(gc.wide_constant(insns, "x8"), 0x4444333322221111)
        self.assertIsNone(gc.wide_constant(insns[:3], "x8"))
        self.assertIsNone(gc.wide_constant(insns, "x9"))

    def test_wide_constant32(self):
        insns = [("mov", "w9, #0x10"), ("movk", "w9, #0x1, lsl #16")]
        self.assertEqual(gc.wide_constant32(insns, "w9"), 0x10010)
        self.assertIsNone(gc.wide_constant32(insns[1:], "w9"))  # movk without a mov


class Casts(unittest.TestCase):
    INSNS = [("mov", "x19, x0"), ("mov", "x20, x1"), ("bl", "#0x7100000100"), ("cbz", "x20, @+0x20"),
             ("bl", "#0x7100000200"), ("ldr", "x8, [x20]"), ("blr", "x8"), ("bl", f"#{gc.IS_KIND_OF:#x}"),
             ("str", "x8, [x19, #0x30]"), ("ret", "")]

    def test_cast_calls(self):
        self.assertEqual(gc.cast_calls(self.INSNS), (0x7100000100, 0x7100000200))
        self.assertIsNone(gc.cast_calls([i for i in self.INSNS if i[0] != "blr"]))
        self.assertIsNone(gc.cast_calls(self.INSNS[:5]))

    def test_static_desc(self):
        insns = [("adrp", "x1, #0x7101000000"), ("add", "x1, x1, #0x120")]
        self.assertEqual(gc.static_desc(insns), 0x7101000120)
        self.assertIsNone(gc.static_desc([("adrp", "x2, #0x7101000000"), ("add", "x2, x2, #0x120")]))

    def test_cast_traits(self):
        lines = gc.cast_traits("T", (0x7100000100, 0x7100000200), "void", "char* s", "s")
        self.assertEqual(lines[0], "const base::reflection::CType* fn_7100000200();")
        self.assertIn("    static const CType* type() { return fn_7100000200(); }", lines)
        self.assertIn("    static void base(char* s) { fn_7100000100(s); }", lines)
        with_desc = gc.cast_traits("T", (0x7100000100, 0x7100000200), "bool", "char* s", "s", 0x7101000120)
        self.assertTrue(with_desc[0].startswith("extern char g_unk_7101000120[]"))
        self.assertIn("    static bool base(char* s) { return fn_7100000100(s); }", with_desc)

    def test_call_and_cast_arg(self):
        text = gc.call_and_cast_arg(FakeOriginal(), 0, "T", self.INSNS)
        self.assertTrue(text.endswith("template void base::reflection::refl_CallAndCastArg<T, 0x30>("
                                      "char*, base::reflection::UnkReflObject*);"))
        self.assertIsNone(gc.call_and_cast_arg(FakeOriginal(), 0, "T", self.INSNS[1:]))  # no mov x19, x0


class Disassembly(unittest.TestCase):
    def test_internal_branches_made_relative(self):
        a = 0x7100000100
        code = struct.pack("<4I",
                           0xB4000040,                                  # cbz x0, +8 (inside)
                           0x94000000 | (((0x7100000010 - (a + 4)) >> 2) & 0x3FFFFFF),  # bl outside
                           0x14000000 | ((-8 >> 2) & 0x3FFFFFF),        # b back to the start
                           0xD65F03C0)
        insns = gc.instructions(FakeOriginal({a: code}), a)
        self.assertEqual(insns, [("cbz", "x0, @+8"), ("bl", "#0x7100000010"), ("b", "@+0"), ("ret", "")])

    def test_got_target_and_list(self):
        insns = [("adrp", "x8, #0x7101000000"), ("ldr", "x8, [x8, #0x18]"),
                 ("adrp", "x9, #0x7101000000"), ("add", "x9, x9, #0x20"),
                 ("adrp", "x10, #0x7101000000"), ("mov", "x0, x1"), ("ldr", "x1, [x10, #0x28]")]
        orig = FakeOriginal(relocs={0x1000018: relative(0x7100000500),
                                    0x1000028: SimpleNamespace(type=257, addend=0)})
        self.assertEqual(gc.got_target(orig, insns, 0), 0x7100000500)
        self.assertIsNone(gc.got_target(orig, insns, 2))  # adrp + add is not a GOT load
        # in order; an adrp used by an add is skipped, a non-RELATIVE slot gives None
        self.assertEqual(gc.got_list(orig, insns), [0x7100000500, None])


if __name__ == "__main__":
    unittest.main()
