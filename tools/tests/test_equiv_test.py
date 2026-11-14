"""tools/equiv_test.py: the parts that need no game data (the result's kind, coverage, the verdict table)."""
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import equiv_test

try:
    import port_lift
except Exception:  # pragma: no cover
    port_lift = None


def words(*ws):
    return b"".join(w.to_bytes(4, "little") for w in ws)


class ReturnType(unittest.TestCase):
    def test_the_definition_s_return_type_without_attributes_and_storage(self):
        text = ("void fn_7100000100(int) {\n}\n"
                "__attribute__((noinline)) static inline const UnkFoo* fn_7100000200(UnkBar* self) {\n}\n"
                "u32 UnkBar::fn_7100000300() {\n}\n"
                "UnkVec3 fn_7100000400(const UnkBar& b) {\n}\n")
        self.assertEqual(equiv_test.return_type(text, 0x7100000100), "void")
        self.assertEqual(equiv_test.return_type(text, 0x7100000200), "UnkFoo*")
        self.assertEqual(equiv_test.return_type(text, 0x7100000300), "u32")
        self.assertEqual(equiv_test.return_type(text, 0x7100000400), "UnkVec3")
        self.assertIsNone(equiv_test.return_type(text, 0x7100000500))

    def test_a_declaration_or_a_call_is_not_the_definition(self):
        text = ("bool fn_7100000600(UnkA* a);  // returns nothing useful\n"
                "void user() {\n    if (fn_7100000600(nullptr)) return;\n}\n"
                "// u64 fn_7100000600(UnkA* a) {\n"
                "u8\nfn_7100000600(UnkA* a,\n                  u32 n) {\n    return n;\n}\n"
                "s32 UnkA::fn_7100000700() const {\n    return 0;\n}\n")
        self.assertEqual(equiv_test.return_type(text, 0x7100000600), "u8")
        self.assertEqual(equiv_test.return_type(text, 0x7100000700), "s32")
        self.assertIsNone(equiv_test.return_type("void fn_7100000800();\n", 0x7100000800))

    def test_what_is_compared_as_the_result(self):
        self.assertIsNone(equiv_test.result_kind("void"))
        self.assertIsNone(equiv_test.result_kind(None))
        self.assertEqual(equiv_test.result_kind("bool"), "x8")
        self.assertEqual(equiv_test.result_kind("s32"), "x32")
        self.assertEqual(equiv_test.result_kind("f32"), "f32")
        self.assertEqual(equiv_test.result_kind("UnkFoo*"), "x64")
        self.assertEqual(equiv_test.result_kind("const CStrId&"), "x64")
        # a small class comes back in registers when the original's callers read it so
        self.assertEqual(equiv_test.result_kind("UnkVec3", ("s", 3)), "f32*3")
        self.assertEqual(equiv_test.result_kind("UnkPair", ("x", 2)), "x64*2")
        # otherwise it is returned in memory, and its bytes are compared as writes
        self.assertIsNone(equiv_test.result_kind("UnkBig"))


class Coverage(unittest.TestCase):
    CODE = words(0x54000040,   # b.eq
                 0xD503201F,   # nop
                 0xB4000040,   # cbz x0
                 0x36000040,   # tbz w0, #0
                 0x14000002,   # b (unconditional: not counted)
                 0xD65F03C0)   # ret

    def test_conditional_branches_are_found(self):
        self.assertEqual(equiv_test.conditional_branches(self.CODE, 0x1000), [0x1000, 0x1008, 0x100C])

    def test_a_branch_seen_one_way_only_is_listed(self):
        branches = equiv_test.conditional_branches(self.CODE, 0x1000)
        seen = {(0x1000, True), (0x1000, False), (0x1008, True), (0x100C, False), (0x100C, True)}
        self.assertEqual(equiv_test.one_way(branches, seen), [0x1008])
        self.assertEqual(equiv_test.one_way(branches, seen | {(0x1008, False)}), [])
        self.assertEqual(equiv_test.one_way([], set()), [])


class Table(unittest.TestCase):
    def test_a_row_counts_only_as_a_pass_of_this_code_under_these_rules(self):
        code = words(0xD65F03C0)
        d = equiv_test.digest(code)
        row = {"address": "0x7100000100", "digest": d, "verdict": "pass", "rules": str(equiv_test.RULES), "detail": ""}
        self.assertTrue(equiv_test.current(row, d))
        self.assertFalse(equiv_test.current(row, equiv_test.digest(words(0xD503201F, 0xD65F03C0))))  # the code changed
        self.assertFalse(equiv_test.current({**row, "verdict": "inconclusive"}, d))
        self.assertFalse(equiv_test.current({**row, "verdict": "fail"}, d))
        self.assertFalse(equiv_test.current({**row, "rules": str(equiv_test.RULES + 1)}, d))           # older rules
        self.assertFalse(equiv_test.current(None, d))

    def test_the_table_round_trips_sorted_by_address(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "equivalence.csv"
            self.assertEqual(equiv_test.read_table(path), {})
            rows = {0x7100000200: {"address": "0x7100000200", "digest": "bb", "verdict": "fail", "rules": "1",
                                   "detail": "input 2: another value passed to fn_7100000300"},
                    0x7100000100: {"address": "0x7100000100", "digest": "aa", "verdict": "pass", "rules": "1", "detail": ""}}
            equiv_test.write_table(rows, path)
            lines = path.read_text(encoding="utf-8").splitlines()
            self.assertEqual(lines[0], ",".join(equiv_test.FIELDS))
            self.assertTrue(lines[1].startswith("0x7100000100,aa,pass,1,"))
            self.assertEqual(equiv_test.read_table(path), rows)

    def test_a_detail_is_one_short_field(self):
        self.assertEqual(equiv_test.short("input 3: call 2,\n  x1 differs", 20), "input 3: call 2; x1 ")
        self.assertEqual(equiv_test.digest(b"abc"), equiv_test.digest(b"abc"))
        self.assertNotEqual(equiv_test.digest(b"abc"), equiv_test.digest(b"abd"))

    def test_the_digest_covers_what_the_relocations_name(self):
        from types import SimpleNamespace as R
        code = words(0x94000000, 0xD65F03C0)   # bl <relocated>; ret: the same bytes whatever is called

        def call(symbol, addend=0):
            return {0: R(type=283, symbol=symbol, addend=addend, section=False, data=None)}
        self.assertEqual(equiv_test.digest(code, call("fn_7100000100")), equiv_test.digest(code, call("fn_7100000100")))
        self.assertNotEqual(equiv_test.digest(code, call("fn_7100000100")), equiv_test.digest(code, call("fn_7100000200")))
        self.assertNotEqual(equiv_test.digest(code, call("g", 0)), equiv_test.digest(code, call("g", 8)))
        self.assertNotEqual(equiv_test.digest(code, call("fn_7100000100")), equiv_test.digest(code))
        # a string counts by its content: moving it inside its section changes nothing
        a = {0: R(type=275, symbol=".rodata.str1.1", addend=0, section=True, data=b"hi\0")}
        b = {0: R(type=275, symbol=".rodata.str1.1", addend=7, section=True, data=b"hi\0")}
        c = {0: R(type=275, symbol=".rodata.str1.1", addend=0, section=True, data=b"ho\0")}
        self.assertEqual(equiv_test.digest(code, a), equiv_test.digest(code, b))
        self.assertNotEqual(equiv_test.digest(code, a), equiv_test.digest(code, c))


class Stale(unittest.TestCase):
    """tools/check.py fails a `pass` row that no longer describes the tree."""

    def setUp(self):
        import check
        self.check = check
        self.code = words(0xD503201F, 0xD65F03C0)
        self.obj = Path("a.o")

    def row(self, **kw):
        return {"address": "0x7100000100", "digest": equiv_test.digest(self.code), "verdict": "pass",
                "rules": str(equiv_test.RULES), "detail": "", **kw}

    def stale(self, table, statuses, code=None):
        where = {"0x7100000100": [(self.obj, "fn_7100000100")]}
        return self.check.stale_verdicts(table, statuses, where,
                                         lambda obj, symbol: equiv_test.digest(code or self.code))

    def test_a_pass_of_the_code_in_the_tree_is_current(self):
        self.assertEqual(self.stale({0x7100000100: self.row()}, {"0x7100000100": "m"}), [])

    def test_changed_code_a_match_a_missing_function_and_older_rules_are_stale(self):
        table = {0x7100000100: self.row()}
        self.assertEqual(self.stale(table, {"0x7100000100": "m"}, words(0xD65F03C0)),
                         [("0x7100000100", "its code, or what it refers to, changed since it was judged")])
        self.assertIn("its status is O", self.stale(table, {"0x7100000100": "O"})[0][1])
        self.assertIn("no function of src/", self.stale(table, {})[0][1])
        old = {0x7100000100: self.row(rules=str(equiv_test.RULES - 1))}
        self.assertIn("judged under rules", self.stale(old, {"0x7100000100": "m"})[0][1])

    def test_rows_that_are_not_a_pass_are_not_checked(self):
        table = {0x7100000100: self.row(verdict="inconclusive", digest="other")}
        self.assertEqual(self.stale(table, {}), [])


class _Run:
    """What more_inputs reads of a finished run of the original."""
    def __init__(self, visited, outcomes, read_words=(), calls=0):
        self.visited, self.outcomes = set(visited), set(outcomes)
        self.read_words, self.read_at, self.calls = list(read_words), {}, calls

    def peek(self, w, n):
        return 0x5000


class _Image:
    relocs: dict = {}
    rodata = (0, 0)

    @staticmethod
    def in_text(off):
        return False


class _Oracle:
    """A four-instruction function, `cbz x0` first: x0 == 0 skips the second instruction and runs the third."""
    A = 0x7100000100
    CODE = (0xB4000040).to_bytes(4, "little") + bytes(12)   # cbz x0, +8; three more words

    def __init__(self, base_runs):
        self.runs = list(base_runs)
        self.preset_orig: dict = {}
        self.tried: list = []
        self.orig = type("O", (), {"sizes": {self.A: 16}, "img": _Image, "function": staticmethod(lambda a: self.CODE)})
        self.af = type("F", (), {"facts": staticmethod(lambda a: type("Facts", (), {"args": frozenset({0, 1})}))})

    def originals(self, a):
        return self.runs

    def out_of_time(self, phase):
        return False

    def run_original(self, a, seed, ovr):
        self.tried.append(dict(ovr))
        if ovr.get(("r", 0)) == 0:
            return _Run({a, a + 8, a + 12}, {(a, True)})
        return _Run({a, a + 4, a + 12}, {(a, False)})


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class MoreInputs(unittest.TestCase):
    A = _Oracle.A

    def test_the_null_argument_is_tried_and_kept_for_what_it_reaches(self):
        oracle = _Oracle([(0, {}, _Run({self.A, self.A + 4, self.A + 12}, {(self.A, False)}))])
        self.assertEqual(equiv_test.more_inputs(oracle, self.A), 1)
        seed, ovr, run = oracle.runs[-1]
        self.assertEqual(ovr, {("r", 0): 0})
        self.assertIn(self.A + 8, run.visited)
        self.assertEqual(equiv_test.more_inputs(oracle, self.A), 0)       # everything is covered: no run at all
        self.assertEqual(len(oracle.runs), 2)

    def test_a_branch_outcome_alone_is_worth_a_run(self):
        # every instruction ran (two runs between them), but the branch was only recorded one way
        runs = [(0, {}, _Run({self.A, self.A + 4, self.A + 12}, {(self.A, False)})),
                (1, {}, _Run({self.A, self.A + 8, self.A + 12}, set()))]
        oracle = _Oracle(runs)
        self.assertEqual(equiv_test.more_inputs(oracle, self.A), 1)
        self.assertIn((self.A, True), oracle.runs[-1][2].outcomes)

    def test_a_run_that_reaches_nothing_new_is_not_kept(self):
        oracle = _Oracle([(0, {}, _Run({self.A, self.A + 4, self.A + 12}, {(self.A, False)}))])
        oracle.run_original = lambda a, seed, ovr: _Run({a, a + 4, a + 12}, {(a, False)})
        self.assertEqual(equiv_test.more_inputs(oracle, self.A), 0)
        self.assertEqual(len(oracle.runs), 1)

    def test_outcomes_of_other_code_do_not_count(self):
        oracle = _Oracle([(0, {}, _Run({self.A, self.A + 4, self.A + 12}, {(self.A, False)}))])
        oracle.run_original = lambda a, seed, ovr: _Run({a, a + 4, a + 12}, {(a, False), (0x7100999000, True)})
        self.assertEqual(equiv_test.more_inputs(oracle, self.A), 0)


CMP_W8_5, B_HI, B_NE, BR_X10, NOP = 0x7100151F, 0x54000208, 0x54000201, 0xD61F0140, 0xD503201F


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class JumpTables(unittest.TestCase):
    def test_a_bounds_check_before_an_indirect_branch_is_a_table(self):
        self.assertEqual(equiv_test.jump_table_bounds(words(CMP_W8_5, B_HI, NOP, NOP, BR_X10)), [5])

    def test_no_indirect_branch_no_table(self):
        self.assertEqual(equiv_test.jump_table_bounds(words(CMP_W8_5, B_HI, NOP, NOP)), [])

    def test_a_signed_or_equality_test_is_not_a_bounds_check(self):
        self.assertEqual(equiv_test.jump_table_bounds(words(CMP_W8_5, B_NE, NOP, BR_X10)), [])

    def test_a_large_bound_is_left_to_the_other_values(self):
        cmp_w8_200 = 0x7103211F  # cmp w8, #0xc8
        self.assertEqual(equiv_test.jump_table_bounds(words(cmp_w8_200, B_HI, BR_X10)), [])


class EqualVariants(unittest.TestCase):
    INSIDE = staticmethod(lambda pc: 0x100 <= pc < 0x200)

    def test_a_word_holding_one_side_takes_the_other(self):
        held = {0x10: 0x5000, 0x18: 0x7}.get
        out = equiv_test.equal_variants([(0x104, 0x5000, 0x6008)], [0x10, 0x18], held, self.INSIDE)
        self.assertEqual(out, [(0x10, 0x6008)])

    def test_both_directions(self):
        held = {0x10: 0x5000, 0x18: 0x6008}.get
        out = equiv_test.equal_variants([(0x104, 0x5000, 0x6008)], [0x10, 0x18], held, self.INSIDE)
        self.assertEqual(sorted(out), [(0x10, 0x6008), (0x18, 0x5000)])

    def test_equal_sides_and_other_code_are_skipped(self):
        held = {0x10: 0x5000}.get
        self.assertEqual(equiv_test.equal_variants([(0x104, 0x5000, 0x5000)], [0x10], held, self.INSIDE), [])
        self.assertEqual(equiv_test.equal_variants([(0x904, 0x5000, 0x6000)], [0x10], held, self.INSIDE), [])

    def test_a_32_bit_compare_matches_the_low_half(self):
        held = {0x10: 0xABCD00000005}.get
        self.assertEqual(equiv_test.equal_variants([(0x104, 5, 9)], [0x10], held, self.INSIDE), [(0x10, 9)])

    def test_bounded(self):
        held = {0x10: 1}.get
        compares = [(0x104, 1, 100 + i) for i in range(40)]
        self.assertEqual(len(equiv_test.equal_variants(compares, [0x10], held, self.INSIDE, limit=4)), 4)


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class MoreInputsEqual(unittest.TestCase):
    """A pointer-equality branch: only a word set to the other side of the run's compare reaches it."""
    A = _Oracle.A

    def test_the_compare_is_made_equal(self):
        import pcode_emu
        w = pcode_emu.BASE + 0x40000
        base = _Run({self.A, self.A + 4, self.A + 12}, {(self.A, False)}, read_words=[w])
        base.compares = [(self.A + 4, 0x5000, 0x6123)]   # the run's word held 0x5000 (_Run.peek)
        oracle = _Oracle([(0, {}, base)])
        oracle.run_original = lambda a, seed, ovr: (
            _Run({a, a + 8, a + 12}, {(a, True)}) if ovr.get(("m", w)) == 0x6123 else _Run({a, a + 4, a + 12}, {(a, False)}))
        self.assertEqual(equiv_test.more_inputs(oracle, self.A), 1)
        self.assertEqual(oracle.runs[-1][1], {("m", w): 0x6123})


if __name__ == "__main__":
    unittest.main()
