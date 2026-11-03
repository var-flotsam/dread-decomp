"""tools/tu_map.py's unit logic on small invented function lists (no game data)."""

import csv
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.tests import _path  # noqa: F401
import tu_map

A = 0x7100000000


def fns(n: int, step: int = 0x10, names: dict[int, str] | None = None) -> list[tu_map.Fn]:
    """n functions at A+0x10, A+0x20, ... each `step` bytes long."""
    names = names or {}
    return [tu_map.Fn(A + step * (i + 1), step, names.get(i, f"fn_{A + step * (i + 1):x}"),
                      "unknown" if i not in names else "rtti", "game", "U") for i in range(n)]


def getter(type_: str, address: int, init=None, fn_0x70=None, fn_0xe0=None):
    return SimpleNamespace(type=type_, address=address, init=init, fn_0x70=fn_0x70, fn_0xe0=fn_0xe0)


def row(index, start, end, hint="", source="", conf="high", init=""):
    return {"index": index, "start": f"0x{start:x}", "end": f"0x{end:x}", "functions": 1, "bytes": end - start,
            "init_fn": init, "class_hint": hint, "hint_source": source, "confidence": conf}


class Qualifier(unittest.TestCase):
    def test_scope_of_a_member(self):
        self.assertEqual(tu_map.qualifier("ns::CFoo::Bar"), "ns::CFoo")

    def test_template_arguments_ignored(self):
        self.assertEqual(tu_map.qualifier("CFoo::Bar<ns::CBaz>"), "CFoo")

    def test_bare_name_kept(self):
        self.assertEqual(tu_map.qualifier("nnMain"), "nnMain")

    def test_placeholders_have_none(self):
        for n in ("fn_7100000010", "FUN_7100000010", "thunk_x", "<EXTERNAL>::y"):
            self.assertIsNone(tu_map.qualifier(n), n)


class Related(unittest.TestCase):
    types = {"CFoo", "GUI"}

    def test_class_and_nested_type(self):
        self.assertTrue(tu_map.related([getter("CFoo", 0)], [getter("CFoo::CTunableFoo", 0)], self.types))

    def test_two_nested_types_of_one_class(self):
        self.assertTrue(tu_map.related([getter("CFoo::CParams", 0)], [getter("CFoo::CTunable", 0)], self.types))

    def test_class_named_scope_counts_unreflected(self):
        self.assertTrue(tu_map.related([getter("SBar::A", 0)], [getter("SBar::B", 0)], self.types))

    def test_namespace_siblings_unrelated(self):
        self.assertFalse(tu_map.related([getter("gui::CA", 0)], [getter("gui::CB", 0)], self.types))

    def test_unrelated_classes(self):
        self.assertFalse(tu_map.related([getter("CFoo", 0)], [getter("CBar", 0)], self.types))

    def test_prefix_without_scope_is_not_nested(self):
        self.assertFalse(tu_map.related([getter("CFoo", 0)], [getter("CFooBar", 0)], self.types))


class SplitGroups(unittest.TestCase):
    def setUp(self):
        self.funcs = fns(8)
        self.addrs = [f.address for f in self.funcs]

    def test_unrelated_groups_split_before_helper(self):
        a = self.addrs
        u = tu_map.Unit(0, 8, init_fn=a[7], bounded_left=True, bounded_right=True,
                        blocks=[[getter("CFoo", a[1])], [getter("CBar", a[5], fn_0x70=a[4])]])
        p1, p2 = tu_map.split_groups(u, a, set())
        self.assertEqual((p1.lo, p1.hi, p2.lo, p2.hi), (0, 4, 4, 8))
        self.assertTrue(p1.bounded_left and not p1.bounded_right)
        self.assertTrue(p2.bounded_right and not p2.bounded_left)
        self.assertIsNone(p1.init_fn)
        self.assertEqual(p2.init_fn, a[7])
        self.assertTrue(p1.split and p2.split)
        self.assertEqual([g.type for b in p2.blocks for g in b], ["CBar"])

    def test_helper_cannot_pull_cut_past_previous_getter(self):
        a = self.addrs
        # CBar's helper sits in front of CFoo's getter: the cut stays right after CFoo's getter
        u = tu_map.Unit(0, 8, blocks=[[getter("CFoo", a[2])], [getter("CBar", a[4], init=a[1])]])
        p1, p2 = tu_map.split_groups(u, a, set())
        self.assertEqual((p1.hi, p2.lo), (3, 3))

    def test_related_groups_kept_together(self):
        a = self.addrs
        u = tu_map.Unit(0, 8, blocks=[[getter("CFoo", a[1])], [getter("CFoo::CTunable", a[5])]])
        pieces = tu_map.split_groups(u, a, {"CFoo"})
        self.assertEqual(pieces, [u])
        self.assertTrue(u.split)


class Carve(unittest.TestCase):
    def setUp(self):
        self.funcs = fns(8)
        self.a = [f.address for f in self.funcs]

    def test_range_inside_one_unit(self):
        a = self.a
        u = tu_map.Unit(0, 8, init_fn=a[7], bounded_right=True)
        before, lib, after = tu_map.carve([u], self.funcs, a[2], a[4], "lua")
        self.assertEqual((before.lo, before.hi, before.init_fn), (0, 2, None))
        self.assertEqual((lib.lo, lib.hi, lib.library), (2, 4, "lua"))
        self.assertEqual((after.lo, after.hi, after.init_fn), (4, 8, a[7]))
        self.assertTrue(after.bounded_left and after.bounded_right)

    def test_range_over_two_units_is_one_library_unit(self):
        a = self.a
        units = [tu_map.Unit(0, 4, init_fn=a[3], bounded_right=True), tu_map.Unit(4, 8, bounded_left=True)]
        out = tu_map.carve(units, self.funcs, a[2], a[6], "dlmalloc")
        self.assertEqual([(u.lo, u.hi, u.library) for u in out], [(0, 2, ""), (2, 6, "dlmalloc"), (6, 8, "")])

    def test_units_outside_untouched(self):
        a = self.a
        units = [tu_map.Unit(0, 2), tu_map.Unit(2, 8)]
        out = tu_map.carve(units, self.funcs, a[4], a[5], "lua")
        self.assertIs(out[0], units[0])
        self.assertEqual(len(out), 4)


class Confidence(unittest.TestCase):
    def setUp(self):
        self.funcs = fns(4)

    def test_bounded_small_is_high(self):
        u = tu_map.Unit(0, 4, bounded_left=True, bounded_right=True, blocks=[[getter("CFoo", 0)]])
        self.assertEqual(tu_map.confidence(u, self.funcs), "high")

    def test_library_split_and_unbounded(self):
        self.assertEqual(tu_map.confidence(tu_map.Unit(0, 4, library="lua"), self.funcs), "low")
        self.assertEqual(tu_map.confidence(tu_map.Unit(0, 4, split=True), self.funcs), "medium")
        self.assertEqual(tu_map.confidence(tu_map.Unit(0, 4, bounded_left=True), self.funcs), "low")

    def test_two_groups_is_low(self):
        u = tu_map.Unit(0, 4, bounded_left=True, bounded_right=True, blocks=[[getter("A", 0)], [getter("B", 0)]])
        self.assertEqual(tu_map.confidence(u, self.funcs), "low")

    def test_large_span(self):
        big = fns(2, step=tu_map.LARGE_SPAN)
        with_group = tu_map.Unit(0, 2, bounded_left=True, bounded_right=True, blocks=[[getter("CFoo", 0)]])
        without = tu_map.Unit(0, 2, bounded_left=True, bounded_right=True)
        self.assertEqual(tu_map.confidence(with_group, big), "medium")
        self.assertEqual(tu_map.confidence(without, big), "low")


class RankClasses(unittest.TestCase):
    def test_outer_then_called_then_child_before_parent(self):
        funcs = fns(6)
        a = [f.address for f in funcs]
        gs = [getter("CBase", a[0]), getter("CFoo::CParams", a[1]), getter("CChild", a[2]), getter("CFoo", a[3])]
        u = tu_map.Unit(0, 6, blocks=[gs])
        callers = {a[3]: {a[4], a[5]}, a[2]: {a[4]}, a[0]: {a[4]}}
        parents = {"CChild": "CBase"}
        self.assertEqual(tu_map.rank_classes(u, funcs, callers, parents),
                         ["CFoo", "CChild", "CBase", "CFoo::CParams"])

    def test_callers_outside_unit_ignored(self):
        funcs = fns(4)
        a = [f.address for f in funcs]
        u = tu_map.Unit(0, 2, blocks=[[getter("CA", a[0]), getter("CB", a[1])]])
        callers = {a[1]: {a[3], a[2]}}  # both outside [a0, a1]
        self.assertEqual(tu_map.rank_classes(u, funcs, callers, {}), ["CA", "CB"])


class Rows(unittest.TestCase):
    def test_unit_rows_columns(self):
        funcs = fns(3)
        units = [tu_map.Unit(0, 2, init_fn=funcs[1].address, bounded_left=True, bounded_right=True,
                             hint="CFoo", source="reflection"),
                 tu_map.Unit(2, 3, library="lua", hint="lua", source="library")]
        rows = tu_map.unit_rows(units, funcs)
        self.assertEqual(list(rows[0]), tu_map.COLUMNS)
        self.assertEqual(rows[0]["start"], "0x7100000010")
        self.assertEqual(rows[0]["end"], "0x7100000030")
        self.assertEqual(rows[0]["bytes"], 0x20)
        self.assertEqual(rows[0]["init_fn"], "0x7100000020")
        self.assertEqual(rows[0]["confidence"], "high")
        self.assertEqual((rows[1]["init_fn"], rows[1]["confidence"], rows[1]["index"]), ("", "low", 1))

    def test_find_unit_with_gaps(self):
        rows = [row(0, A + 0x10, A + 0x30), row(1, A + 0x40, A + 0x50)]
        starts = tu_map.unit_starts(rows)
        self.assertEqual(starts, [A + 0x10, A + 0x40])
        self.assertEqual(tu_map.find_unit(rows, A + 0x10)["index"], 0)
        self.assertEqual(tu_map.find_unit(rows, A + 0x2f, starts)["index"], 0)
        self.assertIsNone(tu_map.find_unit(rows, A + 0x30, starts))  # end is exclusive, gap
        self.assertIsNone(tu_map.find_unit(rows, A, starts))
        self.assertEqual(tu_map.find_unit(rows, A + 0x4f, starts)["index"], 1)
        self.assertIsNone(tu_map.find_unit(rows, A + 0x50, starts))

    def test_load_map_round_trip(self):
        with tempfile.TemporaryDirectory() as d:
            out = Path(d) / "tu_map.csv"
            with out.open("w", newline="", encoding="utf-8") as f:
                w = csv.DictWriter(f, tu_map.COLUMNS, lineterminator="\n")
                w.writeheader()
                w.writerows([row(0, A + 0x10, A + 0x30, "CFoo;CBar", "reflection")])
            with mock.patch.object(tu_map, "OUT", out):
                rows = tu_map.load_map()
        self.assertEqual(rows[0]["index"], 0)
        self.assertEqual(rows[0]["bytes"], 0x20)
        self.assertEqual((rows[0]["start_i"], rows[0]["end_i"]), (A + 0x10, A + 0x30))
        self.assertEqual(rows[0]["class_hint"], "CFoo;CBar")

    def test_load_map_missing(self):
        with tempfile.TemporaryDirectory() as d:
            with mock.patch.object(tu_map, "OUT", Path(d) / "tu_map.csv"), \
                    mock.patch.object(tu_map, "ROOT", Path(d)):
                with self.assertRaises(SystemExit):
                    tu_map.load_map()


class ResolveFunction(unittest.TestCase):
    def setUp(self):
        self.fns = fns(3, names={0: "CFoo::Bar", 1: "dup", 2: "dup"})

    def test_by_name(self):
        self.assertEqual(tu_map.resolve_function(" CFoo::Bar ", self.fns), A + 0x10)

    def test_duplicate_name_refused(self):
        with self.assertRaises(SystemExit):
            tu_map.resolve_function("dup", self.fns)

    def test_address_spellings(self):
        for t in ("fn_7100000020", "0x7100000020", "7100000020", "0x20", "FN_7100000020"):
            self.assertEqual(tu_map.resolve_function(t, self.fns), A + 0x20, t)

    def test_not_an_address(self):
        with self.assertRaises(SystemExit):
            tu_map.resolve_function("CNotThere::x", self.fns)


if __name__ == "__main__":
    unittest.main()
