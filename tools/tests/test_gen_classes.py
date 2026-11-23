"""tools/gen_classes.py and the declared-class pass of tools/gen_reflection.py, on made-up tables (no game data)."""

import unittest

from tools.tests import _path  # noqa: F401
import gen_classes as gc
import gen_reflection as gr

TYPES = {
    "base::core::CRoot": {"type": "base::core::CRoot", "kind": "class", "parent": "", "size": "0x10"},
    "CMid": {"type": "CMid", "kind": "class", "parent": "base::core::CRoot", "size": "0x20"},
    "CLeaf": {"type": "CLeaf", "kind": "class", "parent": "CMid", "size": "0x28"},
}
FIELDS = {
    "CMid": [(0x10, "sName", "base::global::CStrId"), (0x18, "fTime", "float")],
    "CLeaf": [(0x1c, "bOn", "bool"), (0x20, "iCount", "int")],
}

BLOCK = """// CLeaf
namespace CLeaf {
void refl_AddAttributes(CClass*) {}
void refl_AddFields(CClass* cls) {
    DREAD_REFLECTED_FIELD(bool, "bOn", 0x1c, (DefaultValue, false));
    DREAD_REFLECTED_FIELD(s32, "iCount", 0x20, (DefaultValue, 0));
}
s32 refl_Hash() { return -1; }
const base::reflection::CType& refl_TypeOf(const void* obj) { return ClassOf(obj); }
void refl_Construct(void* obj);
void refl_Destroy(void* obj) { Destroy(obj); }
struct refl_Description : ClassDescription {
    static constexpr u64 size = 0x28;
    static constexpr auto hash = &refl_Hash;
};
}  // namespace CLeaf
DREAD_REFLECTED_CLASS(CLeaf, CMid);
"""


class ClassLines(unittest.TestCase):
    def test_tail_padding_reuse(self):
        # CMid is 0x20 bytes, but CLeaf's first member is at 0x1c: CMid's data ends there.
        self.assertEqual(gc.data_size("CMid", TYPES, FIELDS), 0x1C)
        lines, includes = gc.class_lines("CMid", TYPES, FIELDS)
        self.assertIn("class CMid : public base::core::CRoot {", lines)
        self.assertIn("    base::global::CStrId sName;  // 0x10", lines)
        self.assertIn("    f32 fTime;  // 0x18", lines)
        self.assertNotIn("pad_0x1c", " ".join(lines))
        self.assertIn('static_assert(sizeof(CMid) == 0x20, "CMid size");', lines)
        self.assertIn('static_assert(offsetof(CMid, fTime) == 0x18, "CMid::fTime");', lines)
        self.assertEqual(lines[-1], "#pragma GCC diagnostic pop")
        self.assertEqual(includes, {"base/global/CStrId.h"})

    def test_root_has_virtual_table_and_namespace(self):
        lines, _ = gc.class_lines("base::core::CRoot", TYPES, FIELDS)
        self.assertEqual(lines[0], "namespace base::core {")
        self.assertNotIn("#pragma GCC diagnostic push", lines)  # no offsetof, no pragma
        self.assertIn("    virtual ~CRoot();", lines)
        self.assertIn("    u8 pad_0x8[0x8];", lines)

    def test_gap_and_unknown_type(self):
        lines, _ = gc.class_lines("CLeaf", TYPES, FIELDS)
        self.assertIn("    bool bOn;  // 0x1c", lines)
        # alignment padding (bool to int) and a leaf's tail padding are the compiler's, not members
        self.assertNotIn("pad_0x1d", " ".join(lines))
        self.assertNotIn("pad_0x24", " ".join(lines))
        gap = {**FIELDS, "CLeaf": [(0x1c, "bOn", "bool"), (0x24, "iCount", "int")]}
        self.assertIn("    u8 pad_0x1d[0x7];", gc.class_lines("CLeaf", TYPES, gap)[0])
        bad = {**FIELDS, "CLeaf": [(0x1c, "tList", "base::global::CRntVector<int>")]}
        with self.assertRaises(gc.LayoutError):
            gc.class_lines("CLeaf", TYPES, bad)

    def test_include_path(self):
        self.assertEqual(gc.include_of("base::core::CRoot"), "reflected/base/core/CRoot.h")


def member(cls, offset, typ, name, init="", note=""):
    return {"class": cls, "offset": offset, "type": typ, "name": name, "init": init, "source": "fn_7100000000",
            "note": note}


def method(cls, kind, declaration="", address="", definition=""):
    return {"class": cls, "kind": kind, "declaration": declaration, "address": address, "definition": definition,
            "source": "fn_7100000000"}


class ExtraTables(unittest.TestCase):
    def test_extra_members_fill_the_gap(self):
        extra = gc.Extra(members={"CLeaf": [member("CLeaf", "0x24", "f32", "field_0x24", note="a time")]})
        lines, _ = gc.class_lines("CLeaf", TYPES, FIELDS, extra)
        self.assertIn("    f32 field_0x24;  // 0x24: a time", lines)
        self.assertIn('static_assert(offsetof(CLeaf, field_0x24) == 0x24, "CLeaf::field_0x24");', lines)
        # a subclass's first extra member ends its base's data
        extra = gc.Extra(members={"CLeaf": [member("CLeaf", "0x18", "u32", "field_0x18")]})
        mid = {**FIELDS, "CMid": [(0x10, "sName", "base::global::CStrId")]}
        self.assertEqual(gc.data_size("CMid", TYPES, mid, extra), 0x18)

    def test_init_row_and_overlaps(self):
        extra = gc.Extra(members={"CLeaf": [member("CLeaf", "0x1c", "bool", "bOn", init="true")]})
        self.assertIn("    bool bOn = true;  // 0x1c", gc.class_lines("CLeaf", TYPES, FIELDS, extra)[0])
        for row in (member("CLeaf", "0x1c", "bool", "field_0x1c"),       # another name at a reflected offset
                    member("CLeaf", "0x1c", "bool", "bOn"),              # a reflected member without init
                    member("CLeaf", "0x22", "u32", "field_0x22"),        # inside iCount
                    member("CLeaf", "0x24", "UnkType", "field_0x24")):   # a type it cannot spell
            with self.assertRaises(gc.LayoutError, msg=row):
                gc.class_lines("CLeaf", TYPES, FIELDS, gc.Extra(members={"CLeaf": [row]}))

    def test_methods(self):
        extra = gc.Extra(methods={
            "base::core::CRoot": [method("base::core::CRoot", "virtual 3", "virtual void vf_0x18()")],
            "CLeaf": [method("CLeaf", "vtable", address="0x7101000010"),
                      method("CLeaf", "ctor", "CLeaf()", definition="default"),
                      method("CLeaf", "plain", "void Reset()", address="0x7100001234"),
                      method("CLeaf", "virtual 4", "virtual void vf_0x20()")]})
        root, _ = gc.class_lines("base::core::CRoot", TYPES, FIELDS, extra)
        self.assertEqual(root[root.index("    virtual ~CRoot();") + 1], "    virtual void vf_0x18();")
        lines, _ = gc.class_lines("CLeaf", TYPES, FIELDS, extra)
        self.assertIn("class CLeaf : public CMid {  // vtable g_unk_7101000010", lines)
        self.assertIn("    CLeaf();", lines)
        self.assertIn('    void Reset() asm("fn_7100001234");', lines)
        self.assertIn("inline CLeaf::CLeaf() = default;", lines)
        self.assertIn("    virtual void vf_0x20();", lines)
        ctor = gc.Extra(methods={"CLeaf": [method("CLeaf", "ctor", "CLeaf()", address="0x7100005678")]})
        self.assertIn("    CLeaf();  // fn_7100005678", gc.class_lines("CLeaf", TYPES, FIELDS, ctor)[0])
        for rows in ([method("CLeaf", "virtual 4", "virtual void vf_0x20()")],  # slot 3 is the next free one
                     [method("CLeaf", "ctor", "CMid()")],
                     [method("CLeaf", "plain", "void f()", definition="default")],
                     [method("CLeaf", "friend", "void f()")]):
            with self.assertRaises(gc.LayoutError, msg=rows):
                gc.class_lines("CLeaf", TYPES, FIELDS, gc.Extra(methods={"CLeaf": rows}))


class Selection(unittest.TestCase):
    def test_rule_and_scope(self):
        body = BLOCK.split("namespace CLeaf {\n", 1)[1].split("}  // namespace")[0]
        refl = ({"base::core::CRoot", "CMid"}, {"CLeaf": body})
        types = {**TYPES, "CLeaf::CInner": {"type": "CLeaf::CInner", "kind": "class", "parent": "", "size": "0x8"}}
        classes, errors = gc.selected(types, FIELDS, gc.NO_EXTRA, None, refl)
        self.assertEqual(classes, ["CLeaf", "CMid", "base::core::CRoot"])  # not the nested class
        classes, errors = gc.selected(types, FIELDS, gc.NO_EXTRA, {"CMid", "CLeaf::CInner"}, refl)
        self.assertEqual(classes, ["CMid"])
        self.assertEqual(len(errors), 1)
        self.assertIn("nested", errors[0])
        self.assertEqual(gc.declared(types, ["CLeaf"]), ["base::core::CRoot", "CMid", "CLeaf"])

    def test_getters_of_declared_classes_are_left_out(self):
        import gen_class_getters
        text = gen_class_getters.render([(0x7100000010, "CMid"), (0x7100000020, "COther")], {"CMid"})
        self.assertNotIn("CMid", text)
        self.assertIn("fn_7100000020();  // COther::refl_GetClass", text)


class DeclaredClassPass(unittest.TestCase):
    INCLUDES = {"CLeaf": "reflected/CLeaf.h", "CMid": "reflected/CMid.h"}
    SIZES = {"CLeaf": 0x28}
    OFFSETS = {("CLeaf", "bOn"): 0x1C, ("CLeaf", "iCount"): 0x20}

    def convert(self, text, offsets=None):
        return gr.with_declared_classes('#include "base/reflection/ReflectedClass.h"\n' + text, self.INCLUDES,
                                        self.SIZES, self.OFFSETS if offsets is None else offsets)

    def test_block_as_members(self):
        out = self.convert(BLOCK)
        self.assertIn('#include "reflected/CLeaf.h"', out)
        self.assertNotIn("namespace CLeaf", out)
        self.assertNotIn("refl_Construct", out)  # its declaration is the class's
        self.assertIn('DREAD_REFLECTED_FIELD(bool, "bOn", offsetof(CLeaf, bOn), (DefaultValue, false));', out)
        self.assertIn("s32 CLeaf::refl_Hash() { return -1; }", out)
        self.assertIn("{ return *static_cast<const CLeaf*>(obj)->vf_0x0(); }", out)
        self.assertIn("void CLeaf::refl_Destroy(void* obj) { static_cast<CLeaf*>(obj)->~CLeaf(); }", out)
        self.assertIn("struct CLeaf::refl_Description : ClassDescription {", out)
        self.assertIn("    static constexpr u64 size = sizeof(CLeaf);", out)
        self.assertIn('// CLeaf\n#pragma GCC diagnostic push\n#pragma GCC diagnostic ignored "-Winvalid-offsetof"\n', out)
        self.assertTrue(out.endswith("DREAD_REFLECTED_DECLARED_CLASS(CLeaf, CMid);\n#pragma GCC diagnostic pop\n"))
        self.assertEqual(gr.unconverted_classes([out], self.INCLUDES), ["CMid"])

    def test_kept_when_an_offset_disagrees_or_the_parent_is_not_declared(self):
        text = '#include "base/reflection/ReflectedClass.h"\n' + BLOCK
        self.assertEqual(self.convert(BLOCK, {("CLeaf", "bOn"): 0x1D, ("CLeaf", "iCount"): 0x20}), text)
        self.assertEqual(gr.with_declared_classes(text, {"CLeaf": "reflected/CLeaf.h"}, self.SIZES, self.OFFSETS),
                         text)

    def test_kept_for_destructor_first_tables(self):
        text = BLOCK.replace("ClassOf(obj)", "ClassOfDestructorFirst(obj)")
        self.assertIn("namespace CLeaf {", self.convert(text))


if __name__ == "__main__":
    unittest.main()
