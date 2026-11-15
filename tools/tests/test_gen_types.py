"""tools/gen_types.py: C++ emission, declaration selection and the constructor emulator on
hand-assembled code in a fake module (no game data)."""

import struct
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import gen_types as gt

BASE = 0x7100000000
DATA_PAGE = 0x7100004000
KIND_STR = DATA_PAGE + 0x100
NAME_STR = DATA_PAGE + 0x200
GOT_SLOT = DATA_PAGE + 0x800
COPY_FN = 0x7100000040        # a type-specific function the constructor stores
POINTEE_GETTER = 0x7100000080
TAIL = 0x71000000C0


class Asm:
    def __init__(self, pc: int):
        self.start = self.pc = pc
        self.words: list[int] = []

    def _emit(self, w):
        self.words.append(w)
        self.pc += 4
        return self

    def movz(self, rd, imm):
        return self._emit(0xD2800000 | (imm << 5) | rd)

    def mov(self, rd, rm):
        return self._emit(0xAA0003E0 | (rm << 16) | rd)

    def add(self, rd, rn, imm):
        return self._emit(0x91000000 | (imm << 10) | (rn << 5) | rd)

    def adrp(self, rd, target):
        off = ((target >> 12) - (self.pc >> 12)) & 0x1FFFFF
        return self._emit(0x90000000 | ((off & 3) << 29) | ((off >> 2) << 5) | rd)

    def addr(self, rd, target):
        return self.adrp(rd, target).add(rd, rd, target & 0xFFF)

    def ldr(self, rt, rn, off):
        return self._emit(0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt)

    def str(self, rt, rn, off):  # rt 31 = xzr
        return self._emit(0xF9000000 | ((off // 8) << 10) | (rn << 5) | rt)

    def bl(self, target):
        return self._emit(0x94000000 | (((target - self.pc) >> 2) & 0x3FFFFFF))

    def b(self, target):
        return self._emit(0x14000000 | (((target - self.pc) >> 2) & 0x3FFFFFF))

    def ret(self):
        return self._emit(0xD65F03C0)

    def code(self):
        return b"".join(struct.pack("<I", w) for w in self.words)


class FakeImage:
    def __init__(self, strings: dict[int, str], pointers: dict[int, int]):
        data = bytearray(DATA_PAGE - BASE + 0x1000)
        for a, s in strings.items():
            data[a - BASE:a - BASE + len(s) + 1] = s.encode() + b"\0"
        self.data = bytes(data)
        self.ptrs = {a - BASE: t - BASE for a, t in pointers.items()}

    def cstring(self, a):
        if not 0 <= a < len(self.data):
            return None
        end = self.data.find(b"\0", a)
        return self.data[a:end].decode("ascii") if end > a else (None if end < 0 else "")

    def pointer(self, a):
        return self.ptrs.get(a)


class FakeOriginal:
    def __init__(self, asms, strings, pointers, extra_sizes=()):
        self.code = {a.start: a.code() for a in asms}
        self.sizes = {a: len(c) for a, c in self.code.items()}
        self.sizes.update({a: 4 for a in extra_sizes})
        self.img = FakeImage(strings, pointers)

    def function(self, address):
        return self.code[address]


def pointer_ctor(start=0x7100001000, size=0x10, flag=1, missing_slot=None) -> Asm:
    """A CPointerType constructor: kind hash, size, flag, slot functions, the pointee getter's result."""
    a = Asm(start)
    a.mov(19, 0).addr(8, KIND_STR).mov(0, 8).movz(1, len(gt.POINTER_KIND)).bl(gt.KIND_HASH)
    a.movz(8, size).str(8, 19, 0x18).movz(8, flag).str(8, 19, 0xE0)
    a.adrp(9, GOT_SLOT).ldr(9, 9, GOT_SLOT & 0xFFF).str(9, 19, 0x28)
    for off in gt.SLOTS:
        if off not in (0x28, missing_slot):
            a.str(31, 19, off)
    a.bl(POINTEE_GETTER).str(0, 19, 0xE8)
    return a.b(TAIL)


def fake(*asms) -> FakeOriginal:
    return FakeOriginal(asms, {KIND_STR: gt.POINTER_KIND, NAME_STR: "CFoo*"}, {GOT_SLOT: COPY_FN},
                        extra_sizes=(COPY_FN,))


class Emulate(unittest.TestCase):
    def test_kind_stores_and_tail(self):
        a = pointer_ctor()
        kind, stores, tail = gt.emulate(fake(a), a.start)
        self.assertEqual(kind, gt.POINTER_KIND)
        self.assertEqual(tail, TAIL)
        self.assertEqual(stores[0x18], ("imm", 0x10))
        self.assertEqual(stores[0xE0], ("imm", 1))
        self.assertEqual(stores[0x28], ("fn", COPY_FN))
        self.assertEqual(stores[0x30], ("imm", 0))
        self.assertEqual(stores[0xE8], ("call", POINTEE_GETTER))

    def test_traits_lines(self):
        a = pointer_ctor()
        decls: set[str] = set()
        lines = gt.traits(fake(a), a.start, decls)
        self.assertEqual(lines[0], "    static constexpr u64 size = 0x10;")
        self.assertEqual(lines[1], "    static constexpr bool owned = true;")
        self.assertIn("    static constexpr UnkFn construct = &fn_7100000040;", lines)
        self.assertIn("    static constexpr UnkFn copy_construct = nullptr;", lines)
        self.assertEqual(lines[-1], "    static const CType* pointee() { return fn_7100000080(); }")
        self.assertEqual(decls, {"void fn_7100000040();", "const base::reflection::CType* fn_7100000080();"})

    def test_traits_rejects_missing_slot_and_bad_flag(self):
        for a in (pointer_ctor(missing_slot=0x100), pointer_ctor(flag=2)):
            with self.subTest():
                self.assertIsNone(gt.traits(fake(a), a.start, set()))

    def test_local_pointee_needs_a_constant(self):
        a = pointer_ctor()
        self.assertIsNone(gt.local_pointee(fake(a), a.start))  # the call's result is stored

    def test_getter_facts(self):
        g = Asm(0x7100002000)
        g.addr(1, NAME_STR).bl(gt.CSTRID_CTOR).bl(0x7100001000).ret()
        self.assertEqual(gt.getter_facts(fake(g), g.start), ("CFoo*", 0x7100001000))

    def test_getter_facts_without_name(self):
        g = Asm(0x7100002000)
        g.bl(0x7100001000).ret()
        self.assertIsNone(gt.getter_facts(fake(g), g.start))


class Emission(unittest.TestCase):
    BODY = ["    static constexpr u64 size = 0x8;"]

    def test_pointer_pair(self):
        out = gt.emit(0x7100000010, 'CFoo"*', 0x7100000020, self.BODY)
        self.assertEqual(out[:2], ["struct UnkRefl_fn_7100000020;", "struct UnkRefl_fn_7100000010;"])
        self.assertIn(self.BODY[0], out)
        self.assertIn('    static constexpr const char* name = "CFoo\\"*";', out)
        self.assertIn("    using Init = UnkRefl_fn_7100000020;", out)
        self.assertTrue(out[-1].startswith("template const base::reflection::CType* "
                                           "base::reflection::refl_GetPointerType<UnkRefl_fn_7100000010>"))

    def test_kinds_name_their_traits(self):
        cases = {
            "EnumTypeDescription": gt.emit_enum(0x7100000020, self.BODY),
            "CollectionTypeDescription": gt.emit_collection(0x7100000020, self.BODY),
            "FlagsetTypeDescription": gt.emit_flagset(0x7100000020, self.BODY),
        }
        for traits, out in cases.items():
            with self.subTest(traits=traits):
                self.assertEqual(out[0], "struct UnkRefl_fn_7100000020;")
                self.assertEqual(out[2], f"struct base::reflection::{traits}<UnkRefl_fn_7100000020> {{")
                self.assertTrue(out[-1].startswith("template base::reflection::C"))

    def test_primitive_and_inline_pointer(self):
        prim = gt.emit_primitive(0x7100000030, self.BODY)
        self.assertIn("refl_GetPrimitiveType<UnkRefl_fn_7100000030>", prim[-1])
        inline = gt.emit_inline_pointer(0x7100000030, "CBar*", self.BODY)
        self.assertEqual(inline[0], "struct UnkReflInline_7100000030;")
        self.assertIn("    using Init = UnkReflInline_7100000030;", inline)
        self.assertFalse(any("CPointerType::CPointerType" in ln for ln in inline))


class EnumValueOps(unittest.TestCase):
    BODY = ["    static constexpr u64 size = 0x4;",
            "    static constexpr UnkFn construct = &fn_7100000040;",
            "    static const CType* underlying() { return fn_7100000050(); }"]

    def test_declaration_scopes_become_namespaces(self):
        self.assertEqual(gt.enum_declaration("EFoo"), "enum class EFoo : u32;")
        self.assertEqual(gt.enum_declaration("base::CBar::EFoo"), "namespace base::CBar { enum class EFoo : u32; }")

    def test_named_form(self):
        out = gt.emit_enum_ops(0x7100000020, "base::EFoo", 0x7101000000, self.BODY)
        self.assertIn("const CType& base::reflection::ValueOps<base::EFoo>::TypeOf(const void*) "
                      "{ return g_unk_7101000000; }", out)
        self.assertIn("template struct base::reflection::ValueOps<base::EFoo>;", out)
        self.assertIn("struct base::reflection::EnumTypeDescription<base::EFoo> "
                      ": base::reflection::ValueOpsDescription<base::EFoo> {", out)
        self.assertNotIn(self.BODY[1], out)  # the slots come from ValueOpsDescription
        self.assertIn(self.BODY[2], out)
        self.assertFalse(any("fn_7100000020" in ln for ln in out))
        self.assertTrue(out[-1].endswith(", base::EFoo*);"))

    def test_qualified_names_only(self):
        self.assertTrue(gt.QUALIFIED_RE.fullmatch("CFoo::EBar"))
        self.assertFalse(gt.QUALIFIED_RE.fullmatch("CFoo<int>::EBar"))

    def test_constructor_alias(self):
        import asmdiff
        self.assertEqual(asmdiff.name_aliases("base::reflection::refl_InitType<base::EFoo>"),
                         ["base::reflection::CEnumType::CEnumType<base::EFoo>",
                          "base::reflection::CFlagsetType::CFlagsetType<base::EFoo>"])

    def test_flagset_named_form(self):
        body = ["    static constexpr u64 size = 0x4;", "    static constexpr UnkFn hash = &fn_7100000044;",
                "    static const CType* underlying() { return fn_7100000050(); }"]
        out = gt.emit_flagset_ops(0x7100000020, "a::TFlags", 0x7101000000, body)
        self.assertEqual(out[0], "namespace a { struct TFlags { u32 bits = 0; bool operator==(const TFlags& other) "
                                 "const { return bits == other.bits; } }; }")
        self.assertIn("struct base::reflection::FlagsetTypeDescription<a::TFlags> "
                      ": base::reflection::ValueOpsDescription<a::TFlags> {", out)
        self.assertNotIn(body[1], out)
        self.assertTrue(out[-1].endswith("(const base::global::CStrId&, a::TFlags*);"))


class TypeNames(unittest.TestCase):
    def test_canonical_as_cxxfilt_prints(self):
        cases = {"char const*": "char const*", "unsigned*": "unsigned int*",
                 "base::global::CRntVector<base::global::CStrId>const*":
                     "base::global::CRntVector<base::global::CStrId> const*",
                 "A<B, C> const *": "A<B, C> const*", "const CFoo*": "CFoo const*",
                 "unsigned int const*": "unsigned int const*", "long long*": "long long*"}
        for text, want in cases.items():
            with self.subTest(text=text):
                self.assertEqual(gt.canonical_type(text), want)
                self.assertEqual(gt.canonical_type(want), want)  # a canonical spelling parses back

    def test_unparsed_names(self):
        for text in ("std::unique_ptr<CFoo>", "CArray<A, EnumClass<E>::Count, E>*", "A<B>::C*", "A<"):
            with self.subTest(text=text):
                self.assertIsNone(gt.parse_type(text))
                self.assertEqual(gt.canonical_type(text), text)

    def test_declarations(self):
        node = gt.parse_type("base::global::CRntDictionary<base::global::CStrId, CFoo::SBar>*")
        self.assertEqual(gt.type_declarations(node), [
            "namespace CFoo { class SBar; }", "namespace base::global { class CStrId; }",
            "namespace base::global { template <class, class> class CRntDictionary; }"])
        for line in gt.type_declarations(node) + [gt.enum_declaration("A::E")]:
            self.assertTrue(gt.TYPE_DECL_RE.match(line), line)

    def test_pointer_named_form(self):
        body = ["    static constexpr u64 size = 0x8;", "    static constexpr UnkFn construct = &fn_7100000040;",
                "    static constexpr UnkFn get = &fn_7100000044;"]
        out = gt.emit_pointer_ops(0x7100000010, "CFoo const*", 0x7100000020, body, "CFoo const*")
        self.assertEqual(out[0], "class CFoo;")
        self.assertIn("template struct base::reflection::ValueOps<CFoo const*>;", out)
        self.assertIn("struct base::reflection::PointerTypeDescription<UnkRefl_fn_7100000020> "
                      ": base::reflection::ValueOpsDescription<CFoo const*> {", out)
        self.assertNotIn(body[1], out)
        self.assertIn(body[2], out)
        self.assertIn('    static constexpr const char* name = "CFoo const*";', out)


    def test_iterator_value_type(self):
        name = "base::reflection::CollectionTypeMapper<base::global::CRntVector<base::core::AssetName> >::TElementIterator"
        canon, node = gt.pointer_value_type(name)
        self.assertEqual(canon, "base::reflection::CollectionTypeMapper<"
                                "base::global::CRntVector<base::core::AssetName>>::TElementIterator")
        self.assertEqual(gt.type_declarations(node), [
            "namespace base::core { class AssetName; }",
            "namespace base::global { template <class> class CRntVector; }"])
        self.assertEqual(gt.pointer_value_type(canon)[0], canon)
        self.assertIsNone(gt.pointer_value_type(
            "base::reflection::CollectionTypeMapper<base::global::CRntVector<X>>::TKeyElementIterator"))
        self.assertIsNone(gt.pointer_value_type("CFoo"))
        self.assertEqual(gt.pointer_value_type("unsigned int*")[0], "unsigned int*")


    def test_iterators_join_without_clashes(self):
        it = "base::reflection::CollectionTypeMapper<{}>::TElementIterator"
        pairs = [(1, "CFoo*", 11, None), (2, "base::global::CRntVector<CBar>*", 12, None),
                 (3, it.format("base::global::CRntVector<CFoo::SItem>"), 13, None),
                 (4, it.format("base::global::CRntVector<CBar, false>"), 14, None),
                 (5, it.format("base::global::CRntVector<CBaz>"), 15, None)]
        with mock.patch.object(gt, "pointer_slots", return_value={0x28: 1}):
            out = gt.named_pointers(None, pairs)
        self.assertEqual(sorted(out), [1, 2, 5])


    def test_primitive_named_form(self):
        body = ['    static constexpr const char* name = "unsigned";', "    static constexpr u64 length = 8;",
                "    static constexpr u64 size = 0x4;"]
        body += [f"    static constexpr UnkFn {m} = &fn_{0x7100000040 + 4 * i:x};"
                 for i, m in enumerate(gt.PRIMITIVE_SLOTS.values())]
        self.assertEqual(gt.primitive_value_type(body), "unsigned int")
        out = gt.emit_primitive(0x7100000010, body, "unsigned int")
        self.assertIn("template struct base::reflection::ValueOps<unsigned int>;", out)
        self.assertIn("struct base::reflection::UnkReflPrimitiveTraits<UnkRefl_fn_7100000010> "
                      ": base::reflection::ValueOpsDescription<unsigned int> {", out)
        self.assertNotIn(body[3], out)
        self.assertIn("    static constexpr UnkFn add_attributes = &fn_7100000064;", out)
        names = gt.primitive_ops_names(body, "unsigned int")
        self.assertEqual(names[0x7100000040], "base::reflection::ValueOps<unsigned int>::Construct")
        self.assertEqual(len(names), 9)
        self.assertIsNone(gt.primitive_value_type([line.replace('"unsigned"', '"CFoo"') for line in body]))
        self.assertIsNone(gt.primitive_value_type([line.replace("&fn_7100000044", "nullptr") for line in body]))


class Declarations(unittest.TestCase):
    DECLS = {"void fn_7100000010();", "extern base::reflection::CType g_unk_7100000020;",
             "void fn_7100000030();"}

    def test_used_decls_sorted_and_filtered(self):
        used = gt.used_decls(["x = &fn_7100000010;", "y = &g_unk_7100000020;"], self.DECLS)
        self.assertEqual(used, ["extern base::reflection::CType g_unk_7100000020;", "void fn_7100000010();"])

    def test_name_must_be_whole(self):
        self.assertEqual(gt.used_decls(["fn_71000000100();"], self.DECLS), [])

    def test_write_chunks(self):
        blocks = [["a = &fn_7100000010;"], ["b = &fn_7100000030;"], ["c = 1;"]]
        with tempfile.TemporaryDirectory() as tmp, mock.patch.object(gt, "OUT", Path(tmp)), \
                mock.patch.object(gt, "CHUNK", 2):
            gt.write_chunks("pointer", blocks, self.DECLS, ["// header"])
            names = sorted(p.name for p in Path(tmp).iterdir())
            first = (Path(tmp) / "pointer_00.cpp").read_text(encoding="utf-8")
            second = (Path(tmp) / "pointer_01.cpp").read_text(encoding="utf-8")
        self.assertEqual(names, ["pointer_00.cpp", "pointer_01.cpp"])
        self.assertTrue(first.startswith("// header\n" + gt.PRELUDE[0]))
        self.assertIn("void fn_7100000010();", first)
        self.assertIn("void fn_7100000030();", first)
        self.assertNotIn("g_unk_7100000020", first)
        self.assertNotIn("fn_7100000010", second)
        self.assertTrue(second.endswith("c = 1;\n"))


if __name__ == "__main__":
    unittest.main()
