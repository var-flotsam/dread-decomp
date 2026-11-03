"""tools/shape_census.py's grouping of undefined U functions by shape, on hand-assembled
AArch64 words and a made-up function list (no game data)."""

import contextlib
import io
import struct
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import shape_census

RET = 0xD65F03C0


def code(*words):
    return struct.pack(f"<{len(words)}I", *words)


def movz(rd, imm):
    return 0xD2800000 | (imm << 5) | rd


def ldr_x(rt, rn, off):
    return 0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt


def bl(pc, target):
    return 0x94000000 | (((target - pc) >> 2) & 0x3FFFFFF)


class FakeOriginal:
    def __init__(self, funcs):
        self.funcs = funcs
        self.sizes = {a: len(c) for a, c in funcs.items()}

    def function(self, a):
        return self.funcs[a]


# Family A: mov + ldr + ret (12 bytes), 3 members with varying operands. Family B: mov + bl + ret
# (12 bytes, other mnemonics), 2 members. Family C: 4 movs + ret (20 bytes), 2 members.
FA = [0x7100040000, 0x7100040010, 0x7100040020]
FB = [0x7100041000, 0x7100041010]
FC = [0x7100042000, 0x7100042020]
DEFINED, CLONED, MATCHED, NOT_IN_EH = 0x7100043000, 0x7100043010, 0x7100043020, 0x7100043030


def funcs():
    out = {}
    for i, a in enumerate(FA):
        out[a] = code(movz(0, i + 1), ldr_x(1, 0, 8), RET)
    for a in FB:
        out[a] = code(movz(0, 1), bl(a + 4, 0x7100000500), RET)
    for i, a in enumerate(FC):
        out[a] = code(movz(0, i), movz(1, 0), movz(2, 0), movz(3, 0), RET)
    for a in (DEFINED, CLONED, MATCHED):
        out[a] = code(movz(0, 7), ldr_x(1, 0, 16), RET)
    return out


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        (root / "data" / "functions").mkdir(parents=True)
        clones = root / "src" / "clones"
        clones.mkdir(parents=True)
        (clones / "UnkClone_000.cpp").write_text(f"template struct UnkClone_fn_{CLONED:x}<int>;\n")
        lines = ["address,size,name,name_source,subsystem,status"]
        for a in [*FA, *FB, *FC, DEFINED, CLONED, NOT_IN_EH]:
            lines.append(f"0x{a:x},12,fn_{a:x},unknown,game,U")
        lines.append(f"0x{MATCHED:x},12,fn_{MATCHED:x},unknown,game,O")
        (root / "data" / "functions" / "main_00.csv").write_text("\n".join(lines) + "\n")
        self.orig = FakeOriginal(funcs())
        self.patches = [mock.patch.object(shape_census, "ROOT", root),
                        mock.patch.object(shape_census.gen_clones, "OUT", clones),
                        mock.patch.object(shape_census.gen_clones, "defined_in_src", lambda: {DEFINED}),
                        mock.patch.object(shape_census.asmdiff, "Original", lambda version: self.orig)]
        for p in self.patches:
            p.start()

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def run_main(self, *argv):
        out = io.StringIO()
        with mock.patch.object(sys, "argv", ["shape_census.py", "--version", "0.0.0", *argv]), \
                contextlib.redirect_stdout(out):
            self.assertEqual(shape_census.main(), 0)
        return out.getvalue().splitlines()


class UndefinedU(Fixture):
    def test_skips_defined_cloned_matched_and_unknown_sizes(self):
        self.assertEqual(shape_census.undefined_u(self.orig), [*FA, *FB, *FC])


class Census(Fixture):
    def test_families_ranked_by_total_bytes(self):
        lines = self.run_main("--min", "2")
        self.assertEqual(lines[0], "7 undefined U functions; 3 families of 2+ hold 7 functions, 100 bytes")
        rows = [l.split() for l in lines[2:]]
        self.assertEqual(rows, [[f"{FC[0]:x}", "20", "2", "40"], [f"{FA[0]:x}", "12", "3", "36"],
                                [f"{FB[0]:x}", "12", "2", "24"]])

    def test_min_and_top(self):
        lines = self.run_main("--min", "3")
        self.assertIn("1 families of 3+", lines[0])
        self.assertEqual(len(lines), 3)
        self.assertEqual(len(self.run_main("--min", "1", "--top", "1")), 3)

    def test_max_size(self):
        lines = self.run_main("--min", "2", "--max-size", "12")
        self.assertTrue(lines[0].startswith("5 undefined U functions; 2 families"))

    def test_branch_targets_do_not_split_a_family(self):
        # FB's bl targets the same callee from different pcs; capstone prints the absolute target,
        # which is outside both functions, so the operand strings agree and the mnemonics surely do
        lines = self.run_main("--family", f"{FB[0]:x}")
        self.assertEqual(lines, [f"{FB[0]:x}: 2 functions of 12 bytes (3 instructions) share its mnemonics"])

    def test_family_lists_varying_operands(self):
        lines = self.run_main("--family", f"{FA[1]:x}")
        self.assertEqual(lines[0], f"{FA[1]:x}: 3 functions of 12 bytes (3 instructions) share its mnemonics")
        self.assertEqual(len(lines), 2)
        idx, mnem, count, *_ = lines[1].split(" ", 3)
        self.assertEqual((idx, mnem, count), ("0", "mov", "3"))

    def test_family_rejects_non_start(self):
        with mock.patch.object(sys, "argv", ["shape_census.py", "--version", "0.0.0", "--family", "7100099999"]):
            with self.assertRaises(SystemExit):
                shape_census.main()


if __name__ == "__main__":
    unittest.main()
