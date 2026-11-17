"""tools/vtables.py on a made-up image: relocations to tables, tables to families, and the views (no game data)."""

import struct
import unittest

from tools.tests import _path  # noqa: F401
import vtables

B = vtables.BASE
DTOR, UPDATE, DRAW, RET, UPDATE2, DRAW2, OTHER_A, OTHER_B, OTHER_C, LONE = (
    B + 0x1000, B + 0x1010, B + 0x1020, B + 0x1030, B + 0x1040, B + 0x1050, B + 0x1060, B + 0x1070, B + 0x1080,
    B + 0x1090)
SIZES = {DTOR: 40, UPDATE: 200, DRAW: 300, RET: 4, UPDATE2: 220, DRAW2: 64, OTHER_A: 16, OTHER_B: 16, OTHER_C: 16,
         LONE: 12}
BASE_TABLE, DERIVED_TABLE, OTHER_TABLE = B + 0x8000, B + 0x8100, B + 0x8200


def pointers():
    p = {}
    for table, run in ((BASE_TABLE, [DTOR, UPDATE, DRAW, RET]),      # the base class
                       (DERIVED_TABLE, [DTOR, UPDATE2, DRAW2, RET]),  # inherits the destructor, overrides two
                       (OTHER_TABLE, [OTHER_A, OTHER_B, OTHER_C, RET])):  # shares only the folded 4-byte body
        for i, fn in enumerate(run):
            p[table + 8 * i] = fn
    p[B + 0x8300] = LONE             # a lone callback pointer
    p[B + 0x8308] = B + 0x9000       # a pointer to data, not to a function
    return p


def funcs():
    names = {DTOR: ("CBase::~CBase", "reflection", "O"), UPDATE: ("fn_update", "unknown", "O"),
             DRAW: ("fn_draw", "unknown", "U"), RET: ("fn_ret", "unknown", "O"),
             UPDATE2: ("fn_update2", "unknown", "U"), DRAW2: ("fn_draw2", "unknown", "m"),
             OTHER_A: ("fn_a", "unknown", "O"), OTHER_B: ("fn_b", "unknown", "L"), OTHER_C: ("fn_c", "unknown", "O"),
             LONE: ("fn_lone", "unknown", "O")}
    return {a: {"address": f"0x{a:x}", "size": str(SIZES[a]), "name": n, "name_source": src, "subsystem": "game?",
                "status": st} for a, (n, src, st) in names.items()}


def slot_rows():
    tables = vtables.find_tables(pointers(), set(SIZES))
    families = vtables.group_families(tables, SIZES)
    return [{"table": addr, "slot": i, "function": fn, "family": fam}
            for (addr, run), fam in zip(tables, families) for i, fn in enumerate(run)]


def make_elf(relocs, vaddr=0x0):
    """A minimal ELF64: one loaded segment holding a dynamic table and a RELA table."""
    rela = b"".join(struct.pack("<QQq", off, info, addend) for off, info, addend in relocs)
    header, phdrs = 0x40, 2 * 0x38
    dyn_off = header + phdrs
    rela_off = dyn_off + 3 * 16
    dyn = struct.pack("<qQ", vtables.DT_RELA, vaddr + rela_off) + struct.pack("<qQ", vtables.DT_RELASZ, len(rela)) \
        + struct.pack("<qQ", 0, 0)
    size = rela_off + len(rela)
    ehdr = b"\x7fELF\x02\x01\x01" + b"\0" * 9 + struct.pack("<HHIQQQIHHHHHH", 3, 183, 1, 0, header, 0, 0, 0x40, 0x38, 2,
                                                             0, 0, 0)
    load = struct.pack("<IIQQQQQQ", vtables.PT_LOAD, 5, 0, vaddr, vaddr, size, size, 0x1000)
    dynamic = struct.pack("<IIQQQQQQ", vtables.PT_DYNAMIC, 6, dyn_off, vaddr + dyn_off, vaddr + dyn_off, len(dyn),
                          len(dyn), 8)
    return ehdr + load + dynamic + dyn + rela


class RelativePointers(unittest.TestCase):
    def test_module_relative_image_is_rebased_and_other_relocations_are_skipped(self):
        elf = make_elf([(0x8000, vtables.R_AARCH64_RELATIVE, 0x1000), (0x8008, 1025 | (3 << 32), 0),
                        (0x8010, vtables.R_AARCH64_RELATIVE, 0x1010)])
        self.assertEqual(vtables.relative_pointers(elf), {B + 0x8000: B + 0x1000, B + 0x8010: B + 0x1010})

    def test_image_already_at_its_base_keeps_its_addresses(self):
        elf = make_elf([(B + 0x8000, vtables.R_AARCH64_RELATIVE, B + 0x1000)], vaddr=B)
        self.assertEqual(vtables.relative_pointers(elf), {B + 0x8000: B + 0x1000})

    def test_a_file_that_is_not_an_elf_is_refused(self):
        with self.assertRaises(ValueError):
            vtables.relative_pointers(b"NSO0" + b"\0" * 64)


class Tables(unittest.TestCase):
    def test_runs_of_function_pointers_are_tables_and_short_runs_are_not(self):
        tables = vtables.find_tables(pointers(), set(SIZES))
        self.assertEqual([a for a, _ in tables], [BASE_TABLE, DERIVED_TABLE, OTHER_TABLE])
        self.assertEqual(tables[0][1], [DTOR, UPDATE, DRAW, RET])

    def test_min_slots_sets_the_shortest_table(self):
        self.assertEqual(len(vtables.find_tables(pointers(), set(SIZES), min_slots=1)), 4)
        self.assertEqual(vtables.find_tables(pointers(), set(SIZES), min_slots=5), [])

    def test_an_inherited_function_joins_two_tables_and_a_folded_body_does_not(self):
        tables = vtables.find_tables(pointers(), set(SIZES))
        # base and derived share the destructor at slot 0; the third shares only the 4-byte body at slot 3
        self.assertEqual(vtables.group_families(tables, SIZES), [0, 0, 1])

    def test_the_same_function_at_another_slot_does_not_join(self):
        tables = [(B + 0x8000, [DTOR, UPDATE, DRAW]), (B + 0x8100, [UPDATE2, DTOR, DRAW2])]
        self.assertEqual(sorted(vtables.group_families(tables, SIZES)), [0, 1])


class Views(unittest.TestCase):
    def test_summary_counts_distinct_functions_without_library_code(self):
        text = "\n".join(vtables.summary(slot_rows(), funcs(), top=10))
        self.assertIn("tables: 3 in 2 families", text)
        # family 0: destructor, two updates, two draws and the folded body; five of the six are unnamed, two are U
        self.assertIn("| 0 | 2 | 4 | 6 | 5 | 788 | 2 | 520 |", text)
        # family 1: fn_b is library code and is left out
        self.assertIn("| 1 | 1 | 4 | 3 | 3 | 36 | 0 | 0 |", text)

    def test_family_lists_each_slot_with_the_name_some_implementation_has(self):
        lines = vtables.family_view(slot_rows(), funcs(), 0, None)
        self.assertIn("| 0 | 1 | 0 | 1 | 0 | ~CBase |", lines)
        self.assertIn("| 1 | 2 | 2 | 1 | 1 |  |", lines)

    def test_family_slot_lists_the_implementations(self):
        lines = vtables.family_view(slot_rows(), funcs(), 0, 2)
        self.assertIn(f"| 0x{DRAW:x} | fn_draw | 300 | U |", lines)
        self.assertIn(f"| 0x{DRAW2:x} | fn_draw2 | 64 | m |", lines)

    def test_function_lists_siblings_matched_first(self):
        lines = vtables.function_view(slot_rows(), funcs(), UPDATE2, limit=10)
        self.assertIn("family 0, slot 1: 1 siblings, 1 matched (matched first, then nearest in size)", lines)
        self.assertIn(f"| 0x{UPDATE:x} | fn_update | 200 | O |", lines)

    def test_function_in_no_table_says_so(self):
        self.assertIn("in no table", vtables.function_view(slot_rows(), funcs(), LONE, limit=10)[0])

    def test_sibling_map_joins_the_functions_of_one_slot_and_is_empty_without_the_file(self):
        import tempfile
        from pathlib import Path
        from unittest import mock
        with tempfile.TemporaryDirectory() as tmp:
            with mock.patch.object(vtables, "BUILD", Path(tmp)):
                self.assertEqual(vtables.sibling_map("9.9.9"), {})
                vtables.write("9.9.9", [{"table": f"0x{r['table']:x}", "slot": r["slot"],
                                         "function": f"0x{r['function']:x}", "family": r["family"]}
                                        for r in slot_rows()])
                sib = vtables.sibling_map("9.9.9")
        self.assertEqual(sib[UPDATE], {UPDATE, UPDATE2})
        self.assertEqual(sib[DRAW2], {DRAW, DRAW2})
        self.assertNotIn(DTOR, sib)       # one implementation: no sibling
        self.assertNotIn(OTHER_A, sib)

    def test_resolve_takes_an_address_or_a_unique_name(self):
        self.assertEqual(vtables.resolve(f"{DRAW:x}", funcs()), DRAW)
        self.assertEqual(vtables.resolve("fn_draw2", funcs()), DRAW2)
        with self.assertRaises(SystemExit):
            vtables.resolve("fn_missing", funcs())


if __name__ == "__main__":
    unittest.main()
