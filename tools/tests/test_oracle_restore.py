"""pcode_emu's Machine.store: a heap write that puts back what a byte held is no change only when no
call event came between the reference (the value loaded, or the byte before the function's first
write) and the store. No game data: hand-built Machines with one heap page."""

import unittest

from tools import pcode_emu

P = 0x7200000000  # a heap address (a seeded page, here preset)


def machine() -> pcode_emu.Machine:
    m = pcode_emu.Machine.__new__(pcode_emu.Machine)
    m.pages = {P // pcode_emu.PAGE: bytearray(pcode_emu.PAGE)}
    m.pages[P // pcode_emu.PAGE][P % pcode_emu.PAGE:P % pcode_emu.PAGE + 4] = (0x11223344).to_bytes(4, "little")
    m.dirty, m.before, m._loaded, m._ckpt, m._touched = {}, {}, None, {}, set()
    m._all_loads, m.putback, m.putback_ev, m.stale, m.stale_ev = set(), {}, {}, set(), {}
    m._loaded_before, m.unsure, m.unsure_ev = set(), set(), {}
    m.undef_mem, m.clipped, m.arg_slots = set(), False, None
    m.stack_written, m.frame_record, m.saves, m.entry_saved = set(), set(), set(), set()
    m.arg_slots, m.own, m.track, m.lift = None, (0, 0), False, None
    m.events, m.snapshot_bytes, m.calls_room, m.scale, m.cut, m.steps = [], 0, 0, 1, None, 0
    m.max_events = lambda: 99
    return m


def call(m, target=0x7100001000):
    m.checkpoint(("call", target, (), ()))


def end(m):
    m.checkpoint(("end", "return"))
    return pcode_emu.written(m.events, len(m.events) - 1)


class Restore(unittest.TestCase):
    def test_load_store_without_a_call_is_no_change(self):
        # (fn_71002aa518: the value just loaded stored back; a lift may drop the store)
        orig = machine()
        orig.store(P, 4, orig.load(P, 4))
        lift = machine()
        lift.load(P, 4)
        self.assertEqual(end(orig), set())
        self.assertEqual(end(lift), set())

    def test_restore_after_a_call_dropped_by_the_lift_differs(self):
        # v = *p; f(); *p = v under an unrecorded callee: the store is the function's, kept
        orig = machine()
        v = orig.load(P, 4)
        call(orig)
        orig.store(P, 4, v)
        lift = machine()
        lift.load(P, 4)
        call(lift)
        self.assertEqual(end(orig), set(range(P, P + 4)))
        self.assertEqual(end(lift), set())
        self.assertNotEqual(orig.events[-1][1], lift.events[-1][1])

    def test_lift_only_restore_after_a_call_differs(self):
        orig = machine()
        orig.load(P, 4)
        call(orig)
        lift = machine()
        v = lift.load(P, 4)
        call(lift)
        lift.store(P, 4, v)
        self.assertNotEqual(end(orig), end(lift))

    def test_first_write_before_a_call_is_no_reference_after_it(self):
        # *p = 1; f(); *p = old: the restore follows a call, so it is a write too
        m = machine()
        old = m.load(P, 4)
        m.store(P, 4, 1)
        call(m)
        m.store(P, 4, old)
        self.assertEqual(end(m), set(range(P, P + 4)))

    def test_load_after_the_call_is_a_reference(self):
        # f(); v = *p; *p = v: no call between the load and the store, no change
        m = machine()
        call(m)
        m.store(P, 4, m.load(P, 4))
        self.assertEqual(end(m), set())

    def test_a_written_store_stays_no_reference(self):
        # v = *p; f(); *p = 1; *p = v: the first write after the call had no reference, nor has the second
        m = machine()
        v = m.load(P, 4)
        call(m)
        m.store(P, 4, 1)
        m.store(P, 4, v)
        self.assertEqual(end(m), set(range(P, P + 4)))

    def test_own_write_reloaded_after_a_call_stays_written(self):
        # *p = 1; f(); v = *p; *p = v: the byte is still the function's write, not back at its old value
        # (710096b334)
        m = machine()
        m.store(P, 4, 1)
        call(m)
        m.store(P, 4, m.load(P, 4))
        self.assertEqual(end(m), set(range(P, P + 4)))
        lift = machine()
        lift.store(P, 4, 1)
        call(lift)
        lift.load(P, 4)
        self.assertEqual(end(lift), set(range(P, P + 4)))

    def test_a_constant_sharing_a_loaded_byte_is_a_write(self):
        # f(); (void)*p; *p = 0x7fc00044: one byte equals what was loaded, the store is a write in all
        # four, as in a run that never loaded *p (710073ee48)
        orig = machine()
        call(orig)
        orig.load(P, 4)
        orig.store(P, 4, 0x7FC00044)
        lift = machine()
        call(lift)
        lift.store(P, 4, 0x7FC00044)
        self.assertEqual(end(orig), set(range(P, P + 4)))
        self.assertEqual(end(lift), set(range(P, P + 4)))
        self.assertEqual(orig.events[-1][1], lift.events[-1][1])

    def test_a_put_back_against_an_equal_constant_decides_nothing(self):
        # f(); (void)*p; *p = 0 against f(); *p = 0 with *p loaded as 0: store cannot tell the
        # original's constant from a put-back, so the input is a skip (710002a074)
        q = P + 16  # (a word holding 0)
        orig, lift = machine(), machine()
        call(orig), call(lift)
        orig.load(q, 4)
        orig.store(q, 4, 0)
        lift.store(q, 4, 0)
        end(orig), end(lift)
        self.assertTrue(pcode_emu.compare(orig, lift, None).startswith("skip"))
        # (without the put-back: a write only the lift made)
        orig = machine()
        call(orig), end(orig)
        self.assertTrue(pcode_emu.compare(orig, lift, None).startswith("memory at event"))

    def test_a_put_back_against_a_stale_store_differs(self):
        # f(); x = *p; *p = x against x = *p; f(); *p = x: the lift's value predates the call
        orig, lift = machine(), machine()
        x = lift.load(P, 4)
        call(orig), call(lift)
        orig.store(P, 4, orig.load(P, 4))
        lift.store(P, 4, x)
        end(orig), end(lift)
        self.assertTrue(pcode_emu.compare(orig, lift, None).startswith("memory at event"))

    def test_restore_with_a_reread_after_the_call_is_no_agreement(self):
        # old = *p; f(); (void)*p; *p = old against a lift without the restore (and the reverse): the
        # re-read makes the stored value look like a put-back of what was just loaded, but it may be
        # the stale one. The emulated callee leaves *p alone, so no input shows it: never an agreement.
        def restoring():
            m = machine()
            old = m.load(P, 4)
            call(m)
            m.load(P, 4)
            m.store(P, 4, old)
            end(m)
            return m

        def dropping():
            m = machine()
            m.load(P, 4)
            call(m)
            m.load(P, 4)
            end(m)
            return m

        self.assertNotEqual(pcode_emu.compare(restoring(), dropping(), None), "")
        self.assertNotEqual(pcode_emu.compare(dropping(), restoring(), None), "")
        self.assertEqual(pcode_emu.compare(restoring(), restoring(), None), "")

    def test_a_math_call_keeps_the_reference(self):
        m = machine()
        v = m.load(P, 4)
        m.checkpoint(("call", ("import", "sqrtf"), (), ()))
        m.store(P, 4, v)
        self.assertEqual(end(m), set())


if __name__ == "__main__":
    unittest.main()
