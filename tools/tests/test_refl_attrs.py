"""tools/refl_attrs.py's value rendering, statement forms and parse() on hand-written event
sequences (events() is replaced, so no code is disassembled and no game data is read)."""

import struct
import unittest
from unittest import mock

from tools.tests import _path  # noqa: F401
import refl_attrs as ra
from refl_attrs import Event

ADDR = 0x7100AB0000
KEY = 0x7100AAAA00
BOOL_T = 0x710000D228
U32_T = 0x7100005178


def bl(target: int, **regs) -> Event:
    return Event("bl", target=target, regs=regs)


def val(*value) -> Event:
    return Event("value", value=tuple(value))


def run_parse(evs: list[Event], class_getters: set[int] = frozenset()):
    with mock.patch.object(ra, "events", return_value=evs):
        return ra.parse(None, ADDR, set(class_getters))


def bits(f: float) -> int:
    return struct.unpack("<I", struct.pack("<f", f))[0]


class Literals(unittest.TestCase):
    def test_c_string_escapes(self):
        self.assertEqual(ra.c_string('a"b\\c'), r'"a\"b\\c"')
        self.assertEqual(ra.c_string("\né"), r'"\012\303\251"')

    def test_f32_shortest_round_trip(self):
        for f in (1.0, 0.1, -2.5, 1e-45, 3.4028234663852886e38, 0.0):
            lit = ra.f32_literal(bits(f))
            self.assertTrue(lit.endswith("f"), lit)
            self.assertEqual(bits(float(lit[:-1])), bits(f), lit)
        self.assertEqual(ra.f32_literal(bits(0.1)), "0.1f")
        self.assertEqual(ra.f32_literal(0x80000000), "-0.0f")

    def test_f32_not_finite(self):
        self.assertIsNone(ra.f32_literal(0x7F800000))
        self.assertIsNone(ra.f32_literal(0x7FC00000))

    def test_signed(self):
        self.assertEqual(ra.signed(0xFF, 1), -1)
        self.assertEqual(ra.signed(0x7F, 1), 127)
        self.assertEqual(ra.signed(0xFFFFFFFE, 4), -2)
        self.assertEqual(ra.signed(0x1_0000_0005, 4), 5)


class IndexExpr(unittest.TestCase):
    def test_forms(self):
        self.assertEqual(ra.index_expr(("imm", 2**64 - 1)), "-1")
        self.assertEqual(ra.index_expr(("count", -3)), "cls->m_array_0x110.size() - 3")
        self.assertIsNone(ra.index_expr(("count", 0)))
        self.assertIsNone(ra.index_expr(("imm", 4)))
        self.assertIsNone(ra.index_expr(None))


class ValueExpr(unittest.TestCase):
    def test_scalars(self):
        self.assertEqual(ra.value_expr("bool", [("imm", 1, 1)]), "true")
        self.assertEqual(ra.value_expr("bool", [("imm", 0, 1)]), "false")
        self.assertIsNone(ra.value_expr("bool", [("imm", 2, 1)]))
        self.assertEqual(ra.value_expr("u32", [("imm", 0x1F, 4)]), "0x1fu")
        self.assertEqual(ra.value_expr("s32", [("imm", 0xFFFFFFFF, 4)]), "-1")
        self.assertEqual(ra.value_expr("f32", [("imm", bits(0.5), 4)]), "0.5f")
        self.assertEqual(ra.value_expr("f32", [("fimm", 2.0, 4)]), "2.0f")

    def test_strings_and_ids(self):
        self.assertEqual(ra.value_expr("const char*", [("str", "abc", 8)]), '"abc"')
        self.assertEqual(ra.value_expr("base::global::CStrId", [("global", 0x7101000010, 8)]),
                         "CStrId(g_unk_7101000010)")
        self.assertEqual(ra.value_expr("base::global::CStrId", [("gfield", (0x7101000020, 0x18), 8)]),
                         "CStrId(*g_unk_7101000020[3])")
        self.assertEqual(ra.value_expr("base::global::CName", [("imm", 0x1234, 8)]),
                         "base::global::CName(base::global::UnkNameHash{}, 0x1234u)")
        self.assertEqual(ra.value_expr("base::global::CFilePathStrId", [("global", ra.GLOBAL_CFP, 8)]),
                         "base::global::CFilePathStrId(g_unk_7101d470e8)")
        self.assertIsNone(ra.value_expr("base::global::CFilePathStrId", [("global", 0x7101000000, 8)]))

    def test_vectors(self):
        xy = bits(1.0) | (bits(2.0) << 32)
        self.assertEqual(ra.value_expr("base::math::CVector2D", [("imm", xy, 8)]),
                         "base::math::CVector2D{1.0f, 2.0f}")
        self.assertEqual(ra.value_expr("base::math::CVector3D", [("imm", xy, 8), ("imm", bits(3.0), 4)]),
                         "base::math::CVector3D{1.0f, 2.0f, 3.0f}")
        zw = bits(3.0) | (bits(-4.0) << 32)
        self.assertEqual(ra.value_expr("base::math::CVector4D", [("imm", xy, 8), ("imm", zw, 8)]),
                         "base::math::CVector4D{1.0f, 2.0f, 3.0f, -4.0f}")
        self.assertIsNone(ra.value_expr("base::math::CVector3D", [("imm", xy, 8)]))

    def test_ambiguous_stores(self):
        self.assertIsNone(ra.value_expr("u32", [("imm", 1, 4), ("imm", 2, 4)]))
        self.assertIsNone(ra.value_expr("u32", [("imm", 1, 8)]))


FN_STMT = ["    if (UnkReflFunction* fn = fn_7100097550(cls, CStrId(\"Get\", true))) {",
           "        fn->name = base::global::UnkStrId(\"Get\", true);", "    }"]


class Statements(unittest.TestCase):
    SRC = "\n".join(["void fn_7100ab0000(CClass* cls) {", "    a();", *FN_STMT, "    b();", "}"])

    def test_statements_group_bodies(self):
        st = ra.statements(self.SRC)
        self.assertEqual([s[0] for s in st], ["    a();", FN_STMT[0], "    b();"])
        self.assertEqual(st[1], FN_STMT)
        self.assertTrue(ra.is_function_statement(st[1]))
        self.assertFalse(ra.is_function_statement(st[0]))

    def test_lambda_forms(self):
        alts = ra.alternatives(self.SRC)
        self.assertEqual(len(alts), 1)  # one function statement: the run and per-statement forms agree
        lines = alts[0].split("\n")
        self.assertEqual(lines[2], "    [&] {")
        self.assertEqual(lines[3], "    " + FN_STMT[0])
        self.assertEqual(lines[-3], "    }();")

    def test_typed_add_alternative(self):
        src = "void fn_7100ab0000(CClass* cls) {\n    x->Add<u8>(fn_7100aaaa00(), fn_7100bbbb00(), 3);\n}"
        self.assertEqual(ra.alternatives(src),
                         ["void fn_7100ab0000(CClass* cls) {\n"
                          "    x->AddTyped<u8, &fn_7100bbbb00>(fn_7100aaaa00(), 3);\n}"])

    def test_no_alternatives(self):
        self.assertEqual(ra.alternatives("void f(CClass* cls) {\n    a();\n}"), [])


class Parse(unittest.TestCase):
    def test_class_attribute(self):
        decls, src = run_parse([bl(ra.GET_ATTRS), bl(KEY), val("imm", 1, 1), bl(BOOL_T), bl(ra.ADD_ATTR)])
        self.assertEqual(decls, [f"const base::reflection::UnkReflAttrKey& fn_{KEY:x}();"])
        self.assertEqual(src, f"void fn_{ADDR:x}(CClass* cls) {{\n"
                              f"    fn_710009dd6c(cls)->Add<bool>(fn_{KEY:x}(), true);\n}}")

    def test_field_with_attribute(self):
        evs = [bl(ra.CSTRID_CTOR, **{"1": ("str", "speed"), "2": ("imm", 1)}),
               bl(U32_T),
               bl(ra.ADD_FIELD, **{"3": ("imm", 0x18), "4": ("imm", 2**64 - 1), "5": ("imm", 2**64 - 1)}),
               bl(ra.CSTRID_RELEASE),
               bl(KEY), val("imm", 7, 4), bl(U32_T), bl(ra.ADD_ATTR)]
        decls, src = run_parse(evs)
        self.assertEqual(src.split("\n")[1:-1], [
            '    UnkReflField* field0 = fn_7100096524(cls, CStrId("speed", true), fn_7100005178(), 0x18, -1, -1);',
            f"    field0->attrs.Add<u32>(fn_{KEY:x}(), 0x7u);"])
        self.assertEqual(decls, [f"const base::reflection::UnkReflAttrKey& fn_{KEY:x}();"])

    def test_field_typed_by_class_getter(self):
        getter = 0x7100CCCC00
        evs = [bl(ra.CSTRID_CTOR, **{"1": ("str", "obj"), "2": ("imm", 1)}), bl(getter),
               bl(ra.ADD_FIELD, **{"3": ("imm", 8), "4": ("imm", 2**64 - 1), "5": ("count", -2)})]
        decls, src = run_parse(evs, {getter})
        self.assertIn(f"const base::reflection::CClass* fn_{getter:x}();", decls)
        self.assertIn("cls->m_array_0x110.size() - 2);", src)

    def test_run_once_prologue(self):
        g = 0x7101500000
        evs = [val("flagtest", g, 1), val("flagset", g, 1), val("nullcheck", 0, 8),
               bl(ra.GET_ATTRS), bl(KEY), val("imm", 0, 1), bl(BOOL_T), bl(ra.ADD_ATTR)]
        decls, src = run_parse(evs)
        self.assertIn(f"extern bool g_unk_{g:x};", decls)
        self.assertEqual(src.split("\n")[1:6], [f"    if (g_unk_{g:x})", "        return;",
                                                f"    g_unk_{g:x} = true;", "    if (!cls)", "        return;"])

    def test_enum_by_global_descriptor(self):
        desc = 0x7101600000
        evs = [bl(ra.GET_ATTRS), bl(KEY), val("imm", 0xFE, 1), val("reftype", desc, 8), bl(ra.ADD_ATTR)]
        decls, src = run_parse(evs)
        self.assertIn(f"extern base::reflection::CType g_unk_{desc:x};", decls)
        self.assertIn(f"Add<u8>(fn_{KEY:x}(), &g_unk_{desc:x}, -2);", src)

    def test_tunable_registration(self):
        slot, obj, reg = 0x7101700000, 0x7101710000, 0x7100DDDD00
        evs = [bl(ra.ALLOC, **{"0": ("imm", 0x40), "1": ("imm", 8), "19": ("global", slot)}),
               bl(ra.TUNABLE_REGISTRY_CTOR),
               bl(reg, **{"1": ("ptr", obj), "2": ("str", "CTunableFoo")})]
        decls, src = run_parse(evs)
        self.assertIn(f"extern UnkTunableRegistry* g_unk_{slot:x};", decls)
        self.assertIn('    fn_7100dddd00(registry, &g_unk_7101710000, "CTunableFoo");', src)
        self.assertIn("fn_71000839a0(0x40, 0x8)", src)

    def test_unknown_shapes(self):
        self.assertIsNone(run_parse([]))
        self.assertIsNone(run_parse([bl(0x7100EEEE00)]))  # a lone call that is no statement
        # The attribute's value does not fit its type: no candidate.
        self.assertIsNone(run_parse([bl(ra.GET_ATTRS), bl(KEY), val("imm", 5, 1), bl(BOOL_T), bl(ra.ADD_ATTR)]))
        # A broken prologue.
        self.assertIsNone(run_parse([val("flagtest", 1, 1), val("nullcheck", 0, 8), bl(ra.GET_ATTRS)]))

    def test_declarations_deduplicated(self):
        evs = [bl(ra.GET_ATTRS), bl(KEY), val("imm", 1, 1), bl(BOOL_T), bl(ra.ADD_ATTR),
               bl(ra.GET_ATTRS), bl(KEY), val("imm", 0, 1), bl(BOOL_T), bl(ra.ADD_ATTR)]
        decls, src = run_parse(evs)
        self.assertEqual(len(decls), 1)
        self.assertEqual(src.count("Add<bool>"), 2)


if __name__ == "__main__":
    unittest.main()


class ReadableFields(unittest.TestCase):
    REG = '    UnkReflField* field{n} = fn_7100096524(cls, CStrId("{name}", true), fn_{getter}(), {off}, -1, -1);'

    def test_one_line_per_field(self):
        lines = [
            self.REG.format(n=0, name="fSpeed", getter="710000abbc", off="0x510"),
            "    field0->attrs.Add<f32>(fn_7100075240(), 1.0f);",
            '    field0->attrs.Add<const char*>(fn_7100007e80(), "Speed");',
            self.REG.format(n=1, name="uCount", getter="7100005178", off="0x514"),
            "    field1->attrs.Add<u32>(fn_7100075240(), 0x2u);",
            self.REG.format(n=2, name="bOn", getter="710000d228", off="0x518"),
        ]
        self.assertEqual(ra.readable_fields(lines), [
            '    DREAD_REFLECTED_FIELD(f32, "fSpeed", 0x510, (DefaultValue, 1.0f), (Description, "Speed"));',
            '    DREAD_REFLECTED_FIELD(u32, "uCount", 0x514, (DefaultValue, u32(0x2u)));',
            '    DREAD_REFLECTED_FIELD(bool, "bOn", 0x518);',
        ])

    def test_unnamed_attribute_keeps_the_variable(self):
        lines = [
            self.REG.format(n=0, name="fSpeed", getter="710000abbc", off="0x510"),
            "    field0->attrs.Add<s32>(fn_710000bf80(), &g_unk_7101d679f0, 0);",
        ]
        self.assertEqual(ra.readable_fields(lines), lines)
