"""tools/objdiff_gen.py's pure parts: address mapping, branch decoding and the target ELF writer.

The target objects are built from made-up instruction words and read back with pyelftools and
objdiff_gen.base_functions; main() (which needs the original binary) is not run.
"""

import io
import struct
import tempfile
import unittest
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

from tools.tests import _path  # noqa: F401
import objdiff_gen
from objdiff_gen import Reloc, TargetFunc

NOP, RET = 0xD503201F, 0xD65F03C0
BL = 0x94000000


def code(*words: int) -> bytes:
    return struct.pack(f"<{len(words)}I", *words)


class ExpectedAddress(unittest.TestCase):
    def test_address_in_name(self):
        for sym in ("fn_7100123456", "_Z13fn_7100ABCDEFv", "g_unk_7100000010", "unk_7100000020"):
            self.assertIsNotNone(objdiff_gen.expected_address(sym, {}, {}), sym)
        self.assertEqual(objdiff_gen.expected_address("_ZN3Foo13fn_7100ABCDEFEv", {}, {}), 0x7100ABCDEF)

    def test_unique_demangled_name(self):
        by_name = {"CFoo::Bar": [0x7100000100], "CFoo::Dup": [0x7100000200, 0x7100000300]}
        demangled = {"_ZN4CFoo3BarEv": "CFoo::Bar", "_ZN4CFoo3DupEv": "CFoo::Dup"}
        self.assertEqual(objdiff_gen.expected_address("_ZN4CFoo3BarEv", demangled, by_name), 0x7100000100)
        self.assertIsNone(objdiff_gen.expected_address("_ZN4CFoo3DupEv", demangled, by_name))
        self.assertIsNone(objdiff_gen.expected_address("unknown", demangled, by_name))

    def test_plain_name_without_demangling(self):
        self.assertEqual(objdiff_gen.expected_address("memcpy", {}, {"memcpy": [0x7100000400]}), 0x7100000400)


class BranchTarget(unittest.TestCase):
    def test_forward_and_backward(self):
        self.assertEqual(objdiff_gen.branch_target(BL | 4, 0x1000), 0x1010)
        self.assertEqual(objdiff_gen.branch_target(BL | 0x3FFFFFF, 0x1000), 0xFFC)
        self.assertEqual(objdiff_gen.branch_target(BL | 0x2000000, 0), -(1 << 27))

    def test_opcode_bits_ignored(self):
        self.assertEqual(objdiff_gen.branch_target(0x14000002, 0x100), 0x108)  # B


class BuildTarget(unittest.TestCase):
    def setUp(self):
        self.funcs = [
            TargetFunc("fn_7100000100", 1, code(BL, NOP, RET), [Reloc(0, 283, "fn_7100000200", 0)]),
            TargetFunc("local_helper", 0, code(RET)),
            TargetFunc("fn_7100000300", 2, code(BL, BL, RET),
                       [Reloc(0, 283, "local_helper", 0), Reloc(4, 282, "orig_7100000999", 0)]),
        ]
        self.raw = objdiff_gen.build_target(self.funcs)
        self.elf = ELFFile(io.BytesIO(self.raw))

    def test_header(self):
        self.assertEqual(self.elf["e_type"], "ET_REL")
        self.assertEqual(self.elf["e_machine"], "EM_AARCH64")
        self.assertEqual(self.elf.get_section(self.elf["e_shstrndx"]).name, ".shstrtab")

    def test_one_text_section_per_function(self):
        names = [s.name for s in self.elf.iter_sections()]
        self.assertEqual(names, ["", ".text.fn_7100000100", ".text.local_helper", ".text.fn_7100000300",
                                 ".rela.text.fn_7100000100", ".rela.text.fn_7100000300",
                                 ".symtab", ".strtab", ".shstrtab"])
        self.assertEqual(self.elf.get_section_by_name(".text.fn_7100000300").data(), code(BL, BL, RET))

    def test_symbols_locals_first(self):
        symtab = self.elf.get_section_by_name(".symtab")
        syms = [(s.name, s["st_info"]["bind"], s["st_info"]["type"]) for s in symtab.iter_symbols()]
        self.assertEqual(syms, [
            ("", "STB_LOCAL", "STT_NOTYPE"),
            ("local_helper", "STB_LOCAL", "STT_FUNC"),
            ("fn_7100000100", "STB_GLOBAL", "STT_FUNC"),
            ("fn_7100000300", "STB_WEAK", "STT_FUNC"),
            ("fn_7100000200", "STB_GLOBAL", "STT_NOTYPE"),
            ("orig_7100000999", "STB_GLOBAL", "STT_NOTYPE"),
        ])
        self.assertEqual(symtab["sh_info"], 2)  # first non-local symbol
        self.assertEqual(symtab.get_symbol(3)["st_size"], 12)

    def test_relocations_point_at_symbols(self):
        symtab = self.elf.get_section_by_name(".symtab")
        found = []
        for sec in self.elf.iter_sections():
            if isinstance(sec, RelocationSection):
                target = self.elf.get_section(sec["sh_info"]).name
                for r in sec.iter_relocations():
                    found.append((target, r["r_offset"], r["r_info_type"],
                                  symtab.get_symbol(r["r_info_sym"]).name))
        self.assertEqual(found, [
            (".text.fn_7100000100", 0, 283, "fn_7100000200"),
            (".text.fn_7100000300", 0, 283, "local_helper"),
            (".text.fn_7100000300", 4, 282, "orig_7100000999"),
        ])

    def test_base_functions_reads_it_back(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp) / "t.o"
            obj.write_bytes(self.raw)
            funcs, relocs = objdiff_gen.base_functions(obj)
        self.assertEqual(sorted((n, b, v, s) for n, b, _, v, s in funcs), [
            ("fn_7100000100", 1, 0, 12), ("fn_7100000300", 2, 0, 12), ("local_helper", 0, 0, 4)])
        shndx = {n: i for n, _, i, _, _ in funcs}
        self.assertEqual(relocs[shndx["fn_7100000300"]],
                         [(0, 283, "local_helper", 0), (4, 282, "orig_7100000999", 0)])

    def test_no_functions(self):
        elf = ELFFile(io.BytesIO(objdiff_gen.build_target([])))
        self.assertEqual([s.name for s in elf.iter_sections()], ["", ".symtab", ".strtab", ".shstrtab"])


class Constants(unittest.TestCase):
    def test_categories_named(self):
        for cat in set(objdiff_gen.CATEGORIES.values()) | {"game"}:
            self.assertIn(cat, objdiff_gen.CATEGORY_NAMES)

    def test_call_relocs(self):
        self.assertEqual(objdiff_gen.CALL_RELOCS, {282, 283})


if __name__ == "__main__":
    unittest.main()
