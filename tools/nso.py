"""Parse Nintendo Switch NSO executables.

Reads the header, decompresses the text/rodata/data segments (LZ4 block), and
checks them against the SHA-256 hashes in the header. Used by verify_dump.py and,
later, by the NSO-to-ELF converter.
"""

from __future__ import annotations

import hashlib
import struct
from dataclasses import dataclass
from pathlib import Path

import lz4.block

MAGIC = b"NSO0"
SEGMENT_NAMES = ("text", "rodata", "data")


@dataclass
class Segment:
    name: str
    file_offset: int
    mem_offset: int
    size: int           # decompressed size
    file_size: int      # size stored in the file (compressed or not)
    compressed: bool
    check_hash: bool
    sha256: bytes
    data: bytes = b""


@dataclass
class Nso:
    path: Path
    build_id: bytes
    module_name: str
    bss_size: int
    segments: list[Segment]
    # (offset, size) pairs relative to the start of rodata.
    api_info: tuple[int, int]
    dynstr: tuple[int, int]
    dynsym: tuple[int, int]

    def segment(self, name: str) -> Segment:
        return next(s for s in self.segments if s.name == name)

    def verify(self) -> list[str]:
        """Return one message per segment whose SHA-256 doesn't match the header."""
        errors = []
        for s in self.segments:
            if not s.check_hash:
                continue
            got = hashlib.sha256(s.data).digest()
            if got != s.sha256:
                errors.append(f"{self.path.name}: {s.name} sha256 {got.hex()} != header {s.sha256.hex()}")
        return errors


def load(path: Path) -> Nso:
    raw = path.read_bytes()
    if raw[:4] != MAGIC:
        raise ValueError(f"{path} is not an NSO (magic {raw[:4]!r})")

    (flags,) = struct.unpack_from("<I", raw, 0x0C)
    seg_headers = [struct.unpack_from("<III", raw, off) for off in (0x10, 0x20, 0x30)]
    (module_name_offset,) = struct.unpack_from("<I", raw, 0x1C)
    (module_name_size,) = struct.unpack_from("<I", raw, 0x2C)
    (bss_size,) = struct.unpack_from("<I", raw, 0x3C)
    build_id = raw[0x40:0x60]
    file_sizes = struct.unpack_from("<III", raw, 0x60)
    api_info, dynstr, dynsym = (struct.unpack_from("<II", raw, off) for off in (0x88, 0x90, 0x98))
    hashes = [raw[off:off + 0x20] for off in (0xA0, 0xC0, 0xE0)]

    segments = []
    for i, name in enumerate(SEGMENT_NAMES):
        file_offset, mem_offset, size = seg_headers[i]
        compressed = bool(flags & (1 << i))
        stored = raw[file_offset:file_offset + file_sizes[i]]
        data = lz4.block.decompress(stored, uncompressed_size=size) if compressed else stored
        if len(data) != size:
            raise ValueError(f"{path}: {name} is {len(data)} bytes after decompression, expected {size}")
        segments.append(Segment(
            name=name, file_offset=file_offset, mem_offset=mem_offset, size=size,
            file_size=file_sizes[i], compressed=compressed,
            check_hash=bool(flags & (1 << (i + 3))), sha256=hashes[i], data=data,
        ))

    name_raw = raw[module_name_offset:module_name_offset + module_name_size]
    module_name = name_raw.split(b"\0", 1)[0].decode("utf-8", "replace")

    return Nso(path=path, build_id=build_id, module_name=module_name, bss_size=bss_size,
               segments=segments, api_info=api_info, dynstr=dynstr, dynsym=dynsym)
