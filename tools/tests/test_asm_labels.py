"""tools/dread/asm_labels.py: a constructor's or destructor's address read from its declaration, and
tools/check.py's rule that a matching function's call to one that nothing names fails. No game data: the
declarations are written here, and the "original" is a few instruction words."""

import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.tests import _path  # noqa: F401
from dread import asm_labels  # noqa: E402
from dread.asm_labels import Labels, is_special, param_key, scan_text, split_member  # noqa: E402

HEADER = """\
#pragma once
struct UnkHalf_cu {
    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");
    ~UnkHalf_cu();  // fn_7100246000, frees the entry
};
namespace base::global {
struct CRntString {
    CRntString(const char* str);  // fn_710007e704
    CRntString(const CRntString& other) asm("fn_710007e7f8");
};
}
template <u64 CTOR>
struct UnkStrIdTable {
    UnkStrIdTable();  // fn_<CTOR>
};
template <int Id>
struct UnkStorage {
    ~UnkStorage();
};
template <> UnkStorage<0>::~UnkStorage();  // fn_7100d90040
template <> UnkStorage<1>::~UnkStorage();  // fn_7100d8f8f8
struct UnkDup {
    UnkDup(u32 a);  // fn_7100000100
};
"""

SOURCE = """\
#include "x.h"
#define STATE(ctor, size)                  \\
    struct UnkState_##ctor {               \\
        UnkState_##ctor(); /* fn_##ctor */ \\
    };
STATE(7100722124, 0x868)
STATE(71010f06bc, 0x1cf0)
STATE(71007977dc, f(0x1e0, g(1)))
struct UnkLocal {
    explicit UnkLocal(s32 a, f32* b);  // fn_7100abc000
    ~UnkLocal() override;  // a destructor (no address)
};
struct UnkDup {
    UnkDup(u32 a);  // fn_7100000200
};
"""


class Scan(unittest.TestCase):
    def test_forms(self):
        found = {(lab.cls, lab.kind, lab.params, lab.address, lab.targs) for lab in scan_text(HEADER, "x.h")}
        self.assertIn(("UnkHalf_cu", "ctor", ("UnkHalf_cu&&",), 0x71002458A4, None), found)
        self.assertIn(("UnkHalf_cu", "dtor", (), 0x7100246000, None), found)
        self.assertIn(("CRntString", "ctor", ("char*",), 0x710007E704, None), found)
        self.assertIn(("UnkStrIdTable", "ctor", (), None, None), found)
        self.assertIn(("UnkStorage", "dtor", (), 0x7100D90040, ("0",)), found)
        self.assertNotIn("UnkStorage", {lab.cls for lab in scan_text(HEADER, "x.h") if lab.targs is None})

    def test_comment_must_start_with_the_address(self):
        self.assertEqual(scan_text("    ~UnkX();  // calls fn_7100000100\n", "x.h"), [])
        self.assertEqual(scan_text("    void Run();  // fn_7100000100\n", "x.h"), [])

    def test_macro_uses(self):
        found = {(lab.cls, lab.address) for lab in scan_text(SOURCE, "a.cpp")}
        self.assertIn(("UnkState_7100722124", 0x7100722124), found)
        self.assertIn(("UnkState_71010f06bc", 0x71010F06BC), found)
        self.assertIn(("UnkState_71007977dc", 0x71007977DC), found)  # an argument with parentheses

    def test_one_line_only(self):
        # a macro use followed by a declaration on the next line is not one declaration
        found = scan_text("STATE(71010f06bc, 0x1cf0)\n    ~UnkBoxQ() override;  // fn_7100215e1c\n", "a.cpp")
        self.assertEqual([(lab.cls, lab.kind, lab.address) for lab in found], [("UnkBoxQ", "dtor", 0x7100215E1C)])

    def test_param_key(self):
        self.assertEqual(param_key("const base::math::CVector2D& s, u32 n, s64 v = 0"),
                         ("CVector2D&", "unsigned int", "long"))
        self.assertEqual(param_key("CVector2D const&, unsigned int, long", named=False),
                         ("CVector2D&", "unsigned int", "long"))
        self.assertEqual(param_key("void"), ())

    def test_split_member(self):
        self.assertEqual(split_member("base::reflection::UnkStrIdTable<485331507348ul>::UnkStrIdTable()"),
                         ("UnkStrIdTable", ("485331507348",), "UnkStrIdTable", ""))
        self.assertEqual(split_member("UnkBox::UnkBox(UnkVec3 const&, UnkVec3 const&)")[3],
                         "UnkVec3 const&, UnkVec3 const&")


class Lookup(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        (root / "include").mkdir()
        (root / "src").mkdir()
        (root / "include" / "x.h").write_text(HEADER, encoding="utf-8")
        (root / "src" / "a.cpp").write_text(SOURCE, encoding="utf-8")
        (root / "src" / "b.cpp").write_text('#include "x.h"\n', encoding="utf-8")
        self.labels = Labels(root)

    def tearDown(self):
        self.tmp.cleanup()

    def look(self, sym, name, source="src/b.cpp"):
        return self.labels.lookup(sym, name, source)

    def test_constructor_variants(self):
        for sym in ("_ZN10UnkHalf_cuC1EOS_", "_ZN10UnkHalf_cuC2EOS_"):
            self.assertEqual(self.look(sym, "UnkHalf_cu::UnkHalf_cu(UnkHalf_cu&&)")[0], 0x71002458A4)
        self.assertEqual(self.look("_ZN4base6global10CRntStringC1EPKc",
                                   "base::global::CRntString::CRntString(char const*)")[0], 0x710007E704)
        self.assertEqual(self.look("_ZN4base6global10CRntStringC1ERKS1_",
                                   "base::global::CRntString::CRntString(base::global::CRntString const&)")[0],
                         0x710007E7F8)

    def test_deleting_destructor_never_maps(self):
        self.assertFalse(is_special("_ZN10UnkHalf_cuD0Ev"))
        self.assertIsNone(self.look("_ZN10UnkHalf_cuD0Ev", "UnkHalf_cu::~UnkHalf_cu()")[0])
        self.assertEqual(self.look("_ZN10UnkHalf_cuD1Ev", "UnkHalf_cu::~UnkHalf_cu()")[0], 0x7100246000)

    def test_template_argument_and_specialization(self):
        self.assertEqual(self.look("_ZN13UnkStrIdTableILm485331507348EEC1Ev",
                                   "UnkStrIdTable<485331507348ul>::UnkStrIdTable()")[0], 0x7100031894)
        self.assertEqual(self.look("_ZN10UnkStorageILi1EED2Ev", "UnkStorage<1>::~UnkStorage()")[0], 0x7100D8F8F8)
        self.assertIsNone(self.look("_ZN10UnkStorageILi2EED2Ev", "UnkStorage<2>::~UnkStorage()")[0])

    def test_own_file_first_then_headers(self):
        self.assertEqual(self.look("_ZN6UnkDupC1Ej", "UnkDup::UnkDup(unsigned int)", "src/a.cpp")[0], 0x7100000200)
        self.assertEqual(self.look("_ZN6UnkDupC1Ej", "UnkDup::UnkDup(unsigned int)", "src/b.cpp")[0], 0x7100000100)
        # another source's declarations are out of reach
        self.assertIsNone(self.look("_ZN8UnkLocalC1EiPf", "UnkLocal::UnkLocal(int, float*)", "src/b.cpp")[0])
        self.assertEqual(self.look("_ZN8UnkLocalC1EiPf", "UnkLocal::UnkLocal(int, float*)", "src/a.cpp")[0],
                         0x7100ABC000)

    def test_parameters_must_match(self):
        addr, why = self.look("_ZN10UnkHalf_cuC1ERKS_", "UnkHalf_cu::UnkHalf_cu(UnkHalf_cu const&)")
        self.assertIsNone(addr)
        self.assertIn("no declaration", why)

    def test_ambiguous(self):
        (Path(self.tmp.name) / "include" / "y.h").write_text("struct UnkDup {\n    UnkDup(u32 a);  // fn_7100000300\n};\n",
                                                             encoding="utf-8")
        addr, why = Labels(Path(self.tmp.name)).lookup("_ZN6UnkDupC1Ej", "UnkDup::UnkDup(unsigned int)", "src/b.cpp")
        self.assertIsNone(addr)
        self.assertIn("different addresses", why)

    def test_digest_follows_labels(self):
        before = self.labels.digest()
        (Path(self.tmp.name) / "include" / "x.h").write_text(HEADER.replace("fn_710007e704", "fn_710007e708"),
                                                             encoding="utf-8")
        self.assertNotEqual(Labels(Path(self.tmp.name)).digest(), before)


def bl(at: int, target: int) -> bytes:
    return struct.pack("<I", 0x94000000 | (((target - at) >> 2) & 0x3FFFFFF))


class SpecialMismatches(unittest.TestCase):
    """check.special_mismatches: a call to a constructor that nothing names fails; one that resolves, an
    import's, and one the object defines itself do not."""

    def setUp(self):
        import check
        self.check = check
        self.at = 0x7100001000
        code = bl(self.at, 0x7100002000) + bl(self.at + 4, 0x7100003000) + bl(self.at + 8, 0x7100004000) \
            + bl(self.at + 12, 0x7100005000)
        self.original = SimpleNamespace(function=lambda a: code,
                                        plt_import=lambda t: "nn::Import::Import()" if t == 0x7100004000 else None)
        import asmdiff
        call = asmdiff.CALL26
        self.res = SimpleNamespace(address=self.at, relocs={
            0: SimpleNamespace(symbol="_ZN7UnknownC1Ev", type=call, addend=0, section=False),
            4: SimpleNamespace(symbol="_ZN5KnownC1Ev", type=call, addend=0, section=False),
            8: SimpleNamespace(symbol="_ZN2nn6ImportC1Ev", type=call, addend=0, section=False),
            12: SimpleNamespace(symbol="_ZN6InlineC2Ev", type=call, addend=0, section=False),
        })
        self.names = {"_ZN7UnknownC1Ev": "Unknown::Unknown()", "_ZN5KnownC1Ev": "Known::Known()"}

    def test_only_the_unnamed_call_fails(self):
        targets = {"_ZN5KnownC1Ev": 0x7100003000}.get
        out = self.check.special_mismatches(self.res, self.original, targets, self.names, None,
                                            defined={"_ZN6InlineC2Ev"})
        self.assertEqual([(m.offset, m.kind, m.actual) for m in out], [(0, "special", 0x7100002000)])
        self.assertIn("Unknown::Unknown", out[0].detail)


if __name__ == "__main__":
    unittest.main()
