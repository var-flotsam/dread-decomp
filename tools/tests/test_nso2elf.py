"""tools/nso2elf.py on a tiny hand-built NSO (no game data).

The fixture is a made-up module: a text segment holding a MOD0 header, a rodata segment with a
two-entry .dynsym, a .dynstr, an .eh_frame_hdr and one .eh_frame record, and a data segment with
the dynamic table and an .init_array. The text and data segments are LZ4-compressed as in a real
NSO. `build_nso` is reused by test_verify_dump.
"""

import contextlib
import hashlib
import io
import struct
import tempfile
import types
import unittest
import unittest.mock
from pathlib import Path

import lz4.block
from elftools.elf.elffile import ELFFile

from tools.tests import _path  # noqa: F401
import nso2elf

DT = nso2elf.DT
TEXT, RODATA, DATA = 0x000, 0x100, 0x200
SEG_SIZE = 0x100
MOD0 = 0x10
DYNSYM, DYNSTR, EH_HDR, EH_FRAME = 0x100, 0x130, 0x140, 0x160
DYNAMIC, INIT_ARRAY = 0x200, 0x260
BSS_SIZE = 0x80


def build_nso(segments, bss_size=0, compress=(True, False, True), check_hash=(True, True, True),
              dynsym=(0, 0), build_id=b"\xab" * 0x20, bad_hash=None) -> bytes:
    """An NSO0 file from three (mem_offset, bytes) segments. bad_hash names a segment index whose
    header hash is wrong."""
    flags = 0
    stored, hashes = [], []
    for i, (_, data) in enumerate(segments):
        if compress[i]:
            flags |= 1 << i
            stored.append(lz4.block.compress(data, store_size=False))
        else:
            stored.append(data)
        if check_hash[i]:
            flags |= 1 << (i + 3)
        digest = hashlib.sha256(data).digest()
        hashes.append(bytes(32) if bad_hash == i else digest)
    header = bytearray(0x100)
    header[0:4] = b"NSO0"
    struct.pack_into("<I", header, 0x0C, flags)
    off = 0x100
    file_offs = []
    for s in stored:
        file_offs.append(off)
        off += len(s)
    name = b"fixture\0"
    for i, (mem, data) in enumerate(segments):
        struct.pack_into("<III", header, 0x10 + 0x10 * i, file_offs[i], mem, len(data))
    struct.pack_into("<I", header, 0x1C, off)          # module name offset
    struct.pack_into("<I", header, 0x2C, len(name))    # module name size
    struct.pack_into("<I", header, 0x3C, bss_size)
    header[0x40:0x60] = build_id
    struct.pack_into("<III", header, 0x60, *(len(s) for s in stored))
    struct.pack_into("<II", header, 0x98, *dynsym)
    for i, h in enumerate(hashes):
        header[0xA0 + 0x20 * i:0xC0 + 0x20 * i] = h
    return bytes(header) + b"".join(stored) + name


def fixture_image(mod0_bss_end=0x340, eh_version=1):
    text = bytearray(SEG_SIZE)
    struct.pack_into("<I", text, 4, MOD0)
    struct.pack_into("<4s6i", text, MOD0, b"MOD0", DYNAMIC - MOD0, 0x300 - MOD0, mod0_bss_end - MOD0,
                     EH_HDR - MOD0, EH_HDR + 12 - MOD0, 0)
    text[0x40:0x44] = b"\xc0\x03\x5f\xd6"  # some made-up code bytes

    rodata = bytearray(SEG_SIZE)
    # .dynsym: null entry plus one symbol (name offset 1)
    struct.pack_into("<IBBHQQ", rodata, DYNSYM - RODATA + 24, 1, 0x12, 0, 1, 0x40, 4)
    rodata[DYNSTR - RODATA:DYNSTR - RODATA + 8] = b"\0fn_one\0"
    # .eh_frame_hdr: version 1, eh_frame_ptr pcrel sdata4, fde count udata4 (0), table sdata4 datarel
    h = EH_HDR - RODATA
    rodata[h:h + 4] = bytes([eh_version, 0x1B, 0x03, 0x3B])
    struct.pack_into("<iI", rodata, h + 4, EH_FRAME - (EH_HDR + 4), 0)
    # .eh_frame: one 16-byte record, then the zero terminator
    struct.pack_into("<I", rodata, EH_FRAME - RODATA, 0x10)
    rodata[EH_FRAME - RODATA + 4:EH_FRAME - RODATA + 20] = b"\x11" * 16

    data = bytearray(SEG_SIZE)
    entries = [(DT["SYMTAB"], DYNSYM), (DT["STRTAB"], DYNSTR), (DT["STRSZ"], 0x10),
               (DT["INIT_ARRAY"], INIT_ARRAY), (DT["INIT_ARRAYSZ"], 8), (DT["NULL"], 0)]
    for i, (tag, val) in enumerate(entries):
        struct.pack_into("<qQ", data, DYNAMIC - DATA + 16 * i, tag, val)
    struct.pack_into("<Q", data, INIT_ARRAY - DATA, 0x40)
    return [(TEXT, bytes(text)), (RODATA, bytes(rodata)), (DATA, bytes(data))]


class Fixture(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)

    def write_nso(self, **kw) -> Path:
        segs = fixture_image(**{k: kw.pop(k) for k in ("mod0_bss_end", "eh_version") if k in kw})
        path = Path(self.tmp.name) / "main"
        path.write_bytes(build_nso(segs, bss_size=kw.pop("bss_size", BSS_SIZE), dynsym=(DYNSYM - RODATA, 48), **kw))
        return path


class ReadModule(Fixture):
    def test_image_and_dynamic_table(self):
        mod = nso2elf.read_module(self.write_nso())
        self.assertEqual(len(mod.image), DATA + SEG_SIZE)
        self.assertEqual(mod.mod0, MOD0)
        self.assertEqual(mod.dynamic, DYNAMIC)
        self.assertEqual(mod.dyn[DT["SYMTAB"]], DYNSYM)
        self.assertEqual(mod.dyn[DT["STRSZ"]], 0x10)
        self.assertNotIn(DT["NULL"], mod.dyn)
        self.assertEqual(mod.eh_frame_hdr, (EH_HDR, EH_HDR + 12))
        self.assertEqual(mod.dynsym_size, 48)

    def test_bss_end_is_the_larger_of_mod0_and_header(self):
        self.assertEqual(nso2elf.read_module(self.write_nso()).bss_end, DATA + SEG_SIZE + BSS_SIZE)
        self.assertEqual(nso2elf.read_module(self.write_nso(mod0_bss_end=0x400)).bss_end, 0x400)

    def test_missing_mod0_rejected(self):
        segs = fixture_image()
        text = bytearray(segs[0][1])
        text[MOD0:MOD0 + 4] = b"XXXX"
        segs[0] = (TEXT, bytes(text))
        path = Path(self.tmp.name) / "bad"
        path.write_bytes(build_nso(segs))
        with self.assertRaisesRegex(ValueError, "MOD0"):
            nso2elf.read_module(path)


class Encoding(unittest.TestCase):
    def test_read_encoded_formats(self):
        buf = struct.pack("<IiQhH", 7, -4, 1 << 40, -2, 9)
        self.assertEqual(nso2elf.read_encoded(buf, 0, 0x03, 0), (7, 4))
        self.assertEqual(nso2elf.read_encoded(buf, 4, 0x0B, 0), (-4, 4))
        self.assertEqual(nso2elf.read_encoded(buf, 8, 0x04, 0), (1 << 40, 8))
        self.assertEqual(nso2elf.read_encoded(buf, 16, 0x0A, 0), (-2, 2))
        self.assertEqual(nso2elf.read_encoded(buf, 18, 0x02, 0), (9, 2))

    def test_read_encoded_pcrel(self):
        buf = struct.pack("<i", -0x10)
        self.assertEqual(nso2elf.read_encoded(buf, 0, 0x1B, 0x100), (0xF0, 4))

    def test_eh_frame_extent_walks_to_terminator(self):
        img = bytearray(0x100)
        img[0:4] = bytes([1, 0x1B, 0x03, 0x3B])
        struct.pack_into("<i", img, 4, 0x40 - 4)  # eh_frame at 0x40
        struct.pack_into("<I", img, 0x40, 8)       # record of 8 bytes
        struct.pack_into("<IQ", img, 0x4C, 0xFFFFFFFF, 4)  # 64-bit length record: 12 + 4 bytes
        struct.pack_into("<I", img, 0x5C, 0)
        self.assertEqual(nso2elf.eh_frame_extent(bytes(img), 0), (0x40, 0x20))

    def test_eh_frame_hdr_version_checked(self):
        with self.assertRaisesRegex(ValueError, "version"):
            nso2elf.eh_frame_extent(bytes([2, 0x1B, 0, 0, 0, 0, 0, 0]), 0)


class GnuHash(unittest.TestCase):
    def module(self, buckets, chains, symoffset=1):
        img = struct.pack(f"<IIII Q {len(buckets)}I {len(chains)}I", len(buckets), symoffset, 1, 6, 0,
                          *buckets, *chains)
        return types.SimpleNamespace(image=img, dyn={DT["GNU_HASH"]: 0})

    def test_chain_walked_to_end_bit(self):
        # Buckets point at symbols 1 and 2; symbol 3 ends the last chain.
        mod = self.module([1, 2], [0x10, 0x20, 0x31])
        self.assertEqual(nso2elf.gnu_hash_size(mod), 16 + 8 + 8 + 12)

    def test_empty_buckets(self):
        mod = self.module([0, 0], [])
        self.assertEqual(nso2elf.gnu_hash_size(mod), 16 + 8 + 8)


class BuildElf(Fixture):
    def elf(self, **kw):
        mod = nso2elf.read_module(self.write_nso(**kw))
        return mod, nso2elf.build_elf(mod)

    def test_sections_tile_each_segment(self):
        mod = nso2elf.read_module(self.write_nso())
        secs = nso2elf.collect_sections(mod)
        layout = [(s.name, s.addr, s.size) for s in secs]
        self.assertEqual(layout, [
            (".text", 0, 0x100),
            (".dynsym", DYNSYM, 48), (".dynstr", DYNSTR, 0x10), (".eh_frame_hdr", EH_HDR, 12),
            (".rodata", EH_HDR + 12, EH_FRAME - EH_HDR - 12), (".eh_frame", EH_FRAME, 0x18),
            (".rodata.1", EH_FRAME + 0x18, 0x200 - EH_FRAME - 0x18),
            (".dynamic", DYNAMIC, 0x60), (".init_array", INIT_ARRAY, 8),
            (".data", INIT_ARRAY + 8, 0x300 - INIT_ARRAY - 8),
            (".bss", 0x300, BSS_SIZE),
        ])

    def test_overlapping_section_skipped_with_warning(self):
        mod = nso2elf.read_module(self.write_nso())
        mod.dyn[DT["STRTAB"]] = DYNSYM + 8  # inside .dynsym
        err = io.StringIO()
        with contextlib.redirect_stderr(err):
            names = [s.name for s in nso2elf.collect_sections(mod)]
        self.assertNotIn(".dynstr", names)
        self.assertIn("overlaps", err.getvalue())

    def test_elf_headers_and_segments(self):
        mod, raw = self.elf()
        elf = ELFFile(io.BytesIO(raw))
        self.assertEqual(elf["e_type"], "ET_DYN")
        self.assertEqual(elf["e_machine"], "EM_AARCH64")
        segs = [(s["p_type"], s["p_vaddr"], s["p_filesz"], s["p_memsz"]) for s in elf.iter_segments()]
        self.assertEqual(segs, [
            ("PT_LOAD", 0, 0x100, 0x100), ("PT_LOAD", 0x100, 0x100, 0x100),
            ("PT_LOAD", 0x200, 0x100, 0x100 + BSS_SIZE),
            ("PT_DYNAMIC", DYNAMIC, 0x60, 0x60), ("PT_GNU_EH_FRAME", EH_HDR, 12, 12),
        ])
        for s in elf.iter_segments():
            self.assertEqual(s["p_offset"], nso2elf.FILE_BASE + s["p_vaddr"])

    def test_section_contents_and_links(self):
        mod, raw = self.elf()
        elf = ELFFile(io.BytesIO(raw))
        dynsym = elf.get_section_by_name(".dynsym")
        self.assertEqual(elf.get_section(dynsym["sh_link"]).name, ".dynstr")
        self.assertEqual(dynsym["sh_entsize"], 24)
        self.assertEqual(elf.get_section_by_name(".text").data(), mod.image[:0x100])
        self.assertEqual(elf.get_section_by_name(".bss")["sh_type"], "SHT_NOBITS")
        self.assertEqual(elf.get_section_by_name(".init_array")["sh_type"], "SHT_INIT_ARRAY")
        self.assertEqual(elf.get_section_by_name(".shstrtab")["sh_type"], "SHT_STRTAB")
        self.assertEqual(elf["e_shstrndx"], elf.num_sections() - 1)

    def test_dynamic_symbols_readable(self):
        _, raw = self.elf()
        elf = ELFFile(io.BytesIO(raw))
        names = [s.name for s in elf.get_section_by_name(".dynsym").iter_symbols()]
        self.assertEqual(names, ["", "fn_one"])

    def test_main_writes_file(self):
        src = self.write_nso()
        out = Path(self.tmp.name) / "sub" / "main.elf"
        with unittest.mock.patch("sys.argv", ["nso2elf.py", str(src), str(out)]), \
                contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(nso2elf.main(), 0)
        self.assertEqual(out.read_bytes()[:4], b"\x7fELF")


if __name__ == "__main__":
    unittest.main()
