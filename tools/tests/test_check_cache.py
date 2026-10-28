"""tools/check.py's caches: the depfile parser, the object cache and the diff cache.

The compile and diff tests build a small fixture with the project compiler and diff it against its
own code (as test_asmdiff.py does), so no game data is involved. Skipped when the project compiler
(tools/fetch_toolchains.py 9.0.1) or pyelftools is not installed; the hosted build job has both.
"""

import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import buildcfg
import fetch_toolchains

try:
    import asmdiff
    import check
except ImportError:  # pyelftools missing
    asmdiff = check = None

HEADER = "extern \"C\" int fn_7100000200(int);\n"
FIXTURE = """
#include "fixture.h"
extern "C" int fn_7100000100(int x) { return fn_7100000200(x + 1) * 3; }
extern "C" int fn_7100000200(int x) { return x ^ 0x55; }
"""


class FakeOriginal:
    def __init__(self, code: dict[int, bytes]):
        self.code = code
        self.sizes = {a: len(c) for a, c in code.items()}

    def function(self, address: int) -> bytes:
        return self.code[address]


@unittest.skipUnless(check, "needs pyelftools")
class Depfile(unittest.TestCase):
    def test_posix(self):
        text = "/b/x.o: /s/x.cpp /i/a.h \\\n  /i/with\\ space.h /i/hash\\#.h /i/dollar$$.h\n"
        self.assertEqual(check.depfile_inputs(text),
                         ["/s/x.cpp", "/i/a.h", "/i/with space.h", "/i/hash#.h", "/i/dollar$.h"])

    def test_windows_target_and_backslashes(self):
        # A drive letter's colon is not the rule's separator; path backslashes stay.
        text = "C:\\b\\x.o: C:\\s\\x.cpp \\\n  C:\\Program\\ Files\\i\\a.h\n"
        with mock.patch.object(check.os.path, "abspath", lambda p: p):
            self.assertEqual(check.depfile_inputs(text), ["C:\\s\\x.cpp", "C:\\Program Files\\i\\a.h"])

    def test_no_rule(self):
        self.assertEqual(check.depfile_inputs(""), [])


@unittest.skipUnless(check and fetch_toolchains.clang_path(buildcfg.COMPILER).is_file(),
                     "needs the project compiler and pyelftools")
class Caches(unittest.TestCase):
    def setUp(self):
        self.dir = Path(tempfile.mkdtemp())
        self.src, self.header = self.dir / "fixture.cpp", self.dir / "fixture.h"
        self.obj = self.dir / "out" / "fixture.o"
        self.src.write_text(FIXTURE, encoding="utf-8")
        self.header.write_text(HEADER, encoding="utf-8")
        cxx = buildcfg.cxx_command
        patches = [mock.patch.object(check, "object_path", lambda version, src: self.obj),
                   mock.patch.object(buildcfg, "cxx_command",
                                     lambda src, obj, extra=None: [*cxx(src, obj, extra), "-I", str(self.dir)])]
        for p in patches:
            p.start()
            self.addCleanup(p.stop)
        check._input_digests.clear()

    def build(self, expect_compile: bool) -> Path:
        real = subprocess.run
        calls = []

        def run(cmd, *a, **kw):
            calls.append(cmd)
            return real(cmd, *a, **kw)
        with mock.patch.object(check.subprocess, "run", run):
            obj = check.build("test", self.src)
        self.assertEqual(bool(calls), expect_compile)
        check._input_digests.clear()  # a new run
        return obj

    def test_object_cache(self):
        self.build(expect_compile=True)
        self.assertTrue(check.inputs_path(self.obj).is_file())
        self.build(expect_compile=False)
        self.header.write_text(HEADER + "// changed\n", encoding="utf-8")   # an included header
        self.build(expect_compile=True)
        self.build(expect_compile=False)
        self.obj.write_bytes(self.obj.read_bytes() + b"\0")                 # the object itself
        self.build(expect_compile=True)
        with mock.patch.object(check.subprocess, "run", wraps=subprocess.run) as run:
            check.build("test", self.src, cache=False)                      # --no-cache
        self.assertTrue(run.called)

    def test_diff_cache(self):
        obj = self.build(expect_compile=True)
        fns = asmdiff.read_object(obj)
        original = FakeOriginal({int(f.symbol[3:], 16): f.code for f in fns})
        pairs = [(self.src, obj)]
        first, reused = check.diff_all(pairs, original, {}, "inputs-a", True, cache=True)
        self.assertEqual(reused, 0)
        self.assertTrue(all(e["matching"] for e in first[self.src]))
        again, reused = check.diff_all(pairs, original, {}, "inputs-a", True, cache=True)
        self.assertEqual((again, reused), (first, 1))
        _, reused = check.diff_all(pairs, original, {}, "inputs-b", True, cache=True)   # other inputs
        self.assertEqual(reused, 0)
        _, reused = check.diff_all(pairs, original, {}, "inputs-b", True, cache=False)  # --no-cache
        self.assertEqual(reused, 0)
        # The cache is keyed on the object's bytes: a different object is diffed again.
        changed = {a: bytes([c[0] ^ 1]) + c[1:] for a, c in original.code.items()}
        obj.write_bytes(obj.read_bytes() + b"\0")
        fresh, reused = check.diff_all(pairs, FakeOriginal(changed), {}, "inputs-b", True, cache=True)
        self.assertEqual(reused, 0)
        self.assertFalse(any(e["matching"] for e in fresh[self.src]))


if __name__ == "__main__":
    unittest.main()
