"""tools/progress.py --quality: the name figures, generated code that reads as source, and data/family_reads.csv
(made-up sources, no game data)."""
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import progress


def found(code: str) -> list[str]:
    return progress.placeholders(progress.code_only(code))


class Tokens(unittest.TestCase):
    def test_each_placeholder_form(self):
        self.assertEqual(found("fn_7100123456(); UnkStruct_0x40 a; p->field_0x18 = 1; x = unk_710b372d68; "
                               "y = p->unk_0x8; g_unk_7101d47108 = 0; p->vf_0x48(); q->vf_12();"),
                         ["fn_7100123456", "UnkStruct_0x40", "field_0x18", "unk_710b372d68", "unk_0x8",
                          "g_unk_7101d47108", "vf_0x48", "vf_12"])

    def test_names_that_are_not_placeholders(self):
        self.assertEqual(found("f(UnkNoArg()); Unknown u; refl_GetClass(); s.pad_0x8; field_0x18_count = 1; "
                               "vf_OnHit(); myfn_7100123456(); CUnkFoo c; DoFn_710012345a();"), [])

    def test_comments_strings_and_preprocessor_lines_do_not_count(self):
        self.assertEqual(found('#include "base/reflection/UnkClassGetters.h"\n'
                               "// calls fn_7100123456\n/* UnkStruct_0x40\n field_0x8 */\n"
                               'const char* s = "g_unk_7101d47108"; char c = \'x\';\nint ok;'), [])

    def test_loose_allows_references_to_other_functions_only(self):
        code = progress.code_only("fn_7100123456(a); atexit(fn_7100abcdef); p->vf_0x10(); UnkFoo::fn_7100fedcba(p); "
                                  "UnkBar(x); p->field_0x8; g_unk_7101d47108;")
        self.assertEqual(progress.placeholders(code, loose=True), ["UnkBar", "field_0x8", "g_unk_7101d47108"])


SOURCE = """\
#include "game/Thing.h"

// the original calls fn_7100000001 here
void CThing::Update(CActor* actor) {
    if (actor->IsAlive()) {
        fn_7100000002(actor);
    }
}

void CThing::Reset() { m_count = 0; }

u32 CThing::Count() const;

CThing::CThing(int n) : m_count{n}, m_owner(nullptr) { m_flags = 0; }

void CThing::Poke(CActor* actor) { actor->field_0x18 = 1; }

void fn_7100000003(CThing* self) { self->Reset(); }
"""


class Definitions(unittest.TestCase):
    def setUp(self):
        self.code = progress.code_only(SOURCE)

    def test_finds_multi_line_one_line_and_constructor_definitions(self):
        self.assertEqual(len(progress.definitions(self.code, "Update")), 1)
        self.assertIn("m_count = 0", progress.definitions(self.code, "Reset")[0][1])
        sig, body = progress.definitions(self.code, "CThing")[0]
        self.assertTrue(body.startswith("{ m_flags"))
        self.assertEqual(progress.definitions(self.code, "Count"), [])   # a declaration

    def test_loose_and_strict_on_the_function_s_own_text(self):
        self.assertEqual(progress.names_of(self.code, "CThing::Update"), (True, False, False))   # a placeholder callee
        self.assertEqual(progress.names_of(self.code, "CThing::Reset"), (True, True, False))
        self.assertEqual(progress.names_of(self.code, "CThing::Poke"), (False, False, False))    # a placeholder member
        self.assertEqual(progress.names_of(self.code, "fn_7100000003"), (False, False, False))   # its own name

    def test_by_file_when_the_definition_is_not_found(self):
        self.assertEqual(progress.names_of(self.code, "CThing::Missing"), (False, False, True))
        clean = progress.code_only("#define F(x) void x() {}\nF(Go)\n")
        self.assertEqual(progress.names_of(clean, "Go"), (True, True, True))

    def test_first_reason_loose_fails(self):
        code = progress.code_only(SOURCE + "void UnkHolder::Run() {}\nvoid Go(UnkArg* a) {}\n"
                                  "void Load(CThing* t) { g_unk_7101d47108 = t; }\n"
                                  "void Make() { UnkLocal x; }\nvoid Both(CThing* t) { t->field_0x8 = g_unk_7101d47108; }\n")
        reason = lambda n: progress.loose_reason(code, n)  # noqa: E731
        self.assertEqual(reason("fn_7100000003"), "own_name")
        self.assertEqual(reason("base::CClass::CClass<UnkRefl_fn_7100e7e428>"), "own_name")   # template arguments
        self.assertEqual(reason("UnkHolder::Run"), "own_class")
        self.assertEqual(reason("UnkHolder::~UnkHolder"), "own_class")   # named for its class
        self.assertEqual(reason("Go"), "signature_type")
        self.assertEqual(reason("CThing::Poke"), "member")
        self.assertEqual(reason("Both"), "member")                       # the first reason in the order
        self.assertEqual(reason("Load"), "global")
        self.assertEqual(reason("Make"), "body_type")
        self.assertIsNone(reason("CThing::Update"))                      # a placeholder callee is allowed

    def test_short_name(self):
        self.assertEqual(progress.short_name("bool base::C<int, ns::T>::Find(int) const"), "Find")
        self.assertEqual(progress.short_name("fn_7100000003"), "fn_7100000003")
        self.assertEqual(progress.short_name("CThing::~CThing"), "~CThing")


class FamilyReads(unittest.TestCase):
    def write(self, root: Path, rows: list[str]) -> Path:
        p = root / "family_reads.csv"
        p.write_text(",".join(progress.FAMILY_COLUMNS) + "\n" + "".join(r + "\n" for r in rows), encoding="utf-8")
        return p

    def test_the_tracked_file_has_the_header_only(self):
        self.assertEqual(progress.FAMILY_READS.read_text(encoding="utf-8").splitlines(),
                         [",".join(progress.FAMILY_COLUMNS)])

    def test_latest_read_of_a_family_decides(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root / "list.txt").write_text("# sampled\nsrc/tables/a.cpp\n0x7100000010\n", encoding="utf-8")
            p = self.write(root, [
                "reflection,src/reflection/*,abc123,2026-11-20,20,pass,reader,",
                "lifted,src/lifted/*,abc123,2026-11-20,20,pass,reader,",
                "lifted,src/lifted/*,def456,2026-11-21,20,fail,reader,two read as machine code",
                "tables,list.txt,abc123,2026-11-21,20,pass,reader,"])
            scopes = progress.passed_scopes(progress.read_family_reads(p), root)
        self.assertTrue(progress.covered(scopes, "src/reflection/x/y.cpp", 1))
        self.assertFalse(progress.covered(scopes, "src/lifted/z.cpp", 1))             # its later read failed
        self.assertTrue(progress.covered(scopes, "src/tables/a.cpp", 1))
        self.assertTrue(progress.covered(scopes, "src/tables/b.cpp", 0x7100000010))   # by address
        self.assertFalse(progress.covered(scopes, "src/tables/b.cpp", 0x7100000011))

    def test_no_reads_cover_nothing(self):
        self.assertEqual(progress.passed_scopes([]), [])


class Report(unittest.TestCase):
    """names_report over a made-up cache and tree."""

    def test_rows(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root / "src" / "game").mkdir(parents=True)
            (root / "src" / "reflection").mkdir(parents=True)
            (root / "src" / "game" / "Thing.cpp").write_text(SOURCE, encoding="utf-8")
            (root / "src" / "reflection" / "Clean.cpp").write_text("// fn_7100000001\nvoid CA::refl_Get() {}\n",
                                                                     encoding="utf-8")
            (root / "src" / "reflection" / "Dirty.cpp").write_text("void fn_7100000020() {}\n", encoding="utf-8")
            reads = root / "family_reads.csv"
            reads.write_text(",".join(progress.FAMILY_COLUMNS) + "\n", encoding="utf-8")
            rows = [{"address": f"{a:x}", "size": "16"} for a in range(0x10, 0x16)]
            rows[0].update(name="CThing::Update", name_source="rtti")
            rows[1].update(name="CThing::Reset", name_source="inferred")
            progress._CACHE_OBJECTS["test"] = [
                (Path("game/Thing.o.diff.json"), [{"address": 0x10, "name": "CThing::Update"},
                                                  {"address": 0x11, "name": "CThing::Reset"},
                                                  {"address": 0x12, "name": "CThing::Poke"}]),
                (Path("reflection/Clean.o.diff.json"), [{"address": 0x13, "name": "CA::refl_Get"},
                                                        {"address": 0x13, "name": "CA::refl_Get"}]),
                (Path("reflection/Dirty.o.diff.json"), [{"address": 0x14, "name": "fn_7100000020"},
                                                        {"address": 0x15, "name": "fn_7100000021"}])]
            qa = {0x10: ("complete", False), 0x11: ("complete", False), 0x12: ("complete", False),
                  0x13: ("match_generated", False), 0x14: ("match_generated", False), 0x15: ("unproven", False)}
            old = progress.ROOT
            progress.ROOT = root
            try:
                n = progress.names_report("test", rows, qa, reads)
                reads.write_text(",".join(progress.FAMILY_COLUMNS) + "\nrefl,src/reflection/Clean.cpp,abc,2026-11-23,"
                                 "20,pass,reader,\n", encoding="utf-8")
                passed = progress.names_report("test", rows, qa, reads)
            finally:
                progress.ROOT = old
                del progress._CACHE_OBJECTS["test"]
        self.assertEqual(n["loose"], {"functions": 2, "bytes": 32})
        self.assertEqual(n["strict"], {"functions": 1, "bytes": 16})
        self.assertEqual(n["generated"]["reflection"]["matching"]["functions"], 2)
        self.assertEqual(n["generated"]["reflection"]["clean"], {"functions": 1, "bytes": 16})
        self.assertEqual(n["read_passed"]["functions"], 0)
        self.assertEqual(passed["read_passed"], {"functions": 1, "bytes": 16})
        self.assertEqual(passed["generated"]["reflection"]["clean"]["functions"], 0)
        self.assertEqual(n["named_sourced"], {"functions": 1, "bytes": 16})
        self.assertEqual(n["named_inferred"], {"functions": 1, "bytes": 16})
        self.assertEqual(n["named_unlisted"]["functions"], 0)
        self.assertEqual(progress.named(n), {"functions": 2, "bytes": 32})
        q = {k: {"functions": 0, "bytes": 0} for k in progress.QUALITIES}
        q["complete"] = {"functions": 2, "bytes": 32}
        self.assertEqual(progress.headline(q, passed), {"functions": 3, "bytes": 48})
        self.assertIn("matching and clean in form **12.50%** (3 functions;",
                      progress.quality_line(q, {"bytes": 384}, passed))
        line = progress.names_line(passed, q, {"bytes": 384})
        self.assertTrue(line.startswith("Names, whole binary"))
        self.assertIn("named **8.33%** (2 functions) = own name from a source 4.17% (1) + own name inferred 4.17% (1)",
                      line)
        self.assertNotIn("readable and matching", line.lower())
        self.assertIn("Names are a floor a tool can see, not a reading.", line)
        self.assertIn("**8.33%** (2 functions) = from a source 4.17% (1) + inferred 4.17% (1)",
                      progress.named_line(passed, {"bytes": 384}))

    def test_name_tiers(self):
        self.assertEqual(progress.name_tier({"name": "CThing::Update", "name_source": "reflection"}), "named_sourced")
        self.assertEqual(progress.name_tier({"name": "nn::os::Lock", "name_source": "library"}), "named_sourced")
        self.assertEqual(progress.name_tier({"name": "CStrId_Construct", "name_source": "inferred"}), "named_inferred")
        # the source spells a real name, the function list does not: shown apart, not counted as named
        self.assertEqual(progress.name_tier({"name": "fn_7100000010", "name_source": "unknown"}), "named_unlisted")
        self.assertEqual(progress.name_tier({"name": "fn_7100000010", "name_source": "inferred"}), "named_unlisted")
        self.assertEqual(progress.name_tier({"name": "CThing::Reset", "name_source": "unknown"}), "named_unlisted")

    def test_handoff_writes_the_line_and_keeps_it_when_not_computed(self):
        overall = {"functions": 1, "bytes": 100, "matching_bytes_pct": 1.0, "usable_bytes_pct": 1.0,
                   "by_status": {s: {"functions": 0, "bytes": 0} for s in progress.STATUSES}}
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "HANDOFF.md"
            p.write_text(f"top\n{progress.BEGIN}\nold\n{progress.END}\nbottom\n", encoding="utf-8")
            progress.update_handoff(p, overall, None, "Completeness, whole binary x", None,
                                    "Names, whole binary y")
            progress.update_handoff(p, overall, None)
            text = p.read_text(encoding="utf-8")
            self.assertIn("\nCompleteness, whole binary x\n\nNames, whole binary y\n", text)
            # a names line written before 2026-11-23 is kept too, until a run with the cache replaces it
            p.write_text(f"top\n{progress.BEGIN}\nReadable and matching, whole binary z\n{progress.END}\n",
                         encoding="utf-8")
            progress.update_handoff(p, overall, None)
            self.assertIn("\nReadable and matching, whole binary z\n", p.read_text(encoding="utf-8"))


if __name__ == "__main__":
    unittest.main()
