"""tools/nearcopy.py's normalisation, reference extraction and ranking on hand-assembled
AArch64 words (no game data)."""

import contextlib
import io
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import nearcopy

RET = 0xD65F03C0
NOP = 0xD503201F


def bl(pc, target):
    return 0x94000000 | (((target - pc) >> 2) & 0x3FFFFFF)


def b(pc, target):
    return 0x14000000 | (((target - pc) >> 2) & 0x3FFFFFF)


def adrp(rd, pc, target):
    imm = ((target & ~0xFFF) - (pc & ~0xFFF)) >> 12
    return 0x90000000 | ((imm & 3) << 29) | (((imm >> 2) & 0x7FFFF) << 5) | rd


def add_imm(rd, rn, imm):
    return 0x91000000 | (imm << 10) | (rn << 5) | rd


def ldr_x(rt, rn, off):
    return 0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt


def str_x(rt, rn, off):
    return 0xF9000000 | ((off // 8) << 10) | (rn << 5) | rt


def ldr_w(rt, rn, off):
    return 0xB9400000 | ((off // 4) << 10) | (rn << 5) | rt


def movz(rd, imm):
    return 0xD2800000 | (imm << 5) | rd


def b_cond(off, cond=0):
    return 0x54000000 | (((off >> 2) & 0x7FFFF) << 5) | cond


def cbz(rt, off):
    return 0xB4000000 | (((off >> 2) & 0x7FFFF) << 5) | rt


def code(*words):
    return struct.pack(f"<{len(words)}I", *words)


class Norm(unittest.TestCase):
    def test_masks_branch_and_call_targets(self):
        a = nearcopy.norm(code(bl(0, 0x1000), b(4, 0x2000), b_cond(8), cbz(0, 8)))
        z = nearcopy.norm(code(bl(0, 0x5000), b(4, 0x9000), b_cond(16), cbz(0, 20)))
        self.assertEqual(a, z)

    def test_masks_adrp_page_add_and_unsigned_offsets(self):
        a = nearcopy.norm(code(adrp(8, 0, 0x10000), add_imm(8, 8, 0x10), ldr_x(0, 8, 0x18), str_x(1, 0, 8)))
        z = nearcopy.norm(code(adrp(8, 0, 0x70000), add_imm(8, 8, 0x40), ldr_x(0, 8, 0x80), str_x(1, 0, 16)))
        self.assertEqual(a, z)

    def test_keeps_registers_and_other_immediates(self):
        self.assertNotEqual(nearcopy.norm(code(adrp(8, 0, 0)))[0], nearcopy.norm(code(adrp(9, 0, 0)))[0])
        self.assertNotEqual(nearcopy.norm(code(ldr_x(0, 8, 8))), nearcopy.norm(code(ldr_x(0, 9, 8))))
        self.assertNotEqual(nearcopy.norm(code(movz(0, 1))), nearcopy.norm(code(movz(0, 2))))
        self.assertNotEqual(nearcopy.norm(code(bl(0, 8))), nearcopy.norm(code(b(0, 8))))

    def test_other_words_untouched(self):
        self.assertEqual(nearcopy.norm(code(RET, NOP)), [RET, NOP])


class Refs(unittest.TestCase):
    BASE = 0x7100004000

    def test_calls_outside_the_function_only(self):
        pc = self.BASE
        body = code(bl(pc, 0x7100009000), b(pc + 4, pc + 8), RET, b(pc + 12, 0x7100001000))
        self.assertEqual(nearcopy.refs(pc, body), [0x7100009000, 0x7100001000])

    def test_backward_call(self):
        self.assertEqual(nearcopy.refs(self.BASE, code(bl(self.BASE, self.BASE - 0x100))), [self.BASE - 0x100])

    def test_adrp_add_and_ldr(self):
        pc = self.BASE
        body = code(adrp(8, pc, 0x7100123000), add_imm(0, 8, 0x456),
                    adrp(9, pc + 8, 0x7100200000), ldr_x(1, 9, 0x18), ldr_w(2, 9, 0x24))
        # the 64-bit ldr scales its offset by 8, the 32-bit one by 4; the first load overwrites x1 only
        self.assertEqual(nearcopy.refs(pc, body), [0x7100123456, 0x7100200018, 0x7100200024])

    def test_page_forgotten_once_overwritten(self):
        pc = self.BASE
        body = code(adrp(8, pc, 0x7100123000), add_imm(8, 8, 0x10), add_imm(0, 8, 0x20))
        self.assertEqual(nearcopy.refs(pc, body), [0x7100123010])

        body = code(adrp(8, pc, 0x7100123000), ldr_x(8, 8, 0x10), ldr_x(0, 8, 0x20))
        self.assertEqual(nearcopy.refs(pc, body), [0x7100123010])

    def test_store_keeps_the_page(self):
        pc = self.BASE
        body = code(adrp(8, pc, 0x7100123000), str_x(8, 8, 0x10), ldr_x(0, 8, 0x20))
        self.assertEqual(nearcopy.refs(pc, body), [0x7100123010, 0x7100123020])

    def test_simd_q_load_scaled_by_16(self):
        pc = self.BASE
        ldr_q = 0x3DC00000 | (2 << 10) | (8 << 5) | 0  # ldr q0, [x8, #0x20]
        self.assertEqual(nearcopy.refs(pc, code(adrp(8, pc, 0x7100123000), ldr_q)), [0x7100123020])

    def test_simd_load_keeps_the_page(self):
        pc = self.BASE
        ldr_q = 0x3DC00000 | (2 << 10) | (8 << 5) | 8  # ldr q8, [x8, #0x20]: writes v8, not x8
        self.assertEqual(nearcopy.refs(pc, code(adrp(8, pc, 0x7100123000), ldr_q, add_imm(0, 8, 4))),
                         [0x7100123020, 0x7100123004])

    def test_negative_page(self):
        pc = 0x7100800000
        self.assertEqual(nearcopy.refs(pc, code(adrp(0, pc, 0x7100100000), add_imm(0, 0, 8))), [0x7100100008])


class DefiningFile(unittest.TestCase):
    def grep(self, lines):
        out = subprocess.CompletedProcess([], 0, stdout="".join(l + "\n" for l in lines), stderr="")
        return mock.patch.object(nearcopy.subprocess, "run", lambda *a, **k: out)

    def test_hand_written_before_generated(self):
        with self.grep(["src/lifted/x.cpp:3:void fn_7100000100() {",
                        "src/game/A.cpp:9:void fn_7100000100(int a) {"]):
            self.assertEqual(nearcopy.defining_file(0x7100000100), "src/game/A.cpp")

    def test_declarations_comments_and_callees_skipped(self):
        with self.grep(["src/a.cpp:1:void fn_7100000100();",
                        "src/b.cpp:2:int x = 0;  // see 7100000100",
                        "src/c.cpp:3:    callee_7100000100(x)"]):
            self.assertEqual(nearcopy.defining_file(0x7100000100), "?")

    def test_clone_template_counts(self):
        with self.grep(["src/clones/c.cpp:5:template struct UnkClone_fn_7100000100<int>;"]):
            self.assertEqual(nearcopy.defining_file(0x7100000100), "src/clones/c.cpp")


class FakeOriginal:
    def __init__(self, funcs):
        self.funcs = funcs
        self.sizes = {a: len(c) for a, c in funcs.items()}

    def function(self, a):
        return self.funcs[a]


class Main(unittest.TestCase):
    O1, O2, U1, U2, U3 = 0x7100010000, 0x7100010100, 0x7100020000, 0x7100020100, 0x7100020200

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        (root / "data" / "functions").mkdir(parents=True)
        body = [movz(0, 1), movz(1, 2), movz(2, 3), movz(3, 4), movz(4, 5), movz(5, 6), movz(6, 7), movz(7, 8)]
        o1 = body + [bl(self.O1 + 32, 0x7100000500), RET]
        o2 = [NOP] * 9 + [RET]
        u1 = body + [bl(self.U1 + 32, 0x7100000500), RET]          # identical to O1 after masking, same callee
        u2 = body[:7] + [movz(7, 9), bl(self.U2 + 32, 0x7100000600), RET]  # one word differs, other callee
        u3 = [movz(0, 0x99)] * 10                                    # no chunk in common
        funcs = {self.O1: code(*o1), self.O2: code(*o2), self.U1: code(*u1), self.U2: code(*u2), self.U3: code(*u3)}
        lines = ["address,size,name,name_source,subsystem,status"]
        for a, st in ((self.O1, "O"), (self.O2, "O"), (self.U1, "U"), (self.U2, "U"), (self.U3, "U")):
            lines.append(f"0x{a:x},{len(funcs[a])},fn_{a:x},unknown,game,{st}")
        (root / "data" / "functions" / "main_00.csv").write_text("\n".join(lines) + "\n")
        self.patches = [mock.patch.object(nearcopy, "ROOT", root),
                        mock.patch.object(nearcopy.asmdiff, "Original", lambda version: FakeOriginal(funcs)),
                        mock.patch.object(nearcopy, "defining_file", lambda a: "src/x.cpp")]
        for p in self.patches:
            p.start()

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def run_main(self, *argv):
        out, err = io.StringIO(), io.StringIO()
        with mock.patch.object(sys, "argv", ["nearcopy.py", "--version", "0.0.0", *argv]), \
                contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            self.assertEqual(nearcopy.main(), 0)
        return out.getvalue().splitlines(), err.getvalue()

    def test_ranking(self):
        lines, err = self.run_main("7100020000", "7100030000")
        self.assertIn("2 candidates: 1 at 100%, 1 at 90-99%", err)
        self.assertEqual(lines[1].split(), ["100.0%", "40", "U=0x7100020000", "O=0x7100010000", "refs=0", "src/x.cpp"])
        self.assertEqual(lines[2].split()[0], "90.0%")
        self.assertIn("refs=1", lines[2])

    def test_range_and_min_size(self):
        lines, err = self.run_main("7100020100", "7100020200")
        self.assertEqual(len(lines), 2)
        self.assertIn("1 candidates", err)
        lines, err = self.run_main("7100020000", "7100030000", "--min", "44")
        self.assertIn("0 candidates", err)

    def test_refs_mode(self):
        lines, _ = self.run_main("--refs", f"{self.U2:x}", f"{self.O1:x}")
        self.assertEqual(lines, ["  0 U 0x7100000600  O 0x7100000500   <- differs"])


if __name__ == "__main__":
    unittest.main()
