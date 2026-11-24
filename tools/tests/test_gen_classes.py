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

    def test_key_function_and_reflection_members(self):
        # every class but a root declares its destructor out of line: the key function
        self.assertIn("    ~CMid() override;", gc.class_lines("CMid", TYPES, FIELDS)[0])
        self.assertNotIn("    ~CRoot() override;", gc.class_lines("base::core::CRoot", TYPES, FIELDS)[0])
        lines = gc.class_lines("CMid", TYPES, FIELDS, refl={"CMid": {"refl_GetClass", "refl_Hash"}})[0]
        self.assertIn("    static s32 refl_Hash();", lines)
        self.assertNotIn("    static void refl_Construct(void* obj);", lines)
        self.assertIn("    struct refl_Description;", lines)
        self.assertIn("    static void refl_Construct(void* obj);", gc.class_lines("CMid", TYPES, FIELDS)[0])

    def test_size_must_be_the_reflected_one(self):
        # a base whose subclass starts inside it, with nothing to pad it out to its reflected size
        types = {**TYPES, "CMid": {**TYPES["CMid"], "size": "0x28"}}
        with self.assertRaises(gc.LayoutError):
            gc.class_lines("CMid", types, {**FIELDS, "CLeaf": [(0x18, "bOn", "bool")]})


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
                      method("CLeaf", "plain", "void Reset()"),
                      method("CLeaf", "virtual 4", "virtual void vf_0x20()")]})
        root, _ = gc.class_lines("base::core::CRoot", TYPES, FIELDS, extra)
        self.assertEqual(root[root.index("    virtual ~CRoot();") + 1], "    virtual void vf_0x18();")
        lines, _ = gc.class_lines("CLeaf", TYPES, FIELDS, extra)
        self.assertIn("class CLeaf : public CMid {  // vtable g_unk_7101000010", lines)
        self.assertIn("    CLeaf();", lines)
        self.assertIn("    void Reset();", lines)
        self.assertIn("inline CLeaf::CLeaf() = default;", lines)
        self.assertIn("    virtual void vf_0x20();", lines)
        ctor = gc.Extra(methods={"CLeaf": [method("CLeaf", "ctor", "CLeaf()", address="0x7100005678")]})
        self.assertIn("    CLeaf();  // fn_7100005678", gc.class_lines("CLeaf", TYPES, FIELDS, ctor)[0])
        for rows in ([method("CLeaf", "virtual 4", "virtual void vf_0x20()")],  # slot 3 is the next free one
                     [method("CLeaf", "ctor", "CMid()")],
                     [method("CLeaf", "plain", "void f()", definition="default")],
                     [method("CLeaf", "plain", "void f()", address="0x7100001234")],  # no asm label
                     [method("CLeaf", "static", "static void f()", address="0x7100001234")],
                     [method("CLeaf", "vtable", address="0x7101000010"),
                      method("CLeaf", "vtable", address="0x7101000020")],
                     [method("CLeaf", "friend", "void f()")]):
            with self.assertRaises(gc.LayoutError, msg=rows):
                gc.class_lines("CLeaf", TYPES, FIELDS, gc.Extra(methods={"CLeaf": rows}))


class TableErrors(unittest.TestCase):
    def assertTableError(self, members=(), methods=(), where=""):
        with self.assertRaises(gc.LayoutError) as e:
            gc.extra_from_rows(list(members), list(methods))
        self.assertIn(where, str(e.exception))

    def test_malformed_rows(self):
        self.assertTableError([member("CLeaf", "24", "f32", "field_0x24")], where="data/class_members.csv:2: offset")
        self.assertTableError([member("CLeaf", "0xzz", "f32", "field_0x24")], where="data/class_members.csv:2")
        short = member("CLeaf", "0x24", "f32", "field_0x24")
        short["note"] = None  # what csv.DictReader gives a short row
        self.assertTableError([member("CLeaf", "0x20", "f32", "field_0x20"), short],
                              where="data/class_members.csv:3: missing note")
        surplus = {**member("CLeaf", "0x24", "f32", "field_0x24"), None: ["surplus"]}
        self.assertTableError([surplus], where="data/class_members.csv:2: extra")
        self.assertTableError(methods=[method("CLeaf", "ctor", "CLeaf()", address="7100001234")],
                              where="data/class_methods.csv:2: address")
        no_kind = method("CLeaf", "ctor", "CLeaf()")
        del no_kind["kind"]
        self.assertTableError(methods=[no_kind], where="data/class_methods.csv:2: missing kind")

    def test_duplicates(self):
        self.assertTableError([member("CLeaf", "0x1c", "bool", "bOn", init="true"),
                               member("CLeaf", "0x1c", "bool", "bOn", init="false")],
                              where="data/class_members.csv:3: CLeaf at 0x1c already has a row "
                                    "(data/class_members.csv:2)")
        self.assertTableError(methods=[method("CLeaf", "vtable", address="0x7101000010"),
                                       method("CLeaf", "vtable", address="0x7101000020")],
                              where="data/class_methods.csv:3: CLeaf already has a vtable row")
        ok = gc.extra_from_rows([member("CLeaf", "0x1c", "bool", "bOn", init="true")],
                                [method("CLeaf", "vtable", address="0x7101000010")])
        self.assertIn("    bool bOn = true;  // 0x1c", gc.class_lines("CLeaf", TYPES, FIELDS, ok)[0])


class TrackedFilesOnly(unittest.TestCase):
    def test_check_runs_without_capstone(self):
        # CI's policy job runs `gen_classes.py --check` with no capstone and no game data.
        import subprocess
        import sys
        from pathlib import Path
        root = Path(gc.__file__).resolve().parent.parent
        code = ("import sys, runpy; sys.modules['capstone'] = None; sys.argv = ['gen_classes.py', '--check']; "
                "runpy.run_path('tools/gen_classes.py', run_name='__main__')")
        run = subprocess.run([sys.executable, "-c", code], cwd=root, capture_output=True, text=True)
        self.assertNotIn("capstone", run.stderr)
        self.assertNotIn("Traceback", run.stderr)


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

    def test_units_that_open_a_declared_class_namespace(self):
        body = BLOCK.split("namespace CLeaf {\n", 1)[1].split("}  // namespace")[0]
        # CMid is in the declared form in its unit, but the unit also keeps a statics line in CMid's namespace
        units = [gc.Unit("CMid.cpp", {"CMid"}, {}, {"CMid"}),
                 gc.Unit("CLeaf.cpp", set(), {"CLeaf": "CMid"}, set())]
        refl = ({"base::core::CRoot", "CMid"}, {"CLeaf": body}, set(), units)
        self.assertEqual(gc.clashes({"base::core::CRoot", "CMid", "CLeaf"}, TYPES, units), {"CMid": "CMid.cpp"})
        why = {}
        classes, _ = gc.selected(TYPES, FIELDS, gc.NO_EXTRA, None, refl, why)
        self.assertEqual(classes, ["base::core::CRoot"])  # CMid and so CLeaf are left out
        self.assertIn("opens a namespace", why["CLeaf"])
        # a namespace-form subclass declares its parent's getter in the parent's namespace
        units = [gc.Unit("CMid.cpp", set(), {"CMid": "base::core::CRoot", "CLeaf": "CMid"}, set())]
        self.assertEqual(gc.clashes({"base::core::CRoot", "CMid"}, TYPES, units), {"CMid": "CMid.cpp"})
        self.assertEqual(gc.clashes({"base::core::CRoot", "CMid", "CLeaf"}, TYPES, units), {})
        # a type src/reflection/ names inside a class keeps the class out
        refl = ({"base::core::CRoot", "CMid"}, {"CLeaf": body}, {"CLeaf"}, [])
        self.assertEqual(gc.selected(TYPES, FIELDS, gc.NO_EXTRA, None, refl)[0], ["CMid", "base::core::CRoot"])

    def test_a_root_needs_its_virtual_table_shown(self):
        body = BLOCK.split("namespace CLeaf {\n", 1)[1].split("}  // namespace")[0]
        everyone = ["CLeaf", "CMid", "base::core::CRoot"]
        # CLeaf's block shows the getter in slot 0 and the destructor after it: the whole tree is declared
        refl = ({"base::core::CRoot", "CMid"}, {"CLeaf": body}, set(), [])
        self.assertEqual(gc.selected(TYPES, FIELDS, gc.NO_EXTRA, None, refl, destroys={"CLeaf"})[0], everyone)
        # declared-form lines show the same
        unit = gc.Unit("CMid.cpp", {"CMid"}, {}, set(), {"CMid": gc.slot_forms(
            "const base::reflection::CType& CMid::refl_TypeOf(const void* obj) "
            "{ return *static_cast<const CMid*>(obj)->refl_GetDynamicClass(); }")})
        self.assertEqual(unit.slots, {"CMid": {"getter"}})
        refl = ({"base::core::CRoot", "CMid", "CLeaf"}, {}, set(), [unit])
        self.assertEqual(gc.selected(TYPES, FIELDS, gc.NO_EXTRA, None, refl, destroys=set())[0], everyone)
        # a refl_Destroy the sources do not show (a lifted one, say) leaves the destructor's slot unproven
        why = {}
        self.assertEqual(gc.selected(TYPES, FIELDS, gc.NO_EXTRA, None, refl, why, destroys={"CLeaf"})[0], [])
        self.assertIn("virtual table", why["CLeaf"])
        # no getter shown, or the destructor first: left out
        refl = ({"base::core::CRoot", "CMid", "CLeaf"}, {}, set(), [])
        self.assertEqual(gc.selected(TYPES, FIELDS, gc.NO_EXTRA, None, refl, destroys=set())[0], [])
        first = body.replace("ClassOf(obj)", "ClassOfDestructorFirst(obj)")
        refl = ({"base::core::CRoot", "CMid"}, {"CLeaf": first}, set(), [])
        self.assertEqual(gc.selected(TYPES, FIELDS, gc.NO_EXTRA, None, refl, destroys=set())[0], [])

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
        self.assertIn("{ return *static_cast<const CLeaf*>(obj)->refl_GetDynamicClass(); }", out)
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

    def test_unused_usings_of_the_namespace_form(self):
        text = ("using base::reflection::ClassOf;\nusing base::reflection::Destroy;\nusing base::reflection::CClass;\n"
                + self.convert(BLOCK))
        out = gr.without_unused_declared_usings(text)
        self.assertNotIn("using base::reflection::ClassOf;", out)
        self.assertNotIn("using base::reflection::Destroy;", out)
        self.assertIn("using base::reflection::CClass;", out)
        namespace_form = "using base::reflection::ClassOf;\n" + BLOCK
        self.assertEqual(gr.without_unused_declared_usings(namespace_form), namespace_form)

    def test_kept_for_destructor_first_tables(self):
        text = BLOCK.replace("ClassOf(obj)", "ClassOfDestructorFirst(obj)")
        self.assertIn("namespace CLeaf {", self.convert(text))


if __name__ == "__main__":
    unittest.main()
