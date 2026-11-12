"""tools/gen_fields.py: members read from a generated reflection source, their type names, a class's layout.

Made-up sources; no game data.
"""

import unittest

from tools.tests import _path  # noqa: F401
import gen_fields

SOURCE = '''// Generated
void fn_7100000100(CClass* cls) {
    fn_710009dd6c(cls)->Add<const char*>(fn_7100007e80(), "A node");
}
void fn_7100000200(CClass* cls) {
    UnkReflField* field0 = fn_7100096524(cls, CStrId("eKind", true), &g_unk_7101d00010, 0x10, -1, -1);
    field0->attrs.Add<s32>(fn_7100075240(), &g_unk_7101d00010, 0);
    UnkReflField* field1 = fn_7100096524(cls, CStrId("fWeight", true), fn_7100000abc(), 0x14, -1, -1);
    UnkReflField* field2 = fn_7100096524(cls, CStrId("pOwner", true), fn_7100000def(), 8, -1, -1);
}
void fn_7100000300(base::reflection::CClass*) {}
DREAD_REFL_CLASS_GETTER(7100000400, 7100000500, "game::CNode", 7100000600, 7100000100, 7100000200);
DREAD_REFL_ROOT_CLASS_GETTER(7100000700, 7100000800, "game::CBase", 7100000100, 7100000300);
'''
TYPE_SOURCE = '''template <>
struct base::reflection::UnkReflPrimitiveTraits<UnkRefl_fn_7100000abc> {
    static constexpr const char* name = "float";
};
template <>
struct base::reflection::UnkReflTypeGetter<UnkRefl_fn_7100000def> {
    static constexpr const char* name = "game::CBase*";
    using Init = UnkRefl_fn_7100000fff;
};
'''


class Members(unittest.TestCase):
    def test_members_of_each_class(self):
        self.assertEqual(gen_fields.members(SOURCE), [
            ("game::CNode", 0x10, "eKind", "&g_unk_7101d00010"),
            ("game::CNode", 0x14, "fWeight", "fn_7100000abc()"),
            ("game::CNode", 8, "pOwner", "fn_7100000def()"),
        ])

    def test_type_names(self):
        getters = gen_fields.getter_names([{"type": "int", "getter": "0x7100000111"}, {"type": "x", "getter": ""}],
                                          [TYPE_SOURCE])
        self.assertEqual(getters, {0x7100000111: "int", 0x7100000abc: "float", 0x7100000def: "game::CBase*"})
        objects = {0x7101d00010: "game::EKind"}
        self.assertEqual([gen_fields.type_name(e, getters, objects) for e in
                          ("fn_7100000abc()", "&g_unk_7101d00010", "fn_7100000999()", "&g_unk_7101d00020")],
                         ["float", "game::EKind", "fn_7100000999()", "&g_unk_7101d00020"])

    def test_layout_includes_the_bases(self):
        rows = [{"class": "game::CNode", "offset": "0x10", "name": "eKind", "type": "game::EKind"},
                {"class": "game::CBase", "offset": "0x8", "name": "sName", "type": "base::global::CStrId"},
                {"class": "game::COther", "offset": "0x8", "name": "x", "type": "int"}]
        types = {"game::CNode": {"parent": "game::CBase"}, "game::CBase": {"parent": ""}}
        self.assertEqual(gen_fields.layout("game::CNode", rows, types), [
            (0x8, "sName", "base::global::CStrId", "game::CBase"),
            (0x10, "eKind", "game::EKind", "game::CNode"),
        ])



def adrp(rd: int, page: int) -> int:
    """An adrp whose page the fake `page_of` below reads straight out of the word."""
    return 0x90000000 | rd | (page << 5)


def add(rd: int, rn: int, imm: int) -> int:
    return 0x91000000 | rd | (rn << 5) | (imm << 10)


def ldr(rd: int, rn: int, imm: int) -> int:
    return 0xF9400000 | rd | (rn << 5) | ((imm // 8) << 10)


BL = 0x94000000
NOP = 0xD503201F


class BuiltObjects(unittest.TestCase):
    STRINGS = {0x3010: "game::EKind", 0x3020: "ssv"}
    SLOTS = {0x5008: 0x9000}

    def run_words(self, words, wanted=frozenset({0x9000})):
        return gen_fields.names_built(words, 0x1000, set(wanted), lambda w, pc: ((w >> 5) & 0xFFFF) << 12,
                                      self.SLOTS.get, self.STRINGS.get)

    def test_name_then_object_through_the_got(self):
        words = [adrp(1, 3), add(1, 1, 0x10), BL,              # the name becomes a string id
                 adrp(8, 5), ldr(0, 8, 8), BL]                 # the object's constructor
        self.assertEqual(self.run_words(words), {0x9000: "game::EKind"})

    def test_object_by_adrp_add(self):
        words = [adrp(1, 3), add(1, 1, 0x10), BL, adrp(0, 9), add(0, 0, 0), BL]
        self.assertEqual(self.run_words(words), {0x9000: "game::EKind"})

    def test_first_call_names_it(self):
        words = [adrp(1, 3), add(1, 1, 0x10), BL, adrp(8, 5), ldr(0, 8, 8), BL,
                 adrp(1, 3), add(1, 1, 0x20), adrp(8, 5), ldr(0, 8, 8), BL]   # a later call with another string
        self.assertEqual(self.run_words(words), {0x9000: "game::EKind"})

    def test_a_far_string_is_not_the_name(self):
        words = [adrp(1, 3), add(1, 1, 0x10), BL] + [NOP] * 41 + [adrp(8, 5), ldr(0, 8, 8), BL]
        self.assertEqual(self.run_words(words), {})

    def test_an_object_not_asked_for(self):
        words = [adrp(1, 3), add(1, 1, 0x10), BL, adrp(8, 5), ldr(0, 8, 8), BL]
        self.assertEqual(self.run_words(words, wanted={0x7777}), {})

    def test_text_at(self):
        self.assertEqual(gen_fields.text_at(b"base::EType\0junk"), "base::EType")
        self.assertIsNone(gen_fields.text_at(b"ab\0"))           # too short to be a type's name
        self.assertIsNone(gen_fields.text_at(b"\x01\x02\x03\x04\0"))
        self.assertIsNone(gen_fields.text_at(None))


if __name__ == "__main__":
    unittest.main()
