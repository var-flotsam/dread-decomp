"""tools/gen_statics.py: fact extraction on hand-assembled words and the C++ each kind emits
(no game data)."""

import struct
import unittest

from tools.tests import _path  # noqa: F401
import gen_statics as gs

BASE = 0x7100000000
DATA_PAGE = 0x7100004000
NAME = DATA_PAGE + 0x40
SLOT = DATA_PAGE + 0x800
OBJECT = 0x7100005000
TYPE_GETTER = 0x7100000080


class Asm:
    def __init__(self, pc):
        self.start = self.pc = pc
        self.words = []

    def _emit(self, w):
        self.words.append(w)
        self.pc += 4
        return self

    def movw(self, rd, imm):  # mov wD, #imm
        return self._emit(0x52800000 | (imm << 5) | rd)

    def add(self, rd, rn, imm):
        return self._emit(0x91000000 | (imm << 10) | (rn << 5) | rd)

    def adrp(self, rd, target):
        off = ((target >> 12) - (self.pc >> 12)) & 0x1FFFFF
        return self._emit(0x90000000 | ((off & 3) << 29) | ((off >> 2) << 5) | rd)

    def ldr(self, rt, rn, off):
        return self._emit(0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt)

    def strw(self, rt, rn, off):  # str wT, [xN, #off]
        return self._emit(0xB9000000 | ((off // 4) << 10) | (rn << 5) | rt)

    def bl(self, target):
        return self._emit(0x94000000 | (((target - self.pc) >> 2) & 0x3FFFFFF))

    def ret(self):
        return self._emit(0xD65F03C0)

    def code(self):
        return b"".join(struct.pack("<I", w) for w in self.words)


class FakeImage:
    def __init__(self, strings, pointers):
        data = bytearray(DATA_PAGE - BASE + 0x1000)
        for a, s in strings.items():
            data[a - BASE:a - BASE + len(s) + 1] = s.encode() + b"\0"
        self.data = bytes(data)
        self.ptrs = {a - BASE: t - BASE for a, t in pointers.items()}

    def cstring(self, a):
        if not 0 <= a < len(self.data):
            return None
        end = self.data.find(b"\0", a)
        return None if end < 0 else self.data[a:end].decode("ascii")

    def pointer(self, a):
        return self.ptrs.get(a)


class FakeOriginal:
    def __init__(self, asm, strings=None, pointers=None):
        self.code = {asm.start: asm.code()}
        self.img = FakeImage(strings or {}, pointers or {})

    def function(self, address):
        return self.code[address]


class Facts(unittest.TestCase):
    def test_attribute_key(self):
        a = Asm(0x7100000100)
        a.adrp(1, NAME).add(1, 1, NAME & 0xFFF).bl(gs.CSTRID_CTOR).bl(TYPE_GETTER)
        a.movw(8, 5).strw(8, 0, 0x10).ret()
        f = gs.facts(FakeOriginal(a, {NAME: "Description"}), a.start)
        self.assertEqual(f["name"], "Description")
        self.assertEqual(f["type"], TYPE_GETTER)
        self.assertEqual(f["u32"], 5)
        self.assertEqual(f["calls"], [gs.CSTRID_CTOR, TYPE_GETTER])
        self.assertEqual(f["x1_adds"], [NAME])

    def test_name_must_be_a_string(self):
        a = Asm(0x7100000100)
        a.bl(gs.CSTRID_CTOR).ret()
        self.assertIsNone(gs.facts(FakeOriginal(a), a.start))

    def test_got_dereference(self):
        a = Asm(0x7100000100)
        a.adrp(0, SLOT).ldr(0, 0, SLOT & 0xFFF).ldr(0, 0, 0).ret()
        f = gs.facts(FakeOriginal(a, pointers={SLOT: OBJECT}), a.start)
        self.assertEqual(f["source"], OBJECT)

    def test_adrp_over_a_got_register_is_a_page_load(self):
        a = Asm(0x7100000100)
        a.adrp(0, SLOT).ldr(0, 0, SLOT & 0xFFF).adrp(0, DATA_PAGE).ldr(0, 0, 0).ret()
        f = gs.facts(FakeOriginal(a, pointers={SLOT: OBJECT}), a.start)
        self.assertNotIn("source", f)

    def test_call_clobbers_constants(self):
        a = Asm(0x7100000100)
        a.movw(8, 5).bl(0x7100000010).strw(8, 0, 0x10).ret()
        self.assertNotIn("u32", gs.facts(FakeOriginal(a), a.start))


class KeyType(unittest.TestCase):
    def test_global_preferred_over_getter(self):
        decls: set[str] = set()
        self.assertEqual(gs.key_type({"type_global": 0x7100000040, "type": 0x7100000080}, decls),
                         "&g_unk_7100000040")
        self.assertEqual(decls, {"extern const base::reflection::CType g_unk_7100000040;"})

    def test_getter_and_none(self):
        decls: set[str] = set()
        self.assertEqual(gs.key_type({"type": 0x7100000080}, decls), "fn_7100000080()")
        self.assertIn("const base::reflection::CType* fn_7100000080();", decls)
        self.assertIsNone(gs.key_type({}, set()))


class Emit(unittest.TestCase):
    ADDR = 0x7100000100
    TAG = "UnkRefl_fn_7100000100"

    def test_key(self):
        decls: set[str] = set()
        out = gs.emit("key", self.ADDR, {"calls": [], "name": 'A"b', "type": 0x7100000080, "u32": 5}, decls, {})
        self.assertEqual(out[:3], [f"struct {self.TAG};", "template <>",
                                   f"struct base::reflection::UnkReflStatic<{self.TAG}> {{"])
        self.assertIn('    static constexpr const char* name = "A\\"b";', out)
        self.assertIn("    static constexpr u32 field_0x10 = 0x5;", out)
        self.assertTrue(out[-1].endswith(f"refl_GetAttrKey<{self.TAG}>();"))

    def test_key_needs_u32(self):
        self.assertIsNone(gs.emit("key", self.ADDR, {"calls": [], "name": "A", "type": 1}, set(), {}))

    def test_key_flags(self):
        f = {"calls": [], "name": "A", "type": 0x7100000080, "u32": 1, "u16": 0x101}
        out = gs.emit("key_flags_b", self.ADDR, f, set(), {})
        self.assertIn("    static constexpr u16 field_0x14 = 0x101;", out)
        self.assertIn("refl_GetAttrKeyFlags<", out[-1])

    def test_plain_strid(self):
        out = gs.emit("strid", self.ADDR, {"calls": [], "name": "ABILITY"}, set(), {})
        self.assertEqual(out[-1], f"template const base::global::CStrId& "
                                  f"base::reflection::refl_GetStaticStrId<{self.TAG}>();")

    def test_strid_local_is_a_plain_function(self):
        out = gs.emit("strid_local", self.ADDR, {"calls": [], "name": "X"}, set(), {})
        self.assertEqual(out, ["const base::global::CStrId& fn_7100000100() {",
                               '    static base::global::CStrId s_id("X", true);', "    return s_id;", "}"])

    def test_find_skips_known_calls(self):
        decls: set[str] = set()
        f = {"calls": [gs.GUARD_ACQUIRE, gs.CSTRID_CTOR, 0x7100000010, 0x7100000020, 0x7100000030], "name": "N"}
        out = gs.emit("find", self.ADDR, f, decls, {})
        self.assertIn("    static const CClass* type() { return fn_7100000020(); }", out)
        self.assertEqual(len(decls), 3)

    def test_registry_map_uses_its_registry(self):
        f = {"calls": [0x7100000010, 0x7100000020], "guard": 0x7101000000}
        out = gs.emit("registry_map", self.ADDR, f, set(), {0x7101000000: 0x7100000200})
        self.assertEqual(out[0], "struct UnkRefl_fn_7100000200;")
        self.assertIn("    using Registry = UnkRefl_fn_7100000200;", out)
        self.assertIsNone(gs.emit("registry_map", self.ADDR, f, set(), {}))  # no registry, no flags

    def test_singleton_part(self):
        out = gs.emit("singleton_part", self.ADDR, {"calls": [1, gs.SINGLETON_CTOR], "first_imm": 0x18}, set(), {})
        self.assertIn("    static constexpr u32 offset = 0x18;", out)
        self.assertIsNone(gs.emit("singleton_part", self.ADDR, {"calls": [1, 2], "first_imm": 0x18}, set(), {}))


class Layout(unittest.TestCase):
    def test_is_instantiation(self):
        self.assertTrue(gs.is_instantiation("template void f<X>();"))
        self.assertFalse(gs.is_instantiation("template <>"))
        self.assertFalse(gs.is_instantiation("struct X;"))

    def test_specialisations_before_instantiations(self):
        blocks = {1: ["struct A;", "template <>", "struct S<A> {};", "template void f<A>();"],
                  2: ["struct B;", "template void f<B>();"],
                  9: ["struct R;", "template void f<R>();"]}
        out = gs.layout([1, 2], blocks, {2: 9})
        # the registry's specialisation is copied in, its instantiation isn't
        self.assertEqual(out, ["struct A;", "template <>", "struct S<A> {};", "struct B;", "struct R;",
                               "template void f<A>();", "template void f<B>();"])


class EmitU32(unittest.TestCase):
    def test_chain_with_check(self):
        target = ("load", ("load", ("self",), 0x10), 0x20)
        f = {"bool": True, "target": target, "checks": {("load", ("self",), 0x10)}, "hash": 0x1234,
             "kind": 3, "value": 7, "a": 0x7100000010, "b": 0x7100000020}
        out = gs.emit_emit_u32(0x7100000100, f)
        self.assertEqual(out, [
            "bool fn_7100000100(char* self) {",
            "    char* p1 = *reinterpret_cast<char**>(self + 0x10);",
            "    if (!p1)",
            "        return false;",
            "    char* p2 = *reinterpret_cast<char**>(p1 + 0x20);",
            "    return base::reflection::refl_EmitU32(p2, 0x1234, 0x3, 0x7, &g_unk_7100000010, &g_unk_7100000020);",
            "}"])

    def test_void_with_hash_parameter(self):
        f = {"bool": False, "target": ("self",), "checks": set(), "hash": None,
             "kind": 1, "value": 0, "a": 0x7100000010, "b": 0x7100000020}
        out = gs.emit_emit_u32(0x7100000100, f)
        self.assertEqual(out[0], "void fn_7100000100(char* self, u64 hash) {")
        self.assertTrue(out[1].startswith("    base::reflection::refl_EmitU32(self, hash, "))


if __name__ == "__main__":
    unittest.main()
