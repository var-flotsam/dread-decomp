"""Original addresses of constructors and destructors, read from their declarations.

Clang 9 ignores an `asm("fn_<addr>")` label on a constructor or destructor: the object calls (or
defines) the Itanium name, `_ZN<class>C1E...` / `C2E` for a constructor, `D1E` / `D2E` for a
destructor, never fn_<addr>. Nothing in the object says which original function that is, so the source
declaration is the one place that does. A declaration names its member's address in one of these ways,
all read here:

    UnkHalf_cu(UnkHalf_cu&& other) asm("fn_71002458a4");          // an asm label
    ~CRntString();  // fn_710007ecd4                               // a comment that starts with it
    template <> UnkIdMapOwnedStorage<0>::~UnkIdMapOwnedStorage();  // fn_7100d90040
                                                                   // one instantiation's member
    UnkStrIdTable();  // fn_<CTOR>                                 // the class template's argument
                                                                   // named CTOR is the address
    #define STATE(ctor, ...) struct UnkState_##ctor ... { UnkState_##ctor(); /* fn_##ctor */ };
                                                                   // a macro: each use STATE(7100722124, ...)
                                                                   // declares UnkState_7100722124 at that address
    struct UnkAtkPart_b0150 : UnkAtkPartBase_b0150 {  // vtable g_unk_7101cb8da0
                                                                   // the class's vtable (_ZTV...) is that object

`Labels.lookup` maps a mangled symbol to that address. The declaration must be visible to the object,
so it is looked for in the object's own source first, then in every header (`.h` under include/ and
src/). Within that scope the class's last name, its template arguments (for a specialization), the
member's kind (constructor or destructor) and its parameter types must match; two declarations that
match with different addresses are ambiguous and resolve to nothing. The deleting destructor (D0) and
the inheriting constructor (C3) are other functions and never map.

A wrong address is caught where it is used: tools/check.py compares it with the original's target
like any other relocation (a RELOC line), and a call in a matching function to a constructor or
destructor that nothing names fails there too (kind `special`).
"""
from __future__ import annotations

import hashlib
import re
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
GENERATED = {"reflection", "reflection_types", "reflection_statics", "clones", "tables", "lifted"}

_PARAMS = r"\((?P<params>[^()]*(?:\([^()]*\)[^()]*)*)\)"
_TAIL = (r"[ 	]*(?:const[ 	]*)?(?:noexcept[ 	]*)?"
         r"(?:asm\(\"(?P<label>(?:fn|unk)_[0-9a-f]{10})\"\)[ 	]*[;{]"
         r"|[^;\n]*;[ \t]*//[ \t]*(?P<comment>fn_(?:[0-9a-f]{10}\b|<(?P<tparam>[A-Za-z_]\w*)>)))")
# `[explicit] [~]Name(params) ...` at the start of a line (no return type before the name).
_DECL = re.compile(r"^[ \t]*(?:(?:explicit|inline|virtual)\s+)*(?P<tilde>~?)(?P<name>[A-Za-z_]\w*)\s*"
                   + _PARAMS + _TAIL, re.M)
# `template <> Name<args>::[~]Name(params);  // fn_ADDR`: one instantiation's member.
_SPEC = re.compile(r"^[ \t]*template\s*<\s*>\s*(?P<cls>[A-Za-z_]\w*)\s*<(?P<targs>[^;{}]*?)>\s*::\s*"
                   r"(?P<tilde>~?)(?P=cls)\s*" + _PARAMS + _TAIL, re.M)
# A macro whose body declares `Prefix##p(params); /* fn_##p */` for one of its parameters p.
_DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+(?P<macro>[A-Za-z_]\w*)\((?P<mparams>[^)]*)\)"
                     r"(?P<body>(?:[^\n]*\\\n)*[^\n]*)", re.M)
_MACRO_DECL = re.compile(r"(?P<tilde>~?)(?P<prefix>[A-Za-z_]\w*)##(?P<p>\w+)\s*" + _PARAMS
                         + r"\s*;\s*/\*\s*fn_##(?P=p)\s*\*/")
# `struct Name ... {  // vtable g_unk_ADDR`: the class's vtable (`_ZTV...`) is the original's object at ADDR.
_VTABLE = re.compile(r"^[ \t]*(?:struct|class)[ \t]+(?P<name>[A-Za-z_]\w*)\b[^;{\n]*\{[ \t]*//[ \t]*vtable[ \t]+"
                     r"(?:g_unk|unk)_(?P<addr>[0-9a-f]{10})\b", re.M)
# The mangled kinds that stand for the declared member: complete and base object constructor and
# destructor. C3 (inheriting constructor) and D0 (deleting destructor) are other functions.
_KIND = re.compile(r"^_ZN.*?(?P<k>C[12]|D[12])E")
_TYPEDEFS = {"u8": "unsigned char", "u16": "unsigned short", "u32": "unsigned int", "u64": "unsigned long",
             "s8": "signed char", "s16": "short", "s32": "int", "s64": "long", "f32": "float", "f64": "double",
             "size_t": "unsigned long", "uintptr_t": "unsigned long", "intptr_t": "long"}
_WORD = re.compile(r"[A-Za-z_]\w*|::|\*|&&|&|\.\.\.|<|>|,|-?\d+")
_NOT_NAMES = {"if", "while", "for", "switch", "return", "sizeof", "alignof", "decltype", "static_assert"}


@dataclass(frozen=True)
class Label:
    path: str      # repository path of the declaring file
    line: int
    cls: str       # the class's own name (its last component, without template arguments)
    kind: str      # "ctor", "dtor" or "vtable"
    params: tuple  # parameter types, normalized (param_key)
    address: int | None  # None: the class template's (single) argument is the address
    targs: tuple | None = None  # a specialization's template arguments, normalized


def is_vtable(symbol: str) -> bool:
    """Whether a mangled symbol is a class's vtable (`_ZTV...`)."""
    return symbol.startswith("_ZTV")


def is_special(symbol: str) -> bool:
    """Whether a mangled symbol is a complete or base constructor or destructor (C1, C2, D1, D2)."""
    return _KIND.match(symbol) is not None


def _split(text: str, sep: str) -> list[str]:
    """`text` split at `sep` outside <> and ()."""
    out, depth, cur, i = [], 0, "", 0
    while i < len(text):
        ch = text[i]
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        if depth == 0 and text.startswith(sep, i):
            out.append(cur)
            cur = ""
            i += len(sep)
            continue
        cur += ch
        i += 1
    out.append(cur)
    return [p.strip() for p in out]


def _balanced(text: str, start: int) -> str:
    """The text from `start` (just after an opening parenthesis) to its closing one."""
    depth = 1
    for i in range(start, len(text)):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                return text[start:i]
    return text[start:]


def _type_key(text: str, named: bool) -> str:
    """One type as comparable text: `const`, `struct` and namespaces dropped, the project's integer and
    float typedefs spelled out, template arguments kept (normalized the same way), pointers and
    references kept in order. A source parameter (`named`) loses its name and default argument."""
    text = _split(text, "=")[0]
    tokens = _WORD.findall(text)
    if named and len(tokens) > 1 and re.fullmatch(r"[A-Za-z_]\w*", tokens[-1]) and tokens[-2] not in ("::", "<", ",") \
            and tokens[-1] not in _TYPEDEFS and not (len(tokens) == 2 and tokens[0] in ("unsigned", "signed")):
        tokens = tokens[:-1]
    out, marks = [], []
    for t in tokens:
        if t in ("const", "volatile", "struct", "class", "union", "enum", "typename"):
            continue
        if t == "::":
            while out and re.fullmatch(r"[A-Za-z_]\w*", out[-1]):
                out.pop()  # a namespace or enclosing class: compare the last name only
                break
            continue
        if t in ("*", "&", "&&") and not any(x == "<" for x in out[-1:]):
            marks.append(t)
            continue
        out.append(_TYPEDEFS.get(t, t))
    return " ".join(out).replace(" < ", "<").replace(" >", ">").replace(" , ", ",") + "".join(marks)


def param_key(params: str, named: bool = True) -> tuple:
    return tuple(_type_key(p, named) for p in _split(params, ",") if p and p != "void")


def _targ_key(text: str) -> tuple:
    """Template arguments as comparable text: integers by value (`0x10`, `16ul` and `16` are equal)."""
    out = []
    for a in _split(text, ","):
        m = re.fullmatch(r"\(?\s*(?:[A-Za-z_]\w*\s*\)\s*)?(0x[0-9a-fA-F]+|\d+)[uUlL]*\)?", a)
        out.append(str(int(m.group(1), 0)) if m else _type_key(a, named=False))
    return tuple(out)


def _address(m) -> int | None:
    text = m.group("label") or m.group("comment")
    return None if m.group("tparam") else int(text.split("_")[1], 16)


def scan_text(text: str, path: str) -> list[Label]:
    out = []
    for m in _VTABLE.finditer(text):
        line = text.count("\n", 0, m.start()) + 1
        out.append(Label(path, line, m.group("name"), "vtable", (), int(m.group("addr"), 16)))
    for m in _DECL.finditer(text):
        if m.group("name") in _NOT_NAMES:
            continue
        out.append(Label(path, text.count("\n", 0, m.start()) + 1, m.group("name"),
                         "dtor" if m.group("tilde") else "ctor", param_key(m.group("params")), _address(m)))
    for m in _SPEC.finditer(text):
        if m.group("tparam"):
            continue
        out.append(Label(path, text.count("\n", 0, m.start()) + 1, m.group("cls"),
                         "dtor" if m.group("tilde") else "ctor", param_key(m.group("params")), _address(m),
                         _targ_key(m.group("targs"))))
    for d in _DEFINE.finditer(text):
        mparams = [p.strip() for p in d.group("mparams").split(",")]
        for decl in _MACRO_DECL.finditer(d.group("body")):
            if decl.group("p") not in mparams:
                continue
            index = mparams.index(decl.group("p"))
            for use in re.finditer(r"^[ \t]*" + d.group("macro") + r"\(", text[d.end():], re.M):
                args = _split(_balanced(text, d.end() + use.end()), ",")
                if index >= len(args) or not re.fullmatch(r"[0-9a-f]{10}", args[index]):
                    continue
                line = text.count("\n", 0, d.end() + use.start()) + 1
                out.append(Label(path, line, decl.group("prefix") + args[index],
                                 "dtor" if decl.group("tilde") else "ctor", param_key(decl.group("params")),
                                 int(args[index], 16)))
    return out


def hand_written_files(root: Path = ROOT) -> list[Path]:
    files = sorted((root / "include").rglob("*.h"))
    for p in sorted((root / "src").rglob("*")):
        if p.suffix in (".cpp", ".h") and p.relative_to(root / "src").parts[0] not in GENERATED:
            files.append(p)
    return files


def split_member(demangled: str) -> tuple[str, tuple | None, str, str]:
    """(class's last name, its template arguments or None, member name, parameter list) of a demangled
    constructor or destructor name such as `ns::Box<16ul>::~Box()`."""
    head, params = demangled, ""
    depth = 0
    for i, ch in enumerate(demangled):
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch == "(" and depth == 0:
            head, params = demangled[:i], demangled[i + 1:].rsplit(")", 1)[0]
            break
    parts = _split(head, "::")
    member = parts[-1]
    owner = parts[-2] if len(parts) > 1 else member
    m = re.fullmatch(r"([A-Za-z_]\w*)(?:<(.*)>)?", owner)
    if not m:
        return owner, None, member, params
    return m.group(1), (_targ_key(m.group(2)) if m.group(2) is not None else None), member, params


class Labels:
    """Every special member's address that a hand-written declaration names (see the module docstring)."""

    def __init__(self, root: Path = ROOT, files: list[Path] | None = None):
        self.root = root
        self.by_file: dict[str, list[Label]] = {}
        for p in (files if files is not None else hand_written_files(root)):
            text = p.read_text(encoding="utf-8", errors="replace")
            if "fn_" not in text and "// vtable" not in text:
                continue
            found = scan_text(text, p.relative_to(root).as_posix())
            if found:
                self.by_file[found[0].path] = found
        self.headers = [lab for path, labs in self.by_file.items() if path.endswith(".h") for lab in labs]

    def in_file(self, source: str) -> list[Label]:
        """The declarations of one file; one outside the scanned set (a draft, a generated source) is read
        when first asked."""
        if source not in self.by_file:
            path = self.root / source
            text = path.read_text(encoding="utf-8", errors="replace") if path.is_file() else ""
            self.by_file[source] = scan_text(text, source)
        return self.by_file[source]

    def digest(self) -> str:
        """Changes whenever a label does (an object's bytes do not, so the check's diff cache needs it)."""
        rows = sorted(repr((lab.path, lab.cls, lab.kind, lab.params, lab.address, lab.targs))
                      for labs in self.by_file.values() for lab in labs)
        return hashlib.sha256("\n".join(rows).encode()).hexdigest()

    def lookup(self, symbol: str, demangled: str, source: str | None) -> tuple[int | None, str]:
        """(address, "") for a constructor or destructor symbol whose declaration names one, else
        (None, why). `demangled` is the symbol's full demangled name (with its parameter list);
        `source` the repository path of the object's source."""
        m = _KIND.match(symbol)
        if symbol.startswith("_ZTV") and demangled.startswith("vtable for "):
            kind, params = "vtable", ()
            cls, targs = split_member(demangled[len("vtable for "):] + "::x()")[:2]
        elif not m:
            return None, "not a constructor, destructor or vtable"
        else:
            kind = "dtor" if m.group("k").startswith("D") else "ctor"
            cls, targs, _, params_text = split_member(demangled)
            params = param_key(params_text, named=False)
        scope = self.in_file(source) if source else []
        for wanted in (scope, self.headers):
            hits = set()
            for lab in wanted:
                if lab.cls != cls or lab.kind != kind or lab.params != params:
                    continue
                if lab.targs is not None and lab.targs != targs:
                    continue
                if lab.address is not None:
                    hits.add(lab.address)
                elif targs and len(targs) == 1 and targs[0].isdigit():
                    hits.add(int(targs[0]))
            if len(hits) == 1:
                return hits.pop(), ""
            if len(hits) > 1:
                return None, "declared with different addresses: " + ", ".join(f"fn_{a:x}" for a in sorted(hits))
        return None, "no declaration names its address"
