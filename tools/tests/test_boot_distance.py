"""tools/boot_distance.py's parsing and report, on made-up run output (no gdb, no dump)."""

import unittest

from tools.tests import _path  # noqa: F401
import boot_distance as bd


class Log(unittest.TestCase):
    def test_traps_in_order_once_each(self):
        log = bd.parse_log([
            "dread: trap 1 (continuing): fn_71000a4598",
            "dread: trap 2 (continuing): nn::hid::GetNpadState",
            "dread: trap 3 (continuing): fn_71000a4598, mismatched call",
            "dread: trap 1 (continuing): fn_71000a4598",
            "dread: Segmentation fault at address 0x10 (near null), instruction 0x1234",
        ])
        self.assertEqual([t["kind"] for t in log["traps"]], ["function", "import", "mismatched"])
        self.assertEqual(log["fault"], {"signal": "Segmentation fault", "address": "0x10"})
        self.assertNotIn("0x1234", str(log))  # the instruction word is never kept

    def test_libc_abort_message(self):
        log = bd.parse_log(["dread: trap 1 (continuing): fn_710008a648", "munmap_chunk(): invalid pointer"])
        self.assertEqual(log["libc"], "munmap_chunk(): invalid pointer")
        self.assertIsNone(bd.parse_log(["free space: 3"])["libc"])

    def test_aborting_trap(self):
        log = bd.parse_log(["dread: call to a function with no source yet: fn_71000a4598 (see data/functions/)"])
        self.assertEqual(log["stop"], "fn_71000a4598")


class Events(unittest.TestCase):
    def test_milestones_lifts_and_end(self):
        reached, lifted, end = bd.parse_events([
            "0.100 m:nninitStartup 0x7173001000",
            "0.200 l:7100001714 0x7173002000",
            "0.300 m:nnMain 0x7173003000",
            "0.400 l:7100001714 0x7173002000",
            'end {"signal": "SIGSEGV", "seconds": 1.0}',
        ])
        self.assertEqual(set(reached), {"nninitStartup", "nnMain"})
        self.assertEqual(lifted, {0x7100001714})
        self.assertEqual(end["signal"], "SIGSEGV")
        self.assertEqual(bd.MILESTONES[bd.furthest(reached)][0], "nnMain")

    def test_furthest_is_by_list_order(self):
        self.assertEqual(bd.furthest({}), -1)
        last = bd.MILESTONES[-1][0]
        self.assertEqual(bd.furthest({last: 9.0, bd.MILESTONES[0][0]: 1.0}), len(bd.MILESTONES) - 1)

    def test_milestone_by_key_or_label(self):
        key, label = bd.MILESTONES[2][:2]
        self.assertEqual(bd.milestone_index(key), 2)
        self.assertEqual(bd.milestone_index(label), 2)
        self.assertIsNone(bd.milestone_index("title screen"))


class Report(unittest.TestCase):
    def test_short_report(self):
        log = bd.parse_log([f"dread: trap {i} (continuing): fn_71000{i:05x}" for i in range(1, 40)])
        r = bd.summarize({"nninitStartup": 0.1}, 3, 10, log, {"timeout": 60})
        lines = bd.report(r)
        self.assertTrue(lines[0].startswith("boot distance: nninitStartup done (1 of "))
        self.assertIn("39 traps, 3 of 10 lifts ran, time-out after 60s", lines[0])
        self.assertLessEqual(len(lines), 21)
        self.assertIn("(+31 more)", lines[-1])


if __name__ == "__main__":
    unittest.main()
