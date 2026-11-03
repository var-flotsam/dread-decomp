"""tools/libcheck_lua.py's luaL_Reg table mapping and luaconf.h patching, on hand-written
sources (no game data, no Lua download)."""

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import libcheck_lua as lc

MATH = """
static int math_abs (lua_State *L) { return 1; }
static const luaL_Reg mathlib[] = {
  {"abs",   math_abs},
  {"floor", math_floor},
  {NULL, NULL}
};
"""

BASE = """
static const luaL_Reg base_funcs[] = {
  {"assert", luaB_assert},
  {"print", luaB_print},
  {NULL, NULL}
};
static const luaL_Reg co_funcs[] = {
  {"create", luaB_cocreate},
  {NULL, NULL}
};
"""


def table(*entries):
    return {"entries": [{"name": n, "func": f} for n, f in entries]}


class AddressMap(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.src = Path(self.tmp.name)
        (self.src / "lmath.c").write_text(MATH, encoding="latin-1")
        (self.src / "lbase.c").write_text(BASE, encoding="latin-1")

    def tearDown(self):
        self.tmp.cleanup()

    def test_tables_matched_by_name_sequence(self):
        tables = [table(("abs", "0x7100050000"), ("floor", "0x7100050010")),
                  table(("assert", "0x7100051000"), ("print", "0x7100051010"))]
        got = lc.address_map(self.src, tables, ["lmath.c", "lbase.c"])
        self.assertEqual(got, {"math_abs": 0x7100050000, "math_floor": 0x7100050010,
                               "luaB_assert": 0x7100051000, "luaB_print": 0x7100051010})

    def test_table_with_other_names_or_order_unmatched(self):
        tables = [table(("floor", "0x7100050010"), ("abs", "0x7100050000")),
                  table(("abs", "0x7100050000"))]
        self.assertEqual(lc.address_map(self.src, tables, ["lmath.c"]), {})

    def test_several_arrays_in_one_file(self):
        tables = [table(("create", "0x7100052000"))]
        self.assertEqual(lc.address_map(self.src, tables, ["lbase.c"]), {"luaB_cocreate": 0x7100052000})

    def test_first_binding_wins(self):
        (self.src / "lb.c").write_text(
            'static const luaL_Reg a[] = {{"x", f_x}, {NULL, NULL}};\n'
            'static const luaL_Reg b[] = {{"y", f_x}, {NULL, NULL}};\n', encoding="latin-1")
        tables = [table(("x", "0x10")), table(("y", "0x20"))]
        self.assertEqual(lc.address_map(self.src, tables, ["lb.c"]), {"f_x": 0x10})

    def test_only_listed_libraries_read(self):
        tables = [table(("abs", "0x1"), ("floor", "0x2"))]
        self.assertEqual(lc.address_map(self.src, tables, ["lbase.c"]), {})

    def test_regexes(self):
        bodies = lc.REG_RE.findall(BASE)
        self.assertEqual(len(bodies), 2)
        self.assertEqual(lc.ENTRY_RE.findall(bodies[0]), [("assert", "luaB_assert"), ("print", "luaB_print")])


LUACONF = (
    "#define LUA_NUMBER_DOUBLE\n"
    "#define LUA_NUMBER\tdouble\n"
    '#define LUA_NUMBER_SCAN\t\t"%lf"\n'
    '#define LUA_NUMBER_FMT\t\t"%.14g"\n'
    "#define lua_str2number(s,p)\tstrtod((s), (p))\n"
)


class Prepare(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        root = Path(self.tmp.name)
        self.fetched = root / "fetched"
        (self.fetched / "lua-5.1.4" / "src").mkdir(parents=True)
        self.patches = [mock.patch.object(lc, "ROOT", root),
                        mock.patch.object(lc.fetch_sources, "source_dir", lambda name: self.fetched / name)]
        for p in self.patches:
            p.start()
        self.root = root

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def write_conf(self, text):
        (self.fetched / "lua-5.1.4" / "src" / "luaconf.h").write_text(text, encoding="latin-1")
        (self.fetched / "lua-5.1.4" / "src" / "lmathlib.c").write_text(MATH, encoding="latin-1")

    def test_float_patch_applied_to_a_copy(self):
        self.write_conf(LUACONF)
        out = lc.prepare("5.1.4")
        self.assertEqual(out, self.root / "build" / "libcheck" / "lua" / "lua-5.1.4")
        conf = (out / "luaconf.h").read_text(encoding="latin-1")
        self.assertNotIn("LUA_NUMBER_DOUBLE", conf)
        self.assertIn("#define LUA_NUMBER\tfloat", conf)
        self.assertIn('"%f"', conf)
        self.assertIn('"%.7g"', conf)
        self.assertIn("strtof", conf)
        self.assertTrue((out / "lmathlib.c").is_file())
        # the fetched source is left alone
        self.assertEqual((self.fetched / "lua-5.1.4" / "src" / "luaconf.h").read_text(encoding="latin-1"), LUACONF)

    def test_prepare_replaces_an_old_copy(self):
        self.write_conf(LUACONF)
        out = lc.prepare("5.1.4")
        (out / "stale.o").write_text("x")
        lc.prepare("5.1.4")
        self.assertFalse((out / "stale.o").exists())

    def test_missing_anchor_exits(self):
        self.write_conf(LUACONF.replace("strtod", "lua_strtod"))
        with self.assertRaises(SystemExit):
            lc.prepare("5.1.4")

    def test_missing_source_exits(self):
        with self.assertRaises(SystemExit):
            lc.prepare("5.1.9")


class Configs(unittest.TestCase):
    def test_variants_differ_only_where_named(self):
        base = lc.CONFIGS["c++ game-flags"]
        self.assertIn("-O3", base)
        o2 = lc.CONFIGS["c++ game-flags -O2"]
        self.assertNotIn("-O3", o2)
        self.assertEqual([f if f != "-O2" else "-O3" for f in o2], base)
        self.assertNotIn("-fasynchronous-unwind-tables", lc.CONFIGS["c++ game-flags no-unwind"])
        self.assertIn("-std=gnu99", lc.CONFIGS["c game-flags"])


if __name__ == "__main__":
    unittest.main()
