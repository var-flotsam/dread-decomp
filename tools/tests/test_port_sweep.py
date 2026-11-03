"""tools/port_sweep.py's diagnostic sorting, source scans and symbol-table readers on made-up
compiler and llvm-nm output (no build, no compiler run)."""

import argparse
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import port_sweep as ps


class Normalise(unittest.TestCase):
    def test_sdk_header(self):
        self.assertEqual(ps.normalise("'nn/os.h' file not found"), "sdk header not found")

    def test_other_header_keeps_its_name(self):
        self.assertEqual(ps.normalise("'foo/bar.h' file not found"), "header not found 'foo/bar.h'")

    def test_static_assert_keeps_text(self):
        self.assertEqual(ps.normalise("static_assert failed due to requirement 'sizeof(A) == 8' \"size of A\""),
                         'static_assert failed "size of A"')
        self.assertEqual(ps.normalise("static_assert failed"), "static_assert failed")

    def test_names_and_numbers_stripped(self):
        a = ps.normalise("no member named 'm_x' in 'CFoo' at offset 0x18 or 24 \"lit\"")
        b = ps.normalise("no member named 'y' in 'CBar' at offset 0x40 or 7 \"other\"")
        self.assertEqual(a, b)
        self.assertEqual(a, "no member named '…' in '…' at offset N or N \"…\"")


class Parse(unittest.TestCase):
    def test_errors_only_and_truncation(self):
        f = str(ps.ROOT / "src" / "x.cpp")
        stderr = "\n".join([
            f"{f}:12:5: error: unknown type name 'Foo'",
            f"{f}:13: warning: unused",
            f"{f}:14:1: note: here",
            "In file included from somewhere:",
            f"{f}:20:2: fatal error: 'nn/os.h' file not found",
            "fatal error: too many errors emitted, stopping now",
            f"{f}:30:1: error: too many errors emitted, stopping now [-ferror-limit=]",
        ])
        errors, truncated = ps.parse(stderr)
        self.assertEqual(errors, [("src/x.cpp", 12, "unknown type name 'Foo'"),
                                  ("src/x.cpp", 20, "'nn/os.h' file not found")])
        self.assertTrue(truncated)

    def test_empty(self):
        self.assertEqual(ps.parse(""), ([], False))


class StripCode(unittest.TestCase):
    def test_comments_and_literals_blanked(self):
        src = 'long a; // long b\nint s = "long"; /* long\nstill long */ char c = \'l\';\nauto w = L"x\\"y";'
        self.assertEqual(ps.strip_code(src), ["long a; ", 'int s = ""; ', " char c = '';", 'auto w = L"";'])

    def test_line_count_kept(self):
        self.assertEqual(len(ps.strip_code("a\n/*\n\n*/\nb")), 5)


class Llp64Checks(unittest.TestCase):
    def kinds(self, line: str) -> list[str]:
        code = ps._LLP64_KEEP.sub("", line)
        return [k for k, p in ps.LLP64_CHECKS if p.search(code)]

    def test_long(self):
        self.assertEqual(self.kinds("unsigned long x;"), ["long"])
        self.assertEqual(self.kinds("long long x; long double y;"), [])

    def test_literals(self):
        self.assertEqual(self.kinds("x = 1ul;"), ["l literal"])
        self.assertEqual(self.kinds("x = 0x10L;"), ["l literal"])
        self.assertEqual(self.kinds("x = 1.5l; y = 10u;"), [])

    def test_wide(self):
        self.assertEqual(self.kinds("wchar_t c;"), ["wchar_t"])
        self.assertEqual(self.kinds('f(L"");'), ["wchar_t"])
        self.assertEqual(self.kinds("wcslen(p);"), ["wchar_t"])
        self.assertEqual(self.kinds("LABEL(x);"), [])

    def test_scan_tree(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "src" / "lifted").mkdir(parents=True)
            (root / "src" / "game").mkdir(parents=True)
            (root / "include").mkdir()
            (root / "src" / "game" / "a.cpp").write_text("int a;\nlong b; // long\n")
            (root / "src" / "lifted" / "b.cpp").write_text("wchar_t w;\n")
            (root / "include" / "types.h").write_text("typedef long s64;\n")  # exempt
            with mock.patch.object(ps, "ROOT", root):
                hits = ps.llp64_scan()
        self.assertEqual([(h["file"], h["line"], h["kind"], h["generated"]) for h in hits],
                         [("src/game/a.cpp", 2, "long", False), ("src/lifted/b.cpp", 1, "wchar_t", True)])


class OwnName(unittest.TestCase):
    def test_parameters_dropped(self):
        self.assertEqual(ps.own_name("fn_71000e027c(base::UnkVector<int>&&)"), "fn_71000e027c")

    def test_template_arguments_dropped(self):
        self.assertEqual(ps.own_name("UnkOutOfLineElement<48u, &fn_710007ecd4>::Get() const"),
                         "UnkOutOfLineElement::Get")

    def test_anonymous_namespace(self):
        self.assertEqual(ps.own_name("(anonymous namespace)::Unk_fn_71005a2dd8::Unk_fn_71005a2dd8()"),
                         "Unk_fn_71005a2dd8::Unk_fn_71005a2dd8")

    def test_fn_addr_pattern(self):
        self.assertEqual(ps.FN_ADDR.search("Unk_fn_71005a2dd8").group(1), "71005a2dd8")
        self.assertIsNone(ps.FN_ADDR.search("fn_7200000000"))


class Targets(unittest.TestCase):
    def test_is_mingw(self):
        self.assertTrue(ps.is_mingw("x86_64-w64-mingw32"))
        self.assertTrue(ps.is_mingw("x86_64-w64-windows-gnu"))
        self.assertFalse(ps.is_mingw("x86_64-linux-gnu"))
        self.assertFalse(ps.is_mingw("x86_64-pc-windows-msvc"))

    def test_stdlib_mode(self):
        ns = argparse.Namespace
        self.assertEqual(ps.stdlib_mode(ns(stdlib=None, system_stdlib=True, target=None)), "host")
        self.assertEqual(ps.stdlib_mode(ns(stdlib="libcxx9", system_stdlib=False, target=None)), "libcxx9")
        self.assertEqual(ps.stdlib_mode(ns(stdlib=None, system_stdlib=False, target=None)), "project")
        self.assertEqual(ps.stdlib_mode(ns(stdlib=None, system_stdlib=False, target="x86_64-w64-mingw32")), "libcxx9")

    def test_stdlib_flags_host_and_project_on_mingw(self):
        self.assertEqual(ps.stdlib_flags("host", "x86_64-linux-gnu"), [])
        with self.assertRaises(SystemExit):
            ps.stdlib_flags("project", "x86_64-w64-mingw32")

    def test_project_includes_drop_nnsdk_and_switch(self):
        fake = ["-I", "/r/include", "-I", "/r/lib/nnsdk/include", "-DSWITCH", "-DNNSDK", "-DFOO", "-isystem", "/x"]
        with mock.patch.object(ps.buildcfg, "project_includes", return_value=fake):
            self.assertEqual(ps.project_includes(), ["-I", "/r/include", "-DFOO", "-isystem", "/x"])

    def test_obj_path_and_rel(self):
        src = ps.ROOT / "src" / "game" / "a.cpp"
        self.assertEqual(ps.obj_path(src, Path("/o")), Path("/o/game/a.o"))
        self.assertEqual(ps.rel(src), "src/game/a.cpp")
        self.assertEqual(ps.rel("/nowhere/else.cpp"), "/nowhere/else.cpp")


class ToolLookup(unittest.TestCase):
    def test_sibling_and_nm(self):
        with tempfile.TemporaryDirectory() as tmp:
            cxx = Path(tmp) / "clang++"
            cxx.write_text("")
            (Path(tmp) / "llvm-nm").write_text("")
            (Path(tmp) / "llvm-objdump").write_text("")
            self.assertEqual(ps.find_nm(str(cxx)), str(Path(tmp) / "llvm-nm"))
            self.assertEqual(ps.sibling_tool(str(cxx), "llvm-objdump"), str(Path(tmp) / "llvm-objdump"))
            with mock.patch.object(ps.shutil, "which", return_value=None):
                self.assertIsNone(ps.sibling_tool(str(cxx), "llvm-cxxfilt"))


def completed(stdout: str, returncode: int = 0):
    return subprocess.CompletedProcess([], returncode, stdout=stdout, stderr="")


class Demangle(unittest.TestCase):
    def test_pairs_in_sorted_order(self):
        with mock.patch.object(ps.subprocess, "run", return_value=completed("a()\nb()\n")) as run:
            self.assertEqual(ps.demangle("cxxfilt", {"_Z1bv", "_Z1av"}), {"_Z1av": "a()", "_Z1bv": "b()"})
        self.assertEqual(run.call_args.kwargs["input"], "_Z1av\n_Z1bv\n")

    def test_failure_keeps_names(self):
        with mock.patch.object(ps.subprocess, "run", return_value=completed("a()\n")):
            self.assertEqual(ps.demangle("cxxfilt", {"_Z1bv", "_Z1av"}), {"_Z1av": "_Z1av", "_Z1bv": "_Z1bv"})
        self.assertEqual(ps.demangle("cxxfilt", set()), {})


class Definitions(unittest.TestCase):
    def test_collisions_and_copies(self):
        a, b, c = (ps.ROOT / "src" / "g" / f"{n}.cpp" for n in "abc")
        oa, ob, oc = "/o/a.o", "/o/b.o", "/o/c.o"
        nm_out = "\n".join([
            f"{oa}:", "0000 T fn_7100000010(int)", "0010 T g_shared", "0020 W inline_fn()", "0030 t fn_7100000020()",
            f"{ob}:", "0000 T fn_7100000010(int)", "0010 D g_shared",
            f"{oc}:", "0000 T fn_7100000020(float)", "0010 T only_here",
        ]) + "\n"
        with mock.patch.object(ps.subprocess, "run", return_value=completed(nm_out)):
            defs = ps.definitions("llvm-nm", [(a, Path(oa)), (b, Path(ob)), (c, Path(oc))])
        self.assertEqual(defs["objects"], 3)
        self.assertEqual(defs["symbol_collisions"],
                         [{"symbol": "fn_7100000010(int)", "sources": ["src/g/a.cpp", "src/g/b.cpp"]},
                          {"symbol": "g_shared", "sources": ["src/g/a.cpp", "src/g/b.cpp"]}])
        self.assertEqual([c["function"] for c in defs["function_copies"]], ["fn_7100000010", "fn_7100000020"])
        self.assertEqual(defs["function_copies"][1]["definitions"],
                         {"src/g/a.cpp": ["fn_7100000020()"], "src/g/c.cpp": ["fn_7100000020(float)"]})

    def test_single_object_has_no_header(self):
        a = ps.ROOT / "src" / "g" / "a.cpp"
        with mock.patch.object(ps.subprocess, "run", return_value=completed("0000 T fn_7100000010()\n")):
            defs = ps.definitions("llvm-nm", [(a, Path("/o/a.o"))])
        self.assertEqual(defs["symbol_collisions"], [])
        self.assertEqual(defs["function_copies"], [])


class CoffComdat(unittest.TestCase):
    def test_comdat_externals(self):
        out = "\n".join([
            "/o/a.o:\tfile format coff-x86-64",
            "SYMBOL TABLE:",
            "[ 0](sec  1)(fl 0x00)(ty   0)(scl   3) (nx 1) 0x00000000 .text",
            "AUX scnlen 0x10 nreloc 0 nlnno 0 checksum 0x0 assoc 0 comdat 0",
            "[ 2](sec  1)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x00000000 strong_fn",
            "[ 3](sec  2)(fl 0x00)(ty   0)(scl   3) (nx 1) 0x00000000 .text$inl",
            "AUX scnlen 0x8 nreloc 0 nlnno 0 checksum 0x0 assoc 0 comdat 2",
            "[ 5](sec  2)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x00000000 inline_fn",
            "[ 6](sec  0)(fl 0x00)(ty  20)(scl   2) (nx 0) 0x00000000 undefined_fn",
            "/o/b.o:\tfile format coff-x86-64",
            "[ 0](sec  1)(fl 0x00)(ty   0)(scl   3) (nx 1) 0x00000000 .data",
            "AUX scnlen 0x8 nreloc 0 nlnno 0 checksum 0x0 assoc 0 comdat 0",
            "[ 2](sec  1)(fl 0x00)(ty   0)(scl   2) (nx 0) 0x00000000 g_data",
        ]) + "\n"
        with mock.patch.object(ps.subprocess, "run", return_value=completed(out)):
            got = ps.coff_comdat("objdump", ["/o/a.o", "/o/b.o"])
        self.assertEqual(dict(got), {"/o/a.o": {"inline_fn"}, "/o/b.o": set()})


if __name__ == "__main__":
    unittest.main()
