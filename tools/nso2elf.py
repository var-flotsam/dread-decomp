#!/usr/bin/env python3
"""Convert a Switch NSO into an ELF64 (AArch64, ET_DYN) that Ghidra and LLVM tools can read.

The output is linked at address 0. Load it in Ghidra with image base
0x7100000000 so addresses match the conventional Switch layout.

Besides the three PT_LOAD segments (text R-X, rodata R--, data+bss RW-), the
ELF gets PT_DYNAMIC and PT_GNU_EH_FRAME. It also gets section headers for every
region the dynamic table and MOD0 header describe (.dynsym, .dynstr,
.rela.dyn, .rela.plt, .hash, .gnu.hash, .eh_frame_hdr, .eh_frame, .dynamic,
.init_array, .fini_array). The gaps are filled with .text/.rodata/.data so the
sections never overlap.

Usage: python tools/nso2elf.py orig/2.1.0/exefs/main orig/2.1.0/main.elf
"""

from __future__ import annotations

import argparse
import struct
import sys
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import nso  # noqa: E402

PAGE = 0x1000
FILE_BASE = 0x1000  # file offset of address 0; keeps p_offset == p_vaddr (mod PAGE)

# ELF constants
ET_DYN, EM_AARCH64 = 3, 183
PT_LOAD, PT_DYNAMIC, PT_GNU_EH_FRAME = 1, 2, 0x6474E550
PF_X, PF_W, PF_R = 1, 2, 4
SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_RELA, SHT_HASH, SHT_DYNAMIC, SHT_NOBITS, SHT_DYNSYM = 1, 2, 3, 4, 5, 6, 8, 11
SHT_INIT_ARRAY, SHT_FINI_ARRAY, SHT_GNU_HASH = 14, 15, 0x6FFFFFF6
SHF_WRITE, SHF_ALLOC, SHF_EXECINSTR = 1, 2, 4

DT = dict(NULL=0, PLTRELSZ=2, PLTGOT=3, HASH=4, STRTAB=5, SYMTAB=6, RELA=7, RELASZ=8, STRSZ=10,
          SYMENT=11, JMPREL=23, INIT_ARRAY=25, FINI_ARRAY=26, INIT_ARRAYSZ=27, FINI_ARRAYSZ=28,
          GNU_HASH=0x6FFFFEF5)


@dataclass
class Section:
    name: str
    addr: int
    size: int
    type: int
    flags: int
    entsize: int = 0
    link: str = ""   # name of the linked section
    info: int = 0
    align: int = 8


@dataclass
class Module:
    image: bytearray        # text..data, zero-filled gaps
    text: nso.Segment
    rodata: nso.Segment
    data: nso.Segment
    bss_end: int
    mod0: int
    dynamic: int
    eh_frame_hdr: tuple[int, int]
    dyn: dict[int, int]
    dynsym_size: int


def read_module(path: Path) -> Module:
    m = nso.load(path)
    text, rodata, data = m.segments
    image = bytearray(data.mem_offset + data.size)
    for s in m.segments:
        image[s.mem_offset:s.mem_offset + s.size] = s.data

    (mod0,) = struct.unpack_from("<I", image, 4)
    magic, dyn_off, bss_start, bss_end, eh_start, eh_end, _ = struct.unpack_from("<4s6i", image, mod0)
    if magic != b"MOD0":
        raise ValueError(f"{path}: MOD0 header not found at {mod0:#x}")
    dynamic = mod0 + dyn_off

    dyn = {}
    off = dynamic
    while True:
        tag, val = struct.unpack_from("<qQ", image, off)
        off += 16
        if tag == DT["NULL"]:
            break
        dyn.setdefault(tag, val)

    return Module(image=image, text=text, rodata=rodata, data=data,
                  bss_end=max(mod0 + bss_end, data.mem_offset + data.size + m.bss_size),
                  mod0=mod0, dynamic=dynamic, eh_frame_hdr=(mod0 + eh_start, mod0 + eh_end),
                  dyn=dyn, dynsym_size=m.dynsym[1])


def read_encoded(buf: bytes, off: int, enc: int, field_addr: int) -> tuple[int, int]:
    """Decode one DW_EH_PE value. Returns (value, bytes consumed)."""
    fmt = {0x03: "<I", 0x0B: "<i", 0x04: "<Q", 0x0C: "<q", 0x02: "<H", 0x0A: "<h"}[enc & 0x0F]
    (val,) = struct.unpack_from(fmt, buf, off)
    if enc & 0x70 == 0x10:  # pcrel
        val += field_addr
    return val, struct.calcsize(fmt)


def eh_frame_extent(img: bytes, hdr_start: int) -> tuple[int, int]:
    """Find .eh_frame from .eh_frame_hdr, then walk CIE/FDE records to the zero terminator."""
    version, ptr_enc = img[hdr_start], img[hdr_start + 1]
    if version != 1:
        raise ValueError(f"unexpected .eh_frame_hdr version {version}")
    start, _ = read_encoded(img, hdr_start + 4, ptr_enc, hdr_start + 4)
    off = start
    while True:
        (length,) = struct.unpack_from("<I", img, off)
        if length == 0:
            return start, off + 4 - start
        if length == 0xFFFFFFFF:
            (length,) = struct.unpack_from("<Q", img, off + 4)
            off += 12 + length
        else:
            off += 4 + length


def collect_sections(mod: Module) -> list[Section]:
    d = mod.dyn
    ro, rw = SHF_ALLOC, SHF_ALLOC | SHF_WRITE
    named: list[Section] = []

    def add(name, addr_tag, size, *args, **kw):
        if addr_tag in d and size:
            named.append(Section(name, d[addr_tag], size, *args, **kw))

    add(".dynsym", DT["SYMTAB"], mod.dynsym_size, SHT_DYNSYM, ro, entsize=24, link=".dynstr", info=1)
    add(".dynstr", DT["STRTAB"], d.get(DT["STRSZ"], 0), SHT_STRTAB, ro, align=1)
    if DT["HASH"] in d:
        nbucket, nchain = struct.unpack_from("<II", mod.image, d[DT["HASH"]])
        add(".hash", DT["HASH"], 8 + 4 * (nbucket + nchain), SHT_HASH, ro, entsize=4, link=".dynsym", align=4)
    if DT["GNU_HASH"] in d:
        named.append(Section(".gnu.hash", d[DT["GNU_HASH"]], gnu_hash_size(mod), SHT_GNU_HASH, ro,
                             link=".dynsym"))
    add(".rela.dyn", DT["RELA"], d.get(DT["RELASZ"], 0), SHT_RELA, ro, entsize=24, link=".dynsym")
    add(".rela.plt", DT["JMPREL"], d.get(DT["PLTRELSZ"], 0), SHT_RELA, ro, entsize=24, link=".dynsym")
    add(".init_array", DT["INIT_ARRAY"], d.get(DT["INIT_ARRAYSZ"], 0), SHT_INIT_ARRAY, rw)
    add(".fini_array", DT["FINI_ARRAY"], d.get(DT["FINI_ARRAYSZ"], 0), SHT_FINI_ARRAY, rw)

    dyn_count = 0
    while struct.unpack_from("<q", mod.image, mod.dynamic + 16 * dyn_count)[0] != 0:
        dyn_count += 1
    named.append(Section(".dynamic", mod.dynamic, 16 * (dyn_count + 1), SHT_DYNAMIC, rw, entsize=16,
                         link=".dynstr"))

    hdr_start, hdr_end = mod.eh_frame_hdr
    if hdr_end > hdr_start:
        named.append(Section(".eh_frame_hdr", hdr_start, hdr_end - hdr_start, SHT_PROGBITS, ro, align=4))
        eh_start, eh_size = eh_frame_extent(mod.image, hdr_start)
        named.append(Section(".eh_frame", eh_start, eh_size, SHT_PROGBITS, ro))

    # Clip each named region to its segment and drop overlaps (first come wins).
    segs = [(mod.text, ".text", SHF_ALLOC | SHF_EXECINSTR),
            (mod.rodata, ".rodata", ro), (mod.data, ".data", rw)]
    result: list[Section] = []
    for seg, fill_name, flags in segs:
        lo, hi = seg.mem_offset, seg.mem_offset + seg.size
        inside = sorted((s for s in named if lo <= s.addr < hi), key=lambda s: s.addr)
        cursor, n_fill = lo, 0
        for s in inside:
            if s.addr < cursor:
                print(f"warning: {s.name} at {s.addr:#x} overlaps previous section; skipped", file=sys.stderr)
                continue
            s.size = min(s.size, hi - s.addr)
            if s.addr > cursor:
                result.append(Section(fill_name + (f".{n_fill}" if n_fill else ""), cursor, s.addr - cursor,
                                      SHT_PROGBITS, flags, align=1 if n_fill else 16))
                n_fill += 1
            result.append(s)
            cursor = s.addr + s.size
        if cursor < hi:
            result.append(Section(fill_name + (f".{n_fill}" if n_fill else ""), cursor, hi - cursor,
                                  SHT_PROGBITS, flags, align=1 if n_fill else 16))
    for s in named:
        if not any(r is s for r in result):
            print(f"warning: {s.name} at {s.addr:#x} lies outside every segment; skipped", file=sys.stderr)

    data_end = mod.data.mem_offset + mod.data.size
    if mod.bss_end > data_end:
        result.append(Section(".bss", data_end, mod.bss_end - data_end, SHT_NOBITS, rw, align=16))
    return result


def gnu_hash_size(mod: Module) -> int:
    addr = mod.dyn[DT["GNU_HASH"]]
    nbuckets, symoffset, bloom_size, _ = struct.unpack_from("<IIII", mod.image, addr)
    buckets = addr + 16 + 8 * bloom_size
    chains = buckets + 4 * nbuckets
    last = max(struct.unpack_from(f"<{nbuckets}I", mod.image, buckets), default=0)
    if last < symoffset:
        return chains - addr
    while not struct.unpack_from("<I", mod.image, chains + 4 * (last - symoffset))[0] & 1:
        last += 1
    return chains + 4 * (last - symoffset + 1) - addr


def build_elf(mod: Module) -> bytes:
    sections = collect_sections(mod)
    image_end = mod.data.mem_offset + mod.data.size

    phdrs = []
    for seg, flags in ((mod.text, PF_R | PF_X), (mod.rodata, PF_R), (mod.data, PF_R | PF_W)):
        memsz = (mod.bss_end - seg.mem_offset) if seg is mod.data else seg.size
        phdrs.append((PT_LOAD, flags, FILE_BASE + seg.mem_offset, seg.mem_offset, seg.size, memsz, PAGE))
    dyn = next(s for s in sections if s.name == ".dynamic")
    phdrs.append((PT_DYNAMIC, PF_R | PF_W, FILE_BASE + dyn.addr, dyn.addr, dyn.size, dyn.size, 8))
    hdr = next((s for s in sections if s.name == ".eh_frame_hdr"), None)
    if hdr:
        phdrs.append((PT_GNU_EH_FRAME, PF_R, FILE_BASE + hdr.addr, hdr.addr, hdr.size, hdr.size, 4))

    # Section name string table
    names = [""] + [s.name for s in sections] + [".shstrtab"]
    shstrtab = bytearray(b"\0")
    name_off = {"": 0}
    for n in names[1:]:
        if n not in name_off:
            name_off[n] = len(shstrtab)
            shstrtab += n.encode() + b"\0"

    shstrtab_off = FILE_BASE + image_end
    shoff = (shstrtab_off + len(shstrtab) + 7) & ~7
    index = {s.name: i + 1 for i, s in enumerate(sections)}

    out = bytearray()
    ehdr = struct.pack("<4sBBBBB7sHHIQQQIHHHHHH",
                       b"\x7fELF", 2, 1, 1, 0, 0, b"\0" * 7,
                       ET_DYN, EM_AARCH64, 1, 0, 64, shoff, 0, 64, 56, len(phdrs), 64,
                       len(sections) + 2, len(sections) + 1)
    out += ehdr
    for p_type, p_flags, p_offset, vaddr, filesz, memsz, align in phdrs:
        out += struct.pack("<IIQQQQQQ", p_type, p_flags, p_offset, vaddr, vaddr, filesz, memsz, align)
    if len(out) > FILE_BASE:
        raise ValueError("ELF headers do not fit before FILE_BASE")
    out += b"\0" * (FILE_BASE - len(out))
    out += mod.image
    out += shstrtab
    out += b"\0" * (shoff - len(out))

    out += b"\0" * 64  # SHN_UNDEF
    for s in sections:
        file_off = FILE_BASE + s.addr if s.type != SHT_NOBITS else FILE_BASE + image_end
        out += struct.pack("<IIQQQQIIQQ", name_off[s.name], s.type, s.flags, s.addr, file_off, s.size,
                           index.get(s.link, 0), s.info, s.align, s.entsize)
    out += struct.pack("<IIQQQQIIQQ", name_off[".shstrtab"], SHT_STRTAB, 0, 0, shstrtab_off, len(shstrtab),
                       0, 0, 1, 0)
    return bytes(out)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("nso", type=Path)
    ap.add_argument("elf", type=Path)
    args = ap.parse_args()

    mod = read_module(args.nso)
    elf = build_elf(mod)
    args.elf.parent.mkdir(parents=True, exist_ok=True)
    args.elf.write_bytes(elf)
    print(f"wrote {args.elf} ({len(elf):,} bytes); text {mod.text.size:#x}, rodata {mod.rodata.size:#x}, "
          f"data {mod.data.size:#x}, bss end {mod.bss_end:#x}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
