"""The generated parts of the docs: the handoff's progress table and the command index."""

import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import gen_commands
import progress


def tally(o: int, u: int) -> dict:
    by = {s: {"functions": 0, "bytes": 0} for s in progress.STATUSES}
    by["O"] = {"functions": o, "bytes": o * 10}
    by["U"] = {"functions": u, "bytes": u * 10}
    total = (o + u) * 10
    return {"by_status": by, "functions": o + u, "bytes": total,
            "matching_bytes_pct": 100 * o * 10 / total, "usable_bytes_pct": 100 * o * 10 / total}


class Handoff(unittest.TestCase):
    def write(self, body: str) -> Path:
        d = tempfile.mkdtemp()
        p = Path(d) / "HANDOFF.md"
        p.write_text(body, encoding="utf-8")
        return p

    def test_whole_binary_only_keeps_the_boot_row(self):
        boot = progress.table_row(progress.BOOT, tally(1, 1))
        p = self.write(f"# H\n\n{progress.BEGIN}\nold\n{boot}\n{progress.END}\n\ntail\n")
        progress.update_handoff(p, tally(3, 1), None)
        text = p.read_text(encoding="utf-8")
        self.assertIn(boot, text)
        self.assertIn(progress.table_row(progress.WHOLE, tally(3, 1)), text)
        self.assertTrue(text.startswith("# H\n\n") and text.endswith(f"{progress.END}\n\ntail\n"))
        self.assertNotIn("\nold\n", text)

    def test_with_slice_replaces_the_boot_row(self):
        p = self.write(f"{progress.BEGIN}\n{progress.END}\n")
        progress.update_handoff(p, tally(3, 1), tally(2, 2))
        self.assertIn(progress.table_row(progress.BOOT, tally(2, 2)), p.read_text(encoding="utf-8"))

    def test_missing_markers_refused(self):
        with self.assertRaises(SystemExit):
            progress.update_handoff(self.write("no markers\n"), tally(1, 0), None)


class Commands(unittest.TestCase):
    def test_every_tool_has_a_group_and_the_index_is_current(self):
        text, other = gen_commands.render()
        self.assertEqual(other, [])
        self.assertEqual(gen_commands.OUT.read_text(encoding="utf-8"), text)


if __name__ == "__main__":
    unittest.main()
