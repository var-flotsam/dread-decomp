"""tools/hubs.py's ranking, readiness and caller listing on a made-up call graph (no game data)."""

import contextlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import callgraph
import hubs

MAIN = callgraph.NNMAIN
H, H2, U1, U2, M1, O1, T, R, LEAF, IMP, FAR = (
    0x7100002000, 0x7100002010, 0x7100002020, 0x7100002030, 0x7100002040, 0x7100002050,
    0x7100002060, 0x7100002070, 0x7100002080, 0x7100002090, 0x71000020a0)


def row(addr, name, status, size):
    return {"address": f"0x{addr:x}", "size": size, "name": name, "name_source": "unknown",
            "subsystem": "game?", "status": status}


def fn(addr, calls=(), thunk_of=None):
    o = {"entry": f"0x{addr:x}", "calls": [f"0x{c:x}" for c in calls]}
    if thunk_of is not None:
        o["thunk_of"] = f"0x{thunk_of:x}"
    return o


def rows():
    return {MAIN: row(MAIN, "nnMain", "U", 64), H: row(H, "fn_H", "U", 100), H2: row(H2, "fn_H2", "U", 300),
            U1: row(U1, "fn_U1", "U", 40), U2: row(U2, "fn_U2", "U", 40), M1: row(M1, "fn_M1", "M", 40),
            O1: row(O1, "fn_O1", "O", 40), T: row(T, "fn_T", "O", 4), R: row(R, "fn_R", "U", 200),
            LEAF: row(LEAF, "fn_LEAF", "U", 8), IMP: row(IMP, "<EXTERNAL>::nnImp", "U", 16),
            FAR: row(FAR, "fn_FAR", "U", 24)}


# nnMain -> U1, U2, M1, O1, R, LEAF. H is called by U1, U2, M1, O1 and itself; H2 only through
# thunk T (from U1). R calls O1 and the import. FAR (outside the boot slice) calls H.
EXPORT = [fn(MAIN, [U1, U2, M1, O1, R, LEAF]), fn(U1, [H, T]), fn(U2, [H]), fn(M1, [H]), fn(O1, [H]),
          fn(H, [H]), fn(T, thunk_of=H2), fn(R, [O1, IMP]), fn(FAR, [H])]


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.path = Path(self.tmp.name) / "main_functions.jsonl"
        self.path.write_text("".join(json.dumps(o) + "\n" for o in EXPORT), encoding="utf-8")
        self.patches = [mock.patch.object(callgraph, "export_path", lambda version: self.path),
                        mock.patch.object(callgraph, "load_rows", rows)]
        for p in self.patches:
            p.start()
        self.g = callgraph.CallGraph("0.0.0")

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def run_main(self, *argv):
        out = io.StringIO()
        with mock.patch.object(sys, "argv", ["hubs.py", "--version", "0.0.0", *argv]), \
                contextlib.redirect_stdout(out):
            self.assertEqual(hubs.main(), 0)
        return out.getvalue()

    def run_json(self, *argv):
        return json.loads(self.run_main("--json", *argv))


class Record(Fixture):
    def test_done(self):
        self.assertTrue(hubs.done(self.g.funcs[O1]))
        self.assertTrue(hubs.done(self.g.funcs[IMP]))  # import stubs count as matched
        self.assertFalse(hubs.done(self.g.funcs[M1]))

    def test_counts_unmatched_callers_and_drops_self_calls(self):
        r = hubs.record(self.g, H, None)
        self.assertEqual((r["callers"], r["unmatched_callers"]), (5, 4))  # U1 U2 M1 FAR unmatched, O1 not
        self.assertEqual(r["callees"], 0)
        self.assertTrue(r["ready"])
        self.assertEqual(r["address"], f"0x{H:x}")

    def test_slice_limits_callers(self):
        within = self.g.reachable(MAIN)
        self.assertNotIn(FAR, within)
        self.assertEqual(hubs.record(self.g, H, within)["unmatched_callers"], 3)

    def test_thunked_callee_is_collapsed(self):
        r = hubs.record(self.g, H2, None)
        self.assertEqual(r["callers"], 1)  # U1 through the O thunk
        u1 = hubs.record(self.g, U1, None)
        self.assertFalse(u1["ready"])  # it calls U functions H and (through T) H2
        self.assertEqual(u1["callees"], 2)

    def test_size_ok(self):
        self.assertTrue(hubs.size_ok(10, None, None))
        self.assertTrue(hubs.size_ok(10, 10, 10))
        self.assertFalse(hubs.size_ok(9, 10, None))
        self.assertFalse(hubs.size_ok(11, None, 10))


class Main(Fixture):
    def test_hub_ranking(self):
        out = self.run_json()
        names = [r["name"] for r in out["functions"]]
        self.assertEqual(names[0], "fn_H")
        self.assertNotIn("<EXTERNAL>::nnImp", names)
        self.assertNotIn("fn_O1", names)  # only U functions are candidates
        self.assertIn("1 import stubs skipped", out["title"])
        self.assertEqual(out["caveat"], callgraph.CAVEAT)

    def test_ties_break_by_callers_then_size(self):
        names = [r["name"] for r in self.run_json()["functions"]]
        # H2 and nnMain-called U functions have 1 unmatched caller (nnMain or U1); larger first
        one = [n for n in names if n in ("fn_H2", "fn_R", "fn_U1", "fn_U2", "fn_LEAF")]
        self.assertEqual(one, ["fn_H2", "fn_R", "fn_U1", "fn_U2", "fn_LEAF"])

    def test_top_limits_rows(self):
        self.assertEqual(len(self.run_json("--top", "2")["functions"]), 2)
        self.assertEqual(len(self.run_json("--top", "0")["functions"]), 8)

    def test_size_filters(self):
        sizes = [r["size"] for r in self.run_json("--min-size", "40", "--max-size", "200")["functions"]]
        self.assertTrue(sizes and all(40 <= s <= 200 for s in sizes))

    def test_ready_largest_first(self):
        out = self.run_json("--ready")
        self.assertEqual([r["name"] for r in out["functions"]], ["fn_H2", "fn_R", "fn_H", "fn_LEAF"])  # FAR calls H
        self.assertIn("leaves", out["title"])

    def test_boot_slice(self):
        names = {r["name"] for r in self.run_json("--boot")["functions"]}
        self.assertNotIn("fn_FAR", names)
        self.assertIn("fn_H", names)

    def test_callers_unmatched_first(self):
        out = self.run_json("--callers", "fn_H")
        self.assertEqual([r["name"] for r in out["functions"]], ["fn_U1", "fn_U2", "fn_M1", "fn_FAR", "fn_O1"])
        self.assertEqual((out["callers_total"], out["unmatched_total"]), (5, 4))
        self.assertEqual(out["function"]["name"], "fn_H")

    def test_callers_resolves_thunk(self):
        out = self.run_json("--callers", f"0x{T:x}")
        self.assertEqual(out["function"]["name"], "fn_H2")
        self.assertIn("resolved to its target", out["title"])
        self.assertEqual([r["name"] for r in out["functions"]], ["fn_U1"])

    def test_text_table(self):
        text = self.run_main("--top", "3")
        lines = text.splitlines()
        self.assertTrue(lines[0].startswith("hubs: "))
        self.assertEqual(lines[1].split(), ["address", "size", "callers", "unmatched", "name", "subsystem"])
        self.assertEqual(len(lines), 1 + 1 + 3 + 1)
        self.assertEqual(lines[-1], callgraph.CAVEAT)

    def test_negative_top_rejected(self):
        with mock.patch.object(sys, "argv", ["hubs.py", "--version", "0.0.0", "--top", "-1"]), \
                contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                hubs.main()


class Table(unittest.TestCase):
    def test_columns_aligned_and_long_names_not_padding_others(self):
        out = io.StringIO()
        rows = [{"a": "x", "name": "n" * 80, "s": "z"}, {"a": "xyz", "name": "short", "s": "q"}]
        with contextlib.redirect_stdout(out):
            hubs.print_table(rows, [("a", "a", "<"), ("name", "name", "<"), ("sub", "s", "<")])
        lines = out.getvalue().splitlines()
        self.assertEqual(lines[0], "a    " + "name".ljust(hubs.MAX_NAME_PAD) + "  sub")
        self.assertEqual(lines[2], "xyz  " + "short".ljust(hubs.MAX_NAME_PAD) + "  q")

    def test_emit_without_rows_prints_no_table(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            hubs.emit("title", [], hubs.HUB_COLUMNS, False)
        self.assertEqual(out.getvalue().splitlines(), ["title", callgraph.CAVEAT])


if __name__ == "__main__":
    unittest.main()
