"""tools/gen_reflection.py: TU layout, emitted C++ and code-pattern recognition on hand-assembled
words in a fake module (no game data)."""

import struct
import unittest

from tools.tests import _path  # noqa: F401
import asmdiff
import gen_reflection as gr
import refl_attrs

BASE = 0x7100000000
GOT_SLOT = 0x7100004010
HELPER = 0x7100000400

RET = 0xD65F03C0
MOV_W0_WZR = 0x2A1F03E0
CMP_X0_X1 = 0xEB01001F
CSET_W0_EQ = 0x1A9F17E0


def words(*ws: int) -> bytes:
    return b"".join(struct.pack("<I", w) for w in ws)


def adrp(rd, pc, target):
    off = ((target >> 12) - (pc >> 12)) & 0x1FFFFF
    return 0x90000000 | ((off & 3) << 29) | ((off >> 2) << 5) | rd


def ldr(rt, rn, off):
    return 0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt


def str_(rt, rn, off):
    return 0xF9000000 | ((off // 8) << 10) | (rn << 5) | rt


def add(rd, rn, imm):
    return 0x91000000 | (imm << 10) | (rn << 5) | rd


def bl(pc, target):
    return 0x94000000 | (((target - pc) >> 2) & 0x3FFFFFF)


class FakeImage:
    def __init__(self, pointers):
        self.ptrs = {a - BASE: t - BASE for a, t in pointers.items()}

    def pointer(self, a):
        return self.ptrs.get(a)


class FakeOriginal:
    def __init__(self, functions: dict[int, bytes], pointers=None):
        self.code = functions
        self.sizes = {a: len(c) for a, c in functions.items()}
        self.img = FakeImage(pointers or {})

    def function(self, address):
        return self.code[address]


def getter(address, guard, descriptor, **kw) -> gr.Getter:
    return gr.Getter(type="CFoo", address=address, guard=guard, descriptor=descriptor, **kw)


class Layout(unittest.TestCase):
    def block(self, gap: int, n: int = 3, guard: int = 0x7101000000):
        first = guard + 8 * n + gap
        return [getter(0x7100000010 + 0x40 * i, guard + 8 * i, first + gr.DESCRIPTOR_SIZE * i) for i in range(n)]

    def test_contiguous_descriptors(self):
        self.assertEqual(gr.layout_gap(self.block(0)), 0)
        self.assertEqual(gr.layout_gap(self.block(0x40)), 0x40)

    def test_misaligned_or_negative_gap(self):
        self.assertIsNone(gr.layout_gap(self.block(4)))
        self.assertIsNone(gr.layout_gap(self.block(-8)))

    def test_descriptors_out_of_order(self):
        b = self.block(0)
        b[1].descriptor, b[2].descriptor = b[2].descriptor, b[1].descriptor
        self.assertIsNone(gr.layout_gap(b))

    def test_padding_pieces_cover_the_gap(self):
        b = self.block(0)
        for gap in (0x8, 0x130, 0x138, 0x400):
            with self.subTest(gap=gap):
                lines = gr.padding_lines(b, gap)
                sizes = [int(ln.split("[")[1].rstrip("];"), 16) for ln in lines if ln.startswith("alignas(8) char")]
                self.assertEqual(sum(sizes), gap)
                self.assertTrue(all(0 < s <= 0x130 for s in sizes))
                self.assertIn("char unk_statics_anchor;", lines)
                self.assertIn(f"void refl_UnkStaticsUse_{b[0].address:x}(void** out) {{", lines)
                self.assertIn(f"    out[{len(sizes)}] = &unk_statics_anchor;", lines)


class GetterOk(unittest.TestCase):
    def test_cases(self):
        other = getter(0x7100000100, 0, 0)
        cases = [
            (getter(1, 0, 0, root=True), True),
            (getter(1, 0, 0, shape_ok=True, parent=0x7100000100), True),
            (getter(1, 0, 0, shape_ok=True), False),           # common shape needs a parent
            (getter(1, 0, 0, inline_name=True), True),
            (getter(1, 0, 0, inline_name=True, local_parent=True), False),
            (getter(1, 0, 0, local_parent=True, parent=0x7100000100), True),  # parent in the block
            (getter(1, 0, 0, local_parent=True, parent=0x7100000200), False),
        ]
        for g, ok in cases:
            with self.subTest(g=g):
                self.assertEqual(gr.getter_ok(g, [g, other]), ok)


class ConstructorLines(unittest.TestCase):
    def test_traits_and_instantiation(self):
        lines = gr.constructor_lines(0x7100000020, 0x50, True, {0x58: 0x7100000058, 0x28: 0x7100000028})
        self.assertEqual(lines[1], "struct base::reflection::UnkReflClassTraits<UnkRefl_fn_7100000020> {")
        self.assertIn("    static constexpr u64 size = 0x50;", lines)
        self.assertIn("    static constexpr bool abstract = true;", lines)
        self.assertIn("    static constexpr EqualsFn fn_0x58 = &fn_7100000058;", lines)
        self.assertIn("    static constexpr SlotFn fn_0x28 = &fn_7100000028;", lines)
        self.assertIn("    static constexpr HashFn fn_0x60 = nullptr;", lines)
        self.assertEqual(sum(ln.startswith("    static constexpr ") and " fn_0x" in ln for ln in lines),
                         len(gr.TRAIT_SLOTS))
        self.assertTrue(lines[-1].endswith("UnkRefl_fn_7100000020*);"))


class FileName(unittest.TestCase):
    def test_sanitised_and_unique_ignoring_case(self):
        taken: set[str] = set()
        self.assertEqual(gr.file_name("base::core::CFoo", taken), "base_core_CFoo")
        self.assertEqual(gr.file_name("base::core::CFoo", taken), "base_core_CFoo_2")
        self.assertEqual(gr.file_name("BASE::core::cfoo", taken), "BASE_core_cfoo_3")
        self.assertEqual(taken, {"base_core_cfoo", "base_core_cfoo_2", "base_core_cfoo_3"})

    def test_template_and_empty_names(self):
        self.assertEqual(gr.file_name("CVector<CFoo*>", set()), "CVector_CFoo")
        self.assertEqual(gr.file_name("<>", set()), "unnamed")
        self.assertEqual(len(gr.file_name("C" * 200, set())), 80)


class Helpers(unittest.TestCase):
    def test_known_shapes(self):
        orig = FakeOriginal({HELPER: words(RET), HELPER + 0x10: words(MOV_W0_WZR, RET),
                             HELPER + 0x20: words(CMP_X0_X1, CSET_W0_EQ, RET)})
        self.assertEqual(gr.helper_definition(orig, 0x30, HELPER), "void fn_7100000400(void*, const void*) {}")
        self.assertEqual(gr.helper_definition(orig, 0x58, HELPER + 0x10),
                         "bool fn_7100000410(const void*, const void*) { return false; }")
        self.assertEqual(gr.helper_definition(orig, 0x58, HELPER + 0x20),
                         "bool fn_7100000420(const void* a, const void* b) { return a == b; }")

    def test_shape_depends_on_slot(self):
        orig = FakeOriginal({HELPER: words(RET)})
        self.assertIsNone(gr.helper_definition(orig, 0x58, HELPER))

    def test_template_braces_formatted(self):
        for (slot, _), src in gr.HELPER_SHAPES.items():
            with self.subTest(slot=slot):
                text = src.format(name="fn_7100000010")
                self.assertEqual(text.count("{"), text.count("}"))
                self.assertIn("fn_7100000010", text)


class ReadableForm(unittest.TestCase):
    """base/reflection/ReflectedClass.h: a class's functions in a namespace of its name, under role names."""

    def foo(self, **kw) -> gr.Getter:
        g = gr.Getter(type="game::CFoo", address=0x7100000010, init=0x7100000020, fn_0x70=HELPER,
                      fn_0xe0=HELPER + 0x40, parent=0x7100000100, **kw)
        g.traits = {0x58: HELPER + 0x20, 0x60: HELPER + 0x30, 0x28: HELPER + 0x50}
        return g

    def test_readable_types(self):
        self.assertTrue(gr.readable_type("CFoo::CTunableFoo"))
        self.assertTrue(gr.readable_type("game::CFoo"))
        self.assertFalse(gr.readable_type("base::reflection::CClass"))  # a class the headers define
        self.assertFalse(gr.readable_type("CVector<CFoo*>"))

    def test_role_names(self):
        names = gr.reflection_names(self.foo())
        self.assertEqual(names, {HELPER + 0x20: "game::CFoo::refl_Equals", HELPER + 0x30: "game::CFoo::refl_Hash",
                                 HELPER + 0x50: "game::CFoo::refl_Construct", HELPER: "game::CFoo::refl_AddAttributes",
                                 HELPER + 0x40: "game::CFoo::refl_AddFields"})
        self.assertEqual(gr.reflection_names(gr.Getter(type="CVector<CFoo*>", address=1)), {})

    def test_class_lines(self):
        mov_m1 = 0x12800000  # mov w0, #-1
        orig = FakeOriginal({HELPER: words(RET), HELPER + 0x20: words(CMP_X0_X1, CSET_W0_EQ, RET),
                             HELPER + 0x30: words(mov_m1, RET), HELPER + 0x40: words(RET, RET),
                             HELPER + 0x50: words(RET, RET)})
        fields = (["void fn_7100000999();"], "void fn_7100000440(CClass* cls) {\n    fn_7100000999();\n}")
        declared: set[str] = set()
        lines, helpers, attrs = gr.readable_class_lines(orig, self.foo(abstract=True), 0x50, {HELPER + 0x40: fields},
                                                        "DREAD_REFLECTED_CLASS", "game::CBar", declared)
        self.assertEqual(lines, [
            "// game::CFoo",
            "void fn_7100000999();",
            "namespace game::CFoo {",
            "void refl_AddAttributes(CClass*) {}",
            "bool refl_Equals(const void* a, const void* b) { return a == b; }",
            "s32 refl_Hash() { return -1; }",
            "void refl_AddFields(CClass* cls) {\n    fn_7100000999();\n}",
            "void refl_Construct(void* obj);",
            "struct refl_Description : ClassDescription {",
            "    static constexpr u64 size = 0x50;",
            "    static constexpr bool abstract = true;",
            "    static constexpr auto equals = &refl_Equals;",
            "    static constexpr auto hash = &refl_Hash;",
            "    static constexpr auto construct = &refl_Construct;",
            "};",
            "}  // namespace game::CFoo",
            "DREAD_REFLECTED_CLASS(game::CFoo, game::CBar);",
        ])
        self.assertEqual((helpers, attrs), (4, 1))
        self.assertEqual(declared, {"void fn_7100000999();"})

    def test_shapes_cover_helper_shapes(self):
        for shape in gr.HELPER_SHAPES.values():
            with self.subTest(shape=shape):
                text = gr.ROLE_SHAPES[shape].format(name="refl_Equals")
                self.assertEqual(text.count("{"), text.count("}"))

    def test_descriptor_constructor_alias(self):
        self.assertEqual(asmdiff.name_aliases("game::CFoo::refl_InitClass"),
                         ["base::reflection::CClass::CClass<game::CFoo::refl_Description>"])
        self.assertEqual(asmdiff.name_aliases("game::CFoo::refl_GetClass"), [])


class InitStores(unittest.TestCase):
    def test_got_values_stored_to_trait_slots(self):
        a = 0x7100000800
        code = words(adrp(8, a, GOT_SLOT), ldr(8, 8, GOT_SLOT & 0xFFF), str_(8, 19, 0x58),
                     str_(8, 19, 0x100),            # not a trait slot
                     bl(a + 16, 0x7100000010), str_(8, 19, 0x60),  # x8 unknown after the call
                     RET)
        orig = FakeOriginal({a: code}, {GOT_SLOT: 0x7100000500})
        self.assertEqual(gr.init_stores(orig, a), {0x58: 0x7100000500})

    def test_abstract_flag(self):
        and_w8 = 0x121E7508  # and w8, w8, #0xfffffffc
        orr_w8 = 0x32000108  # orr w8, w8, #1
        a = 0x7100000800
        self.assertTrue(gr.is_abstract(FakeOriginal({a: words(and_w8, orr_w8, RET)}), a))
        self.assertFalse(gr.is_abstract(FakeOriginal({a: words(and_w8, RET)}), a))
        self.assertFalse(gr.is_abstract(FakeOriginal({a: words(orr_w8, and_w8, RET)}), a))


class Scans(unittest.TestCase):
    def test_registration_functions(self):
        a, b, c = 0x7100000100, 0x7100000200, 0x7100000300
        orig = FakeOriginal({a: words(RET),
                             b: words(RET, bl(b + 4, refl_attrs.ADD_FIELD), RET),
                             c: words(bl(c, 0x7100000010), RET)})
        self.assertEqual(gr.registration_functions(orig), [b])

    def test_initialiser_candidates(self):
        hand = next(iter(gr.HAND_WRITTEN))
        a = 0x7100000100
        store = gr.STRH_B2_X19 | 8  # strh w8, [x19, #0xb2]
        orig = FakeOriginal({a: words(store, RET), a + 0x10: words(RET), hand: words(store, RET)})
        self.assertEqual(gr.initialiser_candidates(orig), [a])

    def test_shape_ignores_descriptor_offset(self):
        one = words(add(0, 19, 0x10), RET)
        two = words(add(0, 19, 0x148), RET)
        other = words(add(0, 20, 0x10), RET)
        self.assertEqual(gr.shape(one), gr.shape(two))
        self.assertNotEqual(gr.shape(one), gr.shape(other))




class TypesByType(unittest.TestCase):
    KEYED = {0x7100000100: ("Primitive", "float"), 0x7100000200: ("Primitive", "base::global::CStrId"),
             0x7100000300: ("Pointer", "a::CFoo*")}

    def test_calls_by_type(self):
        text = "\n".join(['#include "base/reflection/CClass.h"', "using base::global::CStrId;", "",
                          "const base::reflection::CType* fn_7100000100();",
                          "void f(UnkReflFunction* fn) {",
                          "    fn->return_type = fn_7100000100();",
                          "    fn->params.PushBack(fn_7100000200());",
                          "    fn->params.PushBack(fn_7100000300());",
                          "    fn->params.PushBack(fn_7100000400());",
                          "}"])
        out = gr.types_by_type(text, self.KEYED).split("\n")
        self.assertIn("    fn->return_type = &base::reflection::PrimitiveType<float>::Get();", out)
        self.assertIn("    fn->params.PushBack(&base::reflection::PrimitiveType<base::global::CStrId>::Get());", out)
        self.assertIn("    fn->params.PushBack(base::reflection::PointerType<a::CFoo*>::Get());", out)
        self.assertIn("    fn->params.PushBack(fn_7100000400());", out)  # not keyed
        self.assertNotIn("const base::reflection::CType* fn_7100000100();", out)
        ext = "extern template const base::reflection::CType& base::reflection::PrimitiveType<float>::Get();"
        self.assertEqual(out[:3], ['#include "base/reflection/CClass.h"', "using base::global::CStrId;",
                                   '#include "base/reflection/UnkReflPrimitiveType.h"'])
        self.assertLess(out.index(ext), out.index("void f(UnkReflFunction* fn) {"))
        self.assertIn("namespace a { class CFoo; }", out)
        self.assertIn("namespace base::global { class CStrId; }", out)

    def test_declaration_kept_while_named_otherwise(self):
        text = "\n".join(["const base::reflection::CType* fn_7100000100();",
                          "void f() { g<&fn_7100000100>(fn_7100000100()); }"])
        out = gr.types_by_type(text, self.KEYED)
        self.assertIn("const base::reflection::CType* fn_7100000100();", out)
        self.assertIn("g<&fn_7100000100>(&base::reflection::PrimitiveType<float>::Get())", out)

    def test_nothing_keyed_leaves_the_text(self):
        self.assertEqual(gr.types_by_type("int x = fn_7100000400();", self.KEYED), "int x = fn_7100000400();")


class AttributeKeysByName(unittest.TestCase):
    def test_keys_by_name(self):
        text = "\n".join(['#include "base/reflection/CClass.h"', "",
                          "const base::reflection::UnkReflAttrKey& fn_7100007e80();  // \"Description\"",
                          "void f(CClass* cls) {",
                          '    TypeAttributes(cls)->Add<const char*>(fn_7100007e80(), "Text");',
                          "    g(fn_7100000400());",
                          "}"])
        out = gr.attribute_keys_by_name(text, {0x7100007e80: "Description"}).split("\n")
        self.assertEqual(out[1], '#include "base/reflection/ReflectedFields.h"')
        self.assertIn('    TypeAttributes(cls)->Add<const char*>(base::reflection::attr::Description(), "Text");', out)
        self.assertIn("    g(fn_7100000400());", out)
        self.assertFalse(any("fn_7100007e80" in line for line in out))

    def test_the_header_names_its_keys(self):
        keys = gr.attribute_keys()
        self.assertEqual(keys.get(0x7100007e80), "Description")
        self.assertNotIn(0x710000bf80, keys)  # Persistence is a typed key, written with its enum


class LooseClassLines(unittest.TestCase):
    def test_namespace_form(self):
        saved = gr.role_helper
        gr.role_helper = lambda orig, slot, addr: "s32 refl_Hash() { return -1; }" if slot == 0x60 else None
        try:
            lines = gr.loose_class_lines(None, "CFoo::SBar", 0x28, True, {0x58: 0x30, 0x60: 0x10, 0x68: 0x20})
        finally:
            gr.role_helper = saved
        self.assertEqual(lines, [
            "// CFoo::SBar",
            "namespace CFoo::SBar {",
            "s32 refl_Hash() { return -1; }",
            "const base::reflection::CType& refl_TypeOf(const void* obj);",
            "bool refl_Equals(const void* a, const void* b);",
            "struct refl_Description : ClassDescription {",
            "    static constexpr u64 size = 0x28;",
            "    static constexpr bool abstract = true;",
            "    static constexpr auto equals = &refl_Equals;",
            "    static constexpr auto hash = &refl_Hash;",
            "    static constexpr auto type_of = &refl_TypeOf;",
            "};",
            "}  // namespace CFoo::SBar",
            "DREAD_REFLECTED_DESCRIPTOR(CFoo::SBar);",
        ])


class KeyValueClass(unittest.TestCase):
    def test_names(self):
        name = "base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId,float>>" \
               "::TKeyElementConstIterator::value_type"
        self.assertEqual(gr.key_value_class(name)[0],
                         "base::reflection::ConstKeyElementValue<base::global::CRntSmallDictionary<base::global::CStrId, float>>")
        self.assertIsNone(gr.key_value_class(
            "base::reflection::CollectionTypeMapper<base::global::CArray<int,4>>::TKeyElementIterator::value_type"))
        self.assertIsNone(gr.key_value_class("CFoo::SBar"))


class TemplateValueNames(unittest.TestCase):
    def test_nested_member_types(self):
        name = "base::reflection::CollectionTypeMapper<base::global::CRntVector<GUI::CDisplayObjectTrack<float>::SKey>>" \
               "::TKeyElementIterator::value_type"
        cls, (_, decls, _) = gr.key_value_class(name)
        self.assertEqual(cls, "base::reflection::KeyElementValue<base::global::CRntVector<GUI::CDisplayObjectTrack<float>::SKey>>")
        self.assertIn("namespace GUI { template <class> class CDisplayObjectTrack { public: struct SKey; }; }", decls)

    def test_value_templates(self):
        self.assertEqual(gr.value_template_class("CGameLink<CActor>")[0], "CGameLink<CActor>")
        self.assertIsNone(gr.value_template_class("CActor"))
        self.assertEqual(gr.template_value_kind("CGameLink<CActor>"), "class_values")

    def test_value_members_are_not_types(self):
        self.assertIsNone(gr.key_value_class(
            "base::reflection::CollectionTypeMapper<base::global::CArray<CActorLayer, EnumClass<EActorLayer>::Count, EActorLayer>>"
            "::TKeyElementIterator::value_type"))


class TypeObjectsByType(unittest.TestCase):
    def test_rewrites_uses_and_declares(self):
        text = "\n".join(['#include "base/reflection/CClass.h"', "extern base::reflection::CType g_unk_7101d4d5d0;",
                          "void f() { use(&g_unk_7101d4d5d0); }"])
        out = gr.type_objects_by_type(text, {"g_unk_7101d4d5d0": ("enum", "base::animation::EBlendFunction")})
        self.assertNotIn("extern base::reflection::CType g_unk_7101d4d5d0;", out)
        self.assertIn("use(&base::reflection::TypeObject<base::animation::EBlendFunction>::object)", out)
        self.assertIn('template <> base::reflection::CType base::reflection::TypeObject<base::animation::EBlendFunction>::object '
                      'asm("g_unk_7101d4d5d0");', out)

    def test_other_declarations_stay(self):
        text = "extern u8 g_unk_7101d4d5d0[0x10];\nvoid f() { use(g_unk_7101d4d5d0); }"
        self.assertEqual(gr.type_objects_by_type(text, {"g_unk_7101d4d5d0": ("enum", "E")}), text)

    def test_layouts_keep_their_type_in_place(self):
        text = "\n".join(['#include "a.h"', "struct UnkEnumType;", "extern UnkEnumType g_unk_7101d4d5d0;",
                          "void f() { use(&g_unk_7101d4d5d0); }"])
        out = gr.type_objects_by_type(text, {"g_unk_7101d4d5d0": ("enum", "a::E")}, layouts=True).split("\n")
        self.assertEqual(out, ['#include "a.h"', '#include "base/reflection/TypeObject.h"', "struct UnkEnumType;",
                               "namespace a { enum class E : u32; }",
                               'template <> UnkEnumType base::reflection::TypeObject<a::E, UnkEnumType>::object '
                               'asm("g_unk_7101d4d5d0");',
                               "void f() { use(&base::reflection::TypeObject<a::E, UnkEnumType>::object); }"])

    def test_twice_is_once(self):
        text = "\n".join(['#include "a.h"', "extern UnkEnumType g_unk_7101d4d5d0;", "extern base::reflection::CType g_unk_7101d4d5e0;",
                          "void f() { use(&g_unk_7101d4d5d0, &g_unk_7101d4d5e0); }"])
        objects = {"g_unk_7101d4d5d0": ("enum", "a::E"), "g_unk_7101d4d5e0": ("enum", "a::F")}
        once = gr.type_objects_by_type(text, objects, layouts=True)
        self.assertEqual(gr.type_objects_by_type(once, objects, layouts=True), once)

    def test_layouts_drop_a_trailing_comment(self):
        text = "extern UnkReflType g_unk_7101d4d5d0;  // a::E\nvoid f() { use(&g_unk_7101d4d5d0); }"
        out = gr.type_objects_by_type(text, {"g_unk_7101d4d5d0": ("enum", "a::E")}, layouts=True)
        self.assertIn('template <> UnkReflType base::reflection::TypeObject<a::E, UnkReflType>::object asm("g_unk_7101d4d5d0");', out)
        self.assertNotIn("// a::E", out)

    def test_builtin_needs_no_declaration(self):
        text = "extern base::reflection::CType g_unk_7101d5af48;\nvoid f() { use(&g_unk_7101d5af48); }"
        out = gr.type_objects_by_type(text, {"g_unk_7101d5af48": ("builtin", "void")})
        self.assertEqual(out.split("\n"), ['#include "base/reflection/TypeObject.h"',
                                            'template <> base::reflection::CType base::reflection::TypeObject<void>::object '
                                            'asm("g_unk_7101d5af48");',
                                            "void f() { use(&base::reflection::TypeObject<void>::object); }"])

    def test_links_declared_once(self):
        text = ("extern base::reflection::CType g_unk_717269bb48;\nextern base::reflection::CType g_unk_71727b6270;\n"
                "void f() { use(&g_unk_717269bb48, &g_unk_71727b6270); }")
        out = gr.type_objects_by_type(text, {"g_unk_717269bb48": ("link", "CGameLink<CActor>"),
                                             "g_unk_71727b6270": ("link", "CGameLink<CEntity>")}).split("\n")
        self.assertEqual(out.count("template <class T> struct CGameLink;"), 1)
        self.assertIn("class CActor;", out)
        self.assertIn("class CEntity;", out)
        self.assertEqual(out[-1], "void f() { use(&base::reflection::TypeObject<CGameLink<CActor>>::object, "
                                  "&base::reflection::TypeObject<CGameLink<CEntity>>::object); }")

    def test_layouts_off_leaves_them(self):
        text = "extern UnkEnumType g_unk_7101d4d5d0;\nvoid f() { use(&g_unk_7101d4d5d0); }"
        self.assertEqual(gr.type_objects_by_type(text, {"g_unk_7101d4d5d0": ("enum", "E")}), text)


class FunctionsInOneLine(unittest.TestCase):
    HEAD = ['    DREAD_REFLECTED_FUNCTION("_x_GetterFunction") {',
            '        fn->name = base::global::UnkStrId("_x_GetterFunction", true);',
            "        fn->return_type = &base::reflection::PointerType<A<B, C>>::Get();",
            "        fn->field_0xb2 = 0x101;",
            "        fn->field_0x40 = nullptr;",
            "        fn->invoker = reinterpret_cast<void*>(&fn_7101049fa8);"]

    def test_description_alone_is_one_line(self):
        out = gr.functions_in_one_line("\n".join(self.HEAD + ["    }", ""]))
        self.assertIn('    DREAD_REFLECTED_FUNCTION_OF("_x_GetterFunction", (&base::reflection::PointerType<A<B, C>>::Get()), '
                      "0x101, fn_7101049fa8);", out)
        self.assertNotIn("field_0xb2", out)

    def test_block_keeps_the_rest(self):
        out = gr.functions_in_one_line("\n".join(self.HEAD + ["        fn_7100601f24(fn);", "    }", ""]))
        self.assertIn('        DREAD_REFLECTED_FUNCTION_DESCRIPTION("_x_GetterFunction", '
                      "(&base::reflection::PointerType<A<B, C>>::Get()), 0x101, fn_7101049fa8);\n        fn_7100601f24(fn);", out)

    def test_initialiser(self):
        text = "\n".join(["void fn_7100026684(UnkReflFunction* fn, const char* name) {",
                          "    fn->name = base::global::UnkStrId(name, true);", "    fn->return_type = &g_unk_7101d5af48;",
                          "    fn->field_0xb2 = 0x0;", "    fn->field_0x40 = nullptr;",
                          "    fn->invoker = reinterpret_cast<void*>(&fn_7100027268);", "}", ""])
        self.assertIn("    DREAD_REFLECTED_FUNCTION_DESCRIPTION(name, &g_unk_7101d5af48, 0x0, fn_7100027268);",
                      gr.functions_in_one_line(text))


class TypedKeysByName(unittest.TestCase):
    def test_key_and_declaration(self):
        keys = {"fn_710000bf80": ("Persistence", "T<E>::object")}
        text = "const base::reflection::UnkReflAttrKey& fn_710000bf80();\nf->attrs.Add<s32>(fn_710000bf80(), &T<E>::object, 0);"
        out = gr.typed_keys_by_name(text, keys)
        self.assertIn("f->attrs.Add<s32>(base::reflection::attr::Persistence(), 0);", out)
        self.assertNotIn("fn_710000bf80", out)



class RegistryInclude(unittest.TestCase):
    def test_added_only_where_called(self):
        inc = '#include "base/reflection/UnkTunableRegistry.h"'
        calls = '#include "a.h"\nvoid f() { g(base::reflection::TunableRegistry()); }'
        self.assertEqual(gr.with_registry_include(calls).split(chr(10))[:2], ['#include "a.h"', inc])
        self.assertEqual(gr.with_registry_include(gr.with_registry_include(calls)).count(inc), 1)
        self.assertEqual(gr.with_registry_include('#include "a.h"\nvoid f();'), '#include "a.h"\nvoid f();')


if __name__ == "__main__":
    unittest.main()
