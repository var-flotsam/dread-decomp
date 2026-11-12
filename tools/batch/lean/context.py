#!/usr/bin/env python3
"""What prepare.py gives a worker about its function: the annotated disassembly and the headers that already
declare the symbols it names. The text it produces is game-derived: it goes under build/ and is never
committed, and a worker reads it without copying any of it into a source.
"""
import csv
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
VERSION = "2.1.0"
BASE = 0x7100000000
SYM_RE = re.compile(r"\b(fn|g_unk)_([0-9a-f]{10})\b")
DECL_STOP = re.compile(r"(if|return|for|while|else|do|switch|case)|[{}]")


def load_functions():
    """address -> (size, name, status) from data/functions."""
    funcs = {}
    for p in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with open(p, newline="") as f:
            for row in csv.DictReader(f):
                funcs[int(row["address"], 16)] = (int(row["size"]), row["name"], row["status"])
    return funcs


class Binary:
    def __init__(self):
        from elftools.elf.elffile import ELFFile
        from elftools.elf.relocation import RelocationSection
        self.f = open(ROOT / "orig" / VERSION / "main.elf", "rb")
        self.elf = ELFFile(self.f)
        self.segs = [(s["p_vaddr"], s["p_filesz"], s.data()) for s in self.elf.iter_segments()
                     if s["p_type"] == "PT_LOAD"]
        self.slots = {}  # GOT-like slot -> target (R_AARCH64_RELATIVE)
        for s in self.elf.iter_sections():
            if isinstance(s, RelocationSection):
                for r in s.iter_relocations():
                    if r["r_info_type"] == 1027:
                        self.slots[BASE + r["r_offset"]] = BASE + r.entry.get("r_addend", 0)

    def read(self, addr, n):
        off = addr - BASE
        for va, sz, data in self.segs:
            if va <= off < va + sz:
                return data[off - va:off - va + n]
        return b""

    def disasm(self, addr, size, funcs):
        """(listing, the fn_/g_unk_ names it mentions): one line per instruction, with the callee, the global
        (through its GOT slot or directly) or the string an instruction refers to written beside it."""
        from capstone import CS_ARCH_ARM64, CS_MODE_ARM, Cs
        md = Cs(CS_ARCH_ARM64, CS_MODE_ARM)
        lines, pages, syms = [], {}, set()

        def name(a):
            n = funcs[a][1] if a in funcs else f"g_unk_{a:x}"
            syms.add(n)
            return n

        for i in md.disasm(self.read(addr, size), addr):
            note = ""
            ops = i.op_str
            if i.mnemonic in ("bl", "b") and ops.startswith("#"):
                t = int(ops[1:], 16)
                if not (addr <= t < addr + size):
                    note = name(t)
            elif i.mnemonic == "adrp":
                reg, imm = ops.split(", #")
                pages[reg] = int(imm, 16)
            else:
                m = re.match(r"(\w+), \[(\w+), #(0x[0-9a-f]+)\]", ops)
                if i.mnemonic == "ldr" and m and m.group(2) in pages:
                    a = pages[m.group(2)] + int(m.group(3), 16)
                    note = f"GOT slot -> {name(self.slots[a])}" if a in self.slots else name(a)
                m2 = re.match(r"(\w+), (\w+), #(0x[0-9a-f]+)$", ops)
                if i.mnemonic == "add" and m2 and m2.group(2) in pages:
                    a = pages[m2.group(2)] + int(m2.group(3), 16)
                    if a in funcs:
                        note = name(a)
                    else:
                        s = self.read(a, 48).split(b"\0")[0]
                        printable = len(s) >= 1 and all(32 <= c < 127 or c in (9, 10) for c in s)
                        note = f'"{s.decode().encode("unicode_escape").decode()}"' if printable else name(a)
            lines.append(f"{i.address:x}: {i.mnemonic} {ops}" + (f"    ; {note}" if note else ""))
        return "\n".join(lines), syms


def header_index():
    """fn_/g_unk_ name -> (header relative to include/, its declaration line), first declaration wins."""
    idx = {}
    for p in sorted((ROOT / "include").rglob("*.h")):
        rel = p.relative_to(ROOT / "include").as_posix()
        for ln in p.read_text(encoding="utf-8", errors="replace").splitlines():
            s = ln.strip()
            if s.startswith("//"):
                continue
            for m in SYM_RE.finditer(s):
                key = m.group(0)
                code = s.split("//")[0].strip()
                is_decl = code.startswith("extern") or (
                    "(" in code and code.endswith(";") and not DECL_STOP.match(code)
                    and re.search(r"[\w>*&]\s+[*&]*" + key + r"\s*\(", code))
                if key not in idx and is_decl:
                    idx[key] = (rel, s)
    return idx
