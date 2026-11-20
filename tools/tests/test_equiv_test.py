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

    def test_a_template_member_found_by_its_name(self):
        text = ("template <class T>\nstruct CVec {\n    CVec(const CVec& other) : m_n(other.m_n) {\n    }\n"
                "    bool operator==(const CVec& other) const;\n    int m_n;\n};\n"
                "template <class T>\nbool CVec<T>::operator==(const CVec& other) const {\n    return true;\n}\n"
                "template <class T>\nvoid ns::CVec<T>::Reserve(int n) {\n}\n"
                "void user() {\n    CVec<int>(other);\n}\n")
        self.assertEqual(equiv_test.return_type(text, 0x7100000900, "ns::CVec<int>::CVec"), "void")
        self.assertEqual(equiv_test.return_type(text, 0x7100000a00, "ns::CVec<int>::operator=="), "bool")
        self.assertEqual(equiv_test.return_type(text, 0x7100000b00, "ns::CVec<int>::Reserve"), "void")
        self.assertIsNone(equiv_test.return_type(text, 0x7100000c00, "ns::CVec<int>::Missing"))

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

    def stale(self, table, statuses, code=None, functional=frozenset()):
        where = {"0x7100000100": [(self.obj, "fn_7100000100")]}
        return self.check.stale_verdicts(table, statuses, where,
                                         lambda obj, symbol: equiv_test.digest(code or self.code),
                                         functional=functional)

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

    def test_a_behaves_row_needs_a_functional_mark_and_current_code(self):
        table = {0x7100000100: self.row(verdict="behaves")}
        marked = {"0x7100000100"}
        self.assertEqual(self.stale(table, {"0x7100000100": "M"}, functional=marked), [])
        self.assertIn("not marked NON_MATCHING(functional)", self.stale(table, {"0x7100000100": "M"})[0][1])
        self.assertIn("its status is m, not M", self.stale(table, {"0x7100000100": "m"}, functional=marked)[0][1])
        self.assertIn("changed since", self.stale(table, {"0x7100000100": "M"}, words(0xD65F03C0), marked)[0][1])

    def test_a_functional_mark_without_a_behaves_row_fails(self):
        self.assertIn("without a `behaves` verdict",
                      self.stale({}, {"0x7100000100": "M"}, functional={"0x7100000100"})[0][1])
        table = {0x7100000100: self.row(verdict="fail")}
        self.assertEqual(len(self.stale(table, {"0x7100000100": "M"}, functional={"0x7100000100"})), 1)

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


class Mutants(unittest.TestCase):
    """The small breakages a `behaves` verdict must reject (equiv_test.mutants)."""
    TEXT = ("int fn_7100000100(int a, int b);\n"
            "int fn_7100000100(int a, int b) {\n"
            "    // 5 in a comment\n"
            "    if (a < 3) return b - a;\n"
            "    return a == 2 ? Box<int>::get(\"s1\") : p->q;\n"
            "}\n")

    def changed(self):
        span = equiv_test.body_span(self.TEXT, 0x7100000100)
        return [next(m for m, o in zip(t.splitlines(), self.TEXT.splitlines()) if m != o).strip()
                for t in equiv_test.mutants(self.TEXT, span)]

    def test_the_body_of_the_definition_not_the_declaration(self):
        lo, hi = equiv_test.body_span(self.TEXT, 0x7100000100)
        self.assertTrue(self.TEXT[lo:hi].strip().startswith("// 5"))

    def test_constants_comparisons_and_subtractions_change_one_at_a_time(self):
        got = self.changed()
        self.assertIn("if (a <= 3) return b - a;", got)
        self.assertIn("if (a < 2) return b - a;", got)
        self.assertIn("if (a < 3) return a - b;", got)
        self.assertIn('return a != 2 ? Box<int>::get("s1") : p->q;', got)

    def test_comments_strings_templates_and_arrows_are_left_alone(self):
        for line in self.changed():
            self.assertNotIn("6 in a comment", line)
            self.assertNotIn('"s2"', line)
            self.assertNotIn("Box<=int", line)
            self.assertIn("->q", line if "->" in line else "->q")

    def test_no_definition_no_span(self):
        self.assertIsNone(equiv_test.body_span("int fn_7100000100(int a);\n", 0x7100000100))


CMP_W8_6, CMP_W8_3 = 0x7100191F, 0x71000D1F   # cmp w8, #6; cmp w8, #3


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class KillValues(unittest.TestCase):
    """What a targeted search tries first on a mutant that passed (equiv_test.kill_values)."""

    def test_the_changed_constant_and_its_neighbours_come_first(self):
        got = equiv_test.kill_values(words(CMP_W8_5, B_NE), words(CMP_W8_5, B_NE), words(CMP_W8_6, B_NE))
        self.assertEqual(got[:2], [6, 7])
        for v in (4, 5, 8, 0, 1, 2, 0xFFFFFFFF, 0xFFFFFFFFFFFFFFFF):
            self.assertIn(v, got)

    def test_the_original_s_compare_constants_are_tried_even_when_no_immediate_changed(self):
        got = equiv_test.kill_values(words(CMP_W8_3, B_NE), words(CMP_W8_5, B_HI), words(CMP_W8_5, B_HI))
        self.assertIn(3, got)
        self.assertIn(4, got)

    def test_no_value_twice_and_none_negative(self):
        got = equiv_test.kill_values(words(CMP_W8_5), words(CMP_W8_5), words(CMP_W8_6))
        self.assertEqual(len(got), len(set(got)))
        self.assertTrue(all(v >= 0 for v in got))


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class KillCandidates(unittest.TestCase):
    """The inputs a mutant that passed is run on (equiv_test.kill_candidates)."""
    A = _Oracle.A

    def candidates(self, base, values):
        oracle = _Oracle([(0, {}, base)])
        return list(equiv_test.kill_candidates(oracle, self.A, oracle.runs, values))

    def test_the_first_value_is_tried_on_every_input_before_the_second(self):
        import pcode_emu
        w = pcode_emu.BASE + 0x40000
        got = self.candidates(_Run({self.A}, set(), read_words=[w], calls=1), [6, 7])
        firsts = [o for _, o in got[:4]]
        self.assertEqual(firsts, [{("r", 0): 6}, {("r", 1): 6}, {("m", w): 6}, {("c", 1): 6}])
        self.assertEqual(got[4][1], {("r", 0): 7})

    def test_a_run_is_never_tried_twice_and_includes_pairs(self):
        got = self.candidates(_Run({self.A}, set()), [0, 5])
        keys = [(s, tuple(sorted(o.items()))) for s, o in got]
        self.assertEqual(len(keys), len(set(keys)))
        self.assertIn({("r", 0): 5, ("r", 1): 5}, [o for _, o in got])

    def test_a_compare_is_made_equal(self):
        import pcode_emu
        w = pcode_emu.BASE + 0x40000
        base = _Run({self.A}, set(), read_words=[w])
        base.compares = [(self.A + 4, 0x5000, 0x6123)]   # the run's word held 0x5000 (_Run.peek)
        self.assertIn({("m", w): 0x6123}, [o for _, o in self.candidates(base, [1])])


class IrFunction(unittest.TestCase):
    """The optimised IR of one function, without what does not bear on its meaning (equiv_test.ir_function)."""
    LL = ("; ModuleID = 'x.cpp'\n"
          "define i32 @_Z3fooi(i32 %0) local_unnamed_addr #0 {\n"
          "  %2 = icmp slt i32 %0, 3, !dbg !12\n"
          "  %3 = load i32, i32* @g, align 4, !tbaa !5\n"
          "  ret i32 %3 ; the result\n"
          "}\n"
          "define void @_Z3barv() #1 {\n  ret void\n}\n")

    def test_metadata_attributes_and_comments_do_not_count(self):
        other = self.LL.replace("!dbg !12", "!dbg !40").replace("#0", "#7").replace("!tbaa !5", "!tbaa !9")
        self.assertEqual(equiv_test.ir_function(self.LL, "_Z3fooi"), equiv_test.ir_function(other, "_Z3fooi"))

    def test_a_changed_constant_or_predicate_counts(self):
        f = equiv_test.ir_function(self.LL, "_Z3fooi")
        self.assertNotEqual(f, equiv_test.ir_function(self.LL.replace("i32 %0, 3", "i32 %0, 4"), "_Z3fooi"))
        self.assertNotEqual(f, equiv_test.ir_function(self.LL.replace("slt", "sle"), "_Z3fooi"))

    def test_only_the_named_function(self):
        self.assertNotIn("_Z3barv", equiv_test.ir_function(self.LL, "_Z3fooi"))
        self.assertIsNone(equiv_test.ir_function(self.LL, "_Z3bazv"))

    def test_the_program_counts_every_function(self):
        # a mutant in an out-of-line callee (a comparator) leaves the function itself unchanged
        callee = self.LL.replace("ret void", "store i32 1, i32* @g\n  ret void")
        self.assertEqual(equiv_test.ir_function(self.LL, "_Z3fooi"), equiv_test.ir_function(callee, "_Z3fooi"))
        self.assertNotEqual(equiv_test.ir_program(self.LL), equiv_test.ir_program(callee))

    def test_the_program_leaves_out_the_module_name_and_metadata(self):
        other = ("; ModuleID = 'y_2.cpp'\nsource_filename = \"y_2.cpp\"\n" + self.LL.split("\n", 1)[1]
                 + "!12 = !DILocation(line: 3)\nattributes #0 = { nounwind }\n")
        self.assertEqual(equiv_test.ir_program(self.LL), equiv_test.ir_program(other))
        self.assertIsNone(equiv_test.ir_program("; ModuleID = 'x'\n"))


class PartingWords(unittest.TestCase):
    """Where a mutant's run and the original's read different words (equiv_test.parting_words)."""

    def test_a_word_only_one_run_read_in_the_order_read(self):
        self.assertEqual(equiv_test.parting_words([1, 2, 3], [1, 2, 3, 4, 5]), [4, 5])   # one element further
        self.assertEqual(equiv_test.parting_words([1, 2, 3, 4], [1, 2, 3]), [4])          # one short
        self.assertEqual(equiv_test.parting_words([1, 2], [2, 1]), [])


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class KillFocus(unittest.TestCase):
    A = _Oracle.A

    def test_the_parting_words_come_first_and_late_words_are_set_too(self):
        import pcode_emu
        late = [pcode_emu.BASE + 0x40000 + 8 * i for i in range(40)]
        base = _Run({self.A}, set(), read_words=late)
        oracle = _Oracle([(0, {}, base)])
        got = list(equiv_test.kill_candidates(oracle, self.A, oracle.runs, [7], {id(base): [late[-1]]}))
        self.assertEqual(got[0][1], {("m", late[-1]): 7})
        self.assertIn({("m", late[35]): 7}, [o for _, o in got])   # past the first SYS_READ words
