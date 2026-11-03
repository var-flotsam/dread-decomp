"""tools/nvn_procs.py's instruction decoders on hand-encoded AArch64 words and its function-list
readers on a made-up data/functions/ (no game data)."""

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import nvn_procs as nv


def enc_adrp(rd: int, pc: int, target: int) -> int:
    imm = ((target >> 12) - (pc >> 12)) & ((1 << 21) - 1)
    return 0x90000000 | ((imm & 3) << 29) | ((imm >> 2) << 5) | rd


def enc_add(rd: int, rn: int, imm: int) -> int:
    return 0x91000000 | (imm << 10) | (rn << 5) | rd


def enc_ldr(rt: int, rn: int, off: int) -> int:
    return 0xF9400000 | ((off // 8) << 10) | (rn << 5) | rt


class Adrp(unittest.TestCase):
    def test_forward_page(self):
        pc = 0x7100154780
        self.assertEqual(nv.adrp(enc_adrp(1, pc, 0x7101234000), pc), (1, 0x7101234000))

    def test_backward_page_and_page_of_pc(self):
        pc = 0x7100155ABC
        self.assertEqual(nv.adrp(enc_adrp(8, pc, 0x7100001000), pc), (8, 0x7100001000))
        self.assertEqual(nv.adrp(enc_adrp(3, pc, 0x7100155000), pc), (3, 0x7100155000))

    def test_not_adrp(self):
        self.assertIsNone(nv.adrp(0x10000001, 0x7100000000))  # ADR, not ADRP
        self.assertIsNone(nv.adrp(enc_add(1, 1, 8), 0x7100000000))


class Decoders(unittest.TestCase):
    def test_add_imm(self):
        self.assertEqual(nv.add_imm(enc_add(1, 1, 0x7F8)), (1, 1, 0x7F8))
        self.assertIsNone(nv.add_imm(0x91400421))  # add x1, x1, #1, lsl #12: shifted, refused
        self.assertIsNone(nv.add_imm(0x11000421))  # 32-bit add

    def test_ldr_x(self):
        self.assertEqual(nv.ldr_x(enc_ldr(8, 8, 0xF80)), (8, 8, 0xF80))
        self.assertIsNone(nv.ldr_x(0xB9400108))  # ldr w8, [x8]

    def test_str_x0(self):
        self.assertEqual(nv.str_x0(0xF9000100), 8)      # str x0, [x8]
        self.assertIsNone(nv.str_x0(0xF9000101))        # str x1, [x8]
        self.assertIsNone(nv.str_x0(0xF9000500))        # str x0, [x8, #8]

    def test_cstring(self):
        self.assertEqual(nv.cstring(b"xx nvnFoo\0bar\0", 3), "nvnFoo")


class Branches(unittest.TestCase):
    def test_branch_kinds(self):
        for w in (0x14000002, 0x94000002, 0x54000040, 0xB4000040, 0x35000040, 0x36000040, 0x37000040,
                  0xD63F0100, 0xD61F0100, 0xD65F03C0):
            self.assertTrue(nv.is_branch(w), hex(w))

    def test_not_branches(self):
        for w in (enc_add(1, 1, 8), enc_ldr(8, 8, 0), 0xF9000100, 0xAA0103F3):
            self.assertFalse(nv.is_branch(w), hex(w))


class WritesRegister(unittest.TestCase):
    def test_data_processing_writes_rd(self):
        self.assertTrue(nv.writes_register(enc_add(5, 1, 8), 5))
        self.assertFalse(nv.writes_register(enc_add(5, 1, 8), 1))

    def test_loads_write_rt(self):
        self.assertTrue(nv.writes_register(enc_ldr(5, 9, 8), 5))
        self.assertFalse(nv.writes_register(enc_ldr(5, 9, 8), 9))
        self.assertTrue(nv.writes_register(0x39400125, 5))   # ldrb w5, [x9]
        self.assertTrue(nv.writes_register(0xF8656925, 5))   # ldr x5, [x9, x5]
        self.assertTrue(nv.writes_register(0x58000005, 5))   # ldr x5, literal

    def test_stores_write_nothing(self):
        self.assertFalse(nv.writes_register(0xF9000125, 5))  # str x5, [x9]
        self.assertFalse(nv.writes_register(0xF9000125, 9))
        self.assertFalse(nv.writes_register(0xB9000125, 5))  # str w5, [x9]

    def test_writeback_writes_base(self):
        self.assertTrue(nv.writes_register(0xF8408D25, 9))   # ldr x5, [x9, #8]!
        self.assertTrue(nv.writes_register(0xF8408D25, 5))
        self.assertTrue(nv.writes_register(0xF8008D25, 9))   # str x5, [x9, #8]!
        self.assertFalse(nv.writes_register(0xF8008D25, 5))

    def test_pairs(self):
        self.assertTrue(nv.writes_register(0xA9400861, 1))   # ldp x1, x2, [x3]
        self.assertTrue(nv.writes_register(0xA9400861, 2))
        self.assertFalse(nv.writes_register(0xA9400861, 3))
        self.assertTrue(nv.writes_register(0xA9C00861, 3))   # ldp x1, x2, [x3, #0]!
        self.assertTrue(nv.writes_register(0xA8C00861, 3))   # ldp x1, x2, [x3], #0
        self.assertTrue(nv.writes_register(0xA9800861, 3))   # stp x1, x2, [x3, #0]!
        self.assertFalse(nv.writes_register(0xA9800861, 1))
        self.assertFalse(nv.writes_register(0x6D400861, 1))  # ldp d1, d2, [x3]: SIMD
        self.assertFalse(nv.writes_register(0xA9000861, 1))  # stp x1, x2, [x3]


class FunctionList(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        (root / "data" / "functions").mkdir(parents=True)
        header = "address,size,name,name_source,subsystem,status\n"
        (root / "data" / "functions" / "main_01.csv").write_text(
            header + "0x7100154770,512,fn_7100154770,unknown,game,U\n0x7100100000,16,fn_7100100000,unknown,game,U\n")
        (root / "data" / "functions" / "main_00.csv").write_text(header + "0x7100000100,8,fn_7100000100,unknown,game,U\n")
        self.patch = mock.patch.object(nv, "ROOT", root)
        self.patch.start()

    def tearDown(self):
        self.patch.stop()
        self.tmp.cleanup()

    def test_loader_size(self):
        self.assertEqual(nv.loader_size(), 512)

    def test_function_starts_sorted(self):
        self.assertEqual(nv.function_starts(), [0x7100000100, 0x7100100000, 0x7100154770])

    def test_loader_missing(self):
        (Path(self.tmp.name) / "data" / "functions" / "main_01.csv").unlink()
        with self.assertRaises(SystemExit):
            nv.loader_size()


if __name__ == "__main__":
    unittest.main()
