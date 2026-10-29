"""tools/port_link.py's choice among the definitions of one address, on made-up names (no build)."""

import unittest
from collections import Counter

from tools.tests import _path  # noqa: F401
import port_link as pl

GET = "base::reflection::CType const* base::reflection::refl_GetPrimitiveType<UnkRefl_fn_7100009658>()"
MAKE = ("base::reflection::CType base::reflection::refl_MakePrimitiveType<UnkRefl_fn_7100009658>"
        "(base::global::CStrId const&)")


class ReturnType(unittest.TestCase):
    def test_class_by_value(self):
        self.assertTrue(pl.returns_class_by_value(MAKE))

    def test_pointer_scalar_void_and_plain_names(self):
        for dem in (GET, "void base::refl_IterSync<Unk_fn_7100000010>(int)", "unsigned long f<Unk_fn_7100000010>()",
                    "fn_7100000010(int)", "void* operator new<Unk_fn_7100000010>(unsigned long)",
                    "base::UnkVector<int>& g<Unk_fn_7100000010>()"):
            self.assertFalse(pl.returns_class_by_value(dem), dem)


class ArgumentClasses(unittest.TestCase):
    def test_each_class_in_its_own_registers(self):
        self.assertEqual(pl.signature_class(["int", "float", "bool"], ["char*", "char*", "float"], False)[0], "same")
        self.assertEqual(pl.signature_class(["int", "float"], ["char*", "char*", "float"], False)[0], "ref_fewer_args")

    def test_integers_on_the_stack_keep_their_order(self):
        ref = ["char*"] * 8 + ["double", "float"] + ["double"] * 6
        dfn = ["char*"] * 8 + ["unsigned long"] + ["double"] * 8
        self.assertEqual(pl.signature_class(ref, dfn, False)[0], "ref_fewer_args")
        mixed = ["char*", "char*", "float", "char*", "char*", "char*", "char*", "char*", "char*"]
        self.assertEqual(pl.signature_class(mixed, ["char*"] * 8 + ["float"], False)[0], "same")

    def test_a_float_for_an_integer_is_still_a_mismatch(self):
        self.assertEqual(pl.signature_class(["float"], ["char*"], False)[0], "class_mismatch")
        self.assertEqual(pl.signature_class(["int", "float"], ["float", "float"], False)[0], "class_mismatch")


class TaggedTemplates(unittest.TestCase):
    def test_getter_over_inline_helper(self):
        # The lifted caller's declaration passes one (leftover) argument, which matches the helper's
        # parameter count better: the helper still loses, its CType result would go through rdi.
        lk = pl.Link.__new__(pl.Link)
        lk.resolver = pl.Resolver({"0x7100009658": {"address": "0x7100009658", "size": "28", "name": "fn_7100009658",
                                                       "name_source": "unknown", "subsystem": "", "status": "O"}})
        lk.demangled = {"_ZN6callee13fn_7100009658Em": "callee::fn_7100009658(unsigned long)",
                        "_Zget": GET, "_Zmake": MAKE}
        lk.undefined = ["_ZN6callee13fn_7100009658Em"]
        lk.relocs = {"_ZN6callee13fn_7100009658Em": Counter({"R_X86_64_PLT32": 1})}
        lk.code_defs = {0x7100009658: [pl.Definition("W", "_Zmake", MAKE, "fn_in_symbol", "tag"),
                                       pl.Definition("W", "_Zget", GET, "fn_in_symbol", "tag")]}
        lk.pointer_only, lk.data_defs = {}, {}
        (e,) = [e for e in pl.classify(lk, {}, set()) if e.symbol == "_ZN6callee13fn_7100009658Em"]
        self.assertEqual(e.target, "_Zget")
        self.assertEqual(e.candidates, 2)


if __name__ == "__main__":
    unittest.main()
