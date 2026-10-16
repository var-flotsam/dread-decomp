"""Loaded module image with relocations and dynamic symbols, for the recon scripts.

Addresses are module-relative (0-based) internally. Use BASE + addr when
reporting so the numbers match Ghidra (image base 0x7100000000).
"""

from __future__ import annotations

import struct
import sys
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import nso2elf  # noqa: E402

BASE = 0x7100000000
R_AARCH64_ABS64, R_AARCH64_GLOB_DAT, R_AARCH64_JUMP_SLOT, R_AARCH64_RELATIVE = 257, 1025, 1026, 1027


@dataclass
class Reloc:
    type: int
    sym: str | None   # symbol name for symbol-based relocations
    addend: int


class Image:
    def __init__(self, nso_path: Path):
        self.mod = nso2elf.read_module(nso_path)
        self.data = bytes(self.mod.image)
        self.text = (self.mod.text.mem_offset, self.mod.text.mem_offset + self.mod.text.size)
        self.rodata = (self.mod.rodata.mem_offset, self.mod.rodata.mem_offset + self.mod.rodata.size)
        self.rwdata = (self.mod.data.mem_offset, self.mod.bss_end)
        self.symbols = self._read_dynsym()
        self.relocs: dict[int, Reloc] = {}
        d = self.mod.dyn
        for tag_addr, tag_size in ((nso2elf.DT["RELA"], nso2elf.DT["RELASZ"]),
                                   (nso2elf.DT["JMPREL"], nso2elf.DT["PLTRELSZ"])):
            if tag_addr in d:
                self._read_rela(d[tag_addr], d[tag_size])

    def _read_dynsym(self) -> list[dict]:
        d = self.mod.dyn
        symtab, strtab = d[nso2elf.DT["SYMTAB"]], d[nso2elf.DT["STRTAB"]]
        out = []
        for i in range(self.mod.dynsym_size // 24):
            name_off, info, other, shndx, value, size = struct.unpack_from("<IBBHQQ", self.data, symtab + 24 * i)
            end = self.data.index(b"\0", strtab + name_off)
            out.append({"name": self.data[strtab + name_off:end].decode("utf-8", "replace"),
                        "type": info & 0xF, "bind": info >> 4, "shndx": shndx, "value": value, "size": size})
        return out

    def _read_rela(self, addr: int, size: int) -> None:
        for off in range(addr, addr + size, 24):
            r_offset, r_info, r_addend = struct.unpack_from("<QQq", self.data, off)
            rtype, rsym = r_info & 0xFFFFFFFF, r_info >> 32
            name = self.symbols[rsym]["name"] if rsym else None
            self.relocs[r_offset] = Reloc(rtype, name, r_addend)

    def in_text(self, a: int) -> bool:
        return self.text[0] <= a < self.text[1]

    def pointer(self, a: int) -> int | None:
        """The module-relative address stored at a (via a RELATIVE reloc), else None."""
        r = self.relocs.get(a)
        if r is not None and r.type == R_AARCH64_RELATIVE:
            return r.addend
        return None

    def u32(self, a: int) -> int:
        return struct.unpack_from("<I", self.data, a)[0]

    def i64(self, a: int) -> int:
        return struct.unpack_from("<q", self.data, a)[0]

    def cstring(self, a: int, limit: int = 4096) -> str | None:
        if not 0 <= a < len(self.data):
            return None
        end = self.data.find(b"\0", a, a + limit)
        if end < 0:
            return None
        try:
            return self.data[a:end].decode("ascii")
        except UnicodeDecodeError:
            return None
