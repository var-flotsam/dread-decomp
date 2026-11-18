"""buildcfg.source_flags: a source's `// build-flags:` line (no game data)."""
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import buildcfg


class SourceFlags(unittest.TestCase):
    def write(self, text: str) -> Path:
        d = tempfile.mkdtemp()
        p = Path(d) / "x.cpp"
        p.write_text(text, encoding="utf-8")
        return p

    def test_none(self):
        self.assertEqual(buildcfg.source_flags(self.write("// A unit.\nvoid f() {}\n")), [])

    def test_default_no_strict_aliasing(self):
        p = self.write("// A unit.\nvoid f() {}\n")
        self.assertIn("-fno-strict-aliasing", buildcfg.cxx_command(p, Path("x.o")))

    def test_strict_aliasing(self):
        p = self.write("// build-flags: strict-aliasing\n// A unit.\nvoid f() {}\n")
        self.assertEqual(buildcfg.source_flags(p), ["-fstrict-aliasing"])
        cmd = buildcfg.cxx_command(p, Path("x.o"))
        # The source's flag comes after the default, so it wins.
        self.assertGreater(cmd.index("-fstrict-aliasing"), cmd.index("-fno-strict-aliasing"))

    def test_unknown(self):
        with self.assertRaises(ValueError):
            buildcfg.source_flags(self.write("// build-flags: no-inline\n"))

    def test_only_the_head(self):
        p = self.write("\n" * buildcfg.SOURCE_FLAGS_LINES + "// build-flags: strict-aliasing\n")
        self.assertEqual(buildcfg.source_flags(p), [])


if __name__ == "__main__":
    unittest.main()
