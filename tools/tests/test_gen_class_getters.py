"""tools/gen_class_getters.py on a made-up function list in a temporary directory (no game data)."""

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import gen_class_getters
from dread import functions

HEADER_ROW = "address,size,name,name_source,subsystem,status\n"


class Getters(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self.tmp.name)
        patcher = mock.patch.object(functions, "FUNCTIONS", self.dir)
        patcher.start()
        self.addCleanup(patcher.stop)
        self.addCleanup(self.tmp.cleanup)

    def write(self, name: str, rows: list[str]) -> None:
        (self.dir / name).write_text(HEADER_ROW + "".join(r + "\n" for r in rows), encoding="utf-8")

    def test_only_reflection_class_getters_sorted(self):
        self.write("main_01.csv", [
            "0x7100100020,64,CBar::refl_GetClass,reflection,game,U",
            "0x7100100010,64,CBar::refl_InitClass,reflection,game,U",
            "0x7100100030,64,CFake::refl_GetClass,string,game,U",
        ])
        self.write("main_00.csv", [
            "0x7100000010,64,base::core::CFoo::refl_GetClass,reflection,engine/base,O",
            "0x7100000020,64,fn_7100000020,unknown,unknown,U",
        ])
        self.assertEqual(gen_class_getters.getters(),
                         [(0x7100000010, "base::core::CFoo"), (0x7100100020, "CBar")])

    def test_duplicate_address_stops(self):
        self.write("main_00.csv", ["0x7100000010,64,CA::refl_GetClass,reflection,game,U"])
        self.write("main_01.csv", ["0x7100000010,64,CB::refl_GetClass,reflection,game,U"])
        with self.assertRaises(SystemExit):
            gen_class_getters.getters()

    def test_empty_class_name_stops(self):
        self.write("main_00.csv", ["0x7100000010,64,::refl_GetClass,reflection,game,U"])
        with self.assertRaises(SystemExit):
            gen_class_getters.getters()

    def test_newline_in_class_name_stops(self):
        self.write("main_00.csv", ['0x7100000010,64,"C\nA::refl_GetClass",reflection,game,U'])
        with self.assertRaises(SystemExit):
            gen_class_getters.getters()

    def test_no_files(self):
        self.assertEqual(gen_class_getters.getters(), [])


class Render(unittest.TestCase):
    def test_declarations_after_header(self):
        text = gen_class_getters.render([(0x7100000010, "CFoo"), (0x7100100020, "base::CBar")])
        self.assertTrue(text.startswith(gen_class_getters.HEADER))
        self.assertTrue(text.endswith("\n") and not text.endswith("\n\n"))
        lines = text[len(gen_class_getters.HEADER):].strip("\n").split("\n")
        self.assertEqual(lines, [
            "const base::reflection::CClass* fn_7100000010();  // CFoo::refl_GetClass",
            "const base::reflection::CClass* fn_7100100020();  // base::CBar::refl_GetClass",
        ])

    def test_address_padded_to_ten_digits(self):
        text = gen_class_getters.render([(0x10, "CLow")])
        self.assertIn("fn_0000000010()", text)

    def test_header_forward_declares_cclass(self):
        self.assertIn("#pragma once", gen_class_getters.HEADER)
        self.assertIn("class CClass;", gen_class_getters.HEADER)

    def test_deterministic(self):
        rows = [(0x7100000010, "CFoo")]
        self.assertEqual(gen_class_getters.render(rows), gen_class_getters.render(list(rows)))


if __name__ == "__main__":
    unittest.main()
