"""Function boundaries from .eh_frame FDEs (exact start and length for every function
the compiler emitted unwind info for)."""

from __future__ import annotations

import struct

from image import Image


def _uleb(buf: bytes, off: int) -> tuple[int, int]:
    result = shift = 0
    while True:
        b = buf[off]
        off += 1
        result |= (b & 0x7F) << shift
        shift += 7
        if not b & 0x80:
            return result, off


def _read_ptr(buf: bytes, off: int, enc: int) -> tuple[int, int]:
    fmt = {0x00: "<Q", 0x03: "<I", 0x0B: "<i", 0x04: "<Q", 0x0C: "<q", 0x02: "<H", 0x0A: "<h"}[enc & 0x0F]
    (val,) = struct.unpack_from(fmt, buf, off)
    if enc & 0x70 == 0x10:  # pcrel
        val += off
    return val & 0xFFFFFFFFFFFFFFFF, off + struct.calcsize(fmt)


def fde_functions(img: Image) -> list[tuple[int, int]]:
    """Return sorted (start, size) pairs, module-relative."""
    buf = img.data
    hdr = img.mod.eh_frame_hdr[0]
    start = struct.unpack_from("<i", buf, hdr + 4)[0] + hdr + 4  # eh_frame_ptr: pcrel sdata4
    cie_enc: dict[int, int] = {}
    out = []
    off = start
    while True:
        (length,) = struct.unpack_from("<I", buf, off)
        if length == 0:
            break
        body = off + 4
        nxt = body + length
        (cie_id,) = struct.unpack_from("<I", buf, body)
        if cie_id == 0:  # CIE: find the FDE pointer encoding from the 'R' augmentation
            p = body + 5
            aug_end = buf.index(b"\0", p)
            aug = buf[p:aug_end].decode()
            p = aug_end + 1
            _, p = _uleb(buf, p)            # code alignment
            _, p = _uleb(buf, p)            # data alignment (sleb, but only skipping)
            p += 1                           # return address register
            enc = 0x00
            if aug.startswith("z"):
                _, p = _uleb(buf, p)
                for ch in aug[1:]:
                    if ch == "R":
                        enc = buf[p]
                        p += 1
                    elif ch == "P":
                        penc = buf[p]
                        _, p = _read_ptr(buf, p + 1, penc)
                    elif ch == "L":
                        p += 1
            cie_enc[off] = enc
        else:
            cie = body - cie_id
            enc = cie_enc.get(cie, 0x1B)
            pc, p = _read_ptr(buf, body + 4, enc)
            size, _ = _read_ptr(buf, p, enc & 0x0F)
            out.append((pc, size))
        off = nxt
    return sorted(set(out))
