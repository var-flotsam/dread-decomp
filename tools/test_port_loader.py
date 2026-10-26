#!/usr/bin/env python3
"""Test the port's data loader on the user's dump: build its core into a library, run it in this
process, and check the mapped image against an independent decoding in Python.

The loader (port/runtime/include/dread_port_loader.h, port/runtime/src/loader.cpp) is freestanding,
so it builds with nothing but clang and lld: on Windows a DLL linked with lld-link against a
kernel32 import library made here by llvm-dlltool from four names (no Windows SDK, no C runtime);
elsewhere a shared object. port/tests/loader_test.cpp adds the host's memory services
(port/runtime/src/host_memory.h), two functions for the function table to point at, and recorders
in place of the runtime's traps. The test then maps orig/<version>/exefs/main at 0x7100000000 in
this process and checks:

  - refusals before anything is mapped: not an NSO, the wrong module ID, an unsorted function
    table, a data reference outside the image;
  - the mapped .text, .rodata and .data byte for byte against tools/nso.py's decompression with
    every relocation applied in Python (relative ones to base + addend), and samples of the .bss
    (zero);
  - every code pointer (a relative relocation into .text): one in two distinct targets is put in
    the function table, alternately at test_code_a and test_code_b, and must read back as that
    function; every other one must be a trap thunk whose argument is its original address and whose
    target is the missing-function recorder; calling one through ctypes must record the address;
  - the imports: __cxa_pure_virtual resolves through the import table; the others are thunks to the
    missing-import recorder carrying their name;
  - the counts the loader reports, the protection of the read-only part (Windows: VirtualQuery),
    and that a second load fails because the range is taken.

With --tables (default: build/port/link/image_tables.s from tools/port_link.py, when present) it
also passes the link's data references, the addresses of every g_unk_ symbol, and the loader must
find each inside the image. Game bytes stay in memory and in build/; nothing is written to the
repository. Exit status 0 when every check passes.

Usage:
  python tools/test_port_loader.py --version 2.1.0
  python tools/test_port_loader.py --version 2.1.0 --tables build/port/link_m1/image_tables.s
"""

from __future__ import annotations

import argparse
import ctypes
import re
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import nso  # noqa: E402
import nso2elf  # noqa: E402
import port_sweep  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
BASE = 0x7100000000
SOURCES = [ROOT / "port" / "runtime" / "src" / "loader.cpp", ROOT / "port" / "tests" / "loader_test.cpp"]
INCLUDES = [ROOT / "include", ROOT / "port" / "runtime" / "include", ROOT / "port" / "runtime" / "src"]
FLAGS = ["-std=c++17", "-O2", "-ffreestanding", "-fno-builtin", "-fno-exceptions", "-fno-rtti",
         "-fno-stack-protector", "-fno-asynchronous-unwind-tables", "-c"]
KERNEL32 = ["VirtualAlloc", "VirtualProtect", "VirtualFree", "GetCurrentProcess", "FlushInstructionCache"]
R_RELATIVE, R_ABS64, R_GLOB_DAT, R_JUMP_SLOT = 1027, 257, 1025, 1026
THUNK = 32
STATUS = ["Ok", "NotNso", "WrongBuild", "BadSegment", "BadModule", "MapFailed", "BadRelocation", "BadTable",
          "ThunkMapFailed", "DataOutsideImage", "ProtectFailed"]


class Function(ctypes.Structure):
    _fields_ = [("address", ctypes.c_uint64), ("code", ctypes.c_void_p)]


class Import(ctypes.Structure):
    _fields_ = [("name", ctypes.c_char_p), ("code", ctypes.c_void_p)]


class Options(ctypes.Structure):
    _fields_ = [("base", ctypes.c_uint64), ("expected_build_id", ctypes.c_void_p),
                ("functions", ctypes.c_void_p), ("function_count", ctypes.c_uint64),
                ("imports", ctypes.c_void_p), ("import_count", ctypes.c_uint64),
                ("data_references", ctypes.c_void_p), ("data_reference_count", ctypes.c_uint64),
                ("missing_function", ctypes.c_void_p), ("missing_import", ctypes.c_void_p)]


class Info(ctypes.Structure):
    _fields_ = [("base", ctypes.c_uint64), ("text_end", ctypes.c_uint64), ("rodata_begin", ctypes.c_uint64),
                ("rodata_end", ctypes.c_uint64), ("data_begin", ctypes.c_uint64), ("data_end", ctypes.c_uint64),
                ("bss_end", ctypes.c_uint64), ("build_id", ctypes.c_uint8 * 0x20),
                ("relocations", ctypes.c_uint64), ("code_pointers", ctypes.c_uint64),
                ("code_translated", ctypes.c_uint64), ("imports", ctypes.c_uint64),
                ("imports_resolved", ctypes.c_uint64), ("thunks", ctypes.c_uint64),
                ("data_references_checked", ctypes.c_uint64), ("first_bad_value", ctypes.c_uint64)]


def run(cmd: list[str]) -> None:
    r = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8", errors="replace")
    if r.returncode != 0:
        raise SystemExit(f"failed: {' '.join(cmd)}\n{r.stdout}{r.stderr}")


def build(cxx: str, out: Path) -> Path:
    """The test library from SOURCES, with nothing but the compiler and lld."""
    out.mkdir(parents=True, exist_ok=True)
    windows = sys.platform == "win32"
    target = "x86_64-pc-windows-msvc" if windows else None
    objs = []
    for src in SOURCES:
        obj = out / (src.stem + ".o")
        run([cxx, *([f"--target={target}"] if target else ["-fPIC"]), *FLAGS, *[f"-I{i}" for i in INCLUDES],
             str(src), "-o", str(obj)])
        objs.append(str(obj))
    if not windows:
        lib = out / "loader_test.so"
        run([cxx, "-shared", "-o", str(lib), *objs])
        return lib
    # kernel32.lib from our own list of names: llvm-dlltool writes the import library.
    dlltool = port_sweep_tool(cxx, "llvm-dlltool")
    lld_link = port_sweep_tool(cxx, "lld-link")
    deffile = out / "kernel32.def"
    deffile.write_text("LIBRARY kernel32.dll\nEXPORTS\n" + "".join(f"  {n}\n" for n in KERNEL32), encoding="ascii")
    implib = out / "kernel32.lib"
    run([dlltool, "-m", "i386:x86-64", "-d", str(deffile), "-l", str(implib)])
    lib = out / "loader_test.dll"
    run([lld_link, "/dll", "/noentry", "/nodefaultlib", "/machine:x64", f"/out:{lib}", *objs, str(implib)])
    return lib


def port_sweep_tool(cxx: str, name: str) -> str:
    exe = Path(cxx)
    cand = exe.parent / (name + exe.suffix)
    if not cand.is_file():
        raise SystemExit(f"{name} not found next to {cxx}")
    return str(cand)


def read_tables(path: Path) -> list[int]:
    """The data references of tools/port_link.py's image_tables.s."""
    text = path.read_text(encoding="utf-8")
    m = re.search(r"^dread_port_data_references:\n(.*?)^\t\.globl", text, re.M | re.S)
    if not m:
        raise SystemExit(f"{path}: no dread_port_data_references table")
    return [int(v, 16) for v in re.findall(r"\.quad\t0x([0-9a-f]+)", m.group(1))]


class Checker:
    def __init__(self) -> None:
        self.failures = 0
        self.checks = 0

    def __call__(self, ok: bool, what: str) -> None:
        self.checks += 1
        if not ok:
            self.failures += 1
            print(f"FAIL {what}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--version", required=True)
    ap.add_argument("--cxx", default=None, help="clang++ (default: as tools/port_sweep.py)")
    ap.add_argument("--tables", default=None, help="tools/port_link.py's image_tables.s for the data references")
    ap.add_argument("--out", default=str(ROOT / "build" / "port" / "loader_test"))
    args = ap.parse_args()

    main_nso = ROOT / "orig" / args.version / "exefs" / "main"
    if not main_nso.is_file():
        raise SystemExit(f"{main_nso} not found: run tools/setup.py first")
    cxx = args.cxx or port_sweep.host_cxx()
    lib_path = build(cxx, Path(args.out))
    lib = ctypes.CDLL(str(lib_path))
    for name in ("test_code_a", "test_code_b", "test_last_missing_function", "test_missing_calls"):
        getattr(lib, name).restype = ctypes.c_uint64
    lib.test_code_a.argtypes = lib.test_code_b.argtypes = [ctypes.c_uint64]
    lib.test_last_missing_import.restype = ctypes.c_void_p
    lib.test_load.restype = ctypes.c_int32
    lib.test_load.argtypes = [ctypes.c_char_p, ctypes.c_uint64, ctypes.POINTER(Options), ctypes.POINTER(Info)]
    lib.test_unmap.restype = ctypes.c_bool
    lib.test_unmap.argtypes = [ctypes.c_uint64, ctypes.c_uint64]
    addr = lambda name: ctypes.cast(getattr(lib, name), ctypes.c_void_p).value  # noqa: E731
    code_a, code_b = addr("test_code_a"), addr("test_code_b")
    rec_fn, rec_imp = addr("test_record_missing_function"), addr("test_record_missing_import")
    print(f"library: {port_sweep.rel(lib_path)}")

    # The independent decoding: tools/nso.py (lz4 package) and nso2elf's MOD0 and dynamic parsing.
    raw = main_nso.read_bytes()
    mod = nso2elf.read_module(main_nso)
    header = nso.load(main_nso)
    img, dyn, D = mod.image, mod.dyn, nso2elf.DT
    text_end = mod.text.mem_offset + mod.text.size
    data_begin, data_end = mod.data.mem_offset, mod.data.mem_offset + mod.data.size
    relocs = []
    for tab, size in ((dyn[D["RELA"]], dyn[D["RELASZ"]]), (dyn[D["JMPREL"]], dyn[D["PLTRELSZ"]])):
        for at in range(tab, tab + size, 24):
            relocs.append(struct.unpack_from("<QQq", img, at))
    symtab, strtab = dyn[D["SYMTAB"]], dyn[D["STRTAB"]]

    def sym(index: int) -> tuple[str, int, int]:
        st_name, _, _, shndx, value, _ = struct.unpack_from("<IBBHQQ", img, symtab + 24 * index)
        end = img.index(b"\0", strtab + st_name)
        return img[strtab + st_name:end].decode(), shndx, value

    code_targets = sorted({a for _, info, a in relocs if info & 0xffffffff == R_RELATIVE and a < text_end})
    table = {BASE + t: (code_a if i % 2 == 0 else code_b) for i, t in enumerate(code_targets) if i % 4 < 2}
    functions = (Function * len(table))(*[Function(a, c) for a, c in sorted(table.items())])
    imports = (Import * 1)(Import(b"__cxa_pure_virtual", code_a))
    build_id = (ctypes.c_uint8 * 0x20)(*header.build_id)
    data_refs = read_tables(Path(args.tables)) if args.tables else []
    if not args.tables:
        default = ROOT / "build" / "port" / "link" / "image_tables.s"
        if default.is_file():
            data_refs = read_tables(default)
    refs = (ctypes.c_uint64 * max(1, len(data_refs)))(*data_refs)

    def options(**over) -> Options:
        o = Options(BASE, ctypes.addressof(build_id), ctypes.addressof(functions), len(functions),
                    ctypes.addressof(imports), 1, ctypes.addressof(refs), len(data_refs), rec_fn, rec_imp)
        for k, v in over.items():
            setattr(o, k, v)
        return o

    def load(o: Options, data: bytes = raw) -> tuple[str, Info]:
        info = Info()
        s = lib.test_load(data, len(data), ctypes.byref(o), ctypes.byref(info))
        return STATUS[s] if 0 <= s < len(STATUS) else str(s), info

    check = Checker()

    # 1. refusals before the mapping
    s, _ = load(options(), b"XXXX" + raw[4:])
    check(s == "NotNso", f"bad magic: {s}")
    wrong = (ctypes.c_uint8 * 0x20)(*bytes(b ^ 0xff for b in header.build_id))
    s, _ = load(options(expected_build_id=ctypes.addressof(wrong)))
    check(s == "WrongBuild", f"wrong module ID: {s}")
    unsorted = (Function * 2)(Function(BASE + 0x100, code_a), Function(BASE + 0x80, code_b))
    s, info = load(options(functions=ctypes.addressof(unsorted), function_count=2))
    check(s == "BadTable" and info.first_bad_value == BASE + 0x80, f"unsorted table: {s}")
    outside = (ctypes.c_uint64 * 1)(BASE + mod.bss_end + 0x1000)
    s, info = load(options(data_references=ctypes.addressof(outside), data_reference_count=1))
    check(s == "DataOutsideImage" and info.first_bad_value == outside[0], f"data reference outside: {s}")

    # 2. the load
    s, info = load(options())
    check(s == "Ok", f"load: {s} ({ctypes.string_at(lib.test_status_text(STATUS.index(s) if s in STATUS else 0))!r},"
                     f" value 0x{info.first_bad_value:x})" if s != "Ok" else "load")
    if s != "Ok":
        print(f"{check.failures} of {check.checks} checks failed")
        return 1
    print(f"loaded: .bss to +0x{info.bss_end:x}, {info.relocations} relocations, {info.code_translated} of"
          f" {info.code_pointers} code pointers translated, {info.imports_resolved} of {info.imports} imports"
          f" resolved, {info.thunks} thunks, {info.data_references_checked} data references checked")
    check(bytes(info.build_id) == header.build_id, "build ID reported")
    check((info.text_end, info.rodata_begin, info.data_begin, info.data_end) ==
          (text_end, mod.rodata.mem_offset, data_begin, data_end), "segment bounds")
    check(info.bss_end >= mod.bss_end, f"bss end 0x{info.bss_end:x} >= 0x{mod.bss_end:x}")
    check(info.data_references_checked == len(data_refs), "data references checked")

    # 3. every relocation, and the bytes around them
    expected = bytearray(img[:data_end])
    slots: dict[int, tuple] = {}  # offset -> ('thunk_fn', original) | ('thunk_import', name) | ('value', v)
    n_code = n_translated = n_imports = n_resolved = n_thunks = 0
    for off, rinfo, addend in relocs:
        t, si = rinfo & 0xffffffff, rinfo >> 32
        if t == R_RELATIVE:
            target = addend
        else:
            name, shndx, value = sym(si)
            add = addend if t == R_ABS64 else 0
            if shndx:
                target = value + add
            else:
                n_imports += 1
                if name == "__cxa_pure_virtual":
                    n_resolved += 1
                    slots[off] = ("value", code_a + add)
                elif add == 0:
                    n_thunks += 1
                    slots[off] = ("thunk_import", name)
                else:
                    slots[off] = ("value", 0)
                continue
        if target < text_end:
            n_code += 1
            if BASE + target in table:
                n_translated += 1
                slots[off] = ("value", table[BASE + target])
            else:
                n_thunks += 1
                slots[off] = ("thunk_fn", BASE + target)
        else:
            slots[off] = ("value", BASE + target)
    check(info.relocations == len(relocs), f"relocations {info.relocations} == {len(relocs)}")
    check((info.code_pointers, info.code_translated) == (n_code, n_translated),
          f"code pointers {info.code_pointers}/{info.code_translated} == {n_code}/{n_translated}")
    check((info.imports, info.imports_resolved, info.thunks) == (n_imports, n_resolved, n_thunks),
          f"imports and thunks {info.imports}/{info.imports_resolved}/{info.thunks} =="
          f" {n_imports}/{n_resolved}/{n_thunks}")
    mapped = ctypes.string_at(BASE, data_end)
    bad = thunk_seen = 0
    one_fn_thunk = one_import_thunk = None
    for off, (kind, v) in slots.items():
        got = struct.unpack_from("<Q", mapped, off)[0]
        expected[off:off + 8] = mapped[off:off + 8]  # compared here, masked for the byte comparison
        if kind == "value":
            ok = got == v
        else:
            thunk = ctypes.string_at(got, THUNK)
            arg, arg2, jump = (struct.unpack_from("<Q", thunk, at)[0] for at in (2, 12, 22))
            ok = thunk[0:2] == b"\x48\xbf" and thunk[20:22] == b"\x48\xb8" and thunk[30:32] == b"\xff\xe0" and arg == arg2
            if kind == "thunk_fn":
                ok = ok and arg == v and jump == rec_fn
                one_fn_thunk = one_fn_thunk or (got, v)
            else:
                ok = ok and ctypes.string_at(arg).decode() == v and jump == rec_imp
                one_import_thunk = one_import_thunk or (got, v)
            thunk_seen += 1
        if not ok:
            bad += 1
            if bad <= 5:
                print(f"  slot +0x{off:x} ({kind} {v if isinstance(v, str) else hex(v)}): got 0x{got:x}")
    check(bad == 0, f"{bad} of {len(slots)} relocated slots wrong")
    diff = next((i for i in range(0, data_end, 0x10000) if mapped[i:i + 0x10000] != expected[i:i + 0x10000]), None)
    check(diff is None, f"image bytes differ near +0x{diff or 0:x}")
    for at in (data_end, (data_end + info.bss_end) // 2 & ~0xfff, info.bss_end - 0x1000):
        check(ctypes.string_at(BASE + at, 0x1000) == bytes(0x1000), f".bss zero at +0x{at:x}")

    # 4. calling through what the loader wrote
    if one_fn_thunk:
        ctypes.CFUNCTYPE(None)(one_fn_thunk[0])()
        check(lib.test_last_missing_function() == one_fn_thunk[1], "a code thunk reports its address")
    if one_import_thunk:
        ctypes.CFUNCTYPE(None)(one_import_thunk[0])()
        check(ctypes.string_at(lib.test_last_missing_import()).decode() == one_import_thunk[1],
              "an import thunk reports its name")
    off_a = next(o for o, (k, v) in slots.items() if k == "value" and v == code_a)
    fn = ctypes.CFUNCTYPE(ctypes.c_uint64, ctypes.c_uint64)(struct.unpack_from("<Q", mapped, off_a)[0])
    check(fn(41) == 42, "a translated code pointer calls the port's function")

    # 5. protection and the second load
    if sys.platform == "win32":
        class MBI(ctypes.Structure):
            _fields_ = [("BaseAddress", ctypes.c_void_p), ("AllocationBase", ctypes.c_void_p),
                        ("AllocationProtect", ctypes.c_uint32), ("PartitionId", ctypes.c_uint16),
                        ("RegionSize", ctypes.c_size_t), ("State", ctypes.c_uint32), ("Protect", ctypes.c_uint32),
                        ("Type", ctypes.c_uint32)]
        vq = ctypes.windll.kernel32.VirtualQuery
        vq.argtypes = [ctypes.c_void_p, ctypes.POINTER(MBI), ctypes.c_size_t]
        for at, want, what in ((0, 0x02, ".text read-only"), (mod.rodata.mem_offset, 0x02, ".rodata read-only"),
                               (data_begin, 0x04, ".data read-write"), (info.bss_end - 0x1000, 0x04, ".bss read-write")):
            mbi = MBI()
            vq(BASE + at, ctypes.byref(mbi), ctypes.sizeof(mbi))
            check(mbi.Protect == want, f"{what}: protection 0x{mbi.Protect:x}")
    s, _ = load(options())
    check(s == "MapFailed", f"second load: {s}")
    check(lib.test_unmap(BASE, info.bss_end), "unmap")

    print(f"{check.checks - check.failures} of {check.checks} checks passed")
    return 0 if check.failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
