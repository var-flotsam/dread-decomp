"""tools/member_batch.py: finding view structs and their uses, the compiler's layouts, type compatibility, request
building, the spend cap, results by custom_id, what `agree` keeps, and the compiler-driven rename. Made-up sources,
a fake client and a fake compiler: no network, no key, no game data."""
import csv
import io
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.tests import _path  # noqa: F401
import member_batch as mb
import name_batch as nb
from tools.tests.test_name_batch import FakeBatches, fake_client, succeeded

VIEW = """\
// A made-up unit.
#include "types.h"

struct UnkFooView {
    void** vtable;
    u8 pad_0x8[8];
    f32 field_0x10;
    bool field_0x14;
    u8 field_0x18[8];
    struct Inner {
        int field_0x0;
    } field_0x20;
    void Touch() { field_0x14 = true; }
};

struct UnkOther {
    f32 field_0x10;
};

void fn_7100000100(UnkFooView* self) {
    self->field_0x10 = 1.0f;
}

float fn_7100000200(UnkOther* o) { return o->field_0x10; }
"""

DUMP = """\

*** Dumping AST Record Layout
         0 | struct UnkFooView::Inner
         0 |   int field_0x0
           | [sizeof=4, dsize=4, align=4,
           |  nvsize=4, nvalign=4]

*** Dumping AST Record Layout
         0 | struct UnkFooView
         0 |   void ** vtable
         8 |   u8 [8] pad_0x8
        16 |   f32 field_0x10
        20 |   _Bool field_0x14
        24 |   u8 [8] field_0x18
        32 |   struct UnkFooView::Inner field_0x20
        32 |     int field_0x0
           | [sizeof=40, dsize=40, align=8,
           |  nvsize=40, nvalign=8]

*** Dumping AST Record Layout
         0 | class base::global::CStrId
         0 |   class base::global::UnkStrIdBase (base)
         0 |     u64 m_hash
           | [sizeof=8, dsize=8, align=8,
           |  nvsize=8, nvalign=8]
"""

FIELDS = [
    {"class": "CFoo", "offset": "0x10", "name": "fSpeed", "type": "float"},
    {"class": "CFoo", "offset": "0x14", "name": "bActive", "type": "bool"},
    {"class": "CFoo", "offset": "0x18", "name": "sName", "type": "base::global::CStrId"},
    {"class": "CFoo", "offset": "0x20", "name": "iCount", "type": "int"},
    {"class": "CFooBase", "offset": "0xffffffff", "name": "bEnabled", "type": "bool"},
]
TYPES = [
    {"type": "CFoo", "kind": "class", "parent": "CFooBase", "size": "0x28", "getter": "0x7100000900"},
    {"type": "CFooBase", "kind": "class", "parent": "", "size": "0x8", "getter": ""},
    {"type": "CCharClassFoo", "kind": "class", "parent": "", "size": "0x30", "getter": "0x7100000a00"},
]


def refl():
    fields = FIELDS + [{"class": "CCharClassFoo", "offset": "0x10", "name": "fRange", "type": "float"}]
    return mb.Reflection(fields, TYPES)


class Root(unittest.TestCase):
    """A temporary tree as ROOT, and a temporary state directory."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        state = self.root / "build" / "name_batch"
        self.patches = [mock.patch.object(mb, "ROOT", self.root), mock.patch.object(nb, "STATE", state),
                        mock.patch.object(nb, "LEDGER", state / "ledger.csv"),
                        mock.patch.object(mb, "TABLE", self.root / "data" / "member_names.csv"),
                        mock.patch("sys.stdout", new_callable=io.StringIO)]
        for p in self.patches:
            p.start()
        self.write("src/game/CFoo/fn_7100000100.cpp", VIEW)

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def write(self, rel, text):
        p = self.root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(text.encode("utf-8"))


class Views(unittest.TestCase):
    def test_direct_placeholders_only(self):
        views = mb.find_views(VIEW)
        self.assertEqual([v["struct"] for v in views], ["UnkFooView", "UnkOther"])
        # the nested struct's member and the method body's use are not the view's own members
        self.assertEqual(views[0]["placeholders"], ["field_0x10", "field_0x14", "field_0x18", "field_0x20"])

    def test_functions_naming_counts_a_parameter(self):
        spans = mb.functions_naming(VIEW, "UnkOther")
        self.assertEqual(len(spans), 1)
        self.assertIn("fn_7100000200", VIEW[spans[0][0]:spans[0][1]])

    def test_layouts(self):
        lay = mb.parse_layouts(DUMP)
        mt = mb.member_types(lay, "UnkFooView")
        self.assertEqual(mt["field_0x10"], {"offset": 16, "type": "f32", "kind": "float", "size": 4, "record": ""})
        self.assertEqual((mt["field_0x14"]["kind"], mt["field_0x14"]["size"]), ("bool", 1))
        self.assertEqual((mt["field_0x18"]["kind"], mt["field_0x18"]["size"]), ("array:int:8", 8))
        self.assertEqual((mt["field_0x20"]["kind"], mt["field_0x20"]["size"]), ("record", 4))
        self.assertNotIn("field_0x0", mt)   # a nested member is not a direct one
        self.assertEqual(mb.record_sizes(lay)["CStrId"], 8)
        self.assertNotIn("m_hash", [f[2] for f in lay["base::global::CStrId"]["fields"]])   # bases are not members


class Compatible(unittest.TestCase):
    def test_kinds(self):
        r = refl()
        sizes = {"CStrId": 8, "CVector3D": 12}
        self.assertTrue(mb.compatible("float", "float", 4, r))
        self.assertFalse(mb.compatible("float", "int", 4, r))
        self.assertTrue(mb.compatible("bool", "bool", 1, r))
        self.assertTrue(mb.compatible("bool", "int", 1, r))
        self.assertFalse(mb.compatible("bool", "int", 4, r))
        self.assertTrue(mb.compatible("int", "int", 4, r))
        self.assertTrue(mb.compatible("base::snd::ESndType", "int", 4, r))
        self.assertTrue(mb.compatible("CActor*", "ptr", 8, r))
        self.assertTrue(mb.compatible("base::global::CStrId", "record", 8, r, "base::global::CStrId", sizes))
        self.assertTrue(mb.compatible("base::global::CStrId", "array:int:8", 8, r, "", sizes))   # opaque storage
        self.assertFalse(mb.compatible("base::global::CStrId", "array:int:16", 16, r, "", sizes))
        self.assertFalse(mb.compatible("base::global::CStrId", "int", 8, r, "", sizes))   # a scalar is not storage
        self.assertFalse(mb.compatible("base::global::CRntString", "array:int:8", 8, r, "", sizes))  # size unknown
        self.assertTrue(mb.compatible("base::math::CVector3D", "array:float:3", 12, r))
        self.assertFalse(mb.compatible("CGameLink<CActor>", "record", 8, r, "CGameLink<CEntity>"))


class Candidates(unittest.TestCase):
    def test_directory_char_class_and_getter(self):
        r = refl()
        got = mb.candidates("src/game/CFoo/x.cpp", "UnkFooView", ["fn_7100000a00();"], r)
        self.assertEqual(got, ["CFoo", "CCharClassFoo"])
        self.assertEqual(mb.candidates("src/game/fn_7100/x.cpp", "U", ["fn_7100000900();"], r), ["CFoo"])

    def test_prescore_and_pick(self):
        r = refl()
        self.assertEqual(mb.prescore(["field_0x10", "field_0x14", "field_0x99"], ["CFoo"], r), 2)
        rows = [{"path": "src/game/CFoo/a.cpp", "struct": "A", "score": 3},
                {"path": "src/game/CFoo/b.cpp", "struct": "B", "score": 2},
                {"path": "src/game/CFoo/c.cpp", "struct": "C", "score": 1},
                {"path": "src/game/fn_71/d.cpp", "struct": "D", "score": 9},
                {"path": "src/game/UnkTU1/e.cpp", "struct": "E", "score": 9},
                {"path": "src/game/CBar/f.cpp", "struct": "F", "score": 0}]
        self.assertEqual([x["struct"] for x in mb.pick(rows, 10)], ["A", "B"])
        self.assertEqual([x["struct"] for x in mb.pick(rows, 10, keep=lambda x: x["struct"] != "A")], ["B", "C"])


class Requests(Root):
    def build(self):
        return mb.build_propose([{"path": "src/game/CFoo/fn_7100000100.cpp", "struct": "UnkFooView"}], refl(),
                                layouts_for=lambda p: mb.parse_layouts(DUMP))

    def test_shape(self):
        reqs, meta = self.build()
        self.assertEqual(len(reqs), 1)
        p = reqs[0]["params"]
        self.assertEqual(p["model"], "claude-opus-5-5")
        self.assertEqual(p["thinking"], {"type": "adaptive"})
        self.assertEqual(p["output_config"]["format"]["schema"], mb.PROPOSE_SCHEMA)
        text = p["messages"][0]["content"][0]["text"]
        self.assertIn("0x0010  f32", text)            # the compiler's offsets
        self.assertIn("fSpeed", text)                 # the candidate's reflected members
        self.assertIn("(the whole file)", text)       # a short defining file is shown whole
        self.assertEqual(meta[0]["candidates"], ["CFoo", "CCharClassFoo"])
        self.assertEqual(meta[0]["members"]["field_0x14"]["offset"], 0x14)

    def test_review_sees_mapping_and_evidence_only(self):
        _, meta = self.build()
        prop = [{"custom_id": meta[0]["custom_id"], "path": meta[0]["path"], "struct": "UnkFooView",
                 "candidates": ["CFoo"], "placeholders": meta[0]["placeholders"], "class": "CFoo",
                 "reason": "SECRET REASONING",
                 "members": [{"member": "field_0x10", "reflected": "fSpeed", "evidence": "stores 1.0f"},
                             {"member": "field_0x18", "reflected": "", "evidence": ""}]}]
        reqs, rmeta = mb.build_review(prop, refl(), layouts_for=lambda p: mb.parse_layouts(DUMP))
        text = reqs[0]["params"]["messages"][0]["content"][0]["text"]
        self.assertIn("field_0x10 -> fSpeed; evidence: stores 1.0f", text)
        self.assertNotIn("SECRET REASONING", text)
        self.assertNotIn("field_0x18 ->", text)
        self.assertEqual(reqs[0]["params"]["output_config"]["format"]["schema"], mb.REVIEW_SCHEMA)
        self.assertEqual(mb.build_review([{**prop[0], "class": "none"}], refl(),
                                         layouts_for=lambda p: {})[0], [])


class CapAndFetch(Root):
    def make_run(self, run, kind=mb.KIND_PROPOSE):
        reqs, meta = mb.build_propose([{"path": "src/game/CFoo/fn_7100000100.cpp", "struct": "UnkFooView"}], refl(),
                                      layouts_for=lambda p: mb.parse_layouts(DUMP))
        return nb.write_run(run, kind, reqs, meta, {"ttl": "5m"})

    def ledger(self, *rows):
        nb.LEDGER.parent.mkdir(parents=True, exist_ok=True)
        with nb.LEDGER.open("w", newline="", encoding="utf-8") as fh:
            w = csv.DictWriter(fh, fieldnames=nb.LEDGER_COLUMNS)
            w.writeheader()
            for run, kind, dollars in rows:
                w.writerow({**{c: "0" for c in nb.LEDGER_COLUMNS}, "run": run, "kind": kind, "dollars": dollars})

    def test_own_cap_counts_only_member_runs(self):
        self.make_run("m1")
        self.ledger(("old", "review", "100"), ("m0", mb.KIND_PROPOSE, "39.99"))
        batches = FakeBatches()
        rc = mb.cmd_submit(SimpleNamespace(run="m1", cap=40.0, budget=180.0), client=fake_client(batches))
        self.assertEqual(rc, 2)
        self.assertEqual(batches.created, [])
        self.ledger(("old", "review", "100"), ("m0", mb.KIND_PROPOSE, "1"))
        self.assertEqual(mb.cmd_submit(SimpleNamespace(run="m1", cap=40.0, budget=180.0),
                                       client=fake_client(batches)), 0)
        self.assertEqual(len(batches.created), 1)

    def test_shared_budget_still_applies(self):
        self.make_run("m1")
        self.ledger(("old", "review", "179.99"))
        batches = FakeBatches()
        self.assertEqual(mb.cmd_submit(SimpleNamespace(run="m1", cap=40.0, budget=180.0),
                                       client=fake_client(batches)), 2)
        self.assertEqual(batches.created, [])

    def test_fetch_by_custom_id_and_ledger(self):
        info = self.make_run("m1")
        mb.cmd_submit(SimpleNamespace(run="m1", cap=40.0, budget=180.0), client=fake_client(FakeBatches()))
        cid = info["items"][0]["custom_id"]
        answer = {"class": "CFoo", "members": [{"member": "field_0x10", "reflected": "fSpeed", "evidence": "e"}],
                  "reason": "r"}
        results = [succeeded("other", answer), succeeded(cid, answer)]
        self.assertEqual(mb.cmd_fetch(SimpleNamespace(run="m1"), client=fake_client(FakeBatches(results))), 0)
        got = json.loads((nb.STATE / "m1" / "answers.json").read_text(encoding="utf-8"))
        self.assertEqual(got[0]["answer"]["class"], "CFoo")
        rows = nb.ledger_rows()
        self.assertEqual((rows[0]["run"], rows[0]["kind"]), ("m1", mb.KIND_PROPOSE))

    def test_refused_and_outside_schema(self):
        self.assertIsNone(mb.parse(succeeded("a", "{}", stop="refusal"), mb.KIND_PROPOSE)[0])
        self.assertIsNone(mb.parse(succeeded("a", {"class": "CFoo", "members": "x", "reason": ""}),
                                   mb.KIND_PROPOSE)[0])
        self.assertIsNone(mb.parse(succeeded("a", {"verdict": "right"}), mb.KIND_REVIEW)[0])


class Agree(unittest.TestCase):
    MT = mb.member_types(mb.parse_layouts(DUMP), "UnkFooView")

    def run_agree(self, members, votes, class_agree=True, cls="CFoo"):
        prop = {"custom_id": "p", "path": "src/game/CFoo/x.cpp", "struct": "UnkFooView", "class": cls,
                "candidates": ["CFoo", "CCharClassFoo"], "placeholders": list(self.MT), "members": members}
        rev = {"path": prop["path"], "struct": "UnkFooView", "answer": {
            "class_agree": class_agree, "class_reason": "", "members": [
                {"member": m, "agree": a, "reason": ""} for m, a in votes.items()]}}
        return mb.agree([prop], [rev], refl(), lambda p: self.MT, lambda p: {"CStrId": 8})

    def test_only_both_agree_exact_offset_and_compatible_type(self):
        members = [{"member": "field_0x10", "reflected": "fSpeed", "evidence": "stores 1.0f"},
                   {"member": "field_0x14", "reflected": "bActive", "evidence": "set true"},
                   {"member": "field_0x18", "reflected": "sName", "evidence": "copied whole"},
                   {"member": "field_0x20", "reflected": "iCount", "evidence": "counted"}]
        rows, left = self.run_agree(members, {"field_0x10": True, "field_0x14": False, "field_0x18": True,
                                              "field_0x20": True})
        self.assertEqual([(r["placeholder"], r["name"]) for r in rows],
                         [("field_0x10", "fSpeed"), ("field_0x18", "sName")])
        why = {x["placeholder"]: x["why"] for x in left}
        self.assertIn("disagrees with the member", why["field_0x14"])
        self.assertIn("type not compatible", why["field_0x20"])   # a struct, not an int

    def test_wrong_offset_and_invented_name(self):
        members = [{"member": "field_0x14", "reflected": "fSpeed", "evidence": "e"},
                   {"member": "field_0x10", "reflected": "fMadeUp", "evidence": "e"}]
        rows, left = self.run_agree(members, {"field_0x14": True, "field_0x10": True})
        self.assertEqual(rows, [])
        why = {x["placeholder"]: x["why"] for x in left}
        self.assertIn("offset differs", why["field_0x14"])
        self.assertIn("not a reflected member", why["field_0x10"])

    def test_class_disagreement_and_none(self):
        members = [{"member": "field_0x10", "reflected": "fSpeed", "evidence": "e"}]
        self.assertEqual(self.run_agree(members, {"field_0x10": True}, class_agree=False)[0], [])
        self.assertEqual(self.run_agree(members, {"field_0x10": True}, cls="none"), ([], []))


class Rename(Root):
    PATH = "src/game/CFoo/fn_7100000100.cpp"

    def test_rename_in_struct_only(self):
        out = mb.rename_in_struct(VIEW, "UnkFooView", {"field_0x10": "fSpeed"})
        self.assertIn("    f32 fSpeed;\n    bool field_0x14;", out)
        self.assertIn("struct UnkOther {\n    f32 field_0x10;", out)   # another struct's member stays
        self.assertIn("self->field_0x10 = 1.0f;", out)                # uses wait for the compiler
        self.assertIsNone(mb.rename_in_struct(VIEW, "UnkFooView", {"field_0x99": "x"}))

    def fake_compiler(self, unrelated=False):
        """Reports each `self->field_0x10` left in the file, as clang would."""
        def errors(paths):
            text = (self.root / self.PATH).read_text(encoding="utf-8")
            out = []
            for i, line in enumerate(text.split("\n"), 1):
                col = line.find("self->field_0x10")
                if col >= 0:
                    out.append((self.PATH, i, col + 7, "no member named 'field_0x10' in 'UnkFooView'"))
            if unrelated:
                out.append((self.PATH, 1, 1, "unknown type name 'Bogus'"))
            return out
        return errors

    def test_uses_follow_the_compiler(self):
        ok, why, originals = mb.apply_group(self.PATH, "UnkFooView", {"field_0x10": "fSpeed"}, [self.PATH],
                                            errors=self.fake_compiler())
        self.assertTrue(ok, why)
        text = (self.root / self.PATH).read_text(encoding="utf-8")
        self.assertIn("self->fSpeed = 1.0f;", text)
        self.assertIn("return o->field_0x10;", text)
        self.assertEqual(originals[self.PATH], VIEW)

    def test_anything_else_puts_it_back(self):
        ok, why, _ = mb.apply_group(self.PATH, "UnkFooView", {"field_0x10": "fSpeed"}, [self.PATH],
                                    errors=self.fake_compiler(unrelated=True))
        self.assertFalse(ok)
        self.assertIn("unrelated", why)
        self.assertEqual((self.root / self.PATH).read_text(encoding="utf-8"), VIEW)

    def test_error_naming_another_type_puts_it_back(self):
        def errors(paths):
            return [(self.PATH, 24, 40, "no member named 'field_0x10' in 'UnkOther'")]
        ok, why, _ = mb.apply_group(self.PATH, "UnkFooView", {"field_0x10": "fSpeed"}, [self.PATH], errors=errors)
        self.assertFalse(ok)
        self.assertEqual((self.root / self.PATH).read_text(encoding="utf-8"), VIEW)

    def test_table_round_trip(self):
        row = {"path": self.PATH, "struct": "UnkFooView", "placeholder": "field_0x10", "name": "fSpeed",
               "class": "CFoo", "offset": "0x10", "reflected_type": "float", "view_type": "f32", "evidence": "e"}
        mb.TABLE.parent.mkdir(parents=True, exist_ok=True)
        mb.write_table([row])
        self.assertEqual(mb.read_table(), [row])


if __name__ == "__main__":
    unittest.main()
