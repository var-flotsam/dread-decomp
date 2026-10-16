"""Itanium C++ RTTI recovery: type_info objects, class hierarchy, and vtables.

type_info objects are found through their first word, which is relocated
against one of the libc++abi type_info vtables (imported symbols). Vtables are
found as [offset_to_top][&type_info][slot...] where the type_info pointer is a
RELATIVE relocation to a known type_info.
"""

from __future__ import annotations

import subprocess
from dataclasses import dataclass, field

from image import R_AARCH64_RELATIVE, Image

KINDS = {
    "_ZTVN10__cxxabiv117__class_type_infoE": "class",
    "_ZTVN10__cxxabiv120__si_class_type_infoE": "si",
    "_ZTVN10__cxxabiv121__vmi_class_type_infoE": "vmi",
}
CXXFILT = r"C:\Program Files\LLVM\bin\llvm-cxxfilt.exe"


@dataclass
class TypeInfo:
    addr: int
    kind: str
    mangled: str
    name: str = ""
    bases: list[dict] = field(default_factory=list)   # {typeinfo, offset, virtual, public}
    vtables: list[dict] = field(default_factory=list)  # {addr, offset_to_top, slots}

    @property
    def size(self) -> int:
        return {"class": 16, "si": 24}.get(self.kind, 24 + 16 * len(self.bases))


def demangle_types(mangled: list[str]) -> list[str]:
    """Demangle bare type names via llvm-cxxfilt (as _ZTS<type>, 'typeinfo name for X')."""
    if not mangled:
        return []
    out = subprocess.run([CXXFILT], input="\n".join("_ZTS" + m for m in mangled), capture_output=True,
                         text=True, check=True).stdout.splitlines()
    prefix = "typeinfo name for "
    return [o[len(prefix):] if o.startswith(prefix) else m for o, m in zip(out, mangled)]


def find_typeinfos(img: Image) -> dict[int, TypeInfo]:
    tis: dict[int, TypeInfo] = {}
    for addr, r in img.relocs.items():
        kind = KINDS.get(r.sym or "")
        if kind is None or r.addend != 16:
            continue
        name_ptr = img.pointer(addr + 8)
        mangled = img.cstring(name_ptr) if name_ptr is not None else None
        if not mangled:
            continue
        ti = TypeInfo(addr, kind, mangled)
        if kind == "si":
            base = img.pointer(addr + 16)
            if base is not None:
                ti.bases.append({"typeinfo": base, "offset": 0, "virtual": False, "public": True})
        elif kind == "vmi":
            count = img.u32(addr + 20)
            for i in range(min(count, 64)):
                entry = addr + 24 + 16 * i
                base = img.pointer(entry)
                flags = img.i64(entry + 8)
                if base is not None:
                    ti.bases.append({"typeinfo": base, "offset": flags >> 8,
                                     "virtual": bool(flags & 1), "public": bool(flags & 2)})
        tis[addr] = ti
    for ti, name in zip(tis.values(), demangle_types([t.mangled for t in tis.values()])):
        ti.name = name
    return tis


def find_vtables(img: Image, tis: dict[int, TypeInfo]) -> None:
    inside_ti = set()
    for ti in tis.values():
        inside_ti.update(range(ti.addr, ti.addr + ti.size, 8))
    for addr, r in sorted(img.relocs.items()):
        if r.type != R_AARCH64_RELATIVE or r.addend not in tis or addr in inside_ti:
            continue
        top = addr - 8
        if top in img.relocs:
            continue
        offset_to_top = img.i64(top)
        if not -0x1000000 < offset_to_top <= 0:
            continue
        slots = []
        s = addr + 8
        while True:
            rr = img.relocs.get(s)
            if rr is None:
                break
            if rr.type == R_AARCH64_RELATIVE and img.in_text(rr.addend):
                slots.append(rr.addend)
            elif rr.sym and rr.type in (257, 1025):   # imported, e.g. __cxa_pure_virtual
                slots.append(rr.sym)
            else:
                break
            # The next vtable in the group starts with offset_to_top (no reloc) + type_info pointer.
            s += 8
            nxt = img.relocs.get(s + 8)
            if s not in img.relocs and nxt is not None and nxt.addend in tis:
                break
        tis[r.addend].vtables.append({"addr": top, "offset_to_top": offset_to_top, "slots": slots})


def recover(img: Image) -> dict[int, TypeInfo]:
    tis = find_typeinfos(img)
    find_vtables(img, tis)
    return tis
