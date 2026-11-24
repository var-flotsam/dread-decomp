"""tools/match_batch.py: request building, splicing a candidate into its file, the screen a candidate passes
before it is compiled, header trimming, reading results, per-model prices and the budget stop. Made-up sources
and a fake client: no network, no key, no game data, no compiler."""
import csv
import io
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.tests import _path  # noqa: F401
import match_batch as mb
import name_batch as nb

FILE = """\
// The foo unit.
#include "types.h"

struct UnkFoo {
    int count;  // 0x0
    int limit;  // 0x4
};

// Returns the count.
int fn_7100000100(UnkFoo* self) {
    // Keeps the pointer in its register.
    asm("" : "+r"(self));
    return *(int*)((char*)self + 0x4);
}

int CFoo::GetLimit(UnkFoo* self) {
    return self->limit;
}
"""

CLEAN = """\
int fn_7100000100(UnkFoo* self) {
    return self->limit;
}"""


def item(pool="clean", address="0x7100000100", path="src/game/CFoo/CFoo.cpp", **more):
    return {"pool": pool, "address": address, "path": path, "size": 16,
            "hits": [(12, "barrier"), (13, "offset-cast")], **more}


def ctx(name="fn_7100000100", text=FILE, **more):
    return {"name": name, "text": text, "sha": mb.sha(text), "notes": [], "diff": "", "listing": "0 ret",
            "barriers": [], "headers": [("include/types.h", "typedef int s32;\n")], **more}


class Splice(unittest.TestCase):
    def test_span_leaves_the_comment_above(self):
        s, e = mb.definition_span(FILE, "fn_7100000100")
        self.assertTrue(FILE[s:e].startswith("int fn_7100000100("))
        self.assertTrue(FILE[s:e].endswith("}"))
        self.assertIn("// Returns the count.\n", FILE[:s])

    def test_splice_replaces_only_the_function(self):
        new = mb.splice(FILE, "fn_7100000100", CLEAN)
        self.assertIn("// Returns the count.\nint fn_7100000100(UnkFoo* self) {\n    return self->limit;\n}\n", new)
        self.assertNotIn("asm", new)
        self.assertIn("int CFoo::GetLimit(UnkFoo* self) {\n    return self->limit;\n}", new)

    def test_qualified_name(self):
        s, e = mb.definition_span(FILE, "CFoo::GetLimit")
        self.assertTrue(FILE[s:e].startswith("int CFoo::GetLimit("))

    def test_missing_function(self):
        self.assertIsNone(mb.splice(FILE, "fn_7100000999", CLEAN))


class Screen(unittest.TestCase):
    def screen(self, code, pool="clean", name="fn_7100000100"):
        return mb.screen("src/game/CFoo/CFoo.cpp", FILE, name, code, pool, FILE)

    def test_clean_candidate_passes(self):
        text, why = self.screen(CLEAN)
        self.assertEqual(why, "")
        self.assertIn("return self->limit;", text)

    def test_clean_must_lose_the_form(self):
        text, why = self.screen("int fn_7100000100(UnkFoo* self) {\n    return *(int*)((char*)self + 0x4);\n}")
        self.assertIsNone(text)
        self.assertIn("offset-cast", why)

    def test_no_new_forbidden_form(self):
        for code, what in (
                ("int fn_7100000100(UnkFoo* self) {\n    if (!self) goto out;\nout:\n    return self->limit;\n}", "goto"),
                ("__attribute__((noinline)) int fn_7100000100(UnkFoo* self) {\n    return self->limit;\n}", "attribute"),
                ("int fn_7100000100(UnkFoo* self) {\n    int v;\n    memcpy(&v, &self->limit, 4);\n    return v;\n}",
                 "untyped store"),
                ("int fn_7100000100(UnkFoo* self) {\n    union { int a; } u; u.a = self->limit;\n    return u.a;\n}",
                 "union")):
            text, why = self.screen(code)
            self.assertIsNone(text, code)
            self.assertIn(what, why)

    def test_no_new_placeholder(self):
        text, why = self.screen("int fn_7100000100(UnkFoo* self) {\n    return fn_7100000abc(self);\n}")
        self.assertIsNone(text)
        self.assertIn("fn_7100000abc", why)

    def test_a_new_view_keeps_placeholder_names_and_needs_a_free_type_name(self):
        view = ("struct UnkFooView_7100000100 {\n    u8 pad_0x0[0x4];\n    int field_0x4;  // 0x4\n};\n\n"
                "int fn_7100000100(UnkFoo* self) {\n"
                "    return reinterpret_cast<UnkFooView_7100000100*>(self)->field_0x4;\n}")
        self.assertEqual(self.screen(view)[1], "")
        text, why = mb.screen("src/game/CFoo/CFoo.cpp", FILE, "fn_7100000100", view, "clean", FILE,
                              {"UnkFooView_7100000100"})
        self.assertIsNone(text)
        self.assertIn("already has: UnkFooView_7100000100", why)
        text, why = self.screen(view.replace("UnkFooView_7100000100", "UnkFoo"))
        self.assertIsNone(text)

    def test_must_define_the_function(self):
        text, why = self.screen("int Other(UnkFoo* self) {\n    return self->limit;\n}")
        self.assertIsNone(text)
        self.assertIn("does not define", why)

    def test_near_may_keep_what_it_had_and_add_none(self):
        kept = "int fn_7100000100(UnkFoo* self) {\n    asm(\"\" : \"+r\"(self));\n    return self->limit;\n}"
        self.assertEqual(self.screen(kept, pool="near")[1], "")
        more = "int CFoo::GetLimit(UnkFoo* self) {\n    return *(int*)((char*)self + 0x4);\n}"
        text, why = self.screen(more, pool="near", name="CFoo::GetLimit")
        self.assertIsNone(text)
        self.assertIn("steering", why)


HEADER = "#pragma once\n\nnamespace base {\n\n" + "".join(
    f"struct UnkPad{i} {{\n    int a;\n    int b;\n}};\n\n" for i in range(400)) + \
    "struct UnkFoo {\n    int count;\n};\n\nint fn_7100000200(UnkFoo* self);\n\n}  // namespace base\n"


class Headers(unittest.TestCase):
    def test_short_header_stays_whole(self):
        self.assertEqual(mb.trim_header("struct A { int a; };\n", {"B"}), ("struct A { int a; };\n", False))

    def test_long_header_keeps_what_is_used(self):
        self.assertGreater(len(HEADER), mb.HEADER_WHOLE)
        text, dropped = mb.trim_header(HEADER, {"UnkFoo"})
        self.assertTrue(dropped)
        self.assertIn("struct UnkFoo {\n    int count;\n};", text)
        self.assertIn("int fn_7100000200(UnkFoo* self);", text)
        self.assertNotIn("UnkPad7", text)
        self.assertIn("namespace base {", text)
        self.assertLess(len(text), 400)

    def test_headers_for_reads_the_tree(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "include").mkdir()
            (root / "src" / "a").mkdir(parents=True)
            (root / "include" / "big.h").write_text(HEADER, encoding="utf-8")
            (root / "src" / "a" / "local.h").write_text("struct UnkLocal { int x; };\n", encoding="utf-8")
            src = '#include "big.h"\n#include "local.h"\n#include <new>\n#include "gone.h"\nint f(UnkFoo* p);\n'
            got, notes = mb.headers_for("src/a/a.cpp", src, root=root)
            self.assertEqual([p for p, _ in got], ["include/big.h (trimmed to what the file uses)", "src/a/local.h"])
            self.assertEqual(notes, [])
            got, notes = mb.headers_for("src/a/a.cpp", src, root=root, budget=40)
            self.assertEqual([p for p, _ in got], ["src/a/local.h"])
            self.assertEqual(notes, ["include/big.h (no room)"])


class Requests(unittest.TestCase):
    def test_shape(self):
        reqs, meta = mb.build_requests([(item(), ctx())])
        p = reqs[0]["params"]
        self.assertEqual(reqs[0]["custom_id"], "cl-7100000100")
        self.assertEqual(p["model"], mb.MODEL)
        self.assertEqual(p["output_config"]["format"], {"type": "json_schema", "schema": mb.SCHEMA})
        self.assertEqual(p["output_config"]["effort"], "high")
        self.assertEqual(p["thinking"], {"type": "adaptive"})
        shared, task = p["messages"][0]["content"]
        self.assertIn("--- include/types.h ---", shared["text"])
        self.assertIn("--- src/game/CFoo/CFoo.cpp ---", shared["text"])
        self.assertIn("Task: CLEAN.", task["text"])
        self.assertIn("line 12: barrier; line 13: offset-cast", task["text"])
        self.assertIn("lines 10 to 14", task["text"])
        self.assertIn("Return 4 different candidates", task["text"])
        self.assertNotIn("cache_control", shared)     # one request: nothing shares the file block
        self.assertNotIn("cache_control", p["system"][0])
        self.assertEqual(meta[0]["sha"], mb.sha(FILE))
        self.assertIn("Never add one of these", p["system"][0]["text"])

    def test_same_file_requests_are_adjacent_and_share_a_cached_block(self):
        other = "int g(int x) {\n    return x;\n}\n"
        entries = [(item(address="0x7100000300"), ctx(name="CFoo::GetLimit")),
                   (item(pool="near", address="0x7100000200", path="drafts/a/fn_7100000200.cpp"),
                    ctx(name="g", text=other, diff="!!   1 cmp x1, x10    cmp x10, x1", differing=1)),
                   (item(), ctx())]
        reqs, _ = mb.build_requests(entries)
        self.assertEqual([r["custom_id"] for r in reqs], ["nm-7100000200", "cl-7100000100", "cl-7100000300"])
        blocks = [r["params"]["messages"][0]["content"][0] for r in reqs]
        self.assertNotIn("cache_control", blocks[0])
        self.assertEqual(blocks[1], blocks[2])
        self.assertIn("cache_control", blocks[1])
        self.assertIn("cache_control", reqs[0]["params"]["system"][0])
        task = reqs[0]["params"]["messages"][0]["content"][1]["text"]
        self.assertIn("Task: NEAR.", task)
        self.assertIn("cmp x10, x1", task)

    def test_output_cap_follows_the_function(self):
        self.assertEqual(mb.max_tokens_for(300, 4), 32_000)
        self.assertGreater(mb.max_tokens_for(9_000, 4), mb.max_tokens_for(3_000, 4))
        self.assertEqual(mb.max_tokens_for(900_000, 4), 48_000)

    def test_take(self):
        pool = list(range(100))
        self.assertEqual(mb.take(pool, 3, False), [0, 1, 2])
        self.assertEqual(mb.take(pool, 4, True), [0, 25, 50, 75])
        self.assertEqual(mb.take(pool, 0, True), pool)

    def test_diff_rows(self):
        left = [f"op{i}" for i in range(30)]
        right = list(left)
        right[20] = "other"
        text = mb.diff_rows(left, right, {20}, context=1)
        self.assertEqual(text.split("\n")[0], "   ...")
        self.assertIn("!!  20 op20", text)
        self.assertIn("    19 op19", text)
        self.assertNotIn("op17", text)


def succeeded(cid, data, stop="end_turn", model="claude-fable-5-2", out=1000):
    return {"custom_id": cid, "result": {"type": "succeeded", "message": {
        "stop_reason": stop, "model": model, "content": [{"type": "thinking", "thinking": ""},
                                                         {"type": "text", "text": json.dumps(data)}],
        "usage": {"input_tokens": 1000, "output_tokens": out}}}}


class Results(unittest.TestCase):
    def test_candidates(self):
        got, status = mb.candidates_of(succeeded("a", {"candidates": [{"code": CLEAN, "change": "typed\n access"},
                                                                     {"code": "  ", "change": "empty"}]}))
        self.assertEqual(status, "ok")
        self.assertEqual(got, [{"code": CLEAN, "change": "typed access"}])

    def test_refusal_truncation_and_errors_give_none(self):
        for r in (succeeded("a", {"candidates": []}, stop="refusal"),
                  succeeded("a", {"candidates": [{"code": "x", "change": ""}]}, stop="max_tokens"),
                  {"custom_id": "a", "result": {"type": "errored", "error": {"type": "invalid_request"}}},
                  succeeded("a", {"candidates": "none"}), succeeded("a", {"candidates": []})):
            self.assertEqual(mb.candidates_of(r)[0], [])


class Prices(unittest.TestCase):
    def test_each_model_at_its_own_rates(self):
        u = nb.usage_of(succeeded("a", {}, out=1_000_000))
        self.assertEqual(u["model"], "claude-fable-5-2")
        self.assertAlmostEqual(nb.dollars(u), (1000 * 8 + 1_000_000 * 40) / 1e6 * 0.5)
        u = nb.usage_of(succeeded("a", {}, model=nb.MODEL, out=1_000_000))
        self.assertAlmostEqual(nb.dollars(u), (1000 * 4 + 1_000_000 * 20) / 1e6 * 0.5)
        del u["model"]
        self.assertAlmostEqual(nb.dollars(u), (1000 * 4 + 1_000_000 * 20) / 1e6 * 0.5)

    def test_estimate_uses_the_request_model(self):
        fable, _ = mb.build_requests([(item(), ctx())])
        opus, _ = mb.build_requests([(item(), ctx())], model=nb.MODEL)
        self.assertGreater(nb.estimate(fable)["worst_dollars"], 1.9 * nb.estimate(opus)["worst_dollars"])
        unknown, _ = mb.build_requests([(item(), ctx())], model="claude-none-9")
        with self.assertRaises(SystemExit):
            nb.estimate(unknown)

    def test_unknown_model_is_billed_at_the_dearest_rates(self):
        self.assertEqual(nb.price_of("claude-none-9"), nb.PRICES["claude-fable-5-2"])


class FakeBatches:
    def __init__(self, results=()):
        self._results, self.created = list(results), []

    def create(self, requests):
        self.created.append(requests)
        return SimpleNamespace(id="msgbatch_test", processing_status="in_progress")

    def retrieve(self, bid):
        return SimpleNamespace(id=bid, processing_status="ended")

    def results(self, bid):
        return iter(self._results)


def fake_client(batches):
    return SimpleNamespace(messages=SimpleNamespace(batches=batches))


class Budget(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        state = Path(self.tmp.name) / "name_batch"
        self.patches = [mock.patch.object(nb, "STATE", state), mock.patch.object(nb, "LEDGER", state / "ledger.csv"),
                        mock.patch("sys.stdout", new_callable=io.StringIO)]
        for p in self.patches:
            p.start()

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def make_run(self, run):
        reqs, meta = mb.build_requests([(item(), ctx())])
        return nb.write_run(run, mb.KIND["clean"], reqs, meta, {"ttl": "5m"})

    def ledger(self, **row):
        nb.LEDGER.parent.mkdir(parents=True, exist_ok=True)
        new = not nb.LEDGER.exists()
        with nb.LEDGER.open("a", newline="", encoding="utf-8") as fh:
            w = csv.DictWriter(fh, fieldnames=nb.LEDGER_COLUMNS)
            if new:
                w.writeheader()
            w.writerow({**{c: "0" for c in nb.LEDGER_COLUMNS}, **row})

    def submit(self, run, batches, budget=180.0, cap=70.0):
        return mb.cmd_submit(SimpleNamespace(run=run, budget=budget, cap=cap), client=fake_client(batches))

    def test_refuses_past_the_match_cap(self):
        self.make_run("match-a")
        self.ledger(run="match-old", kind="match-near", batch_id="b1", dollars="69.99")
        batches = FakeBatches()
        self.assertEqual(self.submit("match-a", batches), 2)
        self.assertEqual(batches.created, [])
        self.assertFalse((nb.STATE / "match-a" / "batch.json").exists())

    def test_other_tools_spend_counts_against_the_budget_not_the_cap(self):
        self.make_run("match-a")
        self.ledger(run="prop-1", kind="propose", batch_id="b1", dollars="100")
        batches = FakeBatches()
        self.assertEqual(self.submit("match-a", batches, budget=100.1), 2)
        self.assertEqual(batches.created, [])
        self.assertEqual(self.submit("match-a", batches, budget=180.0), 0)
        self.assertEqual(len(batches.created), 1)

    def test_pending_match_batches_count_against_the_cap(self):
        self.make_run("match-a")
        self.make_run("match-b")
        batches = FakeBatches()
        self.assertEqual(self.submit("match-a", batches), 0)
        worst = json.loads((nb.STATE / "match-a" / "batch.json").read_text())["worst_dollars"]
        self.assertEqual(self.submit("match-b", batches, cap=worst * 1.5), 2)
        self.assertEqual(len(batches.created), 1)

    def test_run_names(self):
        with self.assertRaises(SystemExit):
            self.submit("prop-1", FakeBatches())

    def test_fetch_writes_a_ledger_row_at_the_model_rates(self):
        self.make_run("match-a")
        batches = FakeBatches([succeeded("cl-7100000100", {"candidates": [{"code": CLEAN, "change": "typed"}]})])
        self.submit("match-a", batches)
        self.assertEqual(mb.cmd_fetch(SimpleNamespace(run="match-a"), client=fake_client(batches)), 0)
        row = nb.ledger_rows()[0]
        self.assertEqual((row["run"], row["kind"], row["verdicts"]), ("match-a", "match-clean", "1"))
        self.assertAlmostEqual(float(row["dollars"]), (1000 * 8 + 1000 * 40) / 1e6 * 0.5, places=4)
        self.assertEqual(mb.asked_before("clean"), {0x7100000100})


if __name__ == "__main__":
    unittest.main()
