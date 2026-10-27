#!/usr/bin/env python3
"""The game's NVN function table: which NVN function each of the loader's globals holds.

The graphics start-up fn_71001587f0 hands nvnDeviceGetProcAddress to the loader fn_7100154770,
which asks it for 523 names in turn and stores each pointer through a GOT slot into a global of
its own; the rest of the game calls NVN through those globals (src/game_s7/fn_71001587f0.cpp calls
them g_unk_716e29xxxx). This tool reads the loader's code from orig/<version>/main.elf and writes
data/nvn_procs.csv: per name, the GOT slot, the global it fills (from the slot's relative
relocation), how many places outside the loader load that slot (the game's call sites through it),
and whether port/nvn/ registers an implementation (the NVN shim; a Register("<name>", ...) line in
port/nvn/src/). Everything in the CSV is a name the game's strings hold or an address: metadata, no
code bytes.

The loader is a run of the same seven instructions per name (decoded below, checked for every
step): adrp x1 / add x1 (the name), mov x0 (the device), blr (GetProcAddress), adrp x8 / ldr x8
(the GOT slot), str x0, [x8]. A call site is an adrp of the slot's page followed, within 8
instructions and before anything else writes that register, by the ldr of the slot through it (how
the compiler loads a GOT entry; it may schedule other instructions in between). Counting is from the
code alone, so a slot reached in another way would be missed; none is known.

Usage: python tools/nvn_procs.py --version 2.1.0 [--check]   (--check: compare with the CSV, write nothing)
Standard library and pyelftools (already a dependency of tools/asmdiff.py).
"""

from __future__ import annotations

import argparse
import csv
import io
import re
import struct
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parent.parent
BASE = 0x7100000000
LOADER = 0x7100154770   # fn_7100154770: the NVN table loader (called twice by fn_71001587f0)
R_AARCH64_RELATIVE = 1027
OUT = ROOT / "data" / "nvn_procs.csv"
SHIM_SRC = ROOT / "port" / "nvn" / "src"


def adrp(w: int, pc: int) -> tuple[int, int] | None:
    """(register, page) of an ADRP, else None."""
    if (w & 0x9F000000) != 0x90000000:
        return None
    imm = (((w >> 5) & 0x7FFFF) << 2) | ((w >> 29) & 3)
    if imm & (1 << 20):
        imm -= 1 << 21
    return w & 31, ((pc >> 12) << 12) + (imm << 12)


def add_imm(w: int) -> tuple[int, int, int] | None:
    """(rd, rn, imm) of a 64-bit ADD (immediate, no shift), else None."""
    if (w & 0xFFC00000) != 0x91000000:
        return None
    return w & 31, (w >> 5) & 31, (w >> 10) & 0xFFF


def ldr_x(w: int) -> tuple[int, int, int] | None:
    """(rt, rn, offset) of LDR Xt, [Xn, #imm], else None."""
    if (w & 0xFFC00000) != 0xF9400000:
        return None
    return w & 31, (w >> 5) & 31, ((w >> 10) & 0xFFF) * 8


def str_x0(w: int) -> int | None:
    """rn of STR X0, [Xn] (offset 0), else None."""
    if (w & 0xFFFFFC1F) != 0xF9000000:
        return None
    return (w >> 5) & 31


def writes_register(w: int, reg: int) -> bool:
    """Whether an instruction may write register `reg` (conservative: any instruction whose Rd/Rt
    field is `reg`, except stores, compares and branches, which do not write it)."""
    if (w & 31) != reg:
        return False
    store = (w & 0x0A000000) == 0x08000000 and not (w & 0x00400000)  # load/store class, L bit clear
    compare = (w & 0x1F000000) in (0x0B000000, 0x11000000, 0x31000000) and reg == 31
    branch = (w & 0x7C000000) == 0x14000000 or (w & 0xFE000000) == 0xD6000000
    return not (store or compare or branch)


def cstring(data: bytes, off: int) -> str:
    end = data.index(b"\0", off)
    return data[off:end].decode("ascii")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--check", action="store_true", help="compare with data/nvn_procs.csv instead of writing it")
    args = ap.parse_args()
    elf_path = ROOT / "orig" / args.version / "main.elf"
    if not elf_path.is_file():
        raise SystemExit(f"{elf_path} missing (tools/nso2elf.py)")
    with elf_path.open("rb") as fh:
        elf = ELFFile(fh)
        segments = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        image = bytearray(max(s["p_vaddr"] + s["p_memsz"] for s in segments))
        for s in segments:
            image[s["p_vaddr"]:s["p_vaddr"] + s["p_filesz"]] = s.data()
        relative = {}
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection):
                for r in sec.iter_relocations():
                    if r["r_info_type"] == R_AARCH64_RELATIVE:
                        relative[r["r_offset"]] = r["r_addend"]
    data = bytes(image)
    words = struct.unpack_from(f"<{len(data) // 4}I", data)

    def word(addr: int) -> int:
        return words[(addr - BASE) // 4]

    # The loader: the seven-instruction step per name, until the epilogue.
    procs = []  # (name, GOT slot)
    pc = LOADER + 0x10  # after the prologue: stp, stp, add x29, mov x19, x1
    if word(LOADER + 0xC) != 0xAA0103F3:  # mov x19, x1
        raise SystemExit("fn_7100154770 does not start as expected (another build?)")
    while True:
        a = adrp(word(pc), pc)
        b = add_imm(word(pc + 4))
        if not a or not b or a[0] != 1 or b[0] != 1 or b[1] != 1:
            break
        name = cstring(data, a[1] + b[2] - BASE)
        if (word(pc + 8) & 0xFFE0FFE0) != 0xAA0003E0 or (word(pc + 12) & 0xFFFFFC1F) != 0xD63F0000:
            raise SystemExit(f"loader step at {pc:#x} is not mov/blr")
        g = adrp(word(pc + 16), pc + 16)
        ld = ldr_x(word(pc + 20))
        st = str_x0(word(pc + 24))
        if not g or not ld or ld[1] != g[0] or ld[0] != g[0] or st != g[0]:
            raise SystemExit(f"loader step at {pc:#x} ({name}) does not store through a GOT slot")
        procs.append((name, g[1] + ld[2]))
        pc += 28
    if len(procs) < 100:
        raise SystemExit(f"only {len(procs)} loader steps decoded")
    loader_end = pc

    # Call sites: an adrp of the slot's page, then the ldr of the slot through that register.
    slot_names = {slot: name for name, slot in procs}
    uses = {slot: 0 for slot in slot_names}
    text_end = BASE + segments[0]["p_filesz"]
    for addr in range(BASE, text_end - 4, 4):
        if LOADER <= addr < loader_end:
            continue
        i = (addr - BASE) // 4
        a = adrp(words[i], addr)
        if not a:
            continue
        for k in range(1, 9):
            if i + k >= len(words):
                break
            nxt = words[i + k]
            ld = ldr_x(nxt)
            if ld and ld[1] == a[0]:
                slot = a[1] + ld[2]
                if slot in uses:
                    uses[slot] += 1
                break
            if writes_register(nxt, a[0]):
                break

    implemented = set()
    for src in sorted(SHIM_SRC.glob("*.cpp")):
        implemented.update(re.findall(r'Register\("(nvn\w+)"', src.read_text(encoding="utf-8")))

    buf = io.StringIO()
    w = csv.writer(buf, lineterminator="\n")
    w.writerow(["name", "got_slot", "storage", "call_sites", "shim"])
    for name, slot in procs:
        storage = relative.get(slot - BASE)
        w.writerow([name, f"{slot:#x}", f"{BASE + storage:#x}" if storage is not None else "",
                    uses[slot], "implemented" if name in implemented else ""])
    text = buf.getvalue()
    used = sum(1 for s in uses.values() if s)
    impl_used = sum(1 for name, slot in procs if uses[slot] and name in implemented)
    print(f"{len(procs)} NVN functions in fn_7100154770; {used} with call sites outside it; "
          f"the shim implements {len(implemented & set(slot_names.values()))} ({impl_used} of the {used} used)")
    unknown = sorted(implemented - set(slot_names.values()))
    if unknown:
        print(f"registered by the shim but not asked for by the game: {', '.join(unknown)}", file=sys.stderr)
    if args.check:
        if not OUT.is_file() or OUT.read_text(encoding="utf-8") != text:
            print(f"{OUT.relative_to(ROOT).as_posix()} is out of date: run without --check", file=sys.stderr)
            return 1
        return 0
    OUT.write_text(text, encoding="utf-8", newline="\n")
    print(f"wrote {OUT.relative_to(ROOT).as_posix()}")
    return 1 if unknown else 0


if __name__ == "__main__":
    sys.exit(main())
