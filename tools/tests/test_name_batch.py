"""tools/name_batch.py: request building, bounded callers, verdict parsing, the budget stop, results out of
order and what apply chooses. Made-up sources and a fake client: no network, no key, no game data."""
import csv
import io
import json
import re
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.tests import _path  # noqa: F401
import name_batch as nb


class FakeTree:
    """Tree's interface over a dict of made-up files."""

    def __init__(self, files, rev="origin/claude/test"):
        self.files, self.rev = files, rev

    def read(self, path):
        return self.files[path]

    def grep(self, word, paths=("src",), generated=False):
        pat = re.compile(rf"(?<!\w){re.escape(word)}(?!\w)")
        return sorted(p for p, t in self.files.items() if p.startswith(tuple(paths)) and pat.search(t)
                      and (generated or not p.startswith(nb.GENERATED)))

    def ls(self, directory):
        return sorted(p for p in self.files if p.startswith(directory + "/"))


FOO = """\
// The foo unit.
#include "types.h"

struct UnkFoo {
    int count;  // 0x0
};

namespace {
int Helper(int x) { return x + 1; }
}

// Returns the count.
int CFoo_GetCount(UnkFoo* self) {
    if (self) {
        return self->count;
    }
    return 0;
}

#define GETTER(name, value) \\
    int name() { return value; }

GETTER(CFoo_GetSeven, 7)

int CFoo_Twice(UnkFoo* self) { return CFoo_GetCount(self) * 2; }
"""

BAR = """\
#include "types.h"
struct UnkFoo;
int CFoo_GetCount(UnkFoo* self);

__attribute__((flatten)) void CBar_Use(UnkFoo* foo) {
    int n = CFoo_GetCount(foo);
    (void)n;
}
"""


def members(cls):
    return f"{cls}: 0x10 bytes\n  0x0000  iCount  int" if cls == "CFoo" else ""


def items(*specs):
    return [{"address": a, "name": n, "lookup": n, "class": nb.class_of(n), "evidence": f"facts about {n}",
             "rev": "r"} for a, n in specs]


class CppText(unittest.TestCase):
    def test_definition_body(self):
        s, e = nb.find_definition(FOO, "CFoo_GetCount")
        text = FOO[s:e]
        self.assertTrue(text.rstrip().endswith("}"))
        self.assertIn("return self->count;", text)
        self.assertEqual(text.count("{"), text.count("}"))

    def test_declaration_is_not_a_definition(self):
        self.assertIsNone(nb.find_definition(BAR, "CFoo_GetCount"))

    def test_macro_definition(self):
        s, e = nb.find_definition(FOO, "CFoo_GetSeven")
        self.assertEqual(FOO[s:e].strip(), "GETTER(CFoo_GetSeven, 7)")

    def test_namespace_definition(self):
        self.assertIsNotNone(nb.find_definition(FOO, "Helper"))

    def test_callers_are_whole_functions(self):
        spans = nb.callers_in(BAR, "CFoo_GetCount")
        self.assertEqual(len(spans), 1)
        text = BAR[spans[0][0]:spans[0][1]]
        self.assertTrue(text.rstrip().endswith("}"))
        self.assertEqual(nb.function_label(BAR, *spans[0]), "CBar_Use")

    def test_comment_braces_do_not_count(self):
        src = "// {\nint F() { /* } */ return G(); }\n"
        (s, e), = nb.callers_in(src, "G")
        self.assertIn("return G();", src[s:e])

    def test_class_of(self):
        self.assertEqual(nb.class_of("CFoo_GetCount"), "CFoo")
        self.assertEqual(nb.class_of("game::CFoo::Update"), "CFoo")
        self.assertEqual(nb.class_of("fn_7100000100", "src/game/CBar/fn_7100000100.cpp"), "CBar")


class BoundedCallers(unittest.TestCase):
    def tree(self, n_callers, long_caller=False):
        files = {"src/game/CFoo/CFoo.cpp": FOO}
        for i in range(n_callers):
            files[f"src/game/CUser{i}/u.cpp"] = f"int CUser{i}_Call(UnkFoo* f) {{\n    return CFoo_GetCount(f) + {i};\n}}\n"
        if long_caller:
            body = "    x += 1;\n" * (nb.CALLER_MAX // 10)
            files["src/game/CBig/big.cpp"] = f"void CBig_Run(UnkFoo* f) {{\n    int x = CFoo_GetCount(f);\n{body}}}\n"
        files["src/lifted/lifted_000.cpp"] = "void lifted() { CFoo_GetCount(0); }\n"
        return FakeTree(files)

    def test_at_most_six_and_notes(self):
        ctx = nb.gather(self.tree(8, long_caller=True), "CFoo_GetCount")
        self.assertEqual(ctx["def_path"], "src/game/CFoo/CFoo.cpp")
        self.assertTrue(ctx["def_whole"])
        self.assertEqual(len(ctx["callers"]), nb.MAX_CALLERS)
        self.assertEqual(ctx["callers_in_def_file"], 1)  # CFoo_Twice, shown with its file
        notes = " ".join(ctx["notes"])
        self.assertIn("2 more calling function(s) were left out", notes)
        self.assertIn("longer than", notes)
        self.assertIn("CBig_Run", notes)
        for c in ctx["callers"]:
            self.assertTrue(c["text"].rstrip().endswith("}"), c["label"])
            self.assertEqual(c["text"].count("{"), c["text"].count("}"))
        self.assertFalse(any(c["path"].startswith("src/lifted/") for c in ctx["callers"]))

    def test_long_defining_file_is_cut_to_the_function(self):
        files = {"src/game/CFoo/CFoo.cpp": FOO + "\n// padding\n" * (nb.DEF_FILE_MAX // 10)}
        ctx = nb.gather(FakeTree(files), "CFoo_GetCount")
        self.assertFalse(ctx["def_whole"])
        self.assertTrue(ctx["def_text"].startswith("// Returns the count."))
        self.assertIn("only the function itself is shown", " ".join(ctx["notes"]))


class Requests(unittest.TestCase):
    def build(self, kind="review", ttl="5m"):
        tree = FakeTree({"src/game/CFoo/CFoo.cpp": FOO, "src/game/CBar/CBar.cpp": BAR})
        its = items(("0x7100000300", "CFoo_Twice"), ("0x7100000200", "CBar_Use"), ("0x7100000100", "CFoo_GetCount"))
        return nb.build_requests(kind, its, lambda it: tree, members, ttl)

    def test_shape(self):
        reqs, meta = self.build()
        self.assertEqual([r["custom_id"] for r in reqs], ["rev-7100000200", "rev-7100000100", "rev-7100000300"])
        for r in reqs:
            p = r["params"]
            self.assertEqual(p["model"], "claude-opus-5-5")
            self.assertEqual(p["thinking"], {"type": "adaptive"})
            self.assertEqual(p["output_config"]["effort"], nb.EFFORT)
            self.assertEqual(p["output_config"]["format"]["type"], "json_schema")
            self.assertNotIn("tool_choice", p)
            self.assertNotIn("fallbacks", p)
            self.assertEqual(p["messages"][-1]["role"], "user")  # no assistant prefill
            self.assertEqual(len(p["messages"]), 1)
            self.assertIn("worse than keeping the placeholder", p["system"][0]["text"])
            self.assertIn("cache_control", p["system"][0])
            tail = p["messages"][0]["content"][-1]
            self.assertNotIn("cache_control", tail)  # what differs per function is after the last breakpoint
        # A class's requests are adjacent and share one cached block; a class of one gets no breakpoint.
        foo = [r for r in reqs if "CFoo" in r["params"]["messages"][0]["content"][1]["text"]]
        self.assertEqual(foo[0]["params"]["messages"][0]["content"][0], foo[1]["params"]["messages"][0]["content"][0])
        self.assertIn("cache_control", foo[0]["params"]["messages"][0]["content"][0])
        self.assertIn("iCount", foo[0]["params"]["messages"][0]["content"][0]["text"])
        bar = reqs[0]["params"]["messages"][0]["content"][0]
        self.assertNotIn("cache_control", bar)
        breakpoints = sum("cache_control" in b for b in reqs[1]["params"]["system"]
                          + reqs[1]["params"]["messages"][0]["content"])
        self.assertLessEqual(breakpoints, 4)
        self.assertEqual({m["custom_id"] for m in meta}, {r["custom_id"] for r in reqs})

    def test_review_sees_only_name_and_evidence(self):
        reqs, _ = self.build()
        tail = reqs[1]["params"]["messages"][0]["content"][-1]["text"]
        self.assertIn("Proposed name: CFoo_GetCount", tail)
        self.assertIn("Stated evidence: facts about CFoo_GetCount", tail)
        self.assertNotIn("verdict", tail.lower())

    def test_propose_and_ttl(self):
        reqs, _ = self.build("propose", ttl="1h")
        p = reqs[1]["params"]
        self.assertEqual(p["output_config"]["format"]["schema"], nb.PROPOSE_SCHEMA)
        self.assertEqual(p["system"][0]["cache_control"], {"type": "ephemeral", "ttl": "1h"})
        self.assertIn("decline", p["messages"][0]["content"][-1]["text"])
        reqs, _ = self.build(ttl="off")
        self.assertFalse(any("cache_control" in json.dumps(r) for r in reqs))

    def test_estimate(self):
        reqs, _ = self.build()
        est = nb.estimate(reqs)
        self.assertEqual(est["requests"], 3)
        self.assertGreater(est["worst_dollars"], est["likely_dollars"])
        self.assertGreater(est["input_tokens"], 0)


def succeeded(cid, payload, stop="end_turn", usage=None):
    return {"custom_id": cid, "result": {"type": "succeeded", "message": {
        "stop_reason": stop, "content": [{"type": "thinking", "thinking": ""},
                                         {"type": "text", "text": payload if isinstance(payload, str)
                                          else json.dumps(payload)}],
        "usage": usage or {"input_tokens": 100, "cache_creation_input_tokens": 0, "cache_read_input_tokens": 0,
                           "output_tokens": 50}}}}


class Parsing(unittest.TestCase):
    def test_ok(self):
        data, status = nb.parse_result(succeeded("a", {"verdict": "right", "evidence_true": True, "reason": "x"}),
                                       "review")
        self.assertEqual((data["verdict"], status), ("right", "ok"))

    def test_refusal_and_truncation_are_no_verdict(self):
        for stop in ("refusal", "max_tokens"):
            data, status = nb.parse_result(succeeded("a", '{"verdict": "ri', stop=stop), "review")
            self.assertIsNone(data)
            self.assertIn(stop, status)

    def test_errors_and_bad_answers(self):
        self.assertEqual(nb.parse_result({"custom_id": "a", "result": {"type": "errored", "error": {
            "type": "invalid_request"}}}, "review"), (None, "errored: invalid_request"))
        self.assertEqual(nb.parse_result({"custom_id": "a", "result": {"type": "expired"}}, "review")[1], "expired")
        self.assertIsNone(nb.parse_result(succeeded("a", "not json"), "review")[0])
        self.assertIsNone(nb.parse_result(succeeded("a", {"verdict": "maybe", "evidence_true": True,
                                                          "reason": ""}), "review")[0])
        self.assertEqual(nb.parse_result(succeeded("a", {"decision": "decline", "name": "", "evidence": "e",
                                                         "reason": "r"}), "propose")[1], "ok")


class FakeBatches:
    def __init__(self, results=(), status="ended"):
        self._results, self.status, self.created = list(results), status, []

    def create(self, requests):
        self.created.append(requests)
        return SimpleNamespace(id="msgbatch_test", processing_status="in_progress")

    def retrieve(self, bid):
        return SimpleNamespace(id=bid, processing_status=self.status, request_counts=SimpleNamespace(
            processing=0, succeeded=len(self._results), errored=0, canceled=0, expired=0))

    def results(self, bid):
        return iter(self._results)


def fake_client(batches):
    return SimpleNamespace(messages=SimpleNamespace(batches=batches))


class StateDir(unittest.TestCase):
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

    def make_run(self, run, kind="review", n=3):
        tree = FakeTree({"src/game/CFoo/CFoo.cpp": FOO})
        its = items(*[(f"0x71000001{i:02x}", f"CFoo_Name{i}") for i in range(n)])
        reqs, meta = nb.build_requests(kind, its, lambda it: tree, members)
        return nb.write_run(run, kind, reqs, meta, {"ttl": "5m"})


class Budget(StateDir):
    def test_refuses_past_budget(self):
        self.make_run("r1")
        nb.LEDGER.parent.mkdir(parents=True, exist_ok=True)
        with nb.LEDGER.open("w", newline="", encoding="utf-8") as fh:
            w = csv.DictWriter(fh, fieldnames=nb.LEDGER_COLUMNS)
            w.writeheader()
            w.writerow({**{c: "0" for c in nb.LEDGER_COLUMNS}, "run": "old", "dollars": "179.99"})
        batches = FakeBatches()
        rc = nb.cmd_submit(SimpleNamespace(run="r1", budget=180.0), client=fake_client(batches))
        self.assertEqual(rc, 2)
        self.assertEqual(batches.created, [])
        self.assertFalse((nb.STATE / "r1" / "batch.json").exists())

    def test_pending_batches_count(self):
        self.make_run("r1")
        self.make_run("r2")
        batches = FakeBatches()
        self.assertEqual(nb.cmd_submit(SimpleNamespace(run="r1", budget=180.0), client=fake_client(batches)), 0)
        worst = json.loads((nb.STATE / "r1" / "batch.json").read_text())["worst_dollars"]
        rc = nb.cmd_submit(SimpleNamespace(run="r2", budget=worst * 1.5), client=fake_client(batches))
        self.assertEqual(rc, 2)
        self.assertEqual(len(batches.created), 1)
        sent = batches.created[0]
        self.assertEqual([r["custom_id"] for r in sent], [r["custom_id"] for r in
                                                          nb.read_jsonl(nb.STATE / "r1" / "requests.jsonl")])

    def test_no_double_submit(self):
        self.make_run("r1")
        batches = FakeBatches()
        nb.cmd_submit(SimpleNamespace(run="r1", budget=180.0), client=fake_client(batches))
        self.assertEqual(nb.cmd_submit(SimpleNamespace(run="r1", budget=180.0), client=fake_client(batches)), 1)


class FetchAndApply(StateDir):
    def fetch(self, results):
        nb.cmd_submit(SimpleNamespace(run="r1", budget=180.0), client=fake_client(FakeBatches()))
        return nb.cmd_fetch(SimpleNamespace(run="r1"), client=fake_client(FakeBatches(results)))

    def test_out_of_order_results(self):
        info = self.make_run("r1")
        ids = [m["custom_id"] for m in info["items"]]
        answers = {ids[0]: ("right", True), ids[1]: ("wrong", False), ids[2]: ("vague", True)}
        cached = {"input_tokens": 10, "cache_creation_input_tokens": 0, "cache_read_input_tokens": 900,
                  "output_tokens": 40}
        results = [succeeded(cid, {"verdict": v, "evidence_true": e, "reason": f"because {cid}"}, usage=cached)
                   for cid, (v, e) in reversed(list(answers.items()))]
        self.assertEqual(self.fetch(results), 0)
        with (nb.STATE / "r1" / "verdicts.csv").open(newline="", encoding="utf-8") as fh:
            rows = {r["custom_id"]: r for r in csv.DictReader(fh)}
        for cid, (v, e) in answers.items():
            self.assertEqual(rows[cid]["verdict"], v)
            self.assertEqual(rows[cid]["reason"], f"because {cid}")
            self.assertEqual(rows[cid]["evidence_true"], "yes" if e else "no")
        led = nb.ledger_rows()
        self.assertEqual(len(led), 1)
        self.assertEqual(led[0]["cache_reads_happened"], "yes")
        self.assertEqual(int(led[0]["cache_read_tokens"]), 2700)
        expect = 3 * (10 * 4.0 + 900 * 0.20 + 40 * 20.0) / 1e6 * 0.5
        self.assertAlmostEqual(float(led[0]["dollars"]), expect, places=4)

    def test_missing_and_refused(self):
        info = self.make_run("r1")
        ids = [m["custom_id"] for m in info["items"]]
        results = [succeeded(ids[2], "{", stop="refusal"),
                   succeeded(ids[0], {"verdict": "right", "evidence_true": True, "reason": "r"})]
        self.fetch(results)
        with (nb.STATE / "r1" / "verdicts.csv").open(newline="", encoding="utf-8") as fh:
            rows = {r["custom_id"]: r for r in csv.DictReader(fh)}
        self.assertEqual(rows[ids[0]]["verdict"], "right")
        self.assertEqual(rows[ids[1]]["status"], "missing from the results")
        self.assertIn("refusal", rows[ids[2]]["status"])
        self.assertEqual(rows[ids[2]]["verdict"], "")

    def test_apply_only_right_with_true_evidence(self):
        info = self.make_run("r1", n=4)
        ids = [m["custom_id"] for m in info["items"]]
        answers = [("right", True), ("right", False), ("vague", True), ("wrong", True)]
        self.fetch([succeeded(cid, {"verdict": v, "evidence_true": e, "reason": "r"})
                    for cid, (v, e) in zip(ids, answers)])
        calls = []

        def runner(addr, name, reason):
            calls.append((addr, name, reason))
            return SimpleNamespace(returncode=0, stdout="", stderr="")

        self.assertEqual(nb.cmd_apply(SimpleNamespace(run="r1"), runner=runner), 0)
        first = info["items"][0]
        self.assertEqual(calls, [(first["address"], first["name"], first["evidence"])])
        report = (nb.STATE / "r1" / "apply.md").read_text(encoding="utf-8")
        self.assertIn("1 applied, 3 rejected", report)
        self.assertIn("per landed name", report)

    def test_proposals_feed_review(self):
        info = self.make_run("p1", kind="propose", n=2)
        ids = [m["custom_id"] for m in info["items"]]
        nb.cmd_submit(SimpleNamespace(run="p1", budget=180.0), client=fake_client(FakeBatches()))
        nb.cmd_fetch(SimpleNamespace(run="p1"), client=fake_client(FakeBatches([
            succeeded(ids[1], {"decision": "decline", "name": "", "evidence": "e", "reason": "r"}),
            succeeded(ids[0], {"decision": "name", "name": "CFoo_Count", "evidence": "reads iCount", "reason": "r"}),
        ])))
        trees = {}

        def factory(rev):
            return trees.setdefault(rev, FakeTree({"src/game/CFoo/CFoo.cpp": FOO}, rev))

        args = SimpleNamespace(run="r2", branch=None, proposals=str(nb.STATE / "p1" / "proposals.csv"),
                               dry_run=True, effort="high", cache_ttl="5m")
        self.assertEqual(nb.cmd_build_review(args, tree_factory=factory, members=members), 0)
        reqs = nb.read_jsonl(nb.STATE / "r2" / "requests.jsonl")
        self.assertEqual(len(reqs), 1)
        tail = reqs[0]["params"]["messages"][0]["content"][-1]["text"]
        self.assertIn("Proposed name: CFoo_Count", tail)
        self.assertIn("Stated evidence: reads iCount", tail)
        self.assertNotIn("reason", tail.lower().replace("the reason", ""))
        meta = json.loads((nb.STATE / "r2" / "meta.json").read_text())
        self.assertEqual(meta["from_run"], "p1")


class Credentials(unittest.TestCase):
    def test_key_from_its_own_variable(self):
        seen = []
        client = nb.make_client({"DREAD_NAMES_API_KEY": "fake-test-value", "ANTHROPIC_API_KEY": "other"},
                                factory=lambda **kw: seen.append(kw) or "client")
        self.assertEqual(client, "client")
        self.assertEqual(seen, [{"api_key": "fake-test-value"}])

    def test_no_fallback_and_value_not_shown(self):
        with self.assertRaises(SystemExit) as cm:
            nb.make_client({"ANTHROPIC_API_KEY": "fake-other-value"}, factory=lambda **kw: self.fail("built"))
        self.assertIn("DREAD_NAMES_API_KEY", str(cm.exception))
        self.assertNotIn("fake-other-value", str(cm.exception))


class Propose(StateDir):
    def test_unnamed_matched(self):
        files = {"src/game/CFoo/fn.cpp": "int fn_7100000100() { return fn_7100000200(); }\n"
                                         "int fn_7100000300() { return 1; }\nint fn_7100000400() { return 2; }\n",
                 "src/lifted/l.cpp": "void x() { fn_7100000400(); }\n"}
        statuses = {"fn_7100000100": "O", "fn_7100000200": "O", "fn_7100000300": "m", "fn_7100000400": "O"}
        found, skipped = nb.unnamed_matched("src/game/CFoo", FakeTree(files), statuses)
        self.assertEqual(found, ["fn_7100000100"])
        self.assertEqual(skipped, ["fn_7100000400"])


if __name__ == "__main__":
    unittest.main()
