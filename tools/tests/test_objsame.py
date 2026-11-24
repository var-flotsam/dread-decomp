"""tools/objsame.py: name cutting and record comparison, an ELF reader check on a hand-built object,
and save/compare on tiny made-up sources with the project compiler (skipped without it)."""

import shutil
import struct
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import buildcfg
import fetch_toolchains
import hostenv
import objsame

try:
    from objdiff_gen import Reloc, TargetFunc, build_target
except ImportError:  # needs pyelftools
    build_target = None

HAVE_COMPILER = fetch_toolchains.clang_path(buildcfg.COMPILER).is_file() and bool(shutil.which(hostenv.cxxfilt()))


class Cut(unittest.TestCase):
    def test_parameters_and_qualifiers(self):
        self.assertEqual(objsame.cut("a::f(UnkX*, int) const"), "a::f")
        self.assertEqual(objsame.cut("a::f() &&"), "a::f")
        self.assertEqual(objsame.cut("fn_7100000100(UnkStruct_0x40*)"), "fn_7100000100")

    def test_local_entities(self):
        self.assertEqual(objsame.cut("f(UnkX*)::s"), "f::s")
        self.assertEqual(objsame.cut("guard variable for f(UnkX)::s"), "guard variable for f::s")

    def test_kept(self):
        self.assertEqual(objsame.cut("(anonymous namespace)::g(int)"), "(anonymous namespace)::g")
        self.assertEqual(objsame.cut("vtable for a::B"), "vtable for a::B")
        self.assertEqual(objsame.cut("h<void (*)(int)>(int)"), "h<void (*)(int)>")
        self.assertEqual(objsame.cut("g_unk_7100000000"), "g_unk_7100000000")

    def test_mangled_part(self):
        self.assertEqual(objsame.mangled_part("_ZN1a1fEv"), ("", "_ZN1a1fEv"))
        self.assertEqual(objsame.mangled_part(".text._ZN1a1fEv"), (".text.", "_ZN1a1fEv"))
        self.assertIsNone(objsame.mangled_part(".text.main"))
        self.assertIsNone(objsame.mangled_part("x_Zy"))


def record(sha="00", relocs=(), symbols=None, name=".text._Z1fP1A"):
    return {"sections": [{"name": name, "type": 1, "flags": 6, "align": 4, "size": 8, "sha256": sha,
                          "relocs": [list(r) for r in relocs]}],
            "symbols": symbols if symbols is not None else [["_Z1fP1A", name, 0, 8, 2, 1, 0]]}


class Differences(unittest.TestCase):
    def test_same(self):
        self.assertEqual(objsame.differences(record(), record()), [])

    def test_bytes(self):
        out = objsame.differences(record(sha="00"), record(sha="11"))
        self.assertEqual(out, ["bytes of .text._Z1fP1A (8 -> 8 bytes)"])

    def test_relocation_target(self):
        out = objsame.differences(record(relocs=[(4, 283, 0, "_Z1gv")]), record(relocs=[(4, 283, 0, "_Z1hv")]))
        self.assertEqual(out[0], "names only: the same bytes, relocations and symbols apart from their names")
        self.assertIn("relocations in .text._Z1fP1A (first at +0x4: _Z1gv)", out)

    def test_relocation_addend_is_not_names_only(self):
        out = objsame.differences(record(relocs=[(4, 283, 0, "g")]), record(relocs=[(4, 283, 8, "g")]))
        self.assertEqual(out, ["relocations in .text._Z1fP1A (first at +0x4: g)"])

    def test_does_not_compile(self):
        out = objsame.differences(record(), {"error": "x.cpp:1:1: error: boom"})
        self.assertEqual(out, ["after: does not compile (x.cpp:1:1: error: boom)"])

    def test_modulo_names(self):
        names = {"_Z1fP1A": "f(A*)", "_Z1fP1B": "f(B*)"}
        rename = lambda n: (objsame.mangled_part(n)[0] + objsame.cut(names[objsame.mangled_part(n)[1]])  # noqa: E731
                            if objsame.mangled_part(n) else n)
        a = record()
        b = record(name=".text._Z1fP1B", symbols=[["_Z1fP1B", ".text._Z1fP1B", 0, 8, 2, 1, 0]])
        self.assertTrue(objsame.differences(objsame.normalised(a), objsame.normalised(b)))
        self.assertEqual(objsame.differences(objsame.normalised(a, rename), objsame.normalised(b, rename)), [])

    def test_relative_command(self):
        tree = Path("/elsewhere/tree")
        cmd = ["clang", "-I", str(objsame.ROOT / "include"), "-c", str(tree / "src/a.cpp"), "-o", "/x/a.o"]
        self.assertEqual(objsame.relative_command(cmd, tree),
                         ["clang", "-I", "include", "-c", str(Path("src/a.cpp")), "-o", "/x/a.o"])


@unittest.skipUnless(build_target, "needs pyelftools")
class ReadElf(unittest.TestCase):
    def test_hand_built_object(self):
        code = struct.pack("<2I", 0x94000000, 0xD65F03C0)  # bl; ret
        data = build_target([TargetFunc("fn_7100000100", 1, code, [Reloc(0, 283, "fn_7100000200", 0)])])
        rec = objsame.read_elf(data)
        text = [s for s in rec["sections"] if s["name"] == ".text.fn_7100000100"]
        self.assertEqual(len(text), 1)
        self.assertEqual(text[0]["size"], 8)
        self.assertEqual(text[0]["relocs"], [[0, 283, 0, "fn_7100000200"]])
        self.assertIn("fn_7100000100", [s[0] for s in rec["symbols"]])
        self.assertNotIn("fn_7100000200", [s[0] for s in rec["symbols"]])  # undefined: not recorded

    def test_not_elf(self):
        with self.assertRaises(ValueError):
            objsame.read_elf(b"not an object")


SOURCE = """\
struct {T} {{ int a; int b; }};
// {comment}
int f({T}* p) {{ return p->a + {k}; }}
int g({T}* p) {{ return f(p) * 2; }}
"""


@unittest.skipUnless(HAVE_COMPILER, "needs the project compiler")
class Compiled(unittest.TestCase):
    def setUp(self):
        self.tree = Path(tempfile.mkdtemp())
        self.addCleanup(shutil.rmtree, self.tree, True)

    def build(self, tag: str, **fields) -> dict:
        text = SOURCE.format(**{"T": "UnkThing", "comment": "one", "k": 1, **fields})
        (self.tree / "a.cpp").write_text(text, encoding="utf-8")
        return objsame.build_record("a.cpp", self.tree / tag, self.tree)

    def compare(self, a: dict, b: dict, modulo_names: bool = False) -> list[str]:
        rename = objsame.renamer([a, b]) if modulo_names else None
        return objsame.differences(objsame.normalised(a, rename), objsame.normalised(b, rename))

    def test_comment_is_unchanged(self):
        self.assertEqual(self.compare(self.build("x"), self.build("y", comment="two")), [])

    def test_constant_is_changed(self):
        out = self.compare(self.build("x"), self.build("y", k=2))
        self.assertTrue(out)
        self.assertTrue(any(line.startswith("bytes of .text._Z1fP8UnkThing") for line in out), out)

    def test_type_rename(self):
        a, b = self.build("x"), self.build("y", T="RenamedThing")
        exact = self.compare(a, b)
        self.assertEqual(exact[0], "names only: the same bytes, relocations and symbols apart from their names")
        self.assertEqual(self.compare(a, b, modulo_names=True), [])

    def test_type_rename_with_a_code_change(self):
        self.assertTrue(self.compare(self.build("x"), self.build("y", T="RenamedThing", k=3), modulo_names=True))

    def test_broken_source_is_changed(self):
        a, b = self.build("x"), self.build("y", k="undeclared_name")
        self.assertIn("error", b)
        self.assertTrue(self.compare(a, b))
        self.assertTrue(self.compare(b, b))  # failing on both sides is still a change to report

    def test_object_is_kept(self):
        self.build("x")
        self.assertTrue((self.tree / "x" / "obj" / "a.cpp.o").is_file())
        self.assertTrue((self.tree / "x" / "rec" / "a.cpp.json").is_file())


if __name__ == "__main__":
    unittest.main()
