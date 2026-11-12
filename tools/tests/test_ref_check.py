"""tools/ref_check.py: comparing what a non-matching function refers to with the original (no game data)."""
import unittest
from dataclasses import dataclass

from tools.tests import _path  # noqa: F401
import ref_check
from ref_check import Refs

try:
    import capstone  # noqa: F401
except ImportError:  # pragma: no cover
    capstone = None

GOT = (0x9000, 0xA000)


@dataclass(frozen=True)
class Reloc:
    type: int
    symbol: str
    addend: int = 0
    section: bool = False


def kinds(found):
    return [k for k, _ in found]


class Compare(unittest.TestCase):
    def test_the_same_references_are_no_finding(self):
        ours = Refs(calls={0x100}, imports={"memcpy"}, got={0x5000}, direct={0x6000})
        theirs = Refs(calls={0x100}, imports={"memcpy"}, direct={0x6000}, cells={0x9008: 0x5000})
        self.assertEqual(ref_check.compare(ours, theirs, got=GOT), [])

    def test_a_global_named_after_its_got_slot(self):
        ours = Refs(got={0x9008})                       # g_unk_<slot>
        theirs = Refs(cells={0x9008: 0x5000})           # the original loads the slot: the object is at 0x5000
        found = ref_check.compare(ours, theirs, got=GOT)
        self.assertEqual(kinds(found), ["slot"])        # (the object it should name is in the same line)
        self.assertIn("0x5000", found[0][1])

    def test_a_pointer_variable_outside_the_got_is_not_a_slot(self):
        ours = Refs(got={0x7000})                       # extern T* g_table[]: reached through the GOT by ours
        theirs = Refs(cells={0x7000: 0x5000})           # read directly by the original
        self.assertEqual(ref_check.compare(ours, theirs, got=GOT), [])

    def test_wrong_callee_and_wrong_global(self):
        ours = Refs(calls={0x100, 0x200}, got={0x5100})
        theirs = Refs(calls={0x100, 0x300}, cells={0x9010: 0x58000})
        found = ref_check.compare(ours, theirs, got=GOT)
        self.assertEqual(kinds(found), ["call", "call", "global", "global"])
        self.assertIn("calls 0x200", found[0][1])
        self.assertIn("the original calls 0x300", found[1][1])

    def test_an_address_close_to_the_other_side_s_is_only_near(self):
        ours = Refs(direct={0x6010})                    # a member, where the original forms the object's start
        theirs = Refs(direct={0x6000, 0x6800})          # and a second member ours reaches by an offset
        found = ref_check.compare(ours, theirs, got=GOT)
        self.assertEqual(kinds(found), ["near", "near", "near"])   # ours' one, and the original's two
        far = ref_check.compare(ours, Refs(direct={0x6000, 0x60000}), got=GOT)
        self.assertEqual(kinds(far), ["near", "near", "global"])

    def test_with_a_name_that_has_no_address_only_ours_is_judged(self):
        ours = Refs(calls={0x200}, unresolved=1)        # one callee could not be placed
        theirs = Refs(calls={0x300}, direct={0x6000}, imports={"memcpy"})
        found = ref_check.compare(ours, theirs, got=GOT)
        self.assertEqual(kinds(found), ["call"])        # what ours names is still wrong; "ours never" is not said
        self.assertIn("calls 0x200", found[0][1])

    def test_the_original_s_strings_and_tables_are_not_missed_references(self):
        theirs = Refs(direct={0x4010, 0x6000})
        found = ref_check.compare(Refs(), theirs, rodata=(0x4000, 0x5000), got=GOT)
        self.assertEqual(len(found), 1)
        self.assertIn("0x6000", found[0][1])

    def test_what_fails(self):
        slot = {"difference": "operations", "findings": [{"kind": "slot", "text": ""}]}
        registers = {"difference": "registers", "findings": [{"kind": "call", "text": ""}, {"kind": "near", "text": ""}]}
        other = {"difference": "control", "findings": [{"kind": "call", "text": ""}]}
        self.assertEqual(len(ref_check.failing(slot)), 1)
        self.assertEqual([f["kind"] for f in ref_check.failing(registers)], ["call"])
        self.assertEqual(ref_check.failing(other), [])


class Ours(unittest.TestCase):
    def test_relocations_become_references(self):
        where = {"fn_7100000100": 0x7100000100, "g_unk_7100005000": 0x7100005000, "g_unk_7100006000": 0x7100006000}
        relocs = {0: Reloc(ref_check.CALL26, "fn_7100000100"),
                  4: Reloc(ref_check.GOT_PAGE, "g_unk_7100005000"),
                  8: Reloc(312, "g_unk_7100005000"),                       # the GOT lo12 half: not a reference
                  12: Reloc(275, "g_unk_7100006000", 8),
                  16: Reloc(275, ".rodata.str1.1", 0, section=True),       # a string: no name
                  20: Reloc(ref_check.JUMP26, "memcpy"),
                  24: Reloc(ref_check.CALL26, "UnkFoo::Bar()")}
        r = ref_check.ours_refs(relocs, where.get, lambda s: s == "memcpy")
        self.assertEqual((r.calls, r.got, r.direct, r.imports, r.unresolved),
                         ({0x7100000100}, {0x7100005000}, {0x7100006008}, {"memcpy"}, 1))

    def test_a_call_of_a_plt_stub_by_its_address_is_a_call_of_the_import(self):
        relocs = {0: Reloc(ref_check.CALL26, "fn_7100000200")}
        r = ref_check.ours_refs(relocs, {"fn_7100000200": 0x7100000200}.get,
                                plt=lambda a: "nn::os::LockMutex" if a == 0x7100000200 else None)
        self.assertEqual((r.calls, r.imports), (set(), {"nn::os::LockMutex"}))


@unittest.skipIf(capstone is None, "capstone is not installed")
class Original(unittest.TestCase):
    def test_an_address_is_followed_through_add_mov_and_a_displacement(self):
        code = b"".join(w.to_bytes(4, "little") for w in (
            0xB0000008,   # adrp x8, 0x2000
            0x91008108,   # add  x8, x8, #0x20
            0xF9400909,   # ldr  x9, [x8, #16]
            0xAA0803F3,   # mov  x19, x8
            0x94000040,   # bl   0x1110      (x8 is lost, x19 is kept)
            0x91044260,   # add  x0, x19, #0x110
            0xF9400909,   # ldr  x9, [x8, #16]   (no longer a known address)
            0xD65F03C0))  # ret
        addresses, targets = ref_check.formed_addresses(code, 0x1000)
        self.assertEqual(addresses, {0x2020, 0x2030, 0x2130})
        self.assertEqual(targets, {0x1110})


if __name__ == "__main__":
    unittest.main()
