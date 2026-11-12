#!/usr/bin/env python3
"""Declaration consistency of the hand-written sources: fn_/g_unk_ symbols declared in several
files or with several signatures.

The hand-written files (src/ without the generated src/reflection/, src/reflection_types/,
src/reflection_statics/, src/clones/, src/lifted/ and src/tables/; plus include/) declare their
callees and globals locally. Each file compiles alone, but two files that declare fn_<addr> with
different signatures can never link into one program, which the PC port needs. This parses every
declaration (not definition) of a symbol named fn_<10 hex> or g_unk_<10 hex> -- `extern` or
plain prototypes, inside or outside namespaces and extern "C" blocks, with attributes -- and
counts:
  declared            distinct symbols with at least one declaration
  multi-file          symbols declared in more than one file; "extra copies" is the number of files
                      beyond the first, summed over all symbols
  conflicting         symbols with more than one distinct normalised signature; "extra signatures" is
                      the number of signatures beyond the first, summed over all symbols (so one more
                      signature of an already conflicting symbol still counts)
  header-redeclared   symbols declared in a header under include/ and again outside include/
A declaration with an asm label, `T Name(...) asm("fn_...")` (a constructor or a differently
named function bound to the symbol), counts as a declaration of the labelled symbol.

Normalised signature: whitespace collapsed, parameter names and default arguments dropped,
`struct`/`class`/`union` elaboration and the `extern` storage class dropped, attributes dropped
(`__attribute__((...))`, `[[...]]`, `noexcept`, the HIDDEN macro: they do not change the type),
`(void)` written `()`; types, return type, `extern "C"` and the asm label are kept. Variables count
only with `extern` (without it they are definitions). Function bodies, initialisers, `static`
prototypes, `__attribute__((alias(...)))` and qualified names (`void CFoo::fn_...(...)`, a member)
are definitions or other symbols and are skipped. Known gaps, none in the tree today: a brace-init
default argument (`X x = X{}`) ends the statement early, so that prototype is dropped; every `#if`
branch is scanned, so alternative declarations of one symbol show as a conflict; in
`void fn_a(int), fn_b(int);` only fn_a is recorded. CRLF is accepted.

Definitions are compared too, in one respect: how the function is called. A hand-written definition
`T fn_<addr>(...) {` at the start of a line, with external linkage, is read as a declaration, and its
call shape (the class of the result and of each parameter: void, bool, an integer by width, float,
double, a pointer or reference, a type passed by value) is compared with the shape of every declaration
of the same function in other files. Another name for a pointed-to struct is the same call, and so is a
64-bit integer against a pointer (one full register on every 64-bit target); a `void`
definition of a function declared to return a value, a missing parameter, or a `u32` where the
declaration has a pointer is not. The match on AArch64 does not see such a difference when the registers
happen to hold the right values; the PC port calls the function wrongly. A float parameter written before
or after the integer ones is reported apart: floats travel in their own registers on AArch64 and on x86-64
Linux, so it is one call there, and another one only on Windows. Not seen: a definition inside a
namespace, or one whose first line starts with an attribute.

A definition's types are compared as well: a definition whose normalised signature is not the text of any
declaration of the same function in another file is counted. The call can be the same (the count above says
when it is not); what differs is the name of a pointed-to struct, the result's type, or a namespace written
out in one place and not the other. Such a function has two mangled names, which the port's link joins with
an alias (tools/port_link.py), and the file that defines it cannot include the header that declares it. The
conflicting-signature count does not include these: it compares declarations with each other. The ones
already in the tree stay; `--base` fails when a function is added to them (a namespace written out on one
side is the same type and never fails).

Usage:
  python tools/decl_check.py                        # counts and the top conflicting symbols; exit 0 (report)
  python tools/decl_check.py --list [--top N]       # every multiply-declared symbol with its signatures
  python tools/decl_check.py --symbol fn_7100d7c990 # each declaration of one symbol: file:line, scope, signature
  python tools/decl_check.py --json                 # the counts as JSON (tools/verify.py prints them)
  python tools/decl_check.py --definitions          # every definition called differently than a declaration says
  python tools/decl_check.py --definition-types     # every definition whose types are not a declaration's
  python tools/decl_check.py --base origin/main     # the same counts for the tree at <ref>; exit 1 if the
                                                    # conflicting-symbol count or the extra signatures grew, or
                                                    # a symbol declared under include/ is re-declared in a file
                                                    # where <ref> had no such re-declaration, or a function's
                                                    # definition newly differs from its declaration, in how it is
                                                    # called or in a type's name (CI runs this on pull
                                                    # requests); growth of identical extra copies (multi-file
                                                    # symbols, extra copies) only warns, unless --strict
Standard library and git only, no game data, so CI can run it. <ref>'s files are read with one
`git cat-file --batch` over `git ls-tree -r --name-only <ref>` (the same bytes `git show
<ref>:<path>` gives, in one process). The current tree is the working tree, tracked and untracked
(not ignored) files.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
import time
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GENERATED = ("src/reflection/", "src/reflection_types/", "src/reflection_statics/", "src/clones/",
             "src/lifted/", "src/tables/")
SUFFIXES = (".cpp", ".cc", ".cxx", ".h", ".hpp")
SYMBOL = re.compile(r"(?:fn|g_unk)_[0-9a-f]{10}")

# Comments and literals, matched together so that a comment marker inside a literal is left alone.
LEXEMES = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'', re.S)
PREPROCESSOR = re.compile(r"^[ \t]*#(?:[^\\\n]|\\\n|\\[^\n])*", re.M)  # with `\` continuation lines
INTERESTING = re.compile(r"[\"'{};]")
STRING = re.compile(r'"(?:[^"\\\n]|\\.)*"')
CHAR = re.compile(r"'(?:[^'\\\n]|\\.)*'")
TOKEN = re.compile(r'"(?:[^"\\]|\\.)*"|::|->|&&|\.\.\.|[A-Za-z_]\w*|\d\w*|\S')
NAMESPACE_HEAD = re.compile(r"(?:inline\s+)?namespace(?:\s+([A-Za-z_][\w:]*))?")
EXTERN_C = re.compile(r'extern\s*"C(?:\+\+)?"')
CLASS_HEAD = re.compile(r"(?:template\s*)?(?:struct|class|union)\b\s*(?:alignas\s*\(.*?\)\s*|__attribute__\s*\(\(.*?\)\)\s*"
                        r"|\[\[.*?\]\]\s*|final\s+)*([A-Za-z_]\w*)?", re.S)
ALIAS = re.compile(r"__attribute__\s*\(\(\s*alias\s*\(")
OPENERS = {"(": ")", "[": "]", "<": ">"}
CLOSERS = {")", "]", ">"}
# Groups opened right after these words are not parameter lists.
NOT_PARAMS = {"__attribute__", "asm", "alignas", "decltype", "noexcept", "sizeof", "throw", "__declspec",
              "static_assert"}
NOT_DECLARATIONS = {"using", "typedef", "static_assert", "friend", "template", "return", "namespace",
                    "enum", "goto"}
TYPE_WORDS = {"void", "bool", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "auto",
              "wchar_t", "char8_t", "char16_t", "char32_t", "size_t", "ssize_t", "ptrdiff_t", "intptr_t",
              "uintptr_t", "nullptr_t", "u8", "u16", "u32", "u64", "s8", "s16", "s32", "s64", "f32", "f64",
              "int8_t", "int16_t", "int32_t", "int64_t", "uint8_t", "uint16_t", "uint32_t", "uint64_t",
              "const", "volatile", "constexpr", "typename", "..."}
QUALIFIERS = {"const", "volatile", "struct", "class", "union", "enum", "typename"}
SPACE_AFTER = {"*", "&", "&&", ">", "]", ")"}  # before a word
# Dropped from the normalised signature (with their following `(...)` group, if any): they do not change
# the function or variable type. HIDDEN is `#define HIDDEN __attribute__((visibility("hidden")))` in src/.
DROPPED_ATTRIBUTES = {"__attribute__", "noexcept", "HIDDEN"}


@dataclass
class Decl:
    name: str
    signature: str
    path: str
    line: int
    scope: str
    # Character span [start, end) of the whole statement, `;` included, in the file's text with
    # line endings normalised to LF (tools/hub_migrate.py removes the statement by it).
    start: int = 0
    end: int = 0


def hand_written(path: str) -> bool:
    return (path.startswith(("src/", "include/")) and path.endswith(SUFFIXES)
            and not path.startswith(GENERATED))


def git(*args: str, data: bytes | None = None) -> bytes:
    return subprocess.run(["git", *args], cwd=ROOT, input=data, capture_output=True, check=True).stdout


def tree_files(ref: str | None) -> dict[str, str]:
    """path -> text of every hand-written file, in the working tree (ref None) or at a git ref."""
    if ref is None:
        names = git("ls-files", "--cached", "--others", "--exclude-standard").decode("utf-8", "replace")
        out = {}
        for p in sorted(set(filter(hand_written, names.splitlines()))):
            f = ROOT / p
            if f.is_file():
                out[p] = f.read_bytes().decode("utf-8", "replace")
        return out
    names = git("ls-tree", "-r", "--name-only", ref).decode("utf-8", "replace")
    paths = sorted(filter(hand_written, names.splitlines()))
    blob = git("cat-file", "--batch", data="".join(f"{ref}:{p}\n" for p in paths).encode("utf-8"))
    out = {}
    pos = 0
    for p in paths:
        nl = blob.index(b"\n", pos)
        header = blob[pos:nl].decode().split()
        if header[-1] in ("missing", "ambiguous"):
            pos = nl + 1
            continue
        size = int(header[2])
        out[p] = blob[nl + 1:nl + 1 + size].decode("utf-8", "replace")
        pos = nl + 1 + size + 1
    return out


def blank(m: re.Match) -> str:
    return re.sub(r"[^\n]", " ", m.group(0))


def strip(text: str) -> str:
    """Comments and preprocessor lines become spaces (newlines kept, so line numbers hold); literals stay."""
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    text = LEXEMES.sub(lambda m: m.group(0) if m.group(0)[0] in "\"'" else blank(m), text)
    return PREPROCESSOR.sub(blank, text)


def drop_angles(s: str) -> str:
    out, depth = [], 0
    for ch in s:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            out.append(ch)
    return "".join(out)


def classify(head: str) -> tuple[str, str]:
    """What the `{` after this text opens: ('ns', name), ('externc', ''), ('struct', name) or ('other', '')."""
    s = " ".join(head.split())
    m = NAMESPACE_HEAD.fullmatch(s)
    if m:
        return "ns", m.group(1) or "(anonymous)"
    if EXTERN_C.fullmatch(s):
        return "externc", ""
    flat = drop_angles(re.sub(r"alignas\s*\(.*?\)|__attribute__\s*\(\(.*?\)\)|\[\[.*?\]\]", "", s))
    m = CLASS_HEAD.match(flat)
    if m and "(" not in flat:
        return "struct", m.group(1) or "(anonymous)"
    return "other", ""


def is_word(t: str) -> bool:
    return t[0].isalnum() or t[0] in '_"'


def is_ident(t: str) -> bool:
    return t[0].isalpha() or t[0] == "_"


def top_level(toks: list[str]) -> list[int]:
    """Indices of the tokens at bracket depth 0."""
    out, depth = [], 0
    for i, t in enumerate(toks):
        if t in CLOSERS:
            depth = max(0, depth - 1)
        if depth == 0:
            out.append(i)
        if t in OPENERS:
            depth += 1
    return out


def group_end(toks: list[str], start: int) -> int:
    """Index of the token closing the group opened at toks[start]."""
    depth = 0
    for i in range(start, len(toks)):
        if toks[i] in OPENERS:
            depth += 1
        elif toks[i] in CLOSERS:
            depth -= 1
            if depth == 0:
                return i
    return len(toks) - 1


def split_top(toks: list[str], sep: str) -> list[list[str]]:
    parts, cur, depth = [], [], 0
    for t in toks:
        if t in CLOSERS:
            depth -= 1
        if t == sep and depth == 0:
            parts.append(cur)
            cur = []
            continue
        cur.append(t)
        if t in OPENERS:
            depth += 1
    parts.append(cur)
    return parts


def strip_param(toks: list[str]) -> list[str]:
    """One parameter without its name and default argument (nested parameter lists included)."""
    toks = split_top(toks, "=")[0]
    for i in range(len(toks) - 3):  # (*name) or (&name): a function pointer or reference declarator
        if toks[i] == "(" and toks[i + 1] in ("*", "&", "&&") and is_ident(toks[i + 2]) and toks[i + 3] == ")":
            return normalise(toks[:i + 2] + toks[i + 3:])
    for i in range(1, len(toks) - 1):  # name[...]
        if (toks[i + 1] == "[" and is_ident(toks[i]) and toks[i] not in TYPE_WORDS and toks[i - 1] != "::"
                and any((is_word(t) and t not in QUALIFIERS) or t in SPACE_AFTER for t in toks[:i])):
            return normalise(toks[:i] + toks[i + 1:])
    if (len(toks) >= 2 and is_ident(toks[-1]) and toks[-1] not in TYPE_WORDS and toks[-2] != "::"
            and any((is_word(t) and t not in QUALIFIERS) or t in SPACE_AFTER for t in toks[:-1])):
        toks = toks[:-1]
    return normalise(toks)


def normalise(toks: list[str]) -> list[str]:
    """Drop parameter names and defaults in every parameter list, the `extern` storage class,
    struct/class/union elaboration and attributes (`__attribute__((...))`, `[[...]]`, `noexcept`, HIDDEN);
    write `(void)` as `()`; keep asm and declarator groups like (*name) verbatim."""
    out: list[str] = []
    i = 0
    while i < len(toks):
        t = toks[i]
        if t in DROPPED_ATTRIBUTES:
            i = group_end(toks, i + 1) + 1 if i + 1 < len(toks) and toks[i + 1] == "(" else i + 1
            continue
        if t == "[" and i + 1 < len(toks) and toks[i + 1] == "[":
            i = group_end(toks, i) + 1  # [[attribute]]
            continue
        if t == "(":
            j = group_end(toks, i)
            inner = toks[i + 1:j]
            if (i and toks[i - 1] in NOT_PARAMS) or (inner and inner[0] in ("*", "&", "&&")):
                out.extend(toks[i:j + 1])
            else:
                out.append("(")
                params = split_top(inner, ",") if inner and inner != ["void"] else []
                for k, p in enumerate(params):
                    if k:
                        out.append(",")
                    out.extend(strip_param(p))
                out.append(")")
            i = j + 1
            continue
        if t == "extern" and not (i + 1 < len(toks) and toks[i + 1][0] == '"'):
            i += 1
            continue
        if t in ("struct", "class", "union"):
            i += 1
            continue
        out.append(t)
        i += 1
    return out


def join(toks: list[str]) -> str:
    out: list[str] = []
    prev = ""
    for t in toks:
        if prev and ((is_word(t) and (is_word(prev) or prev in SPACE_AFTER)) or prev == ","):
            out.append(" ")
        out.append(t)
        prev = t
    return "".join(out)


def variable_name(toks: list[str]) -> str | None:
    """The symbol a variable declarator names: the last top-level one, or the one inside (*name)."""
    syms = [toks[i] for i in top_level(toks) if SYMBOL.fullmatch(toks[i])]
    if syms:
        return syms[-1]
    for i in range(len(toks) - 3):
        if toks[i] == "(" and toks[i + 1] in ("*", "&", "&&") and SYMBOL.fullmatch(toks[i + 2]) and toks[i + 3] == ")":
            return toks[i + 2]
    return None


def declarations(stmt: str, scope_kind: str) -> list[tuple[str, str]]:
    """(declared symbol, normalised signature) pairs for one statement at namespace or struct scope."""
    toks = TOKEN.findall(stmt)
    if not toks or ALIAS.search(stmt):
        return []
    first = next((t for t in toks if is_ident(t)), "")
    if first in NOT_DECLARATIONS:
        return []
    top = top_level(toks)
    top_toks = [toks[i] for i in top]
    if "=" in top_toks or (scope_kind == "ns" and "static" in top_toks):
        return []  # an initialiser (a definition), or internal linkage

    label = None
    for i in range(len(toks) - 3):
        if toks[i] == "asm" and toks[i + 1] == "(" and toks[i + 2][0] == '"' and toks[i + 3] == ")":
            label = toks[i + 2][1:-1]
    if label and SYMBOL.fullmatch(label):
        return [(label, join(normalise(toks)))]
    if scope_kind == "struct":
        return []  # a member named fn_... is a different (mangled) symbol

    # A function prototype: the declarator name precedes the first top-level group that is not an attribute.
    for i in top:
        if toks[i] != "(":
            continue
        if i and toks[i - 1] in NOT_PARAMS:
            continue
        if i + 1 < len(toks) and toks[i + 1] in ("*", "&", "&&"):
            break  # T (*name)(...): a variable
        if i and is_ident(toks[i - 1]):
            if i >= 2 and toks[i - 2] == "::":
                return []  # T CFoo::fn_...(...): a member of CFoo, a different (mangled) symbol
            return [(toks[i - 1], join(normalise(toks)))] if SYMBOL.fullmatch(toks[i - 1]) else []
        break

    # A variable, possibly several declarators: without extern it is a definition.
    if "extern" not in top_toks:
        return []
    parts = split_top(toks, ",")
    name = variable_name(parts[0])
    if name is None:
        return []  # the symbol is only mentioned (a template argument, a default argument, ...)
    out = [(name, join(normalise(parts[0])))]
    type_toks = parts[0][:parts[0].index(name)]
    while type_toks and type_toks[-1] in ("*", "&", "&&", "("):
        type_toks.pop()
    for part in parts[1:]:
        name = variable_name(part)
        if name:
            out.append((name, join(normalise(type_toks + part))))
    return out


def scope_name(stack: list[tuple[str, str]]) -> str:
    ns = "::".join(n for k, n in stack if k == "ns")
    parts = [ns] if ns else []
    if any(k == "externc" for k, _ in stack):
        parts.append('extern "C"')
    structs = [n for k, n in stack if k == "struct"]
    if structs:
        parts.append("struct " + "::".join(structs))
    return " ".join(parts) or "(global)"


def scan(text: str, path: str) -> list[Decl]:
    text = strip(text)
    out: list[Decl] = []
    stack: list[tuple[str, str]] = []
    start = pos = 0
    while True:
        m = INTERESTING.search(text, pos)
        if not m:
            break
        i = m.start()
        c = m.group(0)
        if c == '"' or c == "'":
            lm = (STRING if c == '"' else CHAR).match(text, i)
            pos = lm.end() if lm else i + 1
            continue
        stmt = text[start:i]
        if c == "{":
            stack.append(classify(stmt))
        elif c == "}":
            if stack:
                stack.pop()
        else:
            kinds = {k for k, _ in stack}
            scope_kind = "ns" if kinds <= {"ns", "externc"} else "struct" if kinds <= {"ns", "externc", "struct"} else None
            if scope_kind and SYMBOL.search(stmt):
                found = declarations(stmt, scope_kind)
                if found:
                    first = start + (len(stmt) - len(stmt.lstrip()))
                    line = text.count("\n", 0, first) + 1
                    out.extend(Decl(name, sig, path, line, scope_name(stack), first, i + 1) for name, sig in found)
        start = pos = i + 1
    return out


class Report:
    def __init__(self, files: dict[str, str]):
        self.files = len(files)
        self.decls: dict[str, list[Decl]] = defaultdict(list)
        for path, text in files.items():
            for d in scan(text, path):
                self.decls[d.name].append(d)
        self.paths = {n: sorted({d.path for d in ds}) for n, ds in self.decls.items()}
        self.sigs = {n: sorted({d.signature for d in ds}) for n, ds in self.decls.items()}
        self.multi = sorted(n for n, ps in self.paths.items() if len(ps) > 1)
        self.conflicting = sorted(n for n, ss in self.sigs.items() if len(ss) > 1)
        # (symbol, file outside include/) for every symbol that a header under include/ declares
        self.redeclared = {(n, p) for n, ps in self.paths.items()
                           if any(p.startswith("include/") for p in ps)
                           for p in ps if not p.startswith("include/")}
        self.header_redeclared = sorted({n for n, _ in self.redeclared})

    def counts(self) -> dict[str, int]:
        return {"files": self.files, "declared": len(self.decls),
                "multi_file": len(self.multi), "extra_files": sum(len(ps) - 1 for ps in self.paths.values()),
                "conflicting": len(self.conflicting), "extra_signatures": sum(len(ss) - 1 for ss in self.sigs.values()),
                "header_redeclared": len(self.header_redeclared)}

    def top(self, n: int) -> list[tuple[str, int, int]]:
        """(symbol, signatures, files) for the n symbols with the most signatures."""
        rows = [(s, len(self.sigs[s]), len(self.paths[s])) for s in self.conflicting]
        rows.sort(key=lambda r: (-r[1], -r[2], r[0]))
        return rows[:n]

    def line(self, label: str) -> str:
        c = self.counts()
        return (f"decl_check: {label:<8} {c['files']} files: {c['declared']} symbols declared, "
                f"{c['multi_file']} in more than one file ({c['extra_files']} extra copies), {c['conflicting']} with "
                f"conflicting signatures ({c['extra_signatures']} extra), {c['header_redeclared']} declared in include/ "
                f"and again outside it")


def print_list(r: Report) -> None:
    for name in sorted(set(r.multi) | set(r.conflicting), key=lambda n: (-len(r.sigs[n]), -len(r.paths[n]), n)):
        print(f"{name}  {len(r.paths[name])} files  {len(r.sigs[name])} signatures")
        if len(r.sigs[name]) > 1:
            by_sig: dict[str, set[str]] = defaultdict(set)
            for d in r.decls[name]:
                by_sig[d.signature].add(d.path)
            for sig, paths in sorted(by_sig.items(), key=lambda kv: (-len(kv[1]), kv[0])):
                print(f"    [{len(paths)} files] {sig}")
    if r.header_redeclared:
        print()
        print(f"{len(r.header_redeclared)} symbols declared under include/ and again outside it:")
        for name in r.header_redeclared:
            headers = [p for p in r.paths[name] if p.startswith("include/")]
            others = [p for p in r.paths[name] if not p.startswith("include/")]
            print(f"  {name}  {' '.join(headers)}  re-declared in {len(others)} files: "
                  f"{' '.join(others[:5])}{' ...' if len(others) > 5 else ''}")


def print_symbol(r: Report, name: str) -> None:
    ds = sorted(r.decls.get(name, []), key=lambda d: (d.path, d.line))
    for d in ds:
        print(f"{d.path}:{d.line}  [{d.scope}]  {d.signature}")
    print(f"{name}: {len(ds)} declarations in {len(r.paths.get(name, []))} files, "
          f"{len(r.sigs.get(name, []))} distinct signatures")


def grown(cur_by: dict[str, list[str]], base_by: dict[str, list[str]]) -> str:
    """The symbols with more signatures (or files) than at the base, beyond the first: `sym (b -> c)`."""
    syms = sorted(s for s, v in cur_by.items() if len(v) > max(len(base_by.get(s, [])), 1))
    return (" ".join(f"{s} ({len(base_by.get(s, []))} -> {len(cur_by[s])})" for s in syms[:8])
            + (" ..." if len(syms) > 8 else ""))


def compare(cur: Report, base: Report, ref: str, strict: bool = False) -> tuple[list[str], list[str]]:
    """(failures, warnings) of the current tree against the base tree.

    Failures: the conflicting-symbol count or the extra signatures summed over all symbols grew (one
    more divergent signature of an already conflicting symbol fails too), or a symbol that include/
    declares is re-declared in a file where the base had none. Those are what stop the sources from
    linking. Warnings: the multi-file count or the extra copies grew (one more local copy of an
    identical signature); with `strict` they fail too."""
    reasons, warnings = [], []
    c, b = cur.counts(), base.counts()
    if c["conflicting"] > b["conflicting"] or c["extra_signatures"] > b["extra_signatures"]:
        reasons.append(f"conflicting symbols {b['conflicting']} -> {c['conflicting']}, extra signatures "
                       f"{b['extra_signatures']} -> {c['extra_signatures']}; more signatures than at {ref}: "
                       + grown(cur.sigs, base.sigs))
    if c["multi_file"] > b["multi_file"] or c["extra_files"] > b["extra_files"]:
        (reasons if strict else warnings).append(
            f"symbols in more than one file {b['multi_file']} -> {c['multi_file']}, extra copies "
            f"{b['extra_files']} -> {c['extra_files']}; declared in more files than at {ref}: "
            + grown(cur.paths, base.paths))
    new_pairs = sorted(cur.redeclared - base.redeclared)
    if new_pairs:
        reasons.append(f"{len(new_pairs)} new re-declarations of symbols that include/ declares: "
                       + " ".join(f"{s} in {p}" for s, p in new_pairs[:5]) + (" ..." if len(new_pairs) > 5 else ""))
    return reasons, warnings


DEFINITION = re.compile(r"^((?:[A-Za-z_][\w:<>,*& \t]*?[\s*&])?fn_[0-9a-f]{10}\s*\([^{};]*\))\s*\{", re.M)
CLASSES = {"void": "void", "bool": "bool", "char": "i8", "s8": "i8", "u8": "i8", "s16": "i16", "u16": "i16",
           "short": "i16", "int": "i32", "s32": "i32", "u32": "i32", "unsigned": "i32", "wchar_t": "i32",
           "s64": "i64", "u64": "i64", "long": "i64", "size_t": "i64", "float": "f32", "f32": "f32",
           "double": "f64", "f64": "f64"}


def definitions(text: str, path: str) -> list[Decl]:
    """The file's top-level function definitions `T fn_<addr>(...) {` with external linkage, as declarations."""
    text = strip(text)
    out: list[Decl] = []
    for m in DEFINITION.finditer(text):
        head = m.group(1)
        if re.match(r"\s*(static|inline|template)\b", head) or "::fn_" in head:
            continue
        line = text.count("\n", 0, m.start(1)) + 1
        out += [Decl(d.name, d.signature, path, line, d.scope) for d in scan(head + ";", path)]
    return out


def type_class(text: str) -> str:
    text = text.strip()
    if text == "...":
        return "..."
    if any(c in text for c in "*&(["):
        return "pointer"
    words = [w for w in re.findall(r"[A-Za-z_]\w*", text) if w not in ("const", "volatile", "signed", "enum", "extern", "C")]
    return CLASSES[words[-1]] if words and all(w in CLASSES for w in words) else "by value"


def call_shape(signature: str, symbol: str) -> tuple[str, tuple[str, ...]] | None:
    """(class of the result, class of each parameter) of a normalised function signature; None when the
    symbol is not declared as a plain function there (an asm label on another name, or a variable)."""
    result, rest = signature.split(symbol, 1)
    if not rest.lstrip().startswith("(") or ")" not in rest:
        return None
    inner = rest[rest.index("(") + 1:rest.rindex(")")]
    params, depth, cur = [], 0, ""
    for ch in inner:
        depth += ch in "<(["
        depth -= ch in ">)]"
        if ch == "," and depth == 0:
            params.append(cur)
            cur = ""
        else:
            cur += ch
    params = [type_class(x) for x in params + [cur] if x.strip()]
    return type_class(result), tuple(params)


def as_passed(shape):
    """A call shape with 64-bit integers counted as pointers: both are one full integer register on every
    64-bit target, so `u64` against `void*` is the same call."""
    if shape is None:
        return None
    return ("pointer" if shape[0] == "i64" else shape[0], tuple("pointer" if p == "i64" else p for p in shape[1]))


def by_register(params: tuple[str, ...]) -> tuple[tuple[str, ...], tuple[str, ...]]:
    """(the float and double parameters in order, the others in order): on AArch64 and on x86-64 Linux the
    two kinds are passed in separate registers, so only the order within each kind decides the call."""
    floats = tuple(p for p in params if p in ("f32", "f64"))
    return floats, tuple(p for p in params if p not in ("f32", "f64"))


def definition_mismatches(files: dict[str, str], report: Report) -> dict[str, tuple[str, str, str, str]]:
    """{function: (what differs, the defining file, the definition's signature, a declaration's signature)} for
    the hand-written definitions whose call shape is not the shape of a declaration in another file."""
    out: dict[str, tuple[str, str, str, str]] = {}
    for path, text in files.items():
        if path.startswith("include/") or "fn_" not in text:
            continue
        for d in definitions(text, path):
            mine = as_passed(call_shape(d.signature, d.name))
            for other in report.sigs.get(d.name, []):
                theirs = as_passed(call_shape(other, d.name))
                if theirs is None or mine is None or theirs == mine:
                    continue
                if (mine[0] == "void") != (theirs[0] == "void"):
                    what = "one returns a value, the other is void"
                elif mine[0] != theirs[0]:
                    what = "the result's class"
                elif len(mine[1]) != len(theirs[1]):
                    what = "the number of parameters"
                elif by_register(mine[1]) == by_register(theirs[1]):
                    what = "the order of float and other parameters (one call on AArch64; not on Windows)"
                else:
                    what = "a parameter's class"
                out[d.name] = (what, path, d.signature, other)
                break
    return out


# a namespace path (`base::global::`), or the global scope written out (`::UnkFoo`, which a file needs when a
# `using namespace` makes the plain name ambiguous)
QUALIFIER = re.compile(r"\b(?:[a-z_][a-z0-9_]*::)+|(?<![\w:>])::(?=\w)")
POINTEE = re.compile(r"\b[A-Za-z_]\w*(?=[*&])")


def definition_type_differences(files: dict[str, str], report: Report) -> dict[str, tuple[str, str, str, str]]:
    """{function: (what differs, the defining file, the definition's signature, a declaration's signature)} for
    the hand-written definitions whose normalised signature is the text of no declaration of the function in
    another file. What differs, the first that applies: a namespace written out, the result's type, the name
    of a pointed-to type, a parameter's type."""
    out: dict[str, tuple[str, str, str, str]] = {}
    for path, text in files.items():
        if path.startswith("include/") or "fn_" not in text:
            continue
        for d in definitions(text, path):
            others = sorted({o.signature for o in report.decls.get(d.name, []) if o.path != path})
            if not others or d.signature in others:
                continue
            other = others[0]
            mine, theirs = QUALIFIER.sub("", d.signature), QUALIFIER.sub("", other)
            if mine == theirs:
                what = NAMESPACE_ONLY
            elif mine.split(d.name)[0] != theirs.split(d.name)[0]:
                what = "the result's type"
            elif POINTEE.sub("T", mine) == POINTEE.sub("T", theirs):
                what = "the name of a pointed-to type"
            else:
                what = "a parameter's type"
            out[d.name] = (what, path, d.signature, other)
    return out


NAMESPACE_ONLY = "a namespace written out"


def beyond_namespace(found: dict[str, tuple[str, str, str, str]]) -> int:
    return sum(1 for v in found.values() if v[0] != NAMESPACE_ONLY)


def new_type_differences(cur: dict[str, tuple[str, str, str, str]],
                         base: dict[str, tuple[str, str, str, str]]) -> list[str]:
    """The functions whose definition names a type differently than a declaration now and did not at the base.
    A namespace written out on one side is the same type and does not count, on either side."""
    return sorted(s for s, v in cur.items()
                  if v[0] != NAMESPACE_ONLY and (s not in base or base[s][0] == NAMESPACE_ONLY))


def file_problems(files: set[str], tree: dict[str, str] | None = None) -> list[str]:
    """What is wrong with the fn_/g_unk_ declarations and definitions of these files (paths relative to the
    repository), given every other file as it is. One text per problem, each saying what to do:

      a declaration of a symbol that a header in include/ declares (include the header);
      a declaration of a function that another file defines with other types (copy the definition's
        signature; not reported when another file already declares it this way, which is that file's fault);
      a declaration whose signature no other file uses for that symbol (copy an existing one);
      a definition here whose types are not those of a declaration elsewhere (use that parameter list).

    A namespace written out on one side is the same type. An identical copy of a declaration in another
    source is not a problem here: tools/decl_plan.py and tools/hub_migrate.py move those to a header."""
    if tree is None:
        tree = tree_files(None)
    r = Report(tree)

    def bare(signature: str) -> str:
        return QUALIFIER.sub("", signature)
    defined = {}
    for path, text in tree.items():
        if not path.startswith("include/") and "fn_" in text:
            for d in definitions(text, path):
                defined[d.name] = d
    out = []
    for name, ds in sorted(r.decls.items()):
        mine = [d for d in ds if d.path in files]
        if not mine:
            continue
        others = [d for d in ds if d.path not in files]
        other_sigs = {d.signature for d in others}
        inc = [d for d in others if d.path.startswith("include/")]
        definition = defined.get(name)
        for d in mine:
            why = None
            if inc:
                why = f"declared in {inc[0].path}: include that header and drop this declaration"
            elif (definition and definition.path not in files and bare(definition.signature) != bare(d.signature)
                  and d.signature not in other_sigs):
                why = (f"{definition.path} defines it with other types; copy the definition's signature exactly:\n"
                       f"    {definition.signature}")
            elif other_sigs and d.signature not in other_sigs:
                why = "signature differs from every existing one; copy one of these exactly:\n    " + \
                      "\n    ".join(f"{s}   ({next(o.path for o in others if o.signature == s)})"
                                    for s in sorted(other_sigs))
            if why:
                out.append(f"{d.path}:{d.line} {name}: {d.signature}\n  -> {why}")
    for name, d in sorted(defined.items()):
        if d.path not in files:
            continue
        sigs = sorted({o.signature for o in r.decls.get(name, []) if o.path != d.path})
        if sigs and all(bare(s) != bare(d.signature) for s in sigs):
            out.append(f"{d.path}:{d.line} {name}: defined as {d.signature}\n  -> declared elsewhere with other "
                       f"types; use this parameter list and result:\n    {sigs[0]}")
    return out


def print_definition_types(found: dict[str, tuple[str, str, str, str]]) -> None:
    kinds = Counter(v[0] for v in found.values())
    print(f"{len(found)} hand-written definitions have a signature that is not a declaration's of the same function:")
    for what, n in kinds.most_common():
        print(f"  {n:5}  {what}")
    for symbol, (what, path, mine, theirs) in sorted(found.items(), key=lambda kv: (kv[1][0], kv[0])):
        print(f"{symbol}  {what}\n    defined   {mine}  ({path})\n    declared  {theirs}")


def print_definitions(found: dict[str, tuple[str, str, str, str]]) -> None:
    kinds = Counter(v[0] for v in found.values())
    print(f"{len(found)} hand-written definitions are called differently than a declaration of the same function says:")
    for what, n in kinds.most_common():
        print(f"  {n:5}  {what}")
    for symbol, (what, path, mine, theirs) in sorted(found.items(), key=lambda kv: (kv[1][0], kv[0])):
        print(f"{symbol}  {what}\n    defined   {mine}  ({path})\n    declared  {theirs}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--base", metavar="REF", help="also count the tree at REF; exit 1 if the conflicting-signature "
                    "counts grew or a symbol that include/ declares gained a re-declaration outside include/ "
                    "(growth of identical extra copies only warns)")
    ap.add_argument("--strict", action="store_true", help="with --base: growth of extra copies fails too")
    ap.add_argument("--list", action="store_true", help="list every multiply-declared symbol and its signatures")
    ap.add_argument("--symbol", metavar="NAME", help="list every declaration of NAME with file:line")
    ap.add_argument("--definitions", action="store_true",
                    help="list every definition whose result or parameters are not what a declaration of it says")
    ap.add_argument("--definition-types", action="store_true",
                    help="list every definition whose signature is not the text of a declaration of it elsewhere")
    ap.add_argument("--json", action="store_true", help="print the counts (and the top symbols) as JSON")
    ap.add_argument("--top", type=int, default=10, help="how many conflicting symbols to show (default 10)")
    args = ap.parse_args()

    start = time.time()
    try:
        cur_files = tree_files(None)
        cur = Report(cur_files)
        cur_defs = definition_mismatches(cur_files, cur)
        cur_types = definition_type_differences(cur_files, cur)
        base_files = tree_files(args.base) if args.base else None
        base = Report(base_files) if args.base else None
        base_defs = definition_mismatches(base_files, base) if args.base else {}
        base_types = definition_type_differences(base_files, base) if args.base else {}
    except subprocess.CalledProcessError as e:
        print(f"FAIL decl_check: git {' '.join(e.cmd[1:3])} failed: {e.stderr.decode('utf-8', 'replace').strip()}")
        return 2
    secs = time.time() - start

    if args.symbol:
        print_symbol(cur, args.symbol)
        return 0
    if args.definitions:
        print_definitions(cur_defs)
        return 0
    if args.definition_types:
        print_definition_types(cur_types)
        return 0

    result: dict = {**cur.counts(), "definition_mismatches": len(cur_defs),
                    "definition_type_differences": len(cur_types), "seconds": round(secs, 2),
                    "top": [{"symbol": s, "signatures": n, "files": f} for s, n, f in cur.top(args.top)]}
    reasons: list[str] = []
    warnings: list[str] = []
    if base is not None:
        reasons, warnings = compare(cur, base, args.base, args.strict)
        new_defs = sorted(set(cur_defs) - set(base_defs))
        if new_defs:
            reasons.append(f"{len(new_defs)} definitions newly differ from a declaration of the same function in how it "
                           "is called: " + "; ".join(f"{s} in {cur_defs[s][1]} ({cur_defs[s][0]})" for s in new_defs[:5])
                           + (" ..." if len(new_defs) > 5 else "") + " (`--definitions` shows both signatures)")
        new_types = new_type_differences(cur_types, base_types)
        if new_types:
            reasons.append(f"{len(new_types)} definitions newly name a type differently than a declaration of the same "
                           "function: " + "; ".join(f"{s} in {cur_types[s][1]} ({cur_types[s][0]})" for s in new_types[:5])
                           + (" ..." if len(new_types) > 5 else "") + " (`--definition-types` shows both signatures; "
                           "give the callers the definition's types, or the definition the header's)")
        result["base"] = {"ref": args.base, **base.counts(), "definition_mismatches": len(base_defs),
                          "definition_type_differences": len(base_types)}
        result["result"] = "FAIL" if reasons else "PASS"
        result["reasons"] = reasons
        result["warnings"] = warnings
    if args.json:
        print(json.dumps(result, indent=2))
        return 1 if reasons else 0

    print(cur.line("HEAD") + f" ({secs:.1f} s)")
    print(f"decl_check: {len(cur_defs)} definitions are called differently than a declaration of the same function "
          "says (`--definitions` lists them)")
    print(f"decl_check: {len(cur_types)} definitions have types that are not a declaration's elsewhere, "
          f"{beyond_namespace(cur_types)} beyond a namespace written out (`--definition-types` lists them; `--base` "
          "fails on a new one)")
    if base is not None:
        print(base.line(args.base))
    if args.list:
        print_list(cur)
    elif cur.conflicting:
        print(f"top {min(args.top, len(cur.conflicting))} conflicting symbols (signatures/files):")
        for s, n, f in cur.top(args.top):
            print(f"  {s}  {n}/{f}")
    if base is None:
        print("NOTE decl_check: report only; `--base <ref>` enforces that no conflicting signature is added and that "
              "no symbol include/ declares gains a local copy (CI runs it on pull requests; `--strict` also rejects "
              "identical extra copies), `--symbol <name>` lists one symbol's declarations")
        return 0
    for w in warnings:
        print(f"WARN decl_check: {w}")
    if reasons:
        for r in reasons:
            print(f"FAIL decl_check: {r}")
        print("NEXT: declare each symbol once, in a header under include/ (reuse one that already declares it), "
              "and include that header instead of a local prototype; `python tools/decl_check.py --symbol <name>` "
              "lists every copy with its signature")
        return 1
    c, b = cur.counts(), base.counts()
    print(f"PASS decl_check: vs {args.base}: conflicting {b['conflicting']} -> {c['conflicting']} "
          f"({b['extra_signatures']} -> {c['extra_signatures']} extra signatures), multi-file {b['multi_file']} -> "
          f"{c['multi_file']} ({b['extra_files']} -> {c['extra_files']} extra copies), header re-declarations "
          f"{b['header_redeclared']} -> {c['header_redeclared']}, definitions that differ from a declaration "
          f"{len(base_defs)} -> {len(cur_defs)}, none new, definitions that name a type differently "
          f"{beyond_namespace(base_types)} -> {beyond_namespace(cur_types)}, none new")
    return 0


if __name__ == "__main__":
    sys.exit(main())
