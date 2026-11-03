"""tools/callgraph.py on a made-up function list and export (no game data)."""

import json
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import callgraph

A, B, C, D, T, T2, X, IMP = (0x7100000010, 0x7100000020, 0x7100000030, 0x7100000040,
                             0x7100000050, 0x7100000060, 0x7100000070, 0x7100000080)


def row(addr, name, status="U", size=16):
    return {"address": f"0x{addr:x}", "size": str(size), "name": name, "name_source": "unknown",
            "subsystem": "game", "status": status}


def make_graph(rows, export):
    """A CallGraph over `rows` (address -> CSV row) and `export` (list of jsonl objects)."""
    tmp = tempfile.TemporaryDirectory()
    path = Path(tmp.name) / "main_functions.jsonl"
    path.write_text("".join(json.dumps(o) + "\n" for o in export), encoding="utf-8")
    with mock.patch.object(callgraph, "export_path", lambda version: path):
        g = callgraph.CallGraph("0.0.0", rows)
    tmp.cleanup()
    return g


def fn(addr, calls=(), thunk_of=None):
    o = {"entry": f"0x{addr:x}", "calls": [f"0x{c:x}" for c in calls]}
    if thunk_of is not None:
        o["thunk_of"] = f"0x{thunk_of:x}"
    return o


# A -> B, A -> T (thunk of C), B -> C, B -> B (self), C -> D, T2 thunk of T; D calls an unlisted address.
ROWS = {A: row(A, "fn_A"), B: row(B, "fn_B", "O"), C: row(C, "fn_C"), D: row(D, "fn_D", "L"),
        T: row(T, "fn_T", "O", 4), T2: row(T2, "fn_T2", "O", 4), X: row(X, "dup"),
        IMP: row(IMP, "<EXTERNAL>::nnFoo")}
EXPORT = [fn(A, [B, T]), fn(B, [C, B]), fn(C, [D]), fn(D, [0x7199999999]),
          fn(T, thunk_of=C), fn(T2, thunk_of=T), fn(0x7188888888, [A])]


class Building(unittest.TestCase):
    def setUp(self):
        self.g = make_graph(ROWS, EXPORT)

    def test_sizes_become_ints_without_touching_input(self):
        self.assertEqual(self.g.funcs[A]["size"], 16)
        self.assertEqual(ROWS[A]["size"], "16")

    def test_edges_and_callers(self):
        self.assertEqual(self.g.callees[A], [B, T])
        self.assertEqual(self.g.callers[C], [B, T])
        self.assertEqual(self.g.callees[T], [C])  # a thunk calls its target

    def test_unlisted_addresses_dropped(self):
        self.assertEqual(self.g.callees[D], [])
        self.assertNotIn(0x7188888888, self.g.funcs)
        self.assertEqual(self.g.callers[A], [])

    def test_edge_count(self):
        # A->B, A->T, B->C, B->B, C->D, T->C, T2->T
        self.assertEqual(self.g.edges, 7)

    def test_thunk_map_and_target_chain(self):
        self.assertEqual(self.g.thunk_of, {T: C, T2: T})
        self.assertEqual(self.g.target(T2), C)
        self.assertEqual(self.g.target(A), A)

    def test_target_survives_a_thunk_cycle(self):
        g = make_graph({A: row(A, "a"), B: row(B, "b")}, [fn(A, thunk_of=B), fn(B, thunk_of=A)])
        self.assertIn(g.target(A), (A, B))  # terminates

    def test_missing_export_exits(self):
        missing = callgraph.ROOT / "build" / "ghidra_export" / "no-such" / "main_functions.jsonl"
        with mock.patch.object(callgraph, "export_path", lambda version: missing):
            with self.assertRaises(SystemExit):
                callgraph.CallGraph("no-such", ROWS)


class Collapsed(unittest.TestCase):
    def setUp(self):
        self.g = make_graph(ROWS, EXPORT)
        self.callees, self.callers = self.g.collapsed()

    def test_calls_through_thunks_reach_the_target(self):
        self.assertEqual(self.callees[A], [B, C])
        self.assertEqual(self.callees[T2], [C])

    def test_thunks_are_not_callers(self):
        self.assertEqual(self.callers[C], [A, B])
        self.assertEqual(self.callers[T], [])

    def test_cached(self):
        self.assertIs(self.g.collapsed(), self.g.collapsed())


class Reach(unittest.TestCase):
    def setUp(self):
        self.g = make_graph(ROWS, EXPORT)

    def test_reachable_follows_thunks(self):
        self.assertEqual(self.g.reachable(A), {A, B, C, D, T})
        self.assertEqual(self.g.reachable(D), {D})

    def test_reachable_rejects_non_start(self):
        with self.assertRaises(KeyError):
            self.g.reachable(0x7100000011)


class Resolve(unittest.TestCase):
    def setUp(self):
        rows = dict(ROWS)
        rows[0x7100000090] = row(0x7100000090, "dup")
        self.g = make_graph(rows, EXPORT)

    def test_name_and_hex(self):
        self.assertEqual(self.g.resolve("fn_B"), B)
        self.assertEqual(self.g.resolve("0x7100000030"), C)
        self.assertEqual(self.g.resolve("7100000030"), C)
        self.assertEqual(self.g.name(C), "fn_C")

    def test_ambiguous_unknown_and_non_start(self):
        for spec in ("dup", "nope_not_hex", "0x7100000011"):
            with self.assertRaises(SystemExit, msg=spec):
                self.g.resolve(spec)

    def test_is_import(self):
        self.assertTrue(callgraph.is_import(ROWS[IMP]))
        self.assertFalse(callgraph.is_import(ROWS[A]))


if __name__ == "__main__":
    unittest.main()
