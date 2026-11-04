"""port_lift --small-out-counts: pcode_emu.Machine's tracking of made-up out-parameter words used as
counts (count_exec, count_keys), Machine.call's small_outs, and Oracle.run_original's rounds and
check()'s hand-over to the lift's run. No game data: Machines are built by hand, as in
test_oracle_compare and test_oracle_repairs. Skipped when pyelftools or capstone is not installed
(port_lift's imports)."""

import types
import unittest

from tools.tests import _path  # noqa: F401

from tools import pcode_emu
from tools.pcode_emu import ENTRY_SP, X0

try:
    import port_lift
except ImportError:  # pyelftools or capstone missing
    port_lift = None

KEY = (1, 0, 8)  # call 1, x0, the word at offset 8: a returned {pointer, count}'s count
WORD = ENTRY_SP - 0x38
BASE = ENTRY_SP - 0x80  # a local array
OPS = {9001: "STORE", 9002: "LOAD", 9003: "COPY", 9004: "INT_LESS", 9005: "INT_EQUAL", 9006: "INT_LEFT",
       9007: "INT_ADD", 9008: "INT_ZEXT", 9009: "SUBPIECE"}
OP = {v: k for k, v in OPS.items()}


class _opcodes:
    def __enter__(self):
        pcode_emu.OPNAMES.update(OPS)

    def __exit__(self, *exc):
        for k in OPS:
            pcode_emu.OPNAMES.pop(k, None)


def xr(n: int, size: int = 8):
    return (2, X0 + 8 * n, size)


def const(v: int, size: int = 8):
    return (0, v, size)


def machine(track: bool = True) -> pcode_emu.Machine:
    """A Machine with the state exec's LOAD, STORE and integer ops touch, its stack pages mapped."""
    m = pcode_emu.Machine.__new__(pcode_emu.Machine)
    page = pcode_emu.PAGE
    m.regs, m.undef_vn, m.state, m.lift, m.vnarrow = bytearray(0x10000), set(), {}, None, {}
    m.vnarrow_at, m.steps, m.target_reg, m.res_read = {}, 0, None, None
    m._loaded, m._all_loads = None, set()
    m.entry_regs, m.save_slots, m.save_src = {}, {}, {}
    m.pages = {n: bytearray(page) for n in range((ENTRY_SP - 0x1000) // page, ENTRY_SP // page + 1)}
    m.dirty, m.track, m.arg_slots, m.uniq, m._touched = {}, False, None, {}, set()
    m.stack_written, m.frame_record, m.out_at, m.own_stack = set(), set(), {}, set()
    m.entry_saved, m.saves, m.undef_mem = set(), set(), set()
    m.ct_vn, m.ct_mem, m.count_uses, m.count_deref = {}, {}, set(), set()
    if track:
        m.track_counts()
    return m


def made_up(m: pcode_emu.Machine, key=KEY, at=WORD, value=0x1234567890ABCDEF) -> None:
    """An out-parameter word at `at`, as Machine.call leaves it: the bytes and their taint."""
    m.store(at, 8, value)
    for i in range(8):
        m.ct_mem[at + i] = {key: 0}


def ex(m, name, out, *ins):
    with _opcodes():
        m.exec(OP[name], out, list(ins))


def load_count(m, reg=8, at=WORD):
    ex(m, "LOAD", xr(reg), const(0), const(at))


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class CountTracking(unittest.TestCase):
    """count_exec: which out-parameter words the run used as a count, and never as a pointer."""

    def test_compared_with_a_register_is_a_count(self):
        """`cmp x20, x8` against the loaded word: a loop's `i < n`."""
        m = machine()
        made_up(m)
        load_count(m)
        ex(m, "INT_LESS", (2, pcode_emu.CY, 1), xr(20), xr(8))
        self.assertEqual(m.count_keys(), {KEY})

    def test_compared_with_a_constant_is_not(self):
        """A null test (`cbz`) or a clamp (`cmp w8, #16`) says nothing about a count."""
        m = machine()
        made_up(m)
        load_count(m)
        ex(m, "INT_EQUAL", (2, pcode_emu.ZR, 1), xr(8), const(0))
        ex(m, "INT_LESS", (2, pcode_emu.CY, 1), xr(8), const(16))
        self.assertEqual(m.count_keys(), set())

    def test_scaled_into_an_address_is_a_count(self):
        """`ldr x9, [x10, x8, lsl #3]`: an index."""
        m = machine()
        made_up(m, value=2)
        load_count(m)
        m.regs[X0 + 80:X0 + 88] = BASE.to_bytes(8, "little")
        ex(m, "INT_LEFT", (1, 0x10, 8), xr(8), const(3))
        ex(m, "INT_ADD", (1, 0x18, 8), xr(10), (1, 0x10, 8))
        ex(m, "LOAD", xr(9), const(0), (1, 0x18, 8))
        self.assertEqual(m.count_keys(), {KEY})

    def test_dereferenced_is_a_pointer(self):
        """A word an address holds as it is (a pointer, `p[i]` or `p != end`) is never made small."""
        m = machine()
        made_up(m, value=BASE)
        load_count(m)
        ex(m, "INT_LESS", (2, pcode_emu.CY, 1), xr(8), xr(9))  # (compared with an end pointer)
        ex(m, "LOAD", xr(9), const(0), xr(8))
        self.assertIn(KEY, m.count_uses)
        self.assertEqual(m.count_keys(), set())

    def test_pointer_plus_scaled_index_keeps_the_pointer_unscaled(self):
        """p + i * 8 with only p made up: p is dereferenced (level 0), not an index."""
        m = machine()
        made_up(m, value=BASE)
        load_count(m)
        ex(m, "INT_LEFT", (1, 0x10, 8), xr(9), const(3))
        ex(m, "INT_ADD", (1, 0x18, 8), xr(8), (1, 0x10, 8))
        ex(m, "LOAD", xr(9), const(0), (1, 0x18, 8))
        self.assertEqual(m.count_keys(), set())
        self.assertEqual(m.count_deref, {KEY})

    def test_carried_through_a_spill_and_a_narrowing(self):
        """Stored to another local, loaded back, its low half taken (w8): still the count."""
        m = machine()
        made_up(m)
        load_count(m)
        ex(m, "STORE", None, const(0), const(ENTRY_SP - 0x10), xr(8))
        ex(m, "LOAD", xr(11), const(0), const(ENTRY_SP - 0x10))
        ex(m, "SUBPIECE", (1, 0x20, 4), xr(11), const(0, 4))
        ex(m, "INT_ZEXT", xr(12), (1, 0x20, 4))
        ex(m, "INT_LESS", (2, pcode_emu.CY, 1), xr(20, 4), xr(12, 4))
        self.assertEqual(m.count_keys(), {KEY})

    def test_overwritten_register_carries_nothing(self):
        m = machine()
        made_up(m)
        load_count(m)
        ex(m, "COPY", xr(8), const(5))
        ex(m, "INT_LESS", (2, pcode_emu.CY, 1), xr(20), xr(8))
        self.assertEqual(m.count_keys(), set())

    def test_overwritten_word_carries_nothing(self):
        """The function's own store over the word: what it loads there is its own value."""
        m = machine()
        made_up(m)
        ex(m, "STORE", None, const(0), const(WORD), xr(3))
        load_count(m)
        ex(m, "INT_LESS", (2, pcode_emu.CY, 1), xr(20), xr(8))
        self.assertEqual(m.count_keys(), set())

    def test_untracked_machine_records_nothing(self):
        """Without track_counts (the default, and the lift's run) exec records nothing."""
        m = machine(track=False)
        made_up(m)
        load_count(m)
        ex(m, "INT_LESS", (2, pcode_emu.CY, 1), xr(20), xr(8))
        self.assertEqual((m.ct_vn, m.count_uses), ({}, set()))
        self.assertFalse(pcode_emu.Machine.count_track)
        self.assertEqual(pcode_emu.Machine.small_outs, {})

    def test_same_values_tracked_or_not(self):
        """Tracking changes no value an op computes."""
        runs = []
        for track in (False, True):
            m = machine(track)
            made_up(m)
            load_count(m)
            m.regs[X0 + 80:X0 + 88] = BASE.to_bytes(8, "little")
            ex(m, "INT_LEFT", (1, 0x10, 8), xr(8), const(3))
            ex(m, "INT_ADD", xr(9), xr(10), (1, 0x10, 8))
            ex(m, "STORE", None, const(0), const(ENTRY_SP - 0x10), xr(9))
            runs.append((bytes(m.regs), {n: bytes(p) for n, p in m.pages.items()}, dict(m.uniq)))
        self.assertEqual(runs[0], runs[1])

    def test_shrink_drops_the_binding(self):
        m = machine()
        m.read_words, m._read_seen = [], set()
        m.shrink()
        self.assertNotIn("exec", m.__dict__)


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class SmallOuts(unittest.TestCase):
    """Machine.call fills a made-up out-parameter word with small_outs' value; ("o", key) still wins."""

    OBJ = ENTRY_SP - 0x40  # a local of 16 bytes

    def machine(self, seed: int, overrides: dict, small: dict | None = None, track: bool = False):
        m = pcode_emu.Machine.__new__(pcode_emu.Machine)
        m.regs, m.undef_vn, m.vnarrow, m.state = bytearray(0x10000), set(), {}, {0: "written"}
        m.vnarrow_at, m.steps = {}, 0
        m.seed, m.overrides, m.dictionary, m.lift, m.guide = seed, overrides, None, None, None
        m.target_reg, m.calls, m.call_log, m.out_words, m.frame_record = None, 0, [], [], set()
        m.out_at, m.results = {}, []
        m.ct_vn, m.ct_mem, m.count_uses, m.count_deref = {}, {}, set(), set()
        m.imports = lambda t: None
        m.facts = lambda t: None
        m.recorded = lambda target, t, modeled: None
        m.checkpoint = lambda event: None
        m.stackref = lambda v: ("stack", (), 16, frozenset(), 16) if pcode_emu.STACK_LO <= v < ENTRY_SP else v
        m.stored = {}
        m.store = lambda a, size, v, track=True: m.stored.__setitem__(a, v)
        if small is not None:
            m.small_outs = small
        if track:
            m.count_track = True
        m.set_reg(X0, 8, self.OBJ)
        return m

    def test_small_word_replaces_the_seeds(self):
        for seed in (0, 1):
            plain = self.machine(seed, {})
            plain.call(0x7100001000)
            m = self.machine(seed, {}, {KEY: 3})
            m.call(0x7100001000)
            self.assertEqual(m.stored, {self.OBJ: plain.stored[self.OBJ], self.OBJ + 8: 3})
            self.assertIn((KEY, 3), m.out_words)

    def test_override_wins(self):
        m = self.machine(0, {("o", KEY): port_lift.POINTER}, {KEY: 3})
        m.call(0x7100001000)
        self.assertEqual(m.stored[self.OBJ + 8], port_lift.POINTER)

    def test_off_is_as_before(self):
        """No small_outs and no tracking: the words and the out_words as the seed gives them."""
        a, b = self.machine(0, {}), self.machine(0, {}, {})
        a.call(0x7100001000)
        b.call(0x7100001000)
        self.assertEqual((a.stored, a.out_words), (b.stored, b.out_words))
        self.assertEqual(a.ct_mem, {})

    def test_tracked_call_marks_its_words(self):
        m = self.machine(0, {}, track=True)
        m.call(0x7100001000)
        self.assertEqual(m.ct_mem[self.OBJ + 8 + 7], {KEY: 0})
        self.assertEqual(m.ct_mem[self.OBJ], {(1, 0, 0): 0})


class _Run:
    """A finished original's run as Oracle.run_original sees it."""

    def __init__(self, keys, small):
        self.keys, self.small_outs = set(keys), dict(small)

    def count_keys(self):
        return self.keys


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class Rounds(unittest.TestCase):
    """Oracle.run_original under small_counts: the original again with its count words small."""

    def oracle(self, plan, small_counts=True):
        """plan: per round, (count keys the run finds, or an exception it raises)."""
        o = port_lift.Oracle.__new__(port_lift.Oracle)
        o.small_counts = small_counts
        o.calls = []

        def one(addr, seed, ovr, scale, small):
            o.calls.append(dict(small))
            r = plan[min(len(o.calls), len(plan)) - 1]
            if isinstance(r, Exception):
                return _Run((), small), r
            return _Run(r, small), None
        o.one_original = one
        return o

    def test_off_runs_once_without_small_words(self):
        o = self.oracle([{KEY}], small_counts=False)
        m = o.run_original(0x7100000000, 0, {})
        self.assertEqual((o.calls, m.small_outs), ([{}], {}))

    def test_count_found_is_run_again_small(self):
        o = self.oracle([{KEY}, {KEY}])
        m = o.run_original(0x7100000000, 4, {})
        want = {KEY: port_lift.small_count(4, KEY)}
        self.assertEqual(o.calls, [{}, want])
        self.assertEqual(m.small_outs, want)
        self.assertLess(want[KEY], port_lift.SMALL_COUNT)

    def test_rounds_add_words_found_later(self):
        k2 = (2, 1, 0)
        o = self.oracle([{KEY}, {KEY, k2}, {KEY, k2}])
        m = o.run_original(0x7100000000, 0, {})
        self.assertEqual(set(m.small_outs), {KEY, k2})
        self.assertEqual(len(o.calls), 3)

    def test_rounds_are_bounded(self):
        o = self.oracle([{(n, 0, 0)} for n in range(1, 20)])
        o.run_original(0x7100000000, 0, {})
        self.assertEqual(len(o.calls), port_lift.COUNT_ROUNDS + 1)

    def test_override_keeps_its_word(self):
        o = self.oracle([{KEY}])
        m = o.run_original(0x7100000000, 0, {("o", KEY): 1})
        self.assertEqual((o.calls, m.small_outs), ([{}], {}))

    def test_inconclusive_round_keeps_the_one_before(self):
        o = self.oracle([{KEY}, pcode_emu.Inconclusive("store in the caller's frame")])
        m = o.run_original(0x7100000000, 0, {})
        self.assertEqual(m.small_outs, {})

    def test_inconclusive_first_run_with_small_words_that_end(self):
        """A run that walked into the caller's frame on a 64-bit count, then ended on a small one."""
        err = pcode_emu.Inconclusive("store in the caller's frame")
        o = self.oracle([err, set()])
        o.one_original = self._first_raises_with_keys(o, err)
        m = o.run_original(0x7100000000, 0, {})
        self.assertEqual(set(m.small_outs), {KEY})

    def _first_raises_with_keys(self, o, err):
        def one(addr, seed, ovr, scale, small):
            o.calls.append(dict(small))
            if len(o.calls) == 1:
                return _Run({KEY}, small), err
            return _Run({KEY}, small), None
        return one

    def test_inconclusive_throughout_raises(self):
        err = pcode_emu.Inconclusive("store in the caller's frame")
        o = self.oracle([err])
        o.one_original = lambda addr, seed, ovr, scale, small: (_Run({KEY} if not small else (), small),
                                                                 err if not small else pcode_emu.Inconclusive("x"))
        with self.assertRaises(pcode_emu.Inconclusive) as cm:
            o.run_original(0x7100000000, 0, {})
        self.assertIs(cm.exception, err)

    def test_default_is_off(self):
        self.assertFalse(port_lift.Oracle.small_counts)


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class LiftGetsTheSmallWords(unittest.TestCase):
    """check(): the lift's run gets the original's small_outs, so both runs see the same input."""

    def run_check(self, small):
        o = port_lift.Oracle.__new__(port_lift.Oracle)
        o.preset_orig, o.dicts = {}, {0x7100000000: ()}
        mo = types.SimpleNamespace(visited={1}, cut=None, noret_call=None, call_log=[], events=[],
                                   small_outs=small)
        o.originals = lambda addr: [(0, {}, mo)]
        made = []

        def machine(*a, **k):
            ml = types.SimpleNamespace(run=lambda start: None)
            made.append(ml)
            return ml
        o.machine = machine
        lo = types.SimpleNamespace(functions={0x7100000000: (0x70C0000000, 16)}, unresolved={},
                                   text=(0x70C0000000, 0x70C0000010), guards=())
        emu = port_lift.pcode_emu  # (port_lift's own import of the module)
        saved = emu.compare
        emu.compare = lambda a, b, ret: ""
        try:
            v = o.check(0x7100000000, lo, None)
        finally:
            emu.compare = saved
        return v, made[0]

    def test_small_words_passed(self):
        v, ml = self.run_check({KEY: 2})
        self.assertEqual(v[0], "pass")
        self.assertEqual(ml.small_outs, {KEY: 2})

    def test_none_leaves_the_lift_alone(self):
        _, ml = self.run_check({})
        self.assertFalse(hasattr(ml, "small_outs"))


if __name__ == "__main__":
    unittest.main()
