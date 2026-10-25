#!/usr/bin/env python3
"""Link every object of the port into one relocatable file and report what is missing.

Milestone M1 of docs/PORT_PLAN.md starts with the whole-program link: the objects of the compile-only
sweep (tools/port_sweep.py, build/port/sweep/), linked with `ld.lld -r` into one relocatable object,
build/port/link/dread_game.o. The link succeeds (a duplicate symbol is a real problem and fails the
run) and leaves a set of undefined symbols, which this tool sorts by what each one needs:

  FUNCTION ALIAS   the address is defined under another symbol: a different parameter list (the
                   declaration debt of hand-written files), a namespace (gen_lift declares callees
                   as callee::fn_<addr>; gen_clones, gen_statics and hand files use others), or a
                   real name (a template instantiation tagged with the address). A --defsym alias
                   resolves it. Whether the reference is called or only has its address taken comes
                   from its relocations (a PLT32 is a call; a GOT or absolute entry is an address);
                   for the calls, the parameter lists of the reference and the definition are
                   compared by x86-64 argument class (integer or pointer against float or double by
                   value, which travel in different registers): the sub-classes say which aliases
                   are safe and which need the declaration fixed. A called alias whose classes
                   differ (class_mismatch) is not emitted: the call gets a trap of its own
                   (dread_port_mismatched_call) so it fails at the call instead of computing with
                   the wrong register; --alias-mismatch emits the alias anyway.
  MISSING FUNCTION an address in data/functions/ with no definition at all, by status: the lifter's
                   coverage work (U) and the matching queue. Each gets a trap thunk that reports the
                   address (port/runtime/src/missing.cpp).
  IMPORT           an SDK or C library function `main` imports (`<EXTERNAL>::` in the CSV),
                   referenced through its 4-byte thunk's fn_<addr> name: an nn:: name aliases to the
                   runtime's function when port/runtime defines one (else a trap, listed under
                   "runtime stub needed"); a C library name becomes a jump to the host's function.
  DATA             g_unk_<addr>/unk_<addr> globals: the game's .data, .rodata and .bss, which the
                   port maps from the user's dump at their original addresses; each is defined as
                   that absolute address. The ones referenced PC-relatively (R_X86_64_PC32: the
                   declarations with visibility("hidden")) are counted, since they tie the final
                   image to within 2 GB of the game's addresses.
  VTABLE           vtables, VTTs and typeinfo of classes whose virtual functions are not all defined.
  STANDARD LIBRARY std::, operator new/delete, __cxa_*, compiler-rt builtins and C names: the host's,
                   even where data/functions/ lists the copy statically linked into main (status L).
                   That includes the L functions reached by address whose CSV name is a C library
                   or allocation operator name (fn_7100083c00 = operator delete, free, ...): a jump
                   to the host's function, not a trap.
  NN RUNTIME       nn:: functions called by their real name (the sources that include the SDK
                   headers): the runtime defines them, or has to.
  NAMED WITHOUT ADDRESS  a name that resolves to no address (UnkIdTable::UnkIdTable(), ...): it
                   needs an fn_<addr> name or a data/functions/ name before anything can resolve it.

Symbols resolve to addresses as tools/check.py does. A definition: fn_<addr> anywhere in the mangled
symbol (asmdiff.NAME_RE; this is how a template instantiation tagged UnkRefl_fn_<addr> counts as
that function), else its demangled name unique in data/functions/. A reference: the demangled name
without its parameter list (asmdiff.strip_params) when it is fn_/g_unk_/unk_<addr> in any namespace
(asmdiff.TARGET_RE), else a name unique in data/functions/ (check.csv_by_name). A symbol whose only
mention of the address is a function pointer (`&fn_<addr>` in a template argument) defines nothing
at that address: it is neither an alias target nor coverage of the boot slice.

The boot slice (tools/callgraph.py: every function reachable from nnMain by direct calls, imports
apart) is reported by coverage: defined, defined but reached through an alias only, host (the
standard library's by address, above), or none, by status and bytes. <out>/boot_missing.csv lists the
functions without a definition by callers in the slice: the lifter's coverage worklist and the
matching queue.

A second relocatable link, <out>/dread_whole.o, takes dread_game.o, the runtime's objects (compiled
here with the sweep's flags, so the tool does not depend on the CMake build), the trap thunks
(<out>/traps.s) and the aliases (<out>/aliases.rsp). Its residue, the undefined symbols left, is
what the port still has to provide outside this mechanism: vtables, the host's standard library,
names without an address, runtime stubs not written yet.

Outputs under <out> (default build/port/link/): runtime/*.o, objects.rsp, dread_game.o, aliases.rsp,
traps.s, traps.o, dread_whole.o, boot_missing.csv and link.json (everything the report prints, with
the full lists). Every output is rebuilt on every run. Exit status 0 unless a link fails (1).
Standard library plus the project's own tools: port_sweep (flags, sources, own_name), asmdiff
(demangle, strip_params, NAME_RE, TARGET_RE), check (load_csv, csv_by_name), callgraph (the boot
slice). Needs clang++, ld.lld and llvm-nm (next to the compiler, or on PATH; llvm-objdump there too
for the call/address split), llvm-cxxfilt (tools/hostenv.py) and build/ghidra_export/<version>/
for the slice.

Usage:
  python tools/port_link.py --version 2.1.0                      # build/port/sweep -> build/port/link
  python tools/port_link.py --version 2.1.0 --objects build/port/sweep --out build/port/link
  python tools/port_link.py --version 2.1.0 --cxx /usr/bin/clang++ --target x86_64-linux-gnu --system-stdlib
  python tools/port_link.py --version 2.1.0 --alias-mismatch     # emit the class_mismatch aliases instead of traps
"""

from __future__ import annotations

import argparse
import csv
import json
import re
import shutil
import subprocess
import sys
import time
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import callgraph  # noqa: E402
import check  # noqa: E402
import port_sweep  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
SAMPLE_DUPLICATES = ROOT / "port" / "sample_duplicates.txt"  # shared with port/CMakeLists.txt
RUNTIME_SRC = ROOT / "port" / "runtime" / "src"
RUNTIME_INCLUDE = port_sweep.RUNTIME_INCLUDE
TRAP = "dread_port_missing_function"  # port/runtime/src/missing.cpp, declared in dread_port.h
MISMATCH_TRAP = "dread_port_mismatched_call"  # same file: a called alias whose argument classes differ
IMPORT_PREFIX = callgraph.IMPORT_PREFIX

# llvm-nm symbol kinds. Uppercase W/V are weak definitions; lowercase w/v are weak undefined.
CODE_KINDS = frozenset("TtW")
DATA_KINDS = frozenset("DdBbRrVGgSsu")
UNDEF_KINDS = frozenset("Uwv")
# Relocation types against a symbol: a call (or tail jump) through the PLT, or its address (a GOT
# entry under -fPIC, a PC-relative reference to a hidden symbol, an absolute word in data).
CALL_RELOCS = frozenset({"R_X86_64_PLT32"})
ADDRESS_RELOCS = frozenset({"R_X86_64_REX_GOTPCRELX", "R_X86_64_GOTPCRELX", "R_X86_64_GOTPCREL", "R_X86_64_64",
                            "R_X86_64_PC32", "R_X86_64_32", "R_X86_64_32S", "R_X86_64_PC64", "R_X86_64_GOTPC32"})
DATA_NAME = re.compile(r"(?:^|::)(?:g_unk|unk)_[0-9a-f]{10}$")
C_IDENT = re.compile(r"^[A-Za-z_]\w*$")
VTABLE_PREFIXES = ("vtable for ", "VTT for ", "construction vtable for ", "typeinfo for ", "typeinfo name for ")
STDLIB_PREFIXES = ("std::", "operator new", "operator delete", "__cxa_", "__cxxabiv1::", "__gxx_personality",
                   "__stack_chk", "__dso_handle", "__tls_get_addr")
# C library names the game's code calls directly or through import thunks. An unmangled name that
# resolves to nothing, or to a copy statically linked into main (status L), is the host's when it
# is listed here or starts with `__` (a C runtime or compiler-rt builtin).
C_LIBRARY = frozenset("""
abort exit atexit free malloc calloc realloc posix_memalign aligned_alloc
memcpy memmove memset memcmp memchr strlen strnlen strcmp strncmp strcpy strncpy strcat strncat strchr strrchr
strstr strtok strdup strerror strtol strtoul strtoll strtoull strtod strtof atoi atol atof
wcslen wcscpy wcsncpy wcscmp wcsncmp wcscat wmemcpy wmemset wmemmove wcschr wcsstr
printf fprintf sprintf snprintf vprintf vfprintf vsnprintf puts putchar fputs fputc fwrite fread fopen fclose
fflush fseek ftell sscanf vsscanf qsort bsearch rand srand abs labs llabs
toupper tolower isalpha isdigit isalnum isspace isupper islower isprint isxdigit setjmp longjmp getenv
sinf cosf tanf asinf acosf atanf atan2f sinhf coshf tanhf expf exp2f logf log2f log10f powf sqrtf cbrtf fmodf
floorf ceilf roundf truncf fabsf fminf fmaxf hypotf ldexpf frexpf modff copysignf sincosf nanf
sin cos tan asin acos atan atan2 sinh cosh tanh exp exp2 log log2 log10 pow sqrt cbrt fmod floor ceil round
trunc fabs fmin fmax hypot ldexp frexp modf copysign sincos nan
pthread_mutex_lock pthread_mutex_unlock pthread_mutex_init pthread_mutex_destroy pthread_self pthread_create
pthread_join clock_gettime gettimeofday time localtime gmtime mktime nanosleep usleep sleep
stdin stdout stderr errno environ
""".split())
# A status-L function reached by its fn_<addr> name whose CSV name is one of these is the host's
# too (a jump thunk, as for an import): the allocation operators by their Itanium mangled names
# (operator new(unsigned long), operator new[](unsigned long), operator delete(void*),
# operator delete[](void*)), and any C_LIBRARY name as itself.
HOST_OPERATORS = {"operator new": "_Znwm", "operator new[]": "_Znam",
                  "operator delete": "_ZdlPv", "operator delete[]": "_ZdaPv"}
PC_RELATIVE = "R_X86_64_PC32"  # a PC-relative reference to a hidden symbol (no GOT)
# A by-value parameter that goes in an integer register (or is a pointer, reference, array or
# function pointer). Anything else named by value is an aggregate or enum: its class is unknown
# without the type's definition.
SCALAR_RE = re.compile(
    r"^(?:const |volatile )*(?:(?:un)?signed(?: (?:char|short|int|long|long long|__int128))?|bool|char|short|int"
    r"|long|long long|wchar_t|char8_t|char16_t|char32_t|__int128|decltype\(nullptr\)|std::nullptr_t)"
    r"(?: const| volatile)*$")
FLOAT_RE = re.compile(r"^(?:const |volatile )*(?:float|double)(?: const| volatile)*$")
SIGNATURE_ORDER = ["same", "same_with_this", "ref_more_args", "ref_fewer_args", "unmangled_definition", "class_mismatch"]
ABI_ORDER = ["address_taken_only", *SIGNATURE_ORDER]
ABI_NOTE = {
    "address_taken_only": "the reference only takes the address (tables, function pointers), never calls: the signature is not involved",
    "same": "same count and class per position: safe",
    "same_with_this": "the reference passes the object as its first argument, the definition is a member: safe",
    "ref_more_args": "the reference passes extra arguments, which the definition ignores: safe",
    "ref_fewer_args": "the definition reads arguments the reference does not pass: check",
    "unmangled_definition": "the definition is an unmangled (extern \"C\" or asm alias) name: unknown",
    "class_mismatch": "a float or double argument against an integer or pointer one: wrong register, unsafe"
                      " (trapped unless --alias-mismatch)",
}
CLASS_ORDER = ["function_alias", "missing_function", "import", "data", "vtable", "stdlib", "nn_runtime",
               "named_without_address"]
CLASS_TITLE = {
    "function_alias": "FUNCTION ALIAS (defined under another symbol)",
    "missing_function": "MISSING FUNCTION (no definition at all)",
    "import": "IMPORT (<EXTERNAL>:: in data/functions/)",
    "data": "DATA (g_unk_/unk_ globals: the dump's segments)",
    "vtable": "VTABLE / VTT / typeinfo",
    "stdlib": "STANDARD LIBRARY (the host's)",
    "nn_runtime": "NN RUNTIME (nn:: by name)",
    "named_without_address": "NAMED WITHOUT ADDRESS",
}


@dataclass
class Definition:
    kind: str  # llvm-nm kind
    symbol: str
    demangled: str
    rule: str  # fn_in_symbol | csv_name
    tag: str  # own (the symbol's own name is fn_<addr>) | tag (an identifier carries it) | pointer (&fn_<addr> only)


@dataclass
class Entry:
    """One undefined symbol and what it needs."""
    symbol: str
    demangled: str
    cls: str
    sub: str = ""
    signature: str = ""  # function_alias: the signature verdict (SIGNATURE_ORDER), also when only the address is taken
    address: int | None = None
    status: str = ""
    name: str = ""
    size: int = 0
    target: str = ""  # --defsym target: a mangled symbol or 0x<address> (a function alias keeps it for the report even when trapped)
    trap: int | None = None  # a trap thunk with this address
    trap_fn: str = TRAP  # the runtime function the trap thunk jumps to
    jump: str = ""  # a jump thunk to this host symbol (a C name, or a mangled allocation operator)
    flags: list[str] = field(default_factory=list)
    candidates: int = 0  # distinct definitions the alias chose from
    pc_relative: int = 0  # data: R_X86_64_PC32 relocations against the symbol


def rel(path: Path | str) -> str:
    return port_sweep.rel(path)


def find_tool(cxx: str, name: str) -> str | None:
    """A binutil next to the compiler, else on PATH."""
    exe = Path(cxx)
    cand = exe.parent / (name + exe.suffix)
    if cand.is_file():
        return str(cand)
    return shutil.which(name)


def quote(path: Path) -> str:
    """A path for a response file: forward slashes (no escaping in either quoting style), quoted."""
    return '"' + path.as_posix() + '"'


def read_sample_duplicates() -> list[str]:
    """port/sample_duplicates.txt: repository-relative sources left out of the link (and of
    dread_game in port/CMakeLists.txt); blank lines and # comments ignored."""
    if not SAMPLE_DUPLICATES.is_file():
        return []
    out = []
    for line in SAMPLE_DUPLICATES.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if line and not line.startswith("#"):
            out.append(line)
    return out


def collect_objects(objects_dir: Path, duplicates: list[str]) -> tuple[list[Path], list[str], list[str]]:
    """The sweep object of every src/**/*.cpp: (objects, sources without one, duplicates left out)."""
    objs, missing, skipped = [], [], []
    dups = set(duplicates)
    for src in port_sweep.sources([]):
        r = rel(src)
        if r in dups:
            skipped.append(r)
            continue
        obj = port_sweep.obj_path(src, objects_dir)
        if obj.is_file():
            objs.append(obj)
        else:
            missing.append(r)
    return objs, missing, skipped


def compile_runtime(cxx: str, flags: list[str], out_dir: Path) -> list[Path]:
    out_dir.mkdir(parents=True, exist_ok=True)
    objs = []
    for src in sorted(RUNTIME_SRC.glob("*.cpp")):
        obj = out_dir / (src.stem + ".o")
        r = subprocess.run([cxx, *flags, str(src), "-o", str(obj)], capture_output=True, text=True,
                           encoding="utf-8", errors="replace", cwd=ROOT)
        if r.returncode != 0:
            raise SystemExit(f"runtime: {rel(src)} does not compile:\n{r.stderr}")
        objs.append(obj)
    return objs


def nm_symbols(nm: str, path: Path) -> list[tuple[str, str]]:
    """(kind, mangled name) for every symbol of one object, as llvm-nm lists them."""
    r = subprocess.run([nm, str(path)], capture_output=True, text=True, encoding="utf-8", errors="replace")
    if r.returncode != 0:
        raise SystemExit(f"{nm} {rel(path)} failed:\n{r.stderr}")
    out = []
    for line in r.stdout.splitlines():
        parts = line.split()
        if len(parts) == 2:  # undefined: no value column
            out.append((parts[0], parts[1]))
        elif len(parts) >= 3:
            out.append((parts[1], parts[2]))
    return out


def relocations(objdump: str | None, path: Path) -> dict[str, Counter[str]] | None:
    """Symbol -> relocation type -> how many the object applies against it (llvm-objdump -r), or
    None without llvm-objdump. Section symbols are left out."""
    if not objdump:
        return None
    r = subprocess.run([objdump, "-r", str(path)], capture_output=True, text=True, encoding="utf-8", errors="replace")
    if r.returncode != 0:
        raise SystemExit(f"{objdump} -r {rel(path)} failed:\n{r.stderr}")
    counts: dict[str, Counter[str]] = defaultdict(Counter)
    for line in r.stdout.splitlines():
        parts = line.split()
        if len(parts) < 3 or not parts[1].startswith("R_"):
            continue
        sym = re.split(r"[+-]", parts[2], 1)[0]
        if sym and not sym.startswith("."):
            counts[sym][parts[1]] += 1
    return counts


def link(lld: str, output: Path, inputs: list[str]) -> tuple[subprocess.CompletedProcess, float]:
    t = time.time()
    r = subprocess.run([lld, "-r", "-o", str(output), *inputs], capture_output=True, text=True,
                       encoding="utf-8", errors="replace", cwd=ROOT)
    return r, time.time() - t


# --- names and addresses -------------------------------------------------------------------------

def split_top(s: str, sep: str) -> list[str]:
    """Split at `sep` outside <>, () and []."""
    out, depth, cur = [], 0, []
    for ch in s:
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1
        if ch == sep and depth == 0:
            out.append("".join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    out.append("".join(cur).strip())
    return out


def param_list(demangled: str) -> list[str] | None:
    """The parameter types of a demangled function name; None when it carries none (an
    unmangled name, a data symbol)."""
    name = asmdiff.strip_params(demangled)
    if name == demangled:
        return None
    tail = asmdiff.QUALIFIERS.sub("", demangled[len(name):])
    if not (tail.startswith("(") and tail.endswith(")")):
        return None
    inner = tail[1:-1].strip()
    if not inner:
        return []
    return split_top(inner, ",")


def arg_class(param: str) -> str:
    """x86-64 SysV class of one by-value parameter from its spelling: I (integer register:
    integers, pointers, references, arrays, function pointers), F (float or double), V (...),
    A (a named type by value: aggregate or enum, unknown without the definition)."""
    p = param.strip()
    if p == "...":
        return "V"
    if FLOAT_RE.match(p):
        return "F"
    if p.endswith(("*", "&", ")", "]")) or SCALAR_RE.match(p):
        return "I"
    return "A"


def class_qualified(demangled: str) -> bool:
    """Whether a definition's own name sits in a class scope (so a call carries a hidden `this`
    or it is a constructor): the scope's last component is capitalised, as the game's classes
    are (CActor, UnkFoo), while namespaces (callee, base::reflection, std) are not."""
    own = port_sweep.own_name(demangled)
    if "::" not in own:
        return False
    scope = own.rsplit("::", 1)[0].rsplit("::", 1)[-1]
    return scope[:1].isupper()


def signature_class(ref: list[str] | None, dfn: list[str] | None, dfn_qualified: bool) -> tuple[str, list[str]]:
    """How a call through the reference's signature lands on the definition's (ABI_NOTE)."""
    flags: list[str] = []
    if ref is None or dfn is None:
        return "unmangled_definition", flags
    cr = [arg_class(p) for p in ref]
    cd = [arg_class(p) for p in dfn]
    if "V" in cr or "V" in cd:
        flags.append("variadic")
        cr = [c for c in cr if c != "V"]
        cd = [c for c in cd if c != "V"]
    if "A" in cr or "A" in cd:
        flags.append("aggregate")

    def agree(a: list[str], b: list[str]) -> bool:
        return all(x == y or "A" in (x, y) for x, y in zip(a, b))

    if len(cr) == len(cd) and agree(cr, cd):
        return "same", flags
    if dfn_qualified and len(cr) == len(cd) + 1 and cr[0] in "IA" and agree(cr[1:], cd):
        return "same_with_this", flags
    n = min(len(cr), len(cd))
    if not agree(cr[:n], cd[:n]):
        return "class_mismatch", flags
    return ("ref_more_args" if len(cr) > len(cd) else "ref_fewer_args"), flags


def own_tail(demangled: str) -> str:
    """The last component of a symbol's own name, without a destructor's tilde."""
    return port_sweep.own_name(demangled).rsplit("::", 1)[-1].lstrip("~")


def address_tag(demangled: str, addr: int) -> str:
    """How a demangled definition mentions fn_<addr>: `own` when that is its own name, `tag` when
    an identifier carries it (UnkRefl_fn_<addr>, Unk_fn_<addr>::Unk_fn_<addr>), `pointer` when it
    only appears as a function pointer (&fn_<addr>) or with a suffix (fn_<addr>_alloc)."""
    needle = f"fn_{addr:010x}"
    if own_tail(demangled) == needle:
        return "own"
    for m in re.finditer(re.escape(needle), demangled):
        end = m.end()
        if end < len(demangled) and (demangled[end].isalnum() or demangled[end] == "_"):
            continue  # a suffix: fn_<addr>_alloc
        start = m.start()
        while start > 0 and (demangled[start - 1].isalnum() or demangled[start - 1] == "_"):
            start -= 1
        if start > 0 and demangled[start - 1] == "&":
            continue  # a function pointer argument
        return "tag"
    return "pointer"


class Resolver:
    """Symbol -> address, the two ways tools/check.py does it."""

    def __init__(self, rows: dict[str, dict]):
        self.rows = rows
        self.by_name = check.csv_by_name(rows)

    def unique(self, name: str) -> int | None:
        hits = self.by_name.get(name, [])
        return hits[0] if len(hits) == 1 else None

    def row(self, addr: int) -> dict | None:
        return self.rows.get(f"0x{addr:010x}")

    def definition(self, symbol: str, demangled: str) -> tuple[int | None, str]:
        """check.py's rule for a compiled function (asmdiff.diff_functions, check.resolver)."""
        m = asmdiff.NAME_RE.search(symbol)
        if m:
            return int(m.group(1), 16), "fn_in_symbol"
        a = self.unique(demangled.split("(")[0])
        return (a, "csv_name") if a is not None else (None, "")

    def reference(self, demangled: str) -> tuple[int | None, str]:
        """check.py's rule for a relocation target (asmdiff.target_resolver with the CSV fallback)."""
        name = asmdiff.strip_params(demangled)
        m = asmdiff.TARGET_RE.match(name)
        if m:
            return int(m.group(1), 16), "target_re"
        a = self.unique(name)
        return (a, "csv_name") if a is not None else (None, "")


def import_group(csv_name: str) -> tuple[str, str]:
    """(group, name) of an <EXTERNAL>:: row: ('nn', 'nn::os' for nn::os::LockMutex), ('c', 'strlen'),
    ('cxx', 'std' or an operator), ('other', 'movie')."""
    inner = csv_name[len(IMPORT_PREFIX):]
    if inner.startswith("operator") or "(" in inner:
        return "cxx", inner
    if "::" not in inner:
        return "c", inner
    top = inner.split("::")[0]
    if top == "nn":
        return "nn", "::".join(inner.split("::")[:2])
    if top == "std":
        return "cxx", "std"
    return "other", top


def is_stdlib(symbol: str, demangled: str) -> str:
    """'C++' or 'C' when the name is the host standard library's, else ''."""
    if demangled.startswith(STDLIB_PREFIXES):
        return "C++"
    if not symbol.startswith("_Z") and C_IDENT.match(symbol) and (symbol in C_LIBRARY or symbol.startswith("__")):
        return "C"
    return ""


# --- classification -------------------------------------------------------------------------------

class Link:
    """The symbols of one relocatable link, resolved to addresses."""

    def __init__(self, symbols: list[tuple[str, str]], demangled: dict[str, str], resolver: Resolver,
                 relocs: dict[str, Counter[str]] | None):
        self.resolver = resolver
        self.demangled = demangled
        self.relocs = relocs
        self.undefined: list[str] = []
        self.defined_entries = 0  # symbol-table entries (a local name such as .L.str repeats across objects)
        self.code_defs: dict[int, list[Definition]] = defaultdict(list)  # own and tagged definitions only
        self.pointer_only: dict[int, list[Definition]] = defaultdict(list)  # &fn_<addr> in a template argument: no definition
        self.data_defs: dict[int, list[tuple[str, str]]] = defaultdict(list)
        self.defined_symbols: set[str] = set()
        self.def_rules: Counter[str] = Counter()
        self.unresolved_code: list[str] = []  # global functions that map to no address (anchors, pads)
        self.vtables_defined = 0
        for kind, sym in symbols:
            dem = demangled.get(sym, sym)
            if kind in UNDEF_KINDS:
                self.undefined.append(sym)
                continue
            self.defined_entries += 1
            self.defined_symbols.add(sym)
            if kind in CODE_KINDS:
                a, rule = resolver.definition(sym, dem)
                if a is None:
                    self.def_rules["none"] += 1
                    if kind != "t":
                        self.unresolved_code.append(dem)
                    continue
                tag = address_tag(dem, a) if rule == "fn_in_symbol" else "own"
                self.def_rules[f"{rule}/{tag}"] += 1
                d = Definition(kind, sym, dem, rule, tag)
                (self.pointer_only if tag == "pointer" else self.code_defs)[a].append(d)
            elif kind in DATA_KINDS:
                if dem.startswith("vtable for "):
                    self.vtables_defined += 1
                a, _ = resolver.reference(dem)
                if a is not None and DATA_NAME.search(asmdiff.strip_params(dem)):
                    self.data_defs[a].append((kind, sym))

    def alias_candidates(self, addr: int) -> list[Definition]:
        """Global definitions of `addr` an alias may point at (a local one cannot be named)."""
        return [d for d in self.code_defs.get(addr, []) if d.kind in "TW"]

    def use(self, sym: str) -> tuple[bool, bool]:
        """(called, address taken) from the relocations against `sym`; (True, True) when unknown."""
        if self.relocs is None:
            return True, True
        kinds = set(self.relocs.get(sym, ()))
        return bool(kinds & CALL_RELOCS), bool(kinds & ADDRESS_RELOCS)

    def pc_relative(self, sym: str) -> int:
        """R_X86_64_PC32 relocations against `sym` (0 when relocations are unknown)."""
        return self.relocs.get(sym, Counter())[PC_RELATIVE] if self.relocs is not None else 0


def host_symbol(row: dict) -> tuple[str, str]:
    """(host symbol, 'C' or 'C++') when a status-L row's name is the host standard library's
    (HOST_OPERATORS, C_LIBRARY), else ('', '')."""
    if row["status"] != "L":
        return "", ""
    name = row["name"]
    if name in HOST_OPERATORS:
        return HOST_OPERATORS[name], "C++"
    if name in C_LIBRARY:
        return name, "C"
    return "", ""


def classify(lk: Link, runtime_by_name: dict[str, list[str]], runtime_symbols: set[str],
             alias_mismatch: bool = False) -> list[Entry]:
    """One Entry per undefined symbol of `lk`. A called function alias with a class_mismatch
    verdict is trapped unless `alias_mismatch`."""
    res = lk.resolver
    out: list[Entry] = []
    for sym in lk.undefined:
        dem = lk.demangled.get(sym, sym)
        e = Entry(sym, dem, "")
        name = asmdiff.strip_params(dem)
        addr: int | None = None
        if asmdiff.TARGET_RE.match(name):
            addr, _ = res.reference(dem)
        elif dem.startswith(VTABLE_PREFIXES):
            e.cls, e.sub = "vtable", dem.split(" for ")[0]
        elif is_stdlib(sym, dem):
            e.cls, e.sub = "stdlib", is_stdlib(sym, dem)
        elif dem.startswith("nn::"):
            e.cls = "nn_runtime"
            e.sub = "defined by the runtime" if sym in runtime_symbols else "stub needed"
        else:
            addr, _ = res.reference(dem)  # a real name unique in data/functions/ (Lua, ...)
            if addr is None:
                e.cls = "named_without_address"
        if e.cls:
            out.append(e)
            continue
        assert addr is not None
        e.address = addr
        if DATA_NAME.search(name):
            e.cls = "data"
            defs = lk.data_defs.get(addr)
            e.sub = "namespaced" if "::" in name else "plain"
            e.target = defs[0][1] if defs else f"0x{addr:x}"
            e.pc_relative = lk.pc_relative(sym)
            if e.pc_relative:
                e.flags.append("pc_relative")
            if defs:
                e.flags.append("defined_in_link")
            if res.row(addr):
                e.flags.append("address_is_a_function")
            out.append(e)
            continue
        called, taken = lk.use(sym)
        e.flags.append("called" if called else "address_taken_only" if taken else "unreferenced")
        row = res.row(addr)
        if row:
            e.status, e.name, e.size = row["status"], row["name"], int(row["size"])
        cands = lk.alias_candidates(addr)
        if cands:
            e.cls = "function_alias"
            ref_params = param_list(dem)
            ranked = []
            for d in cands:
                sig, flags = signature_class(ref_params, param_list(d.demangled), class_qualified(d.demangled))
                ranked.append((SIGNATURE_ORDER.index(sig), 0 if d.kind == "T" else 1, d.symbol, sig, flags, d))
            ranked.sort(key=lambda t: t[:3])
            _, _, _, e.signature, flags, best = ranked[0]
            e.flags += flags
            e.sub = e.signature if called else "address_taken_only"
            e.target = best.symbol
            e.candidates = len({d.demangled for d in cands})
            if best.tag == "tag":
                e.flags.append("tagged_template")
            if e.sub == "class_mismatch" and not alias_mismatch:
                e.trap, e.trap_fn = addr, MISMATCH_TRAP  # the alias stays in the report, not in aliases.rsp
                e.flags.append("trapped")
        elif lk.code_defs.get(addr):
            e.cls, e.sub, e.trap = "missing_function", "local definition only", addr
        elif row and host_symbol(row)[0]:
            e.jump, kind = host_symbol(row)
            e.cls, e.sub = "stdlib", f"{kind} (status L, by address)"
        elif row and row["name"].startswith(IMPORT_PREFIX):
            e.cls = "import"
            group, gname = import_group(row["name"])
            if group == "nn":
                e.sub = gname  # nn::<namespace>
                rt = runtime_by_name.get(row["name"][len(IMPORT_PREFIX):])
                if rt:
                    e.target = rt[0]
                    e.flags.append("runtime")
                else:
                    e.trap = addr
                    e.flags.append("stub_needed")
            elif group == "c":
                e.sub, e.jump = "C library", gname
            elif group == "cxx":
                e.sub, e.trap = "host C++ library", addr
                e.flags.append("mangled_name_needed")
            else:
                e.sub, e.trap = gname, addr  # movie:: and any other SDK namespace
                e.flags.append("stub_needed")
        elif row:
            e.cls, e.sub, e.trap = "missing_function", row["status"], addr
        else:
            e.cls, e.sub, e.status, e.trap = "missing_function", "not a function start", "-", addr
        out.append(e)
    return out


# --- outputs --------------------------------------------------------------------------------------

def write_rsp(path: Path, lines: list[str]) -> None:
    path.write_text("\n".join(lines) + ("\n" if lines else ""), encoding="utf-8", newline="\n")


def write_traps(path: Path, entries: list[Entry]) -> tuple[int, int]:
    """traps.s: a thunk per trapped symbol (loads the address, jumps to the runtime's trap: the
    missing-function one, or the mismatched-call one for a refused alias) and per host-library
    function reached by an fn_<addr> name (jumps to the host's symbol). Returns (traps, jumps)."""
    lines = ["# Generated by tools/port_link.py. One thunk per function the link has no definition for.",
             "\t.text"]
    traps = jumps = 0
    for e in entries:
        if e.trap is None and not e.jump:
            continue
        lines += [f"\t.globl\t{e.symbol}", f"\t.type\t{e.symbol},@function", f"{e.symbol}:"]
        if e.jump:
            lines.append(f"\tjmp\t{e.jump}")
            jumps += 1
        else:
            lines += [f"\tmovabsq\t$0x{e.trap:x}, %rdi", f"\tjmp\t{e.trap_fn}"]
            traps += 1
        lines.append(f"\t.size\t{e.symbol}, .-{e.symbol}")
    lines.append('\t.section\t.note.GNU-stack,"",@progbits')
    path.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    return traps, jumps


def summarize(entries: list[Entry], examples: int, keep_empty: bool) -> dict:
    """Counts per class and sub-class, with examples, for the report and the JSON."""
    out: dict = {}
    for cls in CLASS_ORDER:
        es = [e for e in entries if e.cls == cls]
        if not es and not keep_empty:
            continue
        d: dict = {"symbols": len(es)}
        addrs = {e.address for e in es if e.address is not None}
        if addrs:
            d["addresses"] = len(addrs)
        subs = Counter(e.sub for e in es)
        d["flags"] = dict(sorted(Counter(f for e in es for f in e.flags).items()))
        if cls == "function_alias":
            d["abi"] = {s: {"symbols": subs.get(s, 0), "note": ABI_NOTE[s],
                            "examples": [{"reference": e.demangled, "definition": e.target}
                                         for e in es if e.sub == s][:examples]}
                        for s in ABI_ORDER}
            d["signature_when_called_or_not"] = dict(sorted(Counter(e.signature for e in es).items()))
            multi = [e for e in es if e.candidates > 1]
            d["several_definitions"] = {"aliases": len(multi), "addresses": len({e.address for e in multi})}
            d["trapped"] = sum(1 for e in es if e.trap is not None)
        elif cls == "missing_function":
            by_status: dict = {}
            for st in sorted(subs):
                sel = [e for e in es if e.sub == st]
                sel_addrs = {e.address: e for e in sel}
                by_status[st] = {"symbols": len(sel), "addresses": len(sel_addrs),
                                 "called": sum(1 for e in sel if "called" in e.flags),
                                 "bytes": sum(e.size for e in sel_addrs.values()),
                                 "examples": [e.demangled for e in sel[:examples]]}
            d["by_status"] = by_status
        elif cls == "import":
            d["by_group"] = {s: {"symbols": subs[s], "addresses": len({e.address for e in es if e.sub == s}),
                                 "flags": dict(sorted(Counter(f for e in es if e.sub == s for f in e.flags).items())),
                                 "examples": [e.demangled for e in es if e.sub == s][:examples]}
                             for s in sorted(subs)}
        elif cls == "data":
            d["namespaced_references"] = subs.get("namespaced", 0)
            pc = [e for e in es if e.pc_relative]
            d["pc_relative"] = {"symbols": len(pc), "addresses": len({e.address for e in pc}),
                                "relocations": sum(e.pc_relative for e in pc),
                                "note": f"{PC_RELATIVE} relocations: the declarations with visibility(\"hidden\"); the final"
                                        " image must sit within 2 GB of the game's addresses"}
            d["examples"] = [e.demangled for e in es[:examples]]
        else:
            d["by_kind"] = dict(sorted(subs.items()))
            d["names"] = sorted(e.demangled + (f" -> {e.jump}" if e.jump else "") for e in es)
        out[cls] = d
    return out


def flags_text(flags: dict) -> str:
    return ", ".join(f"{k} {v}" for k, v in flags.items())


def print_summary(title: str, summary: dict, examples: int) -> None:
    total = sum(d["symbols"] for d in summary.values())
    print(f"{title}: {total} undefined symbols")
    for cls, d in summary.items():
        line = f"  {d['symbols']:6d} symbols"
        if "addresses" in d:
            line += f" {d['addresses']:6d} addresses"
        print(f"{line}  {CLASS_TITLE[cls]}")
        if cls == "function_alias":
            for s in ABI_ORDER:
                a = d["abi"][s]
                if a["symbols"]:
                    print(f"           {a['symbols']:6d}  {s}: {a['note']}")
                    for ex in a["examples"][:examples]:
                        print(f"                   ref {ex['reference']}")
                        print(f"                   def {ex['definition']}")
            print(f"           signature verdict over all {d['symbols']} (called or not): "
                  + flags_text(d["signature_when_called_or_not"]))
            sd = d["several_definitions"]
            print(f"           flags: {flags_text(d['flags'])}; several distinct definitions (chosen by signature):"
                  f" {sd['aliases']} aliases ({sd['addresses']} addresses); trapped instead of aliased: {d['trapped']}")
        elif cls == "missing_function":
            for st, s in d["by_status"].items():
                print(f"           {s['symbols']:6d} symbols {s['addresses']:6d} addresses {s['bytes']:10,d} bytes"
                      f"  status {st} (called {s['called']})"
                      + (f"  e.g. {', '.join(s['examples'][:examples])}" if s["examples"] else ""))
        elif cls == "import":
            for g, s in d["by_group"].items():
                fl = flags_text(s["flags"])
                print(f"           {s['symbols']:6d} symbols {s['addresses']:6d} addresses  {g}" + (f"  [{fl}]" if fl else "")
                      + f"  e.g. {', '.join(s['examples'][:examples])}")
        elif cls == "data":
            pc = d["pc_relative"]
            print(f"           namespaced references {d['namespaced_references']}"
                  + (f", flags: {flags_text(d['flags'])}" if d["flags"] else "")
                  + f"  e.g. {', '.join(d['examples'][:examples])}")
            print(f"           PC-relative ({PC_RELATIVE}, the hidden declarations): {pc['relocations']} relocations"
                  f" against {pc['symbols']} symbols ({pc['addresses']} addresses)")
        else:
            print(f"           {flags_text(d['by_kind'])}  e.g. {', '.join(d['names'][:examples])}")


def boot_slice(version: str, lk: Link, entries: list[Entry], runtime_by_name: dict[str, list[str]],
               out_csv: Path) -> dict | None:
    """Coverage of every function reachable from nnMain; writes out_csv (the ones without a definition)."""
    try:
        g = callgraph.CallGraph(version)
    except SystemExit as e:
        print(f"boot slice: skipped ({e})")
        return None
    reach = g.reachable(callgraph.NNMAIN)
    imports = {a for a in reach if callgraph.is_import(g.funcs[a])}
    funcs = reach - imports
    aliased = {e.address for e in entries if e.cls == "function_alias"}
    hosted = {e.address for e in entries if e.cls == "stdlib" and e.address is not None}  # the host's by address
    _, callers = g.collapsed()
    table: dict[str, Counter] = defaultdict(Counter)
    bytes_none: Counter = Counter()
    missing_rows = []
    for a in funcs:
        row = g.funcs[a]
        if a in lk.code_defs:
            cover = "aliased" if a in aliased else "defined"
        elif a in hosted:
            cover = "host"
        else:
            cover = "none"
            bytes_none[row["status"]] += row["size"]
            missing_rows.append((a, row, len(set(callers[a]) & reach)))
        table[row["status"]][cover] += 1
    missing_rows.sort(key=lambda t: (-t[2], t[0]))
    with out_csv.open("w", newline="", encoding="utf-8") as fh:
        w = csv.writer(fh, lineterminator="\n")
        w.writerow(["address", "size", "name", "status", "callers_in_slice"])
        for a, row, n in missing_rows:
            w.writerow([f"0x{a:x}", row["size"], row["name"], row["status"], n])
    imp: dict[str, Counter] = defaultdict(Counter)
    for a in imports:
        group, name = import_group(g.funcs[a]["name"])
        key = name if group == "nn" else {"c": "C library", "cxx": "host C++ library"}.get(group, name)
        imp[key]["reached"] += 1
        if group == "nn" and g.funcs[a]["name"][len(IMPORT_PREFIX):] in runtime_by_name:
            imp[key]["runtime"] += 1
    result = {
        "root": f"0x{callgraph.NNMAIN:x}", "reachable": len(reach), "imports_reached": len(imports),
        "functions": len(funcs),
        "by_status": {st: {"functions": sum(c.values()), "defined": c["defined"], "aliased": c["aliased"],
                           "host": c["host"], "none": c["none"], "bytes_none": bytes_none[st]}
                      for st, c in sorted(table.items())},
        "totals": {k: sum(c[k] for c in table.values()) for k in ("defined", "aliased", "host", "none")},
        "bytes_none": sum(bytes_none.values()),
        "imports": {k: dict(v) for k, v in sorted(imp.items())},
        "missing_csv": rel(out_csv),
        "caveat": callgraph.CAVEAT,
    }
    print(f"boot slice (reachable from nnMain, 0x{callgraph.NNMAIN:x}): {len(reach)} functions,"
          f" {len(imports)} imports, {len(funcs)} game functions")
    print("  status  functions   defined   aliased      host      none   bytes(none)")
    for st, c in result["by_status"].items():
        print(f"  {st:6s} {c['functions']:10d} {c['defined']:9d} {c['aliased']:9d} {c['host']:9d} {c['none']:9d}"
              f" {c['bytes_none']:13,d}")
    t = result["totals"]
    print(f"  {'all':6s} {len(funcs):10d} {t['defined']:9d} {t['aliased']:9d} {t['host']:9d} {t['none']:9d}"
          f" {result['bytes_none']:13,d}")
    print("  imports reached: " + ", ".join(f"{k} {v['reached']}" + (f" (runtime {v['runtime']})" if v.get("runtime") else "")
                                           for k, v in result["imports"].items()))
    print(f"  worklist: {rel(out_csv)} ({len(missing_rows)} functions without a definition, by callers in the slice)")
    print(f"  {callgraph.CAVEAT}")
    return result


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True, help="game version (data/functions/, the Ghidra export)")
    ap.add_argument("--objects", default=str(ROOT / "build" / "port" / "sweep"), help="the sweep's objects")
    ap.add_argument("--out", default=str(ROOT / "build" / "port" / "link"), help="output directory")
    ap.add_argument("--cxx", default=None, help="host clang++ (default: $CXX, the LLVM install, PATH)")
    ap.add_argument("--target", default=None, help="target triple (default: as tools/port_sweep.py)")
    ap.add_argument("--system-stdlib", action="store_true",
                    help="compile the runtime against the host's standard library headers, as the sweep's option")
    ap.add_argument("--examples", type=int, default=3, help="examples printed per class (default 3)")
    ap.add_argument("--alias-mismatch", action="store_true",
                    help="emit the --defsym for a called alias whose argument classes differ (default: a trap at the call)")
    args = ap.parse_args()

    cxx = args.cxx or port_sweep.host_cxx()
    target = args.target or port_sweep.default_target(cxx)
    lld, nm, objdump = find_tool(cxx, "ld.lld"), find_tool(cxx, "llvm-nm"), find_tool(cxx, "llvm-objdump")
    if not lld or not nm:
        raise SystemExit(f"ld.lld and llvm-nm are needed next to {cxx} or on PATH (found: lld={lld}, nm={nm})")
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    flags = [f"--target={target}", *port_sweep.HOST_FLAGS, "-w"]
    if not args.system_stdlib:
        flags += buildcfg.system_includes()
    flags += [*port_sweep.project_includes(), "-I", str(RUNTIME_INCLUDE)]
    print(f"port link: target {target}, {port_sweep.compiler_version(cxx)}, {Path(lld).name}, {Path(nm).name}"
          + (f", {Path(objdump).name}" if objdump else ", no llvm-objdump (calls and address takes not told apart)"))

    # a. the runtime, compiled as the sweep compiles, and the sweep's objects
    runtime_objs = compile_runtime(cxx, flags, out / "runtime")
    objs, missing_objs, skipped = collect_objects(Path(args.objects), read_sample_duplicates())
    if not objs:
        raise SystemExit(f"no objects under {rel(Path(args.objects))}: run tools/port_sweep.py --version {args.version} first")
    print(f"objects: {len(objs)} from {rel(Path(args.objects))}, {len(skipped)} sample duplicates left out"
          f" ({rel(SAMPLE_DUPLICATES)}), {len(runtime_objs)} runtime objects compiled to {rel(out / 'runtime')}")
    if missing_objs:
        print(f"  {len(missing_objs)} sources have no object (the sweep must run first; continuing without them):")
        for m in missing_objs[:10]:
            print(f"    {m}")

    # b. the relocatable link of the game
    objects_rsp = out / "objects.rsp"
    write_rsp(objects_rsp, [quote(o) for o in objs])
    game_o = out / "dread_game.o"
    r, dt1 = link(lld, game_o, [f"@{objects_rsp.as_posix()}"])
    if r.returncode != 0:
        print(f"LINK FAILED ({rel(game_o)}):")
        print("\n".join(r.stderr.splitlines()[:40]))
        return 1
    print(f"link 1: {rel(game_o)} in {dt1:.1f} s ({game_o.stat().st_size / 1e6:.1f} MB), no duplicate symbols")

    # c. symbols, demangled once, resolved to addresses
    symbols = nm_symbols(nm, game_o)
    rt_symbols = [(obj, nm_symbols(nm, obj)) for obj in runtime_objs]
    names = sorted({s for _, s in symbols} | {s for _, syms in rt_symbols for _, s in syms})
    try:
        demangled = asmdiff.demangle(names, strip=False)
    except OSError as e:
        raise SystemExit(f"llvm-cxxfilt is needed ({asmdiff.cxxfilt()}): {e}")
    rows, _ = check.load_csv()
    resolver = Resolver(rows)
    lk = Link(symbols, demangled, resolver, relocations(objdump, game_o))
    runtime_symbols = {s for _, syms in rt_symbols for k, s in syms if k in CODE_KINDS}
    runtime_by_name: dict[str, list[str]] = defaultdict(list)
    for s in sorted(runtime_symbols):
        runtime_by_name[asmdiff.strip_params(demangled.get(s, s))].append(s)
    print(f"symbols: {lk.defined_entries} defined entries ({len(lk.defined_symbols)} distinct names),"
          f" {len(lk.undefined)} undefined;"
          f" {len(lk.code_defs)} addresses have a function definition"
          f" ({', '.join(f'{k} {v}' for k, v in sorted(lk.def_rules.items()))};"
          f" a pointer-only mention defines nothing: {len(lk.pointer_only)} addresses),"
          f" {len(lk.unresolved_code)} global functions resolve to no address (anchors, pads, statics' users),"
          f" {lk.vtables_defined} vtables defined;"
          f" runtime: {len(runtime_symbols)} functions")

    # d. classification
    entries = classify(lk, runtime_by_name, runtime_symbols, args.alias_mismatch)
    summary = summarize(entries, args.examples, keep_empty=True)
    assert sum(d["symbols"] for d in summary.values()) == len(entries) == len(lk.undefined)
    print()
    print_summary("classification", summary, args.examples)

    # e. the boot slice
    print()
    slice_ = boot_slice(args.version, lk, entries, runtime_by_name, out / "boot_missing.csv")

    # f. aliases, traps, the second link
    aliases = [e for e in entries if e.target and e.trap is None and e.cls in ("function_alias", "data", "import")]
    aliases_rsp = out / "aliases.rsp"
    write_rsp(aliases_rsp, [f"--defsym={e.symbol}={e.target}" for e in aliases])
    traps_s, traps_o = out / "traps.s", out / "traps.o"
    traps, jumps = write_traps(traps_s, entries)
    r = subprocess.run([cxx, f"--target={target}", "-c", str(traps_s), "-o", str(traps_o)],
                       capture_output=True, text=True, encoding="utf-8", errors="replace")
    if r.returncode != 0:
        print(f"LINK FAILED: {rel(traps_s)} does not assemble:\n" + "\n".join(r.stderr.splitlines()[:20]))
        return 1
    alias_counts = Counter(e.cls for e in aliases)
    print()
    print(f"link inputs: {rel(aliases_rsp)} with {len(aliases)} --defsym"
          f" (function {alias_counts['function_alias']}, data {alias_counts['data']}, import {alias_counts['import']});"
          f" {rel(traps_s)} with {traps} traps and {jumps} jumps to the host's library")
    whole_o = out / "dread_whole.o"
    r, dt2 = link(lld, whole_o, [str(game_o), *map(str, runtime_objs), str(traps_o), f"@{aliases_rsp.as_posix()}"])
    if r.returncode != 0:
        print(f"LINK FAILED ({rel(whole_o)}):")
        print("\n".join(r.stderr.splitlines()[:40]))
        return 1
    whole = Link(nm_symbols(nm, whole_o), demangled, resolver, relocations(objdump, whole_o))
    residue_entries = classify(whole, runtime_by_name, runtime_symbols, args.alias_mismatch)
    residue = summarize(residue_entries, args.examples, keep_empty=False)
    print(f"link 2: {rel(whole_o)} in {dt2:.1f} s ({whole_o.stat().st_size / 1e6:.1f} MB)")
    print_summary("residue", residue, args.examples)
    stub_namespaces = sorted({e.sub for e in entries if e.cls == "import" and "stub_needed" in e.flags})
    stub_names = sorted({e.demangled for e in residue_entries if e.cls == "nn_runtime"})
    print(f"runtime stubs needed: imports in {len(stub_namespaces)} namespaces ({', '.join(stub_namespaces)});"
          f" by name {len(stub_names)}" + (f" ({', '.join(stub_names)})" if stub_names else ""))

    # g. the JSON
    missing_list: dict[int, dict] = {}
    for e in entries:
        if e.cls == "missing_function" and e.address is not None:
            m = missing_list.setdefault(e.address, {"address": f"0x{e.address:x}", "size": e.size, "name": e.name,
                                                    "status": e.status, "called": False, "symbols": []})
            m["symbols"].append(e.demangled)
            m["called"] = m["called"] or "called" in e.flags
    report = {
        "version": args.version, "target": target, "compiler": port_sweep.compiler_version(cxx),
        "tools": {"cxx": cxx, "lld": lld, "nm": nm, "objdump": objdump, "cxxfilt": asmdiff.cxxfilt()},
        "flags": flags, "out": rel(out),
        "inputs": {"objects": len(objs), "objects_dir": rel(Path(args.objects)), "sources_without_object": missing_objs,
                   "sample_duplicates": skipped, "runtime_objects": [rel(o) for o in runtime_objs]},
        "options": {"alias_mismatch": args.alias_mismatch},
        "link1": {"output": rel(game_o), "bytes": game_o.stat().st_size, "seconds": round(dt1, 2),
                  "defined_entries": lk.defined_entries, "defined_names": len(lk.defined_symbols),
                  "undefined": len(lk.undefined)},
        "definitions": {"addresses": len(lk.code_defs), "rules": dict(sorted(lk.def_rules.items())),
                        "pointer_only_addresses": len(lk.pointer_only),
                        "unresolved_global_functions": sorted(lk.unresolved_code), "vtables_defined": lk.vtables_defined,
                        "runtime_functions": sorted(demangled.get(s, s) for s in runtime_symbols)},
        "classes": summary,
        "aliases": [{"symbol": e.demangled, "mangled": e.symbol, "target": e.target, "abi": e.sub,
                     "signature": e.signature, "flags": e.flags, "candidates": e.candidates,
                     "trapped": e.trap is not None}
                    for e in entries if e.cls == "function_alias"],
        "host_by_address": [{"symbol": e.demangled, "address": f"0x{e.address:x}", "name": e.name, "host": e.jump}
                            for e in entries if e.cls == "stdlib" and e.address is not None],
        "missing_functions": sorted(missing_list.values(), key=lambda m: m["address"]),
        "imports": [{"symbol": e.demangled, "address": f"0x{e.address:x}", "name": e.name, "group": e.sub,
                     "flags": e.flags, "target": e.target or e.jump} for e in entries if e.cls == "import"],
        "boot_slice": slice_,
        "link_inputs": {"aliases": rel(aliases_rsp), "defsyms": len(aliases), "defsyms_by_class": dict(alias_counts),
                        "traps": rel(traps_s), "trap_thunks": traps, "jump_thunks": jumps},
        "link2": {"output": rel(whole_o), "bytes": whole_o.stat().st_size, "seconds": round(dt2, 2),
                  "undefined": len(whole.undefined), "residue": residue,
                  "runtime_stubs_needed": {"import_namespaces": stub_namespaces, "by_name": stub_names}},
    }
    link_json = out / "link.json"
    link_json.write_text(json.dumps(report, indent=1, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"json: {rel(link_json)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
