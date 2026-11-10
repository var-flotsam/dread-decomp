"""tools/gen_functions_csv.py's pure helpers: subsystem rules, names, status codes (no game data).

main() reads the recon and Ghidra exports in one pass, so the sharding and status preservation
it does inline are not covered here; the rules they rely on are.
"""

import os
import shutil
import unittest

from tools.tests import _path  # noqa: F401
import gen_functions_csv as g


class Subsystems(unittest.TestCase):
    def check(self, cases: dict) -> None:
        for name, expected in cases.items():
            with self.subTest(name=name):
                self.assertEqual(g.subsystem_for(name), expected)

    def test_sdk_and_middleware(self):
        self.check({"nn::atk::detail::Foo": "middleware/nn-atk", "nn::os::Mutex": "sdk"})

    def test_runtime(self):
        self.check({"std::vector<int>::push_back": "runtime", "__cxa_atexit": "runtime",
                    "operator new": "runtime", "memcpy": "runtime", "strlen": "runtime",
                    "nnMain": "runtime", "_DT_INIT": "runtime"})

    def test_engine_namespaces(self):
        self.check({"base::reflection::CClass": "engine/reflection", "base::core::CObject": "engine/base",
                    "engine::scene::CScene": "engine", "GUI::CWidget": "gui",
                    "lua_lib::__index": "script/bindings", "lua_bind::Foo": "script/bindings"})

    def test_game_classes(self):
        self.check({"CSamusMovement": "game/player", "CFooComponent": "game/components",
                    "CCharClassFoo": "game/characters", "CFooAction": "game/ai",
                    "CMinimapManager": "game/world", "CGameFoo": "game/core", "CFooBar": "game",
                    "SFooBar": "game"})

    def test_first_match_wins(self):
        # The AI rule comes before the player rule, the reflection rule before base::.
        self.assertEqual(g.subsystem_for("CSamusAttack"), "game/ai")
        self.assertEqual(g.subsystem_for("base::reflection::X"), "engine/reflection")

    def test_backreference_expands_namespace(self):
        self.check({"game::Foo": "game", "gameeditor::Foo": "gameeditor", "msapi::x": "msapi",
                    "media::y": "media"})

    def test_unmatched(self):
        for name in ("fn_7100000010", "foo", "Cfoo", ""):
            with self.subTest(name=name):
                self.assertIsNone(g.subsystem_for(name))


class Statuses(unittest.TestCase):
    def test_library_subsystems_come_from_rules(self):
        produced = {sub for _, sub in g.SUBSYSTEM_RULES if "\\" not in sub}
        # every library subsystem a rule produces is one main() marks L
        for sub in ("middleware/nn-atk", "sdk", "runtime"):
            self.assertIn(sub, produced)
            self.assertIn(sub, g.LIBRARY_SUBSYSTEMS)
        self.assertNotIn("game", g.LIBRARY_SUBSYSTEMS)
        # the game's own Lua binding tables (lua_lib::__index, ...) are game code, not lib/lua
        self.assertNotIn("script/bindings", g.LIBRARY_SUBSYSTEMS)
        self.assertNotIn("lua/stdlib", produced)
        # an unnamed function between lib/lua matches ("lua?") is the game's own addition
        self.assertIn("lua", g.INFERRED_NOT_LIBRARY)

    def test_inferred_runtime_is_game_code_unless_named(self):
        # GUI::CSlotInfoComposition's unit: its constructor and getter sit between std:: thunks
        self.assertIn("runtime", g.INFERRED_NOT_LIBRARY)
        self.assertFalse(g.is_library("runtime?", "unknown"))
        self.assertTrue(g.is_library("runtime?", "symbol"))
        self.assertTrue(g.is_library("runtime", "unknown"))
        self.assertFalse(g.is_library("lua?", "unknown"))
        self.assertTrue(g.is_library("lua", "library"))
        self.assertTrue(g.is_library("sdk?", "unknown"))
        self.assertFalse(g.is_library("game?", "unknown"))

    def test_decomp_statuses_are_the_src_codes(self):
        self.assertEqual(g.DECOMP_STATUSES, {"O", "m", "M", "W"})
        self.assertFalse(g.DECOMP_STATUSES & {"U", "L"})


class FakeImage:
    """What non_code reads from image.Image: the module's bytes and little-endian words."""

    def __init__(self, words: list[int]):
        self.data = b"".join(w.to_bytes(4, "little") for w in words)

    def u32(self, off: int) -> int:
        return int.from_bytes(self.data[off:off + 4], "little")


class NonCode(unittest.TestCase):
    PLT = [0x90000010, 0xF9400211, 0x91000210, 0xD61F0220]  # adrp x16; ldr x17, [x16]; add x16, x16; br x17
    RET, NOP, FILL = 0xD65F03C0, 0xD503201F, 0xE7FFDEFE

    def kind(self, words: list[int], off: int, size: int):
        return g.non_code(FakeImage(words), g.BASE + off, size)

    def test_import_placeholders_lie_past_the_module(self):
        self.assertEqual(self.kind([self.RET], 0x100, 1), "import")

    def test_plt_stub(self):
        self.assertEqual(self.kind(self.PLT, 0, 16), "linker")

    def test_padding(self):
        self.assertEqual(self.kind([self.RET, self.NOP], 4, 4), "linker")
        self.assertEqual(self.kind([self.FILL, self.FILL], 0, 8), "linker")

    def test_code_is_left_alone(self):
        self.assertIsNone(self.kind([self.RET], 0, 4))
        self.assertIsNone(self.kind([self.NOP, self.RET], 0, 8))  # a body that starts with a nop
        self.assertIsNone(self.kind(self.PLT[:3] + [self.RET], 0, 16))

    def test_both_are_library_status(self):
        self.assertTrue(g.is_library("import", "unknown"))
        self.assertTrue(g.is_library("linker", "unknown"))


class GenericNames(unittest.TestCase):
    def test_ghidra_placeholders(self):
        for name in ("FUN_7100000010", "thunk_FUN_7100000010", "_INIT_3", "LAB_7100000010", "entry"):
            with self.subTest(name=name):
                self.assertTrue(g.GENERIC_NAME.match(name))

    def test_real_names(self):
        for name in ("nnMain", "_INIT_x", "entrypoint", "CFoo::Bar"):
            with self.subTest(name=name):
                self.assertFalse(g.GENERIC_NAME.match(name))


class Demangle(unittest.TestCase):
    def test_empty_needs_no_tool(self):
        self.assertEqual(g.demangle([]), [])

    @unittest.skipUnless(os.path.isfile(g.CXXFILT) or shutil.which(g.CXXFILT), "llvm-cxxfilt not found")
    def test_parameters_dropped_plain_names_kept(self):
        self.assertEqual(g.demangle(["_ZN4base4core1XEv", "_ZN3FooC2Ei", "plain_name"]),
                         ["base::core::X", "Foo::Foo", "plain_name"])


if __name__ == "__main__":
    unittest.main()
