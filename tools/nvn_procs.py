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
       python tools/nvn_procs.py --version 2.1.0 --sites NAME[,NAME...]
         (prints each call site of those functions and the function it is in; writes nothing)
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


def is_branch(w: int) -> bool:
    """B, BL, B.cond, CBZ/CBNZ, TBZ/TBNZ, BR/BLR/RET: the window of a GOT load ends at one (a call
    also clobbers x0-x18)."""
    return ((w & 0x7C000000) == 0x14000000 or (w & 0xFF000010) == 0x54000000 or (w & 0x7E000000) == 0x34000000
            or (w & 0x7E000000) == 0x36000000 or (w & 0xFE000000) == 0xD6000000)


def writes_register(w: int, reg: int) -> bool:
    """Whether an instruction may write general register `reg` (conservative: unknown kinds that
    name it as Rd count). Loads write Rt (Rt and Rt2 for a pair) unless they load a SIMD register;
    pre- and post-indexed forms write their base; stores write nothing else."""
    if (w & 0x0A000000) == 0x08000000:  # loads and stores
        simd = (w >> 26) & 1
        rt, rn = w & 31, (w >> 5) & 31
        kind = (w >> 27) & 7
        if kind == 0b101:  # pairs: LDP/STP and their pre/post-indexed forms
            if ((w >> 23) & 3) in (1, 3) and rn == reg:
                return True
            return bool((w >> 22) & 1) and not simd and reg in (rt, (w >> 10) & 31)
        if kind == 0b111:  # single register: unsigned offset, register offset, pre/post-index
            if not (w >> 24) & 1 and not (w >> 21) & 1 and ((w >> 10) & 3) in (1, 3) and rn == reg:
                return True
            return ((w >> 22) & 3) != 0 and not simd and rt == reg  # opc 0 is a store
        if kind == 0b011:  # load literal
            return not simd and rt == reg
        return rt == reg  # exclusives and the rest: conservative
    return (w & 31) == reg


def loader_size() -> int:
    """fn_7100154770's size from data/functions/ (the eh_frame bounds)."""
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(encoding="utf-8") as fh:
            for row in csv.DictReader(fh):
                if int(row["address"], 16) == LOADER:
                    return int(row["size"])
    raise SystemExit("fn_7100154770 is not in data/functions/")


def function_starts() -> list[int]:
    """Every function's start address from data/functions/, sorted."""
    starts = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(encoding="utf-8") as fh:
            starts.extend(int(row["address"], 16) for row in csv.DictReader(fh))
    return sorted(starts)


def cstring(data: bytes, off: int) -> str:
    end = data.index(b"\0", off)
    return data[off:end].decode("ascii")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--check", action="store_true", help="compare with data/nvn_procs.csv instead of writing it")
    ap.add_argument("--sites", help="comma-separated NVN names: print their call sites, write nothing")
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
        mov = word(pc + 8)  # mov x20, x0 (the first step keeps the device) or mov x0, x20
        if (mov & 0xFFE0FFE0) != 0xAA0003E0 or ((mov & 31) != 0 and ((mov >> 16) & 31) != 0) or                 (word(pc + 12) & 0xFFFFFC1F) != 0xD63F0000:
            raise SystemExit(f"loader step at {pc:#x} is not mov x0, xN or mov xN, x0, then blr")
        g = adrp(word(pc + 16), pc + 16)
        ld = ldr_x(word(pc + 20))
        st = str_x0(word(pc + 24))
        if not g or not ld or ld[1] != g[0] or ld[0] != g[0] or st != g[0]:
            raise SystemExit(f"loader step at {pc:#x} ({name}) does not store through a GOT slot")
        procs.append((name, g[1] + ld[2]))
        pc += 28
    # The steps must run to the epilogue: ldp x29, x30 / ldp x20, x19 / ret at the function's end
    # (its size from data/functions/).
    size = loader_size()
    if pc + 12 != LOADER + size or word(pc + 8) != 0xD65F03C0:
        raise SystemExit(f"loader decode stopped at {pc:#x} ({len(procs)} names), not at the epilogue before "
                         f"{LOADER + size:#x}: another build, or a step of another shape")
    loader_end = pc

    # Call sites: an adrp of the slot's page, then the ldr of the slot through that register.
    slot_names = {slot: name for name, slot in procs}
    uses = {slot: 0 for slot in slot_names}
    sites = {slot: [] for slot in slot_names}
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
                    sites[slot].append(addr)
                break
            if is_branch(nxt) or writes_register(nxt, a[0]):
                break

    if args.sites:
        import bisect
        starts = function_starts()
        by_name = {name: slot for name, slot in procs}
        for name in args.sites.split(","):
            if name not in by_name:
                raise SystemExit(f"{name} is not one of the loader's names")
            for site in sites[by_name[name]]:
                fn = starts[bisect.bisect_right(starts, site) - 1]
                print(f"{name} {site:#x} fn_{fn:x}")
        return 0

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
        return 1 if unknown else 0
    OUT.write_text(text, encoding="utf-8", newline="\n")
    print(f"wrote {OUT.relative_to(ROOT).as_posix()}")
    return 1 if unknown else 0


if __name__ == "__main__":
    sys.exit(main())
