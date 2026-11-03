"""tools/unit_move.py's naming, planning and include rewriting on a temporary tree (no game data)."""

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import tu_map
import unit_move


def unit(index, start, end, hint, source="reflection", conf="high"):
    return {"index": index, "start": f"0x{start:x}", "end": f"0x{end:x}", "functions": 1, "bytes": end - start,
            "init_fn": "", "class_hint": hint, "hint_source": source, "confidence": conf,
            "start_i": start, "end_i": end}


class Patterns(unittest.TestCase):
    def test_address_dirs(self):
        for name in ("game_s2", "game_lo", "game_hi", "game_7100a0", "game_7100a0b0c0"):
            self.assertTrue(unit_move.ADDRESS_DIRS.match(name), name)
        for name in ("game", "game_CFoo", "game_s", "engine_s2", "game_7100a"):
            self.assertFalse(unit_move.ADDRESS_DIRS.match(name), name)

    def test_file_names(self):
        self.assertEqual(unit_move.FILE.match("fn_7100000010.cpp").group(1), "7100000010")
        self.assertIsNone(unit_move.FILE.match("fn_7100000010.h"))
        self.assertIsNone(unit_move.FILE.match("CFoo.cpp"))
        self.assertIsNone(unit_move.FILE.match("fn_71000000.cpp"))


class UnitDir(unittest.TestCase):
    def test_plain_class_goes_under_game(self):
        self.assertEqual(unit_move.unit_dir("CPlaythrough"), unit_move.SRC / "game" / "CPlaythrough")

    def test_namespace_split(self):
        self.assertEqual(unit_move.unit_dir("navmesh::SNavMeshPath"),
                         unit_move.SRC / "game" / "navmesh" / "SNavMeshPath")

    def test_top_level_namespaces_stay_at_src(self):
        self.assertEqual(unit_move.unit_dir("base::snd::CSound"), unit_move.SRC / "base" / "snd" / "CSound")
        self.assertEqual(unit_move.unit_dir("engine::CX"), unit_move.SRC / "engine" / "CX")
        self.assertEqual(unit_move.unit_dir("game::CY"), unit_move.SRC / "game" / "CY")

    def test_odd_characters_replaced(self):
        self.assertEqual(unit_move.unit_dir("TFoo<int>::CBar"), unit_move.SRC / "game" / "TFoo_int" / "CBar")


class Tree(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name).resolve()
        self.src = self.root / "src"
        for p in self.patches():
            p.start()
            self.addCleanup(p.stop)
        self.addCleanup(self._tmp.cleanup)

    def patches(self):
        return [mock.patch.object(unit_move, "ROOT", self.root), mock.patch.object(unit_move, "SRC", self.root / "src")]

    def touch(self, rel: str, text: str = "") -> Path:
        p = self.root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding="utf-8")
        return p


class Plan(Tree):
    def setUp(self):
        super().setUp()
        self.touch("src/game_s2/fn_7100000010.cpp")   # high, reflection -> moves
        self.touch("src/game_s2/fn_7100000020.cpp")   # medium
        self.touch("src/game_s2/fn_7100000030.cpp")   # unnamed
        self.touch("src/game_s2/fn_7100000040.cpp")   # string hint does not count
        self.touch("src/game_s2/fn_7100000090.cpp")   # outside every unit
        self.touch("src/game_s2/helper.cpp")          # not fn_<addr>.cpp
        self.touch("src/game_s3/sub/fn_7100000050.cpp")  # nested, names source
        self.touch("src/game/CDone/fn_7100000060.cpp")   # not an address directory
        A = 0x7100000000
        self.rows = [unit(0, A + 0x10, A + 0x20, "CFoo;CBar"),
                     unit(1, A + 0x20, A + 0x30, "CMed", conf="medium"),
                     unit(2, A + 0x30, A + 0x40, "", source=""),
                     unit(3, A + 0x40, A + 0x50, "x/y.cpp", source="string"),
                     unit(4, A + 0x50, A + 0x70, "base::CQux", source="names")]

    def test_plan(self):
        with mock.patch.object(tu_map, "load_map", return_value=self.rows):
            sources, why = unit_move.plan()
        moves = {p.relative_to(self.root).as_posix(): q.relative_to(self.root).as_posix() for p, q in sources.items()}
        self.assertEqual(moves, {
            "src/game_s2/fn_7100000010.cpp": "src/game/CFoo/fn_7100000010.cpp",
            "src/game_s3/sub/fn_7100000050.cpp": "src/base/CQux/fn_7100000050.cpp",
        })
        self.assertEqual(why["moves"], 2)
        self.assertEqual(why["stays: medium confidence"], 1)
        self.assertEqual(why["stays: unit has no name"], 2)
        self.assertEqual(why["stays: no unit"], 1)
        self.assertEqual(why["stays: not fn_<addr>.cpp"], 1)

    def test_keep_list(self):
        with mock.patch.object(tu_map, "load_map", return_value=self.rows), \
                mock.patch.dict(unit_move.KEEP, {"src/game_s2/fn_7100000010.cpp": "test reason"}, clear=True):
            sources, why = unit_move.plan()
        self.assertEqual(why["stays: test reason"], 1)
        self.assertEqual(len(sources), 1)

    def test_existing_destination_refused(self):
        self.touch("src/game/CFoo/fn_7100000010.cpp")
        with mock.patch.object(tu_map, "load_map", return_value=self.rows):
            with self.assertRaises(SystemExit):
                unit_move.plan()


class RewriteIncludes(Tree):
    def test_neighbour_include_repointed(self):
        old = self.touch("src/game_s2/fn_7100000010.cpp")
        self.touch("src/game_s2/local.h")
        new = self.src / "game" / "CFoo" / old.name
        text = '#include "local.h"\n#include "base/Thing.h"\n#include <cstdint>\n'
        out = unit_move.rewrite_includes(text, old, new, {old.resolve(): new})
        self.assertEqual(out, '#include "../../game_s2/local.h"\n#include "base/Thing.h"\n#include <cstdint>\n')

    def test_include_of_moved_file_follows_it(self):
        a = self.touch("src/game_s2/a.h")
        inc = self.touch("src/game_s2/fn_7100000010.cpp")
        new_a = self.src / "game" / "CFoo" / "a.h"
        out = unit_move.rewrite_includes('#include "a.h"\n', inc, inc, {a.resolve(): new_a})
        self.assertEqual(out, '#include "../game/CFoo/a.h"\n')

    def test_both_moved_together(self):
        a = self.touch("src/game_s2/a.h")
        b = self.touch("src/game_s2/fn_7100000010.cpp")
        dest = self.src / "game" / "CFoo"
        moved = {a.resolve(): dest / "a.h", b.resolve(): dest / b.name}
        self.assertEqual(unit_move.rewrite_includes('  #  include "a.h" // x\n', b, dest / b.name, moved),
                         '  #  include "a.h" // x\n')

    def test_unmoved_file_unchanged(self):
        self.touch("src/game_s2/local.h")
        p = self.touch("src/game_s2/fn_7100000010.cpp")
        text = '#include "local.h"\n'
        self.assertEqual(unit_move.rewrite_includes(text, p, p, {}), text)


if __name__ == "__main__":
    unittest.main()
