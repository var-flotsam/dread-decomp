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

    def test_facts_cover_a_lifted_definitions_extra_registers(self):
        # fn_71001f530c's lift declares x0-x7 (it passes them on) but reads x0 only; a lifted caller
        # passes (char*, float, float, float): every register it reads is set, so the call is safe
        ref, dfn = ["char*", "float", "float", "float"], ["char*"] * 8
        self.assertEqual(pl.signature_class(ref, dfn, False)[0], "class_mismatch")
        sig, flags = pl.signature_class(ref, dfn, False, reads=(1, 0, 0))
        self.assertEqual(sig, "ref_more_args")
        self.assertIn("covered_by_facts", flags)
        # fn_7100dfde4c's lift declares d0 (handed to a variadic callee) but reads no float argument
        self.assertEqual(pl.signature_class(["char*", "unsigned long"], ["char*", "double"], False,
                                            reads=(1, 0, 0))[0], "ref_more_args")

    def test_facts_do_not_cover_a_register_the_caller_leaves_unset(self):
        # the definition really reads x1, which a caller passing (char*, float) does not set
        self.assertEqual(pl.signature_class(["char*", "float"], ["char*", "char*"], False, reads=(2, 0, 0))[0],
                         "class_mismatch")

    def test_facts_do_not_cover_aggregates_or_stack_arguments(self):
        # a {float, float, float} by value is packed into two xmm registers on x86-64
        self.assertEqual(pl.signature_class(["void*", "float", "float", "float"], ["void*", "CVector3D", "void*"],
                                            False, reads=(2, 3, 0))[0], "class_mismatch")
        # a function that reads its caller's stack: where each argument lands differs by architecture
        ref, dfn = ["char*", "float", "float"], ["char*"] * 9
        self.assertEqual(pl.signature_class(ref, dfn, False, reads=(1, 0, 0))[0], "ref_more_args")
        self.assertEqual(pl.signature_class(ref, dfn, False, reads=(1, 0, 1))[0], "class_mismatch")


class RuntimeOverloads(unittest.TestCase):
    CTORS = {"_ZN5movie6BufferC1Ev": "movie::Buffer::Buffer()",
             "_ZN5movie6BufferC1Em": "movie::Buffer::Buffer(unsigned long)",
             "_ZN5movie6BufferC1ERKS0_": "movie::Buffer::Buffer(movie::Buffer const&)",
             "_ZN5movie6BufferC1EPvm": "movie::Buffer::Buffer(void*, unsigned long)"}

    def test_the_consoles_name_picks_the_overload(self):
        # A lifted call passes `this` as its own argument: (char*) is the default constructor, not
        # the copy constructor its parameters look like.
        cands = sorted(self.CTORS)
        for imported, want in (("movie::Buffer::Buffer()", "_ZN5movie6BufferC1Ev"),
                               ("movie::Buffer::Buffer(movie::Buffer const&)", "_ZN5movie6BufferC1ERKS0_"),
                               ("movie::Buffer::Buffer(unsigned long)", "_ZN5movie6BufferC1Em")):
            self.assertEqual(pl.runtime_overload(cands, "callee::fn_7101239c90(char*)", self.CTORS, imported), want)

    def test_llp64_definition_and_fallback(self):
        dem = {"_ZN5movie6BufferC1Ey": "movie::Buffer::Buffer(unsigned long long)",
               "_ZN5movie6BufferC1Ev": "movie::Buffer::Buffer()"}
        cands = sorted(dem)
        self.assertEqual(pl.runtime_overload(cands, "f(char*, unsigned long)", dem,
                                             "movie::Buffer::Buffer(unsigned long)"), "_ZN5movie6BufferC1Ey")
        # No console name: the argument ranking, as before.
        self.assertEqual(pl.runtime_overload(cands, "f()", dem), "_ZN5movie6BufferC1Ev")


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


class VaListHandoffs(unittest.TestCase):
    def test_lifted_reference_to_a_va_list_definition(self):
        # fn_71010c0b40 (lifted, variadic) calls fn_71010bf400, hand-written with a va_list: its
        # AArch64 va_list would be read as the host's. A call to a plain definition is fine.
        rows = {f"0x{a:x}": {"address": f"0x{a:x}", "size": "4", "name": f"fn_{a:x}", "name_source": "unknown",
                             "subsystem": "", "status": "O"} for a in (0x71010BF400, 0x7100000010)}
        lk = pl.Link.__new__(pl.Link)
        lk.resolver = pl.Resolver(rows)
        vdef = "fn_71010bf400(char*, char const*, __va_list_tag*)"
        lk.demangled = {"_Zref": "fn_71010bf400(unsigned long, unsigned long, unsigned long)",
                        "_Zplain": "fn_7100000010(int)"}
        lk.code_defs = {0x71010BF400: [pl.Definition("T", "_Zdef", vdef, "fn_in_symbol", "own")],
                        0x7100000010: [pl.Definition("T", "_Zp", "fn_7100000010(int)", "fn_in_symbol", "own")]}
        obj = pl.Path("lifted/lifted_000.o")
        saved = pl.undefined_by_object
        pl.undefined_by_object = lambda nm, paths: {obj: ["_Zref", "_Zplain"]}
        try:
            (h,) = pl.va_list_handoffs("nm", lk, [obj])
        finally:
            pl.undefined_by_object = saved
        self.assertEqual((h["callee"], h["definition"]), ("0x71010bf400", vdef))
        self.assertTrue(pl.takes_va_list("f(std::__va_list)"))
        self.assertFalse(pl.takes_va_list("f(int, ...)"))


class SplitStatics(unittest.TestCase):
    """A placed static of a non-inline function (a local symbol) cannot be bound: reported, not bound."""

    def test_local_static_is_reported(self):
        symbols = [("V", "_ZZ13fn_7100000100vE1s"), ("V", "_ZGVZ13fn_7100000100vE1s"),
                   ("b", "_ZZ13fn_7100000200vE6s_heap"), ("b", "_ZZ13fn_7100000300vE6s_free")]
        known = {"_ZZ13fn_7100000100vE1s": 0x7101000000, "_ZGVZ13fn_7100000100vE1s": 0x7101000010,
                 "_ZZ13fn_7100000200vE6s_heap": 0x7101000100}
        bound, left = pl.bound_statics(symbols, {}, known, set())
        self.assertEqual([s for s, _ in bound], ["_ZGVZ13fn_7100000100vE1s", "_ZZ13fn_7100000100vE1s"])
        self.assertEqual(left[pl.SPLIT_STATIC], ["_ZZ13fn_7100000200vE6s_heap"])


if __name__ == "__main__":
    unittest.main()


class Bindings(unittest.TestCase):
    ROWS = {"0x7100000010": {}, "0x7100000020": {}}

    def read(self, text: str) -> dict[str, int]:
        import tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "bindings.tsv"
            p.write_text(text, encoding="utf-8")
            return pl.read_bindings(p)

    def test_rows_comments_and_blank_lines(self):
        got = self.read("# header\n\n_ZN1AC1Ev\t0x7100000010\tA::A()\n_ZN1AD1Ev\t0x7100000020\n")
        self.assertEqual(got, {"_ZN1AC1Ev": 0x7100000010, "_ZN1AD1Ev": 0x7100000020})

    def test_malformed_and_duplicate_rows_stop(self):
        for text in ("A::A()\t0x7100000010\n", "_ZN1AC1Ev 0x7100000010\n", "_ZN1AC1Ev\t7100000010\n",
                     "_ZN1AC1Ev\t0x7100000010\n_ZN1AC1Ev\t0x7100000020\n"):
            with self.assertRaises(SystemExit, msg=text):
                self.read(text)

    def test_matched_code_wins(self):
        known = {"_ZN1AC1Ev": 0x7100000010, "_ZN1BC1Ev": 0x7100000020}
        bindings = {"_ZN1AC1Ev": 0x7100000010, "_ZN1BC1Ev": 0x7100000010, "_ZN1CD1Ev": 0x7100000020,
                    "_ZN1DD1Ev": 0x7100000030}
        merged, notes = pl.merge_bindings(known, bindings, self.ROWS)
        self.assertEqual(merged, {**known, "_ZN1CD1Ev": 0x7100000020})
        self.assertEqual(notes["redundant"], ["_ZN1AC1Ev"])
        self.assertEqual(notes["placed elsewhere by matched code"], ["_ZN1BC1Ev"])
        self.assertEqual(notes["not a function start"], ["_ZN1DD1Ev"])
        self.assertEqual(notes["bound"], ["_ZN1CD1Ev"])
        self.assertEqual(known, {"_ZN1AC1Ev": 0x7100000010, "_ZN1BC1Ev": 0x7100000020})  # not modified


class ShippedBindings(unittest.TestCase):
    def test_port_bindings_parse_and_name_functions(self):
        import check
        rows, _ = check.load_csv()
        bindings = pl.read_bindings(pl.BINDINGS)
        self.assertTrue(bindings)
        for sym, a in bindings.items():
            self.assertIn(f"0x{a:010x}", rows, sym)


class BoundVtables(unittest.TestCase):
    def test_a_placed_weak_vtable_is_bound_to_the_image(self):
        # unk_handle::UnkHandle's vtable, emitted weak by every object that constructs one: the
        # copy's slots name methods no source defines, so the image's (placed) replaces it
        known = {"_ZTVN10unk_handle9UnkHandleE": 0x7101a209e0, "_ZTV3Foo": 0x7101000000}
        symbols = [("V", "_ZTVN10unk_handle9UnkHandleE"), ("V", "_ZTV3Bar"), ("D", "_ZTV3Foo"),
                   ("U", "_ZTV3Foo"), ("V", "_ZZ3fnvE1s")]
        self.assertEqual(pl.bound_vtables(symbols, known), [("_ZTVN10unk_handle9UnkHandleE", 0x7101a209e0)])
