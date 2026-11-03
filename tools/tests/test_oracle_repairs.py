"""tools/port_lift.py's repair constants (Oracle.originals) against Machine.setup's argument
objects, and pcode_emu.Machine.call's out-parameter override ("o", (call, arg, offset)). No game
data: the objects' addresses come from setup's formula, and the Machine is built by hand.
Skipped when pyelftools or capstone is not installed."""

import unittest

from tools.tests import _path  # noqa: F401

try:
    import gen_lift
    import pcode_emu
    import port_lift
except ImportError:  # pyelftools or capstone missing
    port_lift = None

MIB = 0x100000


def setup_objects(seed: int) -> list[int]:
    """Machine.setup's argument objects x0-x8 of a seed: ARENA + 1 MiB * (seed * 16 + i + 1) + 0x100."""
    return [pcode_emu.ARENA + MIB * (seed * 16 + i + 1) + 0x100 for i in range(9)]


def all_setup_objects() -> set[int]:
    return {p for seed in range(port_lift.MAX_SEEDS) for p in setup_objects(seed)}


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class RepairConstants(unittest.TestCase):
    def test_repair_pointer_objects_clear_of_every_seed(self):
        """REPAIR_POINTER + 1 MiB * k, k up to REPAIRS deep, lies above every seed's argument objects."""
        highest = max(all_setup_objects()) + MIB  # (each object reaches up to the next MiB)
        self.assertGreater(port_lift.REPAIR_POINTER, highest)
        repairs = {port_lift.REPAIR_POINTER + MIB * k for k in range(port_lift.REPAIRS + 2)}
        self.assertFalse(repairs & all_setup_objects())

    def test_aliased_seed_is_the_one_pointer_meets(self):
        """POINTER + 1 MiB * k (k = 1..9) is exactly seed REPAIR_ALIASED_SEED's x(k-1)."""
        seed = port_lift.REPAIR_ALIASED_SEED
        self.assertEqual(seed, 7)
        self.assertEqual([port_lift.POINTER + MIB * k for k in range(1, 10)], setup_objects(seed))

    def test_no_other_seed_aliases_shallow_repairs(self):
        """Up to 16 repairs deep, POINTER's objects meet no seed's but REPAIR_ALIASED_SEED's."""
        shallow = {port_lift.POINTER + MIB * k for k in range(16)}
        for seed in range(port_lift.MAX_SEEDS):
            if seed != port_lift.REPAIR_ALIASED_SEED:
                self.assertFalse(shallow & set(setup_objects(seed)), seed)

    def test_repair_objects_never_meet_their_seeds_arguments(self):
        """repair_object(seed, k), k up to REPAIRS deep, is none of that seed's x0-x8; seed 8 at depth
        17 (POINTER + 17 MiB, its x0) moves to REPAIR_POINTER + 17 MiB."""
        self.assertEqual(port_lift.POINTER + MIB * 17, setup_objects(8)[0])
        self.assertEqual(port_lift.repair_object(8, 17), port_lift.REPAIR_POINTER + MIB * 17)
        for seed in range(port_lift.MAX_SEEDS):
            objs = set(setup_objects(seed))
            for k in range(port_lift.REPAIRS + 2):
                self.assertNotIn(port_lift.repair_object(seed, k), objs, (seed, k))

    def test_repair_objects_stay_put_unless_aliased(self):
        """A depth whose POINTER object meets none of the seed's arguments keeps it; seed 7 keeps
        REPAIR_POINTER's at every depth."""
        for seed in range(port_lift.MAX_SEEDS):
            objs = set(setup_objects(seed))
            for k in range(port_lift.REPAIRS + 2):
                low = port_lift.POINTER + MIB * k
                want = low if seed != port_lift.REPAIR_ALIASED_SEED and low not in objs \
                    else port_lift.REPAIR_POINTER + MIB * k
                self.assertEqual(port_lift.repair_object(seed, k), want, (seed, k))
        self.assertEqual(port_lift.repair_object(8, 16), port_lift.POINTER + MIB * 16)
        self.assertEqual(port_lift.repair_object(8, 26), port_lift.POINTER + MIB * 26)

    def test_repair_depths_get_distinct_objects(self):
        """Each depth's object (base + 1 MiB * len(overrides)) is its own, and the two bases never meet."""
        n = port_lift.REPAIRS + 2
        low = [port_lift.POINTER + MIB * k for k in range(n)]
        high = [port_lift.REPAIR_POINTER + MIB * k for k in range(n)]
        self.assertEqual(len(set(low + high)), 2 * n)
        self.assertLess(max(low) + MIB, min(high))

    def test_pointers_are_in_the_arena(self):
        """POINTER and REPAIR_POINTER are arena addresses with setup's 0x100 offset into their MiB."""
        for p in (port_lift.POINTER, port_lift.REPAIR_POINTER):
            self.assertGreaterEqual(p, pcode_emu.ARENA)
            self.assertEqual(p % MIB, 0x100)

    def test_max_size_above_gen_lift(self):
        """The semantic lift's size cap is at least byte mode's candidates' cap."""
        self.assertGreaterEqual(port_lift.MAX_SIZE, gen_lift.MAX_SIZE)

    def test_cut_repairs_positive_and_within_budget(self):
        """CUT_REPAIRS is a positive part of the REPAIRS budget."""
        self.assertGreater(port_lift.CUT_REPAIRS, 0)
        self.assertLessEqual(port_lift.CUT_REPAIRS, port_lift.REPAIRS)


@unittest.skipIf(port_lift is None, "pyelftools or capstone is not installed")
class OutParameterOverride(unittest.TestCase):
    """Machine.call fills a stack object passed in x0 with made-up words; ("o", key) sets one."""

    OBJ = pcode_emu.ENTRY_SP - 0x40  # a local of 16 bytes

    def machine(self, seed: int, overrides: dict):
        m = pcode_emu.Machine.__new__(pcode_emu.Machine)
        m.regs, m.undef_vn, m.vnarrow, m.state = bytearray(0x10000), set(), {}, {0: "written"}
        m.seed, m.overrides, m.dictionary, m.lift, m.guide = seed, overrides, None, None, None
        m.target_reg, m.calls, m.call_log, m.out_words, m.frame_record = None, 0, [], [], set()
        m.out_at, m.results = {}, []
        m.imports = lambda t: None
        m.facts = lambda t: None
        m.recorded = lambda target, t, modeled: None
        m.checkpoint = lambda event: None
        m.stackref = lambda v: ("stack", (), 16, frozenset(), 16) if pcode_emu.STACK_LO <= v < pcode_emu.ENTRY_SP else v
        m.stored = {}
        m.store = lambda a, size, v, track=True: m.stored.__setitem__(a, v)
        m.set_reg(pcode_emu.X0, 8, self.OBJ)
        return m

    def test_out_words_lists_the_words_written(self):
        """Each 8-byte word of the object is stored and listed as ((call, argument, offset), word)."""
        m = self.machine(0, {})
        m.call(0x7100001000)
        self.assertEqual([k for k, _ in m.out_words], [(1, 0, 0), (1, 0, 8)])
        self.assertEqual({self.OBJ + j: v for (_, _, j), v in m.out_words}, m.stored)

    def test_override_sets_one_word(self):
        """("o", (1, 0, 8)) replaces the second word only; the first stays what the seed gives."""
        plain = self.machine(1, {})
        plain.call(0x7100001000)
        m = self.machine(1, {("o", (1, 0, 8)): port_lift.POINTER})
        m.call(0x7100001000)
        self.assertEqual(m.stored[self.OBJ + 8], port_lift.POINTER)
        self.assertEqual(m.stored[self.OBJ], plain.stored[self.OBJ])
        self.assertIn(((1, 0, 8), port_lift.POINTER), m.out_words)

    def test_override_of_another_call_is_ignored(self):
        """An override keyed to call 2 does not touch call 1's words."""
        plain = self.machine(0, {})
        plain.call(0x7100001000)
        m = self.machine(0, {("o", (2, 0, 0)): 1})
        m.call(0x7100001000)
        self.assertEqual(m.stored, plain.stored)


if __name__ == "__main__":
    unittest.main()
