#!/usr/bin/env python3
"""Parse Metroid Dread's `.bshdat` shader packages and extract each program's GLSL source.

Every package under romfs/system/shd/ holds one or more programs, each a vertex and a fragment
stage compiled by NVN's GLSL compiler; the debug part of each stage carries the GLSL source the
compiler was given. This tool writes those sources to build/shaders/<version>/<package>/ with a
manifest.json per package (programs, variant keys, #defines, includes, the compiler's reflection
of every stage, the package's name tables), and with --compile builds each one with glslang to
OpenGL and Vulkan SPIR-V (build/shaders/<version>/compile_report.csv and a printed summary).
The extracted text is game data: it stays in build/.

Layout (derived from the 2.1.0 dump's bytes; holds for all 478 packages; little-endian; every
"->" is a u64 file offset; names in brackets are ours):

  Package
    0x00  "MSHD", u16 1, u16 0x13
    0x08  -> [program list]          list of program records
    0x10  -> [variant map]           list of {u32 key, u32 key2, u64 -> program record}; key ==
                                     key2 in every file; the i-th entry names the i-th program
                                     (keys 0..n-1, reversed in the one four-program package)
    0x18  -> a program record        the last of the program list in every file (role unknown)
    0x20  u64 0                      in every file (unknown)
    0x28  -> [uniform list 1]        names with engine IDs (below)
    0x30  -> [uniform list 2]        names with engine IDs
    0x38  -> [attribute list]        vertex attribute names with engine attribute IDs
    0x40  0xff up to 0x80, then the stage blobs, each 0x80-aligned, gaps filled with 0xff; then,
          8-aligned, the tables to the end of the file: list nodes, program records, map entries,
          name entries, NUL-terminated names. The parser accounts for every byte of every package
          (what it does not explain is 0xff padding; checked per package, `unaccounted_bytes`).

    List: chain of 16-byte nodes {-> item, -> next node or 0}; a 0 head is an empty list.
    Program record, 0x30 bytes: three stage slots {u32 size, u32 0xffffffff, u64 offset}. Slot 0
      is empty in every file, slot 1 is the vertex stage, slot 2 the fragment stage (their
      assembly listings begin !!NVvp5.0 and !!NVfp5.0). The engine's program loader
      (fn_71001b0050) creates slot 0 as its stage type 2, which its program binding
      (fn_710015d900) binds as NVN stage 2 with stage bit 4: a geometry stage, unused.
    Name entry, 12 bytes (+4 bytes 0xff): {-> name, u32 id}. Both uniform lists use one ID space
      (uView and g_mView are both 0, g_fTime 116) and a name has the same ID in every package.
      Names are expanded as GL program introspection lists them (`g_aLight[0].vPos`,
      `vs_tx[1]`), including names no stage of the package uses. List 1 holds mostly per-object
      names (u*), list 2 the scene, light and material ones (g_*); what decides the split, and
      where the IDs come from, is unknown (the engine's own enumeration).

  Stage blob (one compiled stage)
    0x00  u32 0x19866891, u32 0, u32 0x00200416, u32 0, u32 0x11, u32 0x15, u32 1, u32 0x10,
          u32 0x58; 0x44 u32 the blob's size, 0x48 u32 the first section's offset (0x3f0),
          0x4c u32 the section count (4); other words to 0x90 are 0 (meaning of the constants
          unknown).
    Section records, 0x90 bytes each from 0x90: u32 size, u32 offset (from the blob), u32 kind,
      then a kind-specific payload; zeros from the end of the records (0x2d0) to 0x3f0. Sections
      are contiguous and 8-aligned; the kinds are 0, 1, 3, 4 in that order in every blob.
    Kind 0, the stage as NVN takes it (from the engine's shader loader, fn_710015c9b0, which
      passes it to nvnProgramSetShaders): record payload +0x2c the stage (0 vertex, 1 fragment),
      +0x30 the control section's offset (0 in every blob), +0x34 the code's offset (the end of
      the control section), +0x38 the code's size (the code ends the section). The engine copies
      the code into GPU memory and hands NVN the control section by pointer; the port's NVN shim
      recognises a stage by that control section (CONTROL_KEY_BYTES). Kind 1: not decoded.
    Reflection (kind 3, starts u32 0x19292919): the stage's active uniform blocks, uniforms
      (samplers included), inputs and outputs, with the bindings the compiler assigned; layout in
      parse_reflection. Not decoded: the 0x108 bytes after the outputs table, payload words 27-35.
    Debug (kind 4, starts u32 0x65040891): +0x18 u64 hash (also in the section record payload),
      +0x28 u32 part count (2), +0x2c u32 the section's size, parts of 0x24 bytes from +0x30:
      {u32 size, u32 offset, u32 part kind, u32 1 for text/0 for binary, u32 stage (0 vertex,
      1 fragment), u32 0x00200416, 3 x u32 0}.
      Part kind 0, text, NUL-terminated: the stage's assembly listing (a cgc-style comment block
      with "#profile gp5vp"/"gp5fp", "#MSDB: Sloc <n> <file id>:<line>" source-location lines)
      followed by the source file table: per file, a line
      `#MSDB: (<name>:<file id>:<length>:SOURCE_FILE|INCLUDED_FILE)` and the file's text as one C
      string literal (escapes \\r \\n \\t \\\\ \\"), then a newline. The decoded length always
      equals <length>. The first record is the stage's own source (named `_shader<number>`, file
      id 13684 in every stage), the others the files it #includes (common_funcs.h, common_ibl.h,
      common_deferred.h, common_shadows.h, ...). Non-ASCII bytes (Latin-1, UTF-8, U+FFFD) occur
      in comments only; they are written unchanged.
      Part kind 1, binary (starts 0x0000c0de): not decoded.
    The file table is found by scanning the text part for its first line starting "#MSDB: (";
    this is the parser's only scan, and the records must then run exactly to the end of the text.

Extraction. A stage's own source begins with the engine's variant prefix, "#define HAS_NORMAL"
lines and a blank line, ahead of "#version 440 core" (or 450): the compiler accepted #version
after them. The written file is that source with each #include line replaced by the included
file's text (between `// bshdat.py: begin/end #include` comments), CRLF turned into LF, and the
first #version line moved to the top, so that the prefix follows it. Fifteen deferred-lighting
fragment sources take #version from an include (common_deferred.h), and common_ibl.h starts with
its own `#version 450`: later #version lines are kept as comments. Nothing else is changed.
Program files are named by variant key: <package>/p<key>.vert and .frag.

Compiling (--compile). As extracted the sources do not compile with glslang: uniform blocks are
declared `layout(location = N) uniform Block {`, which GLSL does not allow (NVN's compiler took
it, ignored N, and numbered the stage's active blocks 0, 1, 2, ... in declaration order; the
reflection has the result), and C-style casts need GL_NV_explicit_typecast. The `_shim` modes
apply the rewrites the port's loader would (shim_rewrite: each block gets the binding from the
reflection; the extension is enabled; FIXUPS; and in fragment stages the inactive inputs the vertex
stage does not write are dropped, drop_uncovered_inputs, which Vulkan's pipeline creation needs)
and keep the SPIR-V next to the source
(<program>.<stage>.<gl|vk>.spv). Each failure is classified by glslang's messages (CATEGORIES).

Installing (--install DIR, the port's build step; port/CMakeLists.txt runs it). Compiles every
stage in the vk_shim mode only, the SPIR-V to DIR/<package>/<program>.<stage>.spv, checks each
module against the NVN reflection (spirv_resources, stage_layout: every active block and sampler at
the binding the shim expects, only the Shader capability) and writes DIR/shaders.manifest, the
table the port's NVN shim loads (format in install()). Any stage that fails stops the install: no
manifest is written. One stage needs a source fix no general rewrite covers (FIXUPS: FXAA's
non-constant texel offset); it is applied in the shim modes and in --install.

Usage:
  python tools/bshdat.py --version 2.1.0 [--only NAME[,NAME...]] [--compile] [--jobs N]
  python tools/bshdat.py --version 2.1.0 --install DIR [--jobs N]     (every package; no --only)
  python tools/bshdat.py --version 2.1.0 --glslang-path               (the glslang it runs)
glslang: python tools/fetch_toolchains.py glslang-16.6.0 (Khronos, BSD-3-Clause and others;
fetched by name only), else glslang or glslangValidator from PATH. Standard library only.
Exit status 1 when a package fails to parse or has bytes the parser does not account for.
"""

from __future__ import annotations

import argparse
import collections
import concurrent.futures
import csv
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hostenv  # noqa: E402
from fetch_toolchains import GLSLANG_VERSION  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
FORMAT_VERSION = (1, 0x13)

PKG_MAGIC = b"MSHD"
BLOB_MAGIC = 0x19866891
REFL_MAGIC = 0x19292919
DEBUG_MAGIC = 0x65040891
STAGES = {1: "vert", 2: "frag"}  # program record slot -> stage
SECTION_KINDS = {0: "kind0", 1: "kind1", 3: "reflection", 4: "debug"}
LIST_LIMIT = 100_000


class FormatError(Exception):
    pass


def u32(d: bytes, o: int) -> int:
    return struct.unpack_from("<I", d, o)[0]


def u64(d: bytes, o: int) -> int:
    return struct.unpack_from("<Q", d, o)[0]


class Cover:
    """Byte ranges the parser has explained, to check that nothing in a package is left over."""

    def __init__(self, size: int):
        self.size = size
        self.spans: list[tuple[int, int, str]] = []

    def add(self, off: int, size: int, what: str) -> None:
        if off < 0 or size < 0 or off + size > self.size:
            raise FormatError(f"{what} at {off:#x}+{size:#x} runs past the end ({self.size:#x})")
        self.spans.append((off, size, what))

    def leftover(self, data: bytes) -> tuple[int, int]:
        """(bytes not explained and not 0xff, bytes explained twice)."""
        unknown = overlap = pos = 0
        for off, size, _ in sorted(self.spans):
            if off < pos:
                overlap += min(pos, off + size) - off
            else:
                gap = data[pos:off]
                unknown += len(gap) - gap.count(0xFF)
            pos = max(pos, off + size)
        tail = data[pos:]
        return unknown + len(tail) - tail.count(0xFF), overlap


def walk_list(d: bytes, head: int, cover: Cover, what: str) -> list[int]:
    items = []
    node = head
    while node:
        cover.add(node, 16, f"{what} node")
        items.append(u64(d, node))
        node = u64(d, node + 8)
        if len(items) > LIST_LIMIT:
            raise FormatError(f"{what}: list does not end")
    return items


def cstring(d: bytes, off: int) -> str:
    end = d.index(b"\0", off)
    return d[off:end].decode("ascii")


# --- stage blob ---------------------------------------------------------------------------------

RECORD = re.compile(rb'#MSDB: \(([^:()\n]*):(\d+):(\d+):(SOURCE_FILE|INCLUDED_FILE)\)"((?:[^"\\\n]|\\.)*)"\n')
ESCAPES = {b"n": b"\n", b"r": b"\r", b"t": b"\t", b"\\": b"\\", b'"': b'"'}


def unescape(esc: bytes) -> bytes:
    def one(m: re.Match) -> bytes:
        try:
            return ESCAPES[m.group(1)]
        except KeyError:
            raise FormatError(f"unknown escape \\{m.group(1).decode('latin-1')}") from None
    return re.sub(rb"\\(.)", one, esc, flags=re.S)


@dataclass
class SourceFile:
    name: str
    file_id: int
    kind: str  # SOURCE_FILE or INCLUDED_FILE
    text: bytes


@dataclass
class Stage:
    stage: str
    offset: int
    size: int
    sections: list[dict]
    reflection: dict
    debug_hash: str
    profile: str | None
    listing_summary: str | None
    files: list[SourceFile]

    @property
    def source(self) -> SourceFile:
        return self.files[0]


SECTION_RECORDS = 0x90   # first section record, in the blob
SECTION_STRIDE = 0x90
# Reflection tables, in the order of their (count, offset) pairs: name, record size.
REFL_TABLES = (("blocks", 0x6C), ("uniforms", 0xA4), ("inputs", 0x5C), ("outputs", 0x5C),
               ("table4", None), ("table5", None), ("table6", None))
STAGE_SLOT = {"vert": 0, "frag": 1}  # index into the reflection's per-stage arrays


def parse_reflection(b: bytes, rec: int, sec: dict, stage: str, where: str) -> dict:
    """The stage's interface as the compiler reports it (active blocks, uniforms, inputs, outputs).

    Section record payload (u32 words from the record start): 3-10 zero, 11-24 seven (count, offset)
    pairs (REFL_TABLES), 25 the name table's size, 26 its offset, 27-35 not decoded. Offsets are from
    the section start. Records begin {u32 name offset, u32 name length + 1}; per-stage arrays hold
    six u32 (vertex, fragment, then four slots that are -1 in every file).
      block, 0x6c:   +0x28 size in bytes, +0x2c member count, +0x30 stage mask (1 vertex, 2 fragment),
                     +0x34 per-stage binding
      uniform, 0xa4: +0x28 type code, +0x2c block index (-1: none), +0x30 offset in the block,
                     +0x34 array size, +0x38 array stride, +0x44 stage mask, +0x48 per-stage binding
                     (samplers)
      input/output, 0x5c: +0x28 type code, +0x2c array size, +0x30 location (-1: built-in)
    Type codes are the compiler's own enumeration (not decoded).
    """
    s = b[sec["offset"]:sec["offset"] + sec["size"]]
    w = struct.unpack_from("<36I", b, rec)
    if any(w[3:11]):
        raise FormatError(f"{where}: reflection record words 3-10 are not zero")
    str_size, str_off = w[25], w[26]
    names = s[str_off:str_off + str_size]

    def name(rec_off: int) -> str:
        n_off, n_len = u32(s, rec_off), u32(s, rec_off + 4)
        if n_off + n_len > len(names) or names[n_off + n_len - 1:n_off + n_len] != b"\0":
            raise FormatError(f"{where}: reflection name at {n_off:#x} malformed")
        return names[n_off:n_off + n_len - 1].decode("ascii")

    def i32(o: int) -> int:
        return struct.unpack_from("<i", s, o)[0]

    slot = STAGE_SLOT[stage]
    out: dict = {}
    tables = [(w[11 + 2 * i], w[12 + 2 * i]) for i in range(len(REFL_TABLES))]
    ends = [t[1] for t in tables[1:]] + [str_off]
    for (tname, stride), (count, t_off), end in zip(REFL_TABLES, tables, ends):
        if stride is None:
            if count:
                raise FormatError(f"{where}: reflection {tname} is not empty ({count})")
            continue
        if t_off + count * stride != end:
            raise FormatError(f"{where}: reflection {tname} table {t_off:#x}+{count}*{stride:#x} != {end:#x}")
        rows = []
        for i in range(count):
            r = t_off + stride * i
            if tname == "blocks":
                rows.append({"name": name(r), "size": u32(s, r + 0x28), "members": u32(s, r + 0x2C),
                             "stages": u32(s, r + 0x30), "binding": i32(r + 0x34 + 4 * slot)})
            elif tname == "uniforms":
                rows.append({"name": name(r), "type": u32(s, r + 0x28), "block": i32(r + 0x2C),
                             "offset": i32(r + 0x30), "array": u32(s, r + 0x34), "stages": u32(s, r + 0x44),
                             "binding": i32(r + 0x48 + 4 * slot)})
            else:
                rows.append({"name": name(r), "type": u32(s, r + 0x28), "array": u32(s, r + 0x2C),
                             "location": i32(r + 0x30)})
        out[tname] = rows
    return out


def parse_blob(d: bytes, off: int, size: int, stage: str) -> Stage:
    b = d[off:off + size]
    where = f"stage blob at {off:#x}"
    if len(b) < SECTION_RECORDS or u32(b, 0) != BLOB_MAGIC:
        raise FormatError(f"{where}: bad magic")
    if u32(b, 0x44) != size:
        raise FormatError(f"{where}: size field {u32(b, 0x44):#x} != {size:#x}")
    count = u32(b, 0x4C)
    sections = []
    pos = u32(b, 0x48)
    for i in range(count):
        rec = SECTION_RECORDS + SECTION_STRIDE * i
        s_size, s_off, kind = u32(b, rec), u32(b, rec + 4), u32(b, rec + 8)
        if s_off < pos or s_off - pos >= 8 or s_off + s_size > size:
            raise FormatError(f"{where}: section {i} at {s_off:#x}+{s_size:#x} out of order")
        pos = s_off + s_size
        sections.append({"kind": kind, "name": SECTION_KINDS.get(kind, f"kind{kind}"),
                         "offset": s_off, "size": s_size, "record": rec})
    if size - pos >= 8:
        raise FormatError(f"{where}: {size - pos} bytes after the last section")
    debug = [s for s in sections if s["kind"] == 4]
    refl = [s for s in sections if s["kind"] == 3]
    if len(debug) != 1 or len(refl) != 1:
        raise FormatError(f"{where}: expected one debug and one reflection section")
    if u32(b, refl[0]["offset"]) != REFL_MAGIC:
        raise FormatError(f"{where}: reflection section magic")
    reflection = parse_reflection(b, refl[0]["record"], refl[0], stage, where)
    for sec in sections:
        del sec["record"]
    s = b[debug[0]["offset"]:debug[0]["offset"] + debug[0]["size"]]
    if u32(s, 0) != DEBUG_MAGIC or u32(s, 0x2C) != len(s):
        raise FormatError(f"{where}: debug section header")
    parts = [struct.unpack_from("<9I", s, 0x30 + 0x24 * j) for j in range(u32(s, 0x28))]
    text_parts = [p for p in parts if p[2] == 0]
    if len(text_parts) != 1:
        raise FormatError(f"{where}: expected one text part, found {len(text_parts)}")
    p_size, p_off = text_parts[0][:2]
    if p_off + p_size > len(s):
        raise FormatError(f"{where}: text part at {p_off:#x}+{p_size:#x} runs past the debug section")
    text = s[p_off:p_off + p_size]
    if not text.endswith(b"\0") or b"\0" in text[:-1]:
        raise FormatError(f"{where}: text part is not one NUL-terminated string")
    text = text[:-1]
    start = text.find(b"\n#MSDB: (") + 1
    if start == 0:
        raise FormatError(f"{where}: no source file table")
    files = []
    while start < len(text):
        m = RECORD.match(text, start)
        if not m:
            raise FormatError(f"{where}: source table record at {start:#x} not understood")
        raw = unescape(m.group(5))
        if len(raw) != int(m.group(3)):
            raise FormatError(f"{where}: {m.group(1)!r} decodes to {len(raw)} bytes, "
                              f"record says {m.group(3).decode()}")
        files.append(SourceFile(m.group(1).decode("ascii"), int(m.group(2)), m.group(4).decode(), raw))
        start = m.end()
    if not files or files[0].kind != "SOURCE_FILE" or any(f.kind != "INCLUDED_FILE" for f in files[1:]):
        raise FormatError(f"{where}: expected one SOURCE_FILE then INCLUDED_FILEs")
    listing = text[:text.find(b"\n#MSDB: (") + 1]
    prof = re.search(rb"^#profile (\S+)", listing, re.M)
    summ = re.search(rb"^# (\d+ instructions[^\n]*)", listing, re.M)
    return Stage(stage, off, size, sections, reflection, s[0x18:0x20][::-1].hex(),
                 prof.group(1).decode() if prof else None,
                 summ.group(1).decode() if summ else None, files)


# --- package ------------------------------------------------------------------------------------

@dataclass
class Program:
    index: int
    record: int
    key: tuple[int, int] | None = None
    stages: dict[str, Stage] = field(default_factory=dict)


@dataclass
class Package:
    name: str
    size: int
    version: tuple[int, int]
    programs: list[Program]
    last_program: int
    uniforms1: list[tuple[str, int]]
    uniforms2: list[tuple[str, int]]
    attributes: list[tuple[str, int]]
    unaccounted: int


def parse_package(path: Path) -> Package:
    d = path.read_bytes()
    if d[:4] != PKG_MAGIC:
        raise FormatError(f"{path.name}: magic {d[:4]!r}")
    cover = Cover(len(d))
    cover.add(0, 0x40, "header")
    version = struct.unpack_from("<HH", d, 4)
    if version != FORMAT_VERSION:
        raise FormatError(f"{path.name}: format version {version}, expected {FORMAT_VERSION}")
    h = [u64(d, 8 + 8 * i) for i in range(7)]
    if h[3]:
        raise FormatError(f"{path.name}: header +0x20 is {h[3]:#x}, 0 in every known package")
    progs = [Program(i, rec) for i, rec in enumerate(walk_list(d, h[0], cover, "program list"))]
    by_record = {p.record: p for p in progs}
    if len(by_record) != len(progs):
        raise FormatError(f"{path.name}: a program record is listed twice")
    for p in progs:
        cover.add(p.record, 0x30, "program record")
        for slot in range(3):
            size, pad, off = struct.unpack_from("<IIQ", d, p.record + 16 * slot)
            if slot not in STAGES:
                if size or off:
                    raise FormatError(f"{path.name}: program {p.index} uses stage slot {slot}")
                continue
            if not size:
                raise FormatError(f"{path.name}: program {p.index} has no {STAGES[slot]} stage")
            cover.add(off, size, "stage blob")
            p.stages[STAGES[slot]] = parse_blob(d, off, size, STAGES[slot])
    for entry in walk_list(d, h[1], cover, "variant map"):
        cover.add(entry, 16, "variant map entry")
        k0, k1, rec = struct.unpack_from("<IIQ", d, entry)
        if rec not in by_record or by_record[rec].key is not None:
            raise FormatError(f"{path.name}: variant map entry {entry:#x} names no single program")
        by_record[rec].key = (k0, k1)
    if any(p.key is None for p in progs):
        raise FormatError(f"{path.name}: a program has no variant key")
    if h[2] not in by_record:
        raise FormatError(f"{path.name}: header +0x18 does not name a program")

    def names(head: int, what: str) -> list[tuple[str, int]]:
        out = []
        for item in walk_list(d, head, cover, what):
            cover.add(item, 12, f"{what} entry")
            name_off = u64(d, item)
            name = cstring(d, name_off)
            cover.add(name_off, len(name) + 1, f"{what} name")
            out.append((name, u32(d, item + 8)))
        return out

    uniforms1 = names(h[4], "uniform list 1")
    uniforms2 = names(h[5], "uniform list 2")
    attributes = names(h[6], "attribute list")
    unknown, overlap = cover.leftover(d)
    if overlap:
        raise FormatError(f"{path.name}: {overlap} bytes claimed twice")
    return Package(path.stem, len(d), version, progs, by_record[h[2]].index,
                   uniforms1, uniforms2, attributes, unknown)


# --- GLSL ---------------------------------------------------------------------------------------

INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"[^\n]*$', re.M)
VERSION = re.compile(r"^[ \t]*#[ \t]*version\b[^\n]*$", re.M)
EXTENSION = re.compile(r"^[ \t]*#[ \t]*extension[ \t]+(\w+)[ \t]*:[ \t]*(\w+)", re.M)


def decode(text: bytes) -> str:
    return text.decode("utf-8", "surrogateescape").replace("\r\n", "\n")


def flatten(files: dict[str, str], name: str, depth: int = 0) -> str:
    if depth > 16:
        raise FormatError(f"#include nesting too deep at {name}")

    def inline(m: re.Match) -> str:
        inc = m.group(1)
        if inc not in files:
            raise FormatError(f"#include \"{inc}\" has no source record")
        body = flatten(files, inc, depth + 1).rstrip("\n")
        return f'// bshdat.py: begin #include "{inc}"\n{body}\n// bshdat.py: end #include "{inc}"'

    return INCLUDE.sub(inline, files[name])


def prefix_defines(source: str) -> list[str]:
    """The engine's variant prefix: the #define lines before the first blank line, ahead of #version."""
    out = []
    for line in source.split("\n"):
        if not line.strip():
            break
        if not line.startswith("#define"):
            return []
        out.append(line[len("#define"):].strip())
    return out


def glsl_for(stage: Stage) -> tuple[str, dict]:
    files = {f.name: decode(f.text) for f in stage.files}
    main = stage.source.name
    flat = flatten(files, main)
    versions = list(VERSION.finditer(flat))
    if not versions:
        raise FormatError(f"{main}: no #version")
    first = versions[0]
    head, tail = flat[:first.start()], flat[first.end():]
    tail = VERSION.sub(lambda m: f"// bshdat.py: repeated {m.group(0).strip()}", tail)
    # "<prefix>#version X\n<rest>" -> "#version X\n<prefix><rest>": line count unchanged.
    text = first.group(0).strip() + "\n" + head + (tail[1:] if tail.startswith("\n") else tail)
    if not text.endswith("\n"):
        text += "\n"
    info = {
        "version": re.match(r"[ \t]*#[ \t]*version[ \t]*(.*)", first.group(0)).group(1).strip(),
        "version_from": next((f.name for f in stage.files if VERSION.search(decode(f.text))), main),
        "defines": prefix_defines(files[main]),
        "extensions": sorted({f"{m.group(1)}:{m.group(2)}" for m in EXTENSION.finditer(flat)}),
        "repeated_version_lines": len(versions) - 1,
    }
    return text, info


BLOCK_DECL = re.compile(r"layout\s*\(([^)]*)\)(\s*uniform\s+)(\w+)(\s*\{)")
SAMPLER_DECL = re.compile(r"layout\s*\(([^)]*)\)\s*uniform\s+(?:\w+\s+)*?(?:[iu]?sampler\w*|[iu]?image\w*)\s+(\w+)")
QUALIFIER = re.compile(r"\b(location|binding)\s*=\s*(\d+)")
COMMENTS = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


def binding_check(text: str, refl: dict) -> dict:
    """Compare the bindings the compiler assigned (reflection) with the source.

    Uniform blocks are declared `layout(location = N) uniform Block {...}`, a few with binding = N.
    The compiler ignored those numbers: it gave the stage's active blocks the bindings 0, 1, 2, ...
    in declaration order (`by_order`), which equals N only when every block declared before is
    active (`same`). Samplers keep their `layout(binding = N)`.
    """
    code = COMMENTS.sub("", text)
    declared: dict[str, set] = collections.defaultdict(set)
    order: list[str] = []
    for m in BLOCK_DECL.finditer(code):
        declared[m.group(3)].update((q, int(n)) for q, n in QUALIFIER.findall(m.group(1)))
        if m.group(3) not in order:
            order.append(m.group(3))
    for m in SAMPLER_DECL.finditer(code):
        declared[m.group(2)].update((q, int(n)) for q, n in QUALIFIER.findall(m.group(1)))
    active_blocks = {b["name"]: b["binding"] for b in refl.get("blocks", []) if b["binding"] >= 0}
    rank = {name: i for i, name in enumerate(n for n in order if n in active_blocks)}
    out = {}
    for kind, rows in (("blocks", refl.get("blocks", [])),
                       ("samplers", [u for u in refl.get("uniforms", []) if u["block"] == -1])):
        rows = [r for r in rows if r["binding"] >= 0]
        same, differ, undeclared = 0, [], []
        for r in rows:
            decl = declared.get(r["name"].split("[", 1)[0])
            if not decl:
                undeclared.append(r["name"])
            elif any(n == r["binding"] for _, n in decl):
                same += 1
            else:
                differ.append(f"{r['name']}: {' '.join(f'{q}={n}' for q, n in sorted(decl))} -> binding {r['binding']}")
        out[kind] = {"active": len(rows), "same": same, "differ": differ, "undeclared": undeclared}
    out["blocks"]["by_order"] = sum(1 for n, b in active_blocks.items() if rank.get(n) == b)
    return out


# --- extraction ---------------------------------------------------------------------------------

def package_paths(version: str, only: list[str] | None) -> list[Path]:
    shd = ROOT / "orig" / version / "romfs" / "system" / "shd"
    paths = sorted(shd.glob("*.bshdat"))
    if not paths:
        raise SystemExit(f"no .bshdat files under {shd} (run tools/setup.py first)")
    if only:
        wanted = {o.removesuffix(".bshdat") for o in only}
        paths = [p for p in paths if p.stem in wanted]
        missing = wanted - {p.stem for p in paths}
        if missing:
            raise SystemExit(f"no such package: {', '.join(sorted(missing))}")
    return paths


def program_name(p: Program) -> str:
    return f"p{p.key[0]}"


def extract(path: Path, out_root: Path) -> dict:
    pkg = parse_package(path)
    out = out_root / pkg.name
    if out.exists():
        shutil.rmtree(out)
    out.mkdir(parents=True)
    programs = []
    for p in pkg.programs:
        entry = {"name": program_name(p), "index": p.index, "key": list(p.key),
                 "record_offset": p.record, "stages": {}}
        defines = None
        for st_name, st in p.stages.items():
            text, info = glsl_for(st)
            fname = f"{program_name(p)}.{st_name}"
            (out / fname).write_bytes(text.encode("utf-8", "surrogateescape"))
            if defines is not None and info["defines"] != defines:
                raise FormatError(f"{pkg.name}: program {p.index}'s stages have different variant prefixes")
            defines = info["defines"]
            entry["stages"][st_name] = {
                "file": fname,
                "blob_offset": st.offset,
                "blob_size": st.size,
                "sections": st.sections,
                "debug_hash": st.debug_hash,
                "profile": st.profile,
                "listing_summary": st.listing_summary,
                "source": {"name": st.source.name, "file_id": st.source.file_id, "length": len(st.source.text)},
                "includes": [{"name": f.name, "file_id": f.file_id, "length": len(f.text)} for f in st.files[1:]],
                **info,
                "binding_check": binding_check(text, st.reflection),
                "reflection": st.reflection,
            }
        entry["defines"] = defines
        programs.append(entry)
    manifest = {
        "package": pkg.name,
        "file_size": pkg.size,
        "format_version": list(pkg.version),
        "unaccounted_bytes": pkg.unaccounted,
        "last_program_index": pkg.last_program,
        "programs": programs,
        "uniforms1": [{"name": n, "id": i} for n, i in pkg.uniforms1],
        "uniforms2": [{"name": n, "id": i} for n, i in pkg.uniforms2],
        "attributes": [{"name": n, "id": i} for n, i in pkg.attributes],
    }
    (out / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n", newline="\n")
    return manifest


# --- compile ------------------------------------------------------------------------------------

def glslang_path() -> str:
    bin_dir = hostenv.cache_dir() / "tools" / f"glslang-{GLSLANG_VERSION}" / "bin"
    for name in ("glslang", "glslangValidator"):  # releases since 2023 name it glslang
        if (bin_dir / hostenv.exe(name)).is_file():
            return str(bin_dir / hostenv.exe(name))
    for name in ("glslang", "glslangValidator"):
        if shutil.which(name):
            return shutil.which(name)
    raise SystemExit(f"glslang not found: run python tools/fetch_toolchains.py glslang-{GLSLANG_VERSION}")


def glslang_version(tool: str) -> str:
    """What the binary reports (`Glslang Version: 11:16.6.0`), whichever copy was found."""
    proc = subprocess.run([tool, "--version"], capture_output=True, text=True, timeout=60)
    first = (proc.stdout or proc.stderr).strip().splitlines()
    return first[0].split(":", 1)[-1].strip() if first else "unknown version"


# The two rewrites a loader in the port's NVN shim makes (applied in memory, never to the files):
#  - uniform blocks are declared `layout(location = N) uniform Block {`, which GLSL does not allow;
#    each block gets `binding = B`, B being the binding the NVN compiler gave it (the stage's
#    reflection; binding_check). Blocks the compiler found inactive get numbers above those.
#  - C-style casts (`(int) x`) need GL_NV_explicit_typecast, which glslang implements.
SHIM_TYPECAST = "#extension GL_NV_explicit_typecast : enable"

# Source fixes for single stages that no general rewrite covers, applied with the shim rewrites
# ("fixups"), keyed by (package, stage): each is a regular expression over the extracted text and
# its replacement, and must match exactly once, so a dump whose source differs fails loudly
# instead of compiling something else. The patterns describe the construct, not the shipped text.
#  - pp_fxaa_nx, fragment: FXAA 3.11's FxaaTexOff(tex, pos, off, rcpFrame) passes its parameter
#    `off` to textureLodOffset, whose offset GLSL requires to be a constant expression (NVIDIA's
#    compiler inlined the function and folded it). Every call passes a literal FxaaInt2(x, y) and
#    the source selects FXAA_GLSL_130 (any other branch fails to compile rather than miscompiles),
#    so the function becomes a macro: the textureLodOffset call in its body samples without the
#    offset (the function is no longer called; it only has to compile), the definition is renamed
#    out of the way, and `#define FxaaTexOff(...)` ahead of it puts each call's literal offset
#    straight into textureLodOffset. Same texels as the original, no rounding.
FIXUPS: dict[tuple[str, str], tuple[tuple[str, str], ...]] = {
    ("pp_fxaa_nx", "frag"): (
        (r"\btextureLodOffset\((\w+),\s*(\w+)\.xy,\s*0\.0,\s*\w+\.xy\)",
         r"textureLod(\1, \2.xy, 0.0) /* bshdat.py: FxaaTexOff is a macro */"),
        (r"^([ \t]*)(\w+[ \t]+)FxaaTexOff([ \t]*\()",
         r"\1#define FxaaTexOff(t, p, o, r) textureLodOffset(t, (p).xy, 0.0, o)\n\1\2FxaaTexOff_bshdat_unused\3"),
    ),
}


def apply_fixups(text: str, fixups: tuple[tuple[str, str], ...], where: str) -> str:
    for pattern, repl in fixups:
        text, n = re.subn(pattern, repl, text, flags=re.M)
        if n != 1:
            raise FormatError(f"{where}: fixup /{pattern}/ matched {n} times, expected once")
    return text


# A fragment stage's input declaration, `layout(location = N) [qualifiers] in <type> <name>[...];`.
# Interpolation and precision qualifiers may come before or after `in`; `component = M` is allowed
# in the layout. A declaration of another shape (several declarators, an `in` block) is left alone:
# install()'s interface check (spirv_interface) then reports any input it should have dropped.
_QUALS = r"(?:(?:flat|smooth|noperspective|centroid|sample|highp|mediump|lowp)\s+)*"
INPUT_DECL = re.compile(r"^([ \t]*)layout\s*\(\s*location\s*=\s*(\d+)\s*(?:,\s*component\s*=\s*\d+\s*)?\)\s*"
                        + _QUALS + r"in\s+" + _QUALS + r"(\w+)\s+(\w+)\s*(\[[^\]]*\])?\s*;",
                        re.M)
# NVN reflection type codes of float vectors: 36 float, 37 vec2, 38 vec3, 39 vec4 (inferred from the
# declarations they belong to: float2 ps_tx0 is 37, float3 ps_pos 38, gl_Position 39). Other codes
# are not decoded and count as covering any input.
FLOAT_COMPONENTS = {36: 1, 37: 2, 38: 3, 39: 4}


def declared_components(type_name: str) -> int:
    """1-4 for a GLSL scalar or vector type name (the engine's float2-style names included), else 0."""
    m = re.fullmatch(r"(?:float|half|int|uint|bool|double|[biud]?vec|vec)([234]?)", type_name)
    if not m:
        return 0
    return int(m.group(1)) if m.group(1) else (1 if not type_name.endswith("vec") else 0)


def drop_uncovered_inputs(text: str, active: set[str], vertex_outputs: dict[int, int]) -> str:
    """Comment out each fragment input NVN's compiler found inactive and the vertex stage does not
    cover (no output at its location, or one with fewer components).

    GL and NVN accept such an input as long as nothing reads it; Vulkan rejects the pipeline
    (VUID-RuntimeSpirv-OpEntryPoint-08743), and glslang keeps every declared input in the SPIR-V.
    In 2.1.0 these are the deferred-lighting and ambient programs' interpolants (found by
    port/nvn/check/vulkan_check.cpp). Inactive inputs the vertex stage does cover stay: many are
    still named in code the compiler removed later. An input dropped and still named would fail to
    compile rather than miscompile. vertex_outputs: location -> components (FLOAT_COMPONENTS; 4 when
    the type code is not decoded).
    """
    def drop(m: re.Match) -> str:
        name, loc = m.group(4), int(m.group(2))
        need = declared_components(m.group(3)) or 4
        covered = loc in vertex_outputs and vertex_outputs[loc] >= need and not m.group(5)
        if name in active or covered:
            return m.group(0)
        return f"{m.group(1)}// bshdat.py: inactive input {name} (location {loc}) dropped: the vertex stage does not write it"

    return INPUT_DECL.sub(drop, text)


def shim_rewrite(text: str, blocks: dict[str, int], rewrites: tuple[str, ...],
                 fixups: tuple[tuple[str, str], ...] = (), where: str = "",
                 inputs: tuple[set[str], dict[int, int]] | None = None) -> str:
    """Apply the named rewrites ("blocks", "typecast", "fixups", "inputs"); "typecast" adds one line
    after #version, and the FXAA fixup one line before the function it replaces. "inputs" applies
    to fragment stages only (inputs: the reflection's input names and the vertex stage's outputs,
    input_rule; None for a vertex stage)."""
    if "fixups" in rewrites and fixups:
        text = apply_fixups(text, fixups, where)
    if "inputs" in rewrites and inputs is not None:
        text = drop_uncovered_inputs(text, *inputs)
    if "blocks" in rewrites:
        spare = iter(range(max(blocks.values(), default=-1) + 1, 1 << 16))
        assigned: dict[str, int] = {}

        def rebind(m: re.Match) -> str:
            name = m.group(3)
            binding = blocks.get(name, -1)
            if binding < 0:
                binding = assigned.setdefault(name, next(spare))
            quals = [q.strip() for q in m.group(1).split(",") if q.strip() and not QUALIFIER.search(q)]
            return f"layout({', '.join([*quals, f'binding = {binding}'])}){m.group(2)}{name}{m.group(4)}"

        text = BLOCK_DECL.sub(rebind, text)
    if "typecast" in rewrites:
        first, _, rest = text.partition("\n")
        text = f"{first}\n{SHIM_TYPECAST}\n{rest}"
    return text


# Modes: glslang flags and the shim rewrites applied. Plain -G (OpenGL SPIR-V) and -V (Vulkan
# SPIR-V) compile the rewritten sources (no uniform outside a block, every block bound, every input
# and output with a location). gl_blocks shows what remains after the block rewrite alone.
# NVN numbers uniform blocks and textures per stage and separately; Vulkan has one (set, binding)
# space per pipeline, so vk_shim gives each stage its own descriptor set and moves the combined
# samplers to bindings 16 and up (blocks stay at the compiler's 0..n-1). With plain -V, 664 of the
# 670 compiled programs put two differently named resources (block type or sampler names) on one
# (set, binding): 583 within one stage (a sampler and a block), 81 only across the two stages.
VK_LAYOUT = ["--resource-set-binding", "vert", "0", "--resource-set-binding", "frag", "1",
             "--shift-combined-sampler-binding", "16"]
MODES = {
    "gl": (["-G"], ()),
    "vk": (["-V"], ()),
    "gl_blocks": (["-G"], ("blocks",)),
    "gl_shim": (["-G"], ("blocks", "typecast", "fixups", "inputs")),
    "vk_shim": (["-V", *VK_LAYOUT], ("blocks", "typecast", "fixups", "inputs")),
}

# Failure categories by glslang message (every error of a stage is classified).
CATEGORIES = [
    ("block_location", re.compile(r"'location' : cannot apply to uniform or buffer block")),
    ("block_binding", re.compile(r"blocks require layout\(binding")),
    ("loose_uniform", re.compile(r"non-opaque uniforms outside a block")),
    ("explicit_typecast", re.compile(r"GL_NV_explicit_typecast")),
    ("nonconstant_texel_offset", re.compile(r"'texel offset' : argument must be compile-time constant")),
    ("bindless_handle", re.compile(r"uint64|bindless|sampler.*constructor", re.I)),
    ("undeclared_identifier", re.compile(r"undeclared identifier")),
    ("no_matching_overload", re.compile(r"no matching overloaded function")),
    ("io", re.compile(r"(?:cannot|could not|unable to|failed to) (?:open|write|create)", re.I)),
]
# A FIXUPS entry that no longer applies is reported as category "fixup" (compile_one).
GLSLANG_TIMEOUT = 300  # seconds per compile; the largest stage takes well under one
# Errors that only follow another one (glslang stops, or an #if was left open by the stop).
FOLLOW_ON = re.compile(r"compilation terminated|missing #endif|\d+ compilation errors|^Linking")
ERROR_LINE = re.compile(r"^ERROR: (?:.*?:\d+: )?(.*?)\s*$", re.M)


def categorise(log: str) -> tuple[list[str], str]:
    errors = [m.group(1) for m in ERROR_LINE.finditer(log) if not FOLLOW_ON.search(m.group(1))]
    if not errors:
        return ["other"], log.strip().splitlines()[-1] if log.strip() else "no output"
    cats = []
    for e in errors:
        cat = next((name for name, rx in CATEGORIES if rx.search(e)), "other")
        if cat not in cats:
            cats.append(cat)
    return cats, errors[0]


def input_rule(stage: str, prog: dict) -> tuple[set[str], dict[int, int]] | None:
    """For a fragment stage, its active inputs by name and the vertex stage's outputs (location ->
    components) from the two reflections (the "inputs" rewrite); None for a vertex stage."""
    if stage != "frag":
        return None
    active = {r["name"].split("[", 1)[0] for r in prog["stages"]["frag"]["reflection"]["inputs"]}
    outputs = {r["location"]: FLOAT_COMPONENTS.get(r["type"], 4) for r in prog["stages"]["vert"]["reflection"]["outputs"]
               if r["location"] >= 0 and r["array"] == 1}
    return active, outputs


def compile_one(tool: str, src: Path, stage: str, blocks: dict[str, int], mode: str,
                fixups: tuple[tuple[str, str], ...] = (), dest: Path | None = None,
                inputs: tuple[set[str], dict[int, int]] | None = None) -> tuple[bool, list[str], str]:
    flags, rewrites = MODES[mode]
    try:
        text = shim_rewrite(src.read_bytes().decode("utf-8", "surrogateescape"), blocks, rewrites,
                            fixups, f"{src.parent.name}/{src.name}", inputs)
    except FormatError as e:
        return False, ["fixup"], str(e)
    # SPIR-V is kept for the full shim modes only (<program>.<stage>.<gl|vk>.spv, or `dest`). glslang
    # writes it to a short temporary path (a long checkout path can pass Windows' MAX_PATH, which
    # glslang does not handle); Python moves it into place.
    keep = mode.endswith("_shim")
    dest = dest or src.with_name(f"{src.name}.{mode.split('_')[0]}.spv")
    tmp = os.devnull
    if keep:
        dest.unlink(missing_ok=True)
        fd, tmp = tempfile.mkstemp(suffix=".spv")
        os.close(fd)
    try:
        proc = subprocess.run([tool, *flags, "--stdin", "-S", stage, "-o", tmp],
                              input=text.encode("utf-8", "surrogateescape"), capture_output=True,
                              timeout=GLSLANG_TIMEOUT)
        if proc.returncode == 0:
            if keep:
                shutil.move(tmp, dest)
            return True, [], ""
        cats, first = categorise((proc.stdout + proc.stderr).decode("utf-8", "replace"))
        return False, cats, first
    except subprocess.TimeoutExpired:
        return False, ["timeout"], f"glslang did not finish in {GLSLANG_TIMEOUT} s"
    except OSError as e:
        return False, ["io"], str(e)
    finally:
        if keep and os.path.exists(tmp):
            os.unlink(tmp)


def compile_all(out_root: Path, manifests: list[dict], jobs: int) -> None:
    tool = glslang_path()
    tasks = []
    for man in manifests:
        for prog in man["programs"]:
            for st_name, st in prog["stages"].items():
                blocks = {b["name"]: b["binding"] for b in st["reflection"]["blocks"] if b["binding"] >= 0}
                tasks.append((man["package"], prog["name"], st_name, out_root / man["package"] / st["file"], blocks,
                              input_rule(st_name, prog)))
    results: dict[tuple, dict] = {}
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as pool:
        futs = {pool.submit(compile_one, tool, t[3], t[2], t[4], mode, FIXUPS.get((t[0], t[2]), ()), None, t[5]): (t, mode)
                for t in tasks for mode in MODES}
        for fut in concurrent.futures.as_completed(futs):
            t, mode = futs[fut]
            results.setdefault(t[:3], {})[mode] = fut.result()
    report = out_root / "compile_report.csv"
    with report.open("w", newline="", encoding="utf-8") as fh:
        w = csv.writer(fh, lineterminator="\n")
        w.writerow(["package", "program", "stage",
                    *[c for m in MODES for c in (f"{m}_ok", f"{m}_categories", f"{m}_first_error")]])
        for key in sorted(results):
            row = list(key)
            for mode in MODES:
                ok, cats, first = results[key][mode]
                row += [int(ok), ";".join(cats), first[:300]]
            w.writerow(row)
    print(f"glslang {glslang_version(tool)} ({tool}): {len(tasks)} stage sources; modes " +
          ", ".join(f"{m} ({' '.join([*f, *(f'+{r}' for r in rw)])})" for m, (f, rw) in MODES.items()))
    summarise(results)
    print(f"report: {report.relative_to(ROOT).as_posix()}")


def summarise(results: dict) -> None:
    programs = collections.defaultdict(dict)
    for (pkg, prog, stage), modes in results.items():
        programs[(pkg, prog)][stage] = modes
    for mode in MODES:
        stage_ok = sum(1 for r in results.values() if r[mode][0])
        prog_ok = sum(1 for st in programs.values() if all(m[mode][0] for m in st.values()))
        print(f"{mode:9} {stage_ok:5}/{len(results)} stages, {prog_ok:4}/{len(programs)} programs compile")
        by_cat = collections.defaultdict(set)
        stages_by_cat = collections.Counter()
        for (pkg, prog, stage), r in results.items():
            for cat in r[mode][1]:
                by_cat[cat].add((pkg, prog))
                stages_by_cat[cat] += 1
        for cat, progs in sorted(by_cat.items(), key=lambda kv: -len(kv[1])):
            print(f"         {cat:<26} {len(progs):4} programs {stages_by_cat[cat]:5} stages")
    for mode in ("gl", "vk"):
        either = sum(1 for r in results.values() if r[mode][0] or r[f"{mode}_shim"][0])
        print(f"{mode} as extracted or with the shim rewrites: {either}/{len(results)} stages")


# --- install (the port's build step) ------------------------------------------------------------

# SPIR-V, as far as the install check reads it: the resource variables of a module and their
# (set, binding). Opcodes and enumerants from the SPIR-V specification (Khronos, unified 1.6).
SPV_MAGIC = 0x07230203
OP_NAME, OP_DECORATE, OP_MEMBER_DECORATE, OP_CAPABILITY = 5, 71, 72, 17
OP_TYPE_BOOL, OP_TYPE_INT, OP_TYPE_FLOAT, OP_TYPE_VECTOR, OP_TYPE_MATRIX = 20, 21, 22, 23, 24
OP_TYPE_IMAGE, OP_TYPE_SAMPLED_IMAGE, OP_TYPE_ARRAY, OP_TYPE_STRUCT, OP_TYPE_POINTER = 25, 27, 28, 30, 32
OP_CONSTANT, OP_VARIABLE = 43, 59
DEC_BLOCK, DEC_ROW_MAJOR, DEC_ARRAY_STRIDE, DEC_MATRIX_STRIDE, DEC_BINDING, DEC_SET, DEC_OFFSET = 2, 4, 6, 7, 33, 34, 35
SC_UNIFORM_CONSTANT, SC_UNIFORM = 0, 2
CAP_SHADER = 1


def spirv_resources(data: bytes, where: str) -> tuple[list[dict], list[int]]:
    """The module's descriptor-bound variables, [{set, binding, kind, count, type, name, size}], and
    its capabilities. kind is "block" (a Uniform struct decorated Block; size: the bytes its explicit
    layout spans) or "sampler" (a combined image-sampler, or an array of them; size 0); anything
    else bound to a descriptor raises FormatError, and so does an array whose length is not a
    32-bit constant."""
    if len(data) < 20 or len(data) % 4 or u32(data, 0) != SPV_MAGIC:
        raise FormatError(f"{where}: not a SPIR-V module")
    words = struct.unpack_from(f"<{len(data) // 4}I", data)
    names: dict[int, str] = {}
    decos: dict[int, dict[int, int]] = collections.defaultdict(dict)
    member_decos: dict[tuple[int, int], dict[int, int]] = collections.defaultdict(dict)
    types: dict[int, tuple] = {}
    consts: dict[int, int] = {}
    variables: list[tuple[int, int, int]] = []  # (pointer type, id, storage class)
    caps: list[int] = []
    i = 5
    while i < len(words):
        count, op = words[i] >> 16, words[i] & 0xFFFF
        if count == 0 or i + count > len(words):
            raise FormatError(f"{where}: malformed instruction at word {i}")
        args = words[i + 1:i + count]
        if op == OP_NAME:
            names[args[0]] = struct.pack(f"<{len(args) - 1}I", *args[1:]).split(b"\0", 1)[0].decode("ascii", "replace")
        elif op == OP_DECORATE:
            decos[args[0]][args[1]] = args[2] if len(args) > 2 else 1
        elif op == OP_MEMBER_DECORATE:
            member_decos[(args[0], args[1])][args[2]] = args[3] if len(args) > 3 else 1
        elif op == OP_CAPABILITY:
            caps.append(args[0])
        elif op in (OP_TYPE_IMAGE, OP_TYPE_SAMPLED_IMAGE, OP_TYPE_ARRAY, OP_TYPE_STRUCT, OP_TYPE_POINTER,
                    OP_TYPE_BOOL, OP_TYPE_INT, OP_TYPE_FLOAT, OP_TYPE_VECTOR, OP_TYPE_MATRIX):
            types[args[0]] = (op, *args[1:])
        elif op == OP_CONSTANT and len(args) == 3:
            consts[args[1]] = args[2]
        elif op == OP_VARIABLE:
            variables.append((args[0], args[1], args[2]))
        i += count
    def size_of(tid: int, matrix_stride: int = 0, row_major: bool = False, depth: int = 0) -> int:
        t = types.get(tid, (None,))
        if depth > 32:
            raise FormatError(f"{where}: type nesting too deep")
        if t[0] == OP_TYPE_BOOL:
            return 4
        if t[0] in (OP_TYPE_INT, OP_TYPE_FLOAT):
            return t[1] // 8
        if t[0] == OP_TYPE_VECTOR:
            return t[2] * size_of(t[1], depth=depth + 1)
        if t[0] == OP_TYPE_MATRIX:  # (column type, column count)
            column = types.get(t[1], (None, None, 0))
            if not matrix_stride:
                return t[2] * size_of(t[1], depth=depth + 1)
            return (column[2] if row_major else t[2]) * matrix_stride
        if t[0] == OP_TYPE_ARRAY:
            if t[2] not in consts or DEC_ARRAY_STRIDE not in decos.get(tid, {}):
                raise FormatError(f"{where}: array type {tid} without a constant length or a stride")
            return consts[t[2]] * decos[tid][DEC_ARRAY_STRIDE]
        if t[0] == OP_TYPE_STRUCT:
            end = 0
            for m, mt in enumerate(t[1:]):
                md = member_decos.get((tid, m), {})
                end = max(end, md.get(DEC_OFFSET, 0) + size_of(mt, md.get(DEC_MATRIX_STRIDE, 0),
                                                              DEC_ROW_MAJOR in md, depth + 1))
            return end
        raise FormatError(f"{where}: type {tid} in a uniform block has no known size")

    out = []
    for ptr, vid, sc in variables:
        d = decos.get(vid, {})
        if DEC_BINDING not in d and DEC_SET not in d:
            continue
        pointee = types.get(ptr, (None, None, None))[2]
        n = 1
        t = types.get(pointee, (None,))
        if t[0] == OP_TYPE_ARRAY:
            n = consts.get(t[2], 0)
            if n < 1:
                raise FormatError(f"{where}: variable {names.get(vid, vid)} is an array without a constant length")
            pointee, t = t[1], types.get(t[1], (None,))
        if sc == SC_UNIFORM and t[0] == OP_TYPE_STRUCT and DEC_BLOCK in decos.get(pointee, {}):
            kind = "block"
        elif sc == SC_UNIFORM_CONSTANT and t[0] == OP_TYPE_SAMPLED_IMAGE:
            kind = "sampler"
        else:
            raise FormatError(f"{where}: variable {names.get(vid, vid)} is bound but is neither a uniform "
                              "block nor a combined image-sampler")
        out.append({"set": d.get(DEC_SET, 0), "binding": d.get(DEC_BINDING, 0), "kind": kind, "count": n,
                    "type": names.get(pointee, ""), "name": names.get(vid, ""),
                    "size": n * size_of(pointee) if kind == "block" else 0})
    return out, caps


OP_ENTRY_POINT, OP_FUNCTION = 15, 54
DEC_LOCATION, DEC_BUILTIN = 30, 11
SC_INPUT, SC_OUTPUT = 1, 3


def spirv_interface(data: bytes, where: str) -> dict[str, dict[int, tuple[str, int]]]:
    """A module's user inputs and outputs: {"in"|"out": {location: (scalar kind, components)}}, one
    entry per location an array or matrix spans; built-ins and variables without a Location are left
    out. Raises FormatError for a module without an entry point or a function (a truncated file)."""
    words = struct.unpack_from(f"<{len(data) // 4}I", data)
    loc: dict[int, int] = {}
    builtin: set[int] = set()
    scalar: dict[int, tuple[str, int]] = {}
    vector: dict[int, tuple[int, int]] = {}
    matrix: dict[int, tuple[int, int]] = {}
    array: dict[int, tuple[int, int]] = {}
    pointer: dict[int, int] = {}
    const: dict[int, int] = {}
    variables: list[tuple[int, int, int]] = []
    seen_ops: set[int] = set()
    i = 5
    while i < len(words):
        count, op = words[i] >> 16, words[i] & 0xFFFF
        if count == 0 or i + count > len(words):
            raise FormatError(f"{where}: malformed instruction at word {i}")
        a = words[i + 1:i + count]
        seen_ops.add(op)
        if op == OP_DECORATE and len(a) >= 2:
            if a[1] == DEC_LOCATION and len(a) >= 3:
                loc[a[0]] = a[2]
            elif a[1] == DEC_BUILTIN:
                builtin.add(a[0])
        elif op == OP_MEMBER_DECORATE and len(a) >= 3 and a[2] == DEC_BUILTIN:
            builtin.add(a[0])
        elif op == OP_TYPE_INT and len(a) >= 3:
            scalar[a[0]] = ("int" if a[2] else "uint", a[1])
        elif op == OP_TYPE_FLOAT and len(a) >= 2:
            scalar[a[0]] = ("float", a[1])
        elif op == OP_TYPE_VECTOR and len(a) >= 3:
            vector[a[0]] = (a[1], a[2])
        elif op == OP_TYPE_MATRIX and len(a) >= 3:
            matrix[a[0]] = (a[1], a[2])
        elif op == OP_TYPE_ARRAY and len(a) >= 3:
            array[a[0]] = (a[1], a[2])
        elif op == OP_TYPE_POINTER and len(a) >= 3:
            pointer[a[0]] = a[2]
        elif op == OP_CONSTANT and len(a) == 3:
            const[a[1]] = a[2]
        elif op == OP_VARIABLE and len(a) >= 3:
            variables.append((a[0], a[1], a[2]))
        i += count
    if OP_ENTRY_POINT not in seen_ops or OP_FUNCTION not in seen_ops:
        raise FormatError(f"{where}: SPIR-V module without an entry point or a function")
    out: dict[str, dict[int, tuple[str, int]]] = {"in": {}, "out": {}}
    for ptr, vid, sc in variables:
        if sc not in (SC_INPUT, SC_OUTPUT) or vid in builtin or vid not in loc:
            continue
        t, n = pointer.get(ptr, 0), 1
        if t in array:
            n = const.get(array[t][1], 1)
            t = array[t][0]
        if t in builtin:
            continue
        if t in matrix:
            n *= matrix[t][1]
            t = matrix[t][0]
        comps = 1
        if t in vector:
            t, comps = vector[t]
        kind = scalar.get(t, ("other", 32))
        if kind[1] != 32:
            kind = (f"{kind[0]}{kind[1]}", 32)
        side = out["in" if sc == SC_INPUT else "out"]
        for k in range(n):
            side[loc[vid] + k] = (kind[0], comps)
    return out


def interface_problems(vert: dict, frag: dict) -> list[str]:
    """What Vulkan's pipeline creation would reject between the two stages (with maintenance4, which
    lets the vertex stage write a wider vector than the fragment stage reads): a fragment input at a
    location the vertex stage does not write, or of another component type, or wider."""
    problems = []
    for location, (kind, comps) in sorted(frag["in"].items()):
        written = vert["out"].get(location)
        if written is None:
            problems.append(f"fragment input at location {location} is not written by the vertex stage")
        elif written[0] != kind:
            problems.append(f"location {location}: the vertex stage writes {written[0]}, the fragment stage reads {kind}")
        elif written[1] < comps:
            problems.append(f"location {location}: the vertex stage writes {written[1]} components, the fragment "
                            f"stage reads {comps}")
    return problems


VK_SET = {"vert": 0, "frag": 1}  # VK_LAYOUT: one descriptor set per stage
VK_SAMPLER_BASE = 16             # VK_LAYOUT: combined samplers from binding 16
INSTALL_FORMAT = 3
# The shim's key for a stage: FNV-1a 64 over the first CONTROL_KEY_BYTES of its control section,
# the only part of the stage nvnProgramSetShaders receives by CPU pointer. On 2.1.0 every control
# section is at least 2,320 bytes, the first 2,048 bytes tell all 1,010 distinct ones apart, and
# stages with the same control section have the same code (install() checks all three).
CONTROL_KEY_BYTES = 2048


def fnv1a64(data: bytes) -> int:
    h = 0xCBF29CE484222325
    for b in data:
        h = ((h ^ b) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h


def stage_control(pkg: bytes, blob_offset: int, where: str) -> tuple[bytes, bytes, int]:
    """The stage's (control section, code, the control section's offset in the package file) from
    section 0 of its blob (layout in the docstring)."""
    rec = blob_offset + SECTION_RECORDS
    size, off, kind = struct.unpack_from("<III", pkg, rec)
    stage, ctrl, code, code_size = struct.unpack_from("<4I", pkg, rec + 0x2C)
    if kind != 0 or ctrl != 0 or not ctrl < code <= code + code_size <= size:
        raise FormatError(f"{where}: section 0 is not a control section followed by code")
    base = blob_offset + off
    return pkg[base + ctrl:base + code], pkg[base + code:base + code + code_size], base + ctrl
INSTALL_MANIFEST = "shaders.manifest"
INSTALL_MARKER = ".bshdat-install"


def round16(n: int) -> int:
    """NVN's reflection gives a block's size rounded up to 16 bytes (checked for every active block)."""
    return (n + 15) & ~15


def stage_layout(st: dict, resources: list[dict], stage: str, where: str) -> tuple[list, list]:
    """Match the SPIR-V's resources to the NVN reflection: (blocks, samplers) as manifest rows.

    Blocks: NVN's per-stage uniform-buffer slot is the reflection's binding (0..n-1, the active
    blocks in declaration order); the Vulkan binding is the same number in set VK_SET[stage].
    Samplers: NVN's texture slot is the declared binding (an array of n takes slots b..b+n-1); the
    Vulkan binding is VK_SAMPLER_BASE + b with descriptorCount n. Every active block and sampler of
    the reflection must be in the SPIR-V at that place. A block or sampler only the SPIR-V has (the
    NVN compiler found it inactive, glslang kept it) is listed with NVN slot -1; such a block's size
    is the span of its SPIR-V layout rounded up to 16, as NVN gives an active block's size (checked
    for every active block), so that the shim can size its placeholder.
    """
    refl = st["reflection"]
    vk = {(r["kind"], r["binding"]): r for r in resources}
    if any(r["set"] != VK_SET[stage] for r in resources):
        raise FormatError(f"{where}: a resource outside descriptor set {VK_SET[stage]}")
    if any(r["kind"] == "block" and r["binding"] >= VK_SAMPLER_BASE for r in resources):
        raise FormatError(f"{where}: a uniform block at binding {VK_SAMPLER_BASE} or above, among the samplers")
    if len({r["binding"] for r in resources}) != len(resources):
        raise FormatError(f"{where}: two resources share a binding")
    blocks, samplers, seen = [], [], set()
    for b in refl["blocks"]:
        if b["binding"] < 0:
            continue
        r = vk.get(("block", b["binding"]))
        if not r or r["type"] != b["name"]:
            raise FormatError(f"{where}: uniform block {b['name']} (NVN slot {b['binding']}) is not at binding "
                              f"{b['binding']} in the SPIR-V")
        seen.add(("block", b["binding"]))
        if b["size"] != round16(r["size"]):
            raise FormatError(f"{where}: uniform block {b['name']} spans {r['size']} bytes in the SPIR-V, "
                              f"{b['size']} for NVN (expected {r['size']} rounded up to 16)")
        blocks.append((b["binding"], b["binding"], b["size"], b["name"]))
    by_base: dict[int, dict] = {}
    for u in refl["uniforms"]:
        if u["block"] != -1 or u["binding"] < 0:
            continue
        base = u["name"].split("[", 1)[0]
        by_base.setdefault(u["binding"], {"name": base, "array": u["array"]})
    for slot, u in sorted(by_base.items()):
        r = vk.get(("sampler", VK_SAMPLER_BASE + slot))
        if not r or r["name"] != u["name"] or r["count"] != u["array"]:
            raise FormatError(f"{where}: sampler {u['name']} (NVN slot {slot}, {u['array']}) is not at binding "
                              f"{VK_SAMPLER_BASE + slot} with that count in the SPIR-V")
        seen.add(("sampler", VK_SAMPLER_BASE + slot))
        samplers.append((slot, VK_SAMPLER_BASE + slot, r["count"], u["name"]))
    for key, r in sorted(vk.items()):
        if key not in seen:
            row = ((-1, r["binding"], round16(r["size"]), r["type"]) if r["kind"] == "block"
                   else (-1, r["binding"], r["count"], r["name"]))
            (blocks if r["kind"] == "block" else samplers).append(row)
    return blocks, samplers


def install(out_root: Path, manifests: list[dict], dest: Path, version: str, jobs: int) -> int:
    """Compile every stage for the port (vk_shim) into `dest` and write `dest`/shaders.manifest.

    The manifest is written last and only when every stage compiled and matched its reflection, so
    a build that stops halfway leaves none and reruns. Format (text, one record per line, fields
    separated by one space, names last since they are the only free text):
      dread-shaders <format> <game version> glslang <version>
      layout <vert set> <frag set> <sampler base>
      package <name> <program count>
        attribute <engine id> <name>             the package's attribute list
        uniform <list 1|2> <engine id> <name>    the package's uniform name lists
        program <key> <key2> <index> <define count>
          define <text>                          the engine's variant prefix, in order
          stage <vert|frag> <spv path> <spv bytes> <control key, 16 hex digits> <control bytes>
                <the control section's offset in the package file>
            block <nvn slot> <vk binding> <size> <name>
            sampler <nvn slot> <vk binding> <count> <name>
            input <location> <name> / output <location> <name>   (location -1: built-in)
      end <package count> <program count> <stage count>
    """
    tool = glslang_path()
    if dest.exists():
        # Only a directory an earlier install wrote (it writes INSTALL_MARKER first) is cleared.
        if any(dest.iterdir()) and not (dest / INSTALL_MARKER).is_file():
            raise SystemExit(f"{dest} exists and is not a shader install directory (no {INSTALL_MARKER}); "
                             "refusing to clear it")
        shutil.rmtree(dest)
    dest.mkdir(parents=True)
    (dest / INSTALL_MARKER).write_text("written by tools/bshdat.py --install\n")
    tasks = []
    for man in manifests:
        (dest / man["package"]).mkdir()
        for prog in man["programs"]:
            for st_name, st in prog["stages"].items():
                blocks = {b["name"]: b["binding"] for b in st["reflection"]["blocks"] if b["binding"] >= 0}
                spv = dest / man["package"] / f"{prog['name']}.{st_name}.spv"
                tasks.append(((man["package"], prog["name"], st_name), out_root / man["package"] / st["file"],
                              blocks, spv, input_rule(st_name, prog)))
    failed = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as pool:
        futs = {pool.submit(compile_one, tool, src, key[2], blocks, "vk_shim", FIXUPS.get((key[0], key[2]), ()),
                            spv, inputs): key for key, src, blocks, spv, inputs in tasks}
        for fut in concurrent.futures.as_completed(futs):
            ok, cats, first = fut.result()
            if not ok:
                failed.append(f"{'/'.join(futs[fut])}: {';'.join(cats)}: {first}")
    for f in sorted(failed):
        print(f"compile failed: {f}", file=sys.stderr)
    if failed:
        print(f"{len(failed)} of {len(tasks)} stages failed; no manifest written", file=sys.stderr)
        return 1
    lines = [f"dread-shaders {INSTALL_FORMAT} {version} glslang {glslang_version(tool)}",
             f"layout {VK_SET['vert']} {VK_SET['frag']} {VK_SAMPLER_BASE}"]
    nprog = nstage = 0
    controls: dict[int, tuple] = {}  # key -> (control, code, layout and SPIR-V, where) of the first stage with it
    shd = ROOT / "orig" / version / "romfs" / "system" / "shd"
    errors = []
    for man in manifests:
        lines.append(f"package {man['package']} {len(man['programs'])}")
        lines += [f"attribute {a['id']} {a['name']}" for a in man["attributes"]]
        for n in (1, 2):
            lines += [f"uniform {n} {u['id']} {u['name']}" for u in man[f"uniforms{n}"]]
        for prog in man["programs"]:
            nprog += 1
            lines.append(f"program {prog['key'][0]} {prog['key'][1]} {prog['index']} {len(prog['defines'])}")
            lines += [f"define {d}" for d in prog["defines"]]
            interfaces: dict[str, dict] = {}
            for st_name, st in prog["stages"].items():
                nstage += 1
                rel = f"{man['package']}/{prog['name']}.{st_name}.spv"
                data = (dest / rel).read_bytes()
                where = f"{man['package']}/{prog['name']}.{st_name}"
                try:
                    resources, caps = spirv_resources(data, where)
                    if caps != [CAP_SHADER]:
                        raise FormatError(f"{where}: SPIR-V capabilities {caps}, expected only Shader")
                    interfaces[st_name] = spirv_interface(data, where)
                    blocks, samplers = stage_layout(st, resources, st_name, where)
                except FormatError as e:
                    errors.append(str(e))
                    continue
                try:
                    control, code, control_offset = stage_control((shd / f"{man['package']}.bshdat").read_bytes(), st["blob_offset"], where)
                    if len(control) < CONTROL_KEY_BYTES:
                        raise FormatError(f"{where}: control section of {len(control)} bytes, under {CONTROL_KEY_BYTES}")
                    key = fnv1a64(control[:CONTROL_KEY_BYTES])
                    shape = (blocks, samplers, st["reflection"]["inputs"], st["reflection"]["outputs"], data)
                    seen = controls.setdefault(key, (control, code, shape, where))
                    if seen[:2] != (control, code):
                        raise FormatError(f"{where}: control key {key:016x} also names {seen[3]}, a different stage")
                    if seen[2] != shape:
                        # The shim gives every stage with this key the first one's SPIR-V and layout.
                        raise FormatError(f"{where}: same stage as {seen[3]} but a different binding layout, "
                                          "interface or SPIR-V")
                except FormatError as e:
                    errors.append(str(e))
                    continue
                lines.append(f"stage {st_name} {rel} {len(data)} {key:016x} {len(control)} {control_offset}")
                lines += [f"block {a} {b} {c} {n}" for a, b, c, n in blocks]
                lines += [f"sampler {a} {b} {c} {n}" for a, b, c, n in samplers]
                for io in ("inputs", "outputs"):
                    lines += [f"{io[:-1]} {r['location']} {r['name']}" for r in st["reflection"][io]]
            if len(interfaces) == 2:
                errors += [f"{man['package']}/{prog['name']}: {p}"
                           for p in interface_problems(interfaces["vert"], interfaces["frag"])]
    if errors:
        for e in errors:
            print(e, file=sys.stderr)
        print(f"{len(errors)} problems (stages that do not match their reflection, or stage interfaces Vulkan "
              "would reject); no manifest written", file=sys.stderr)
        return 1
    lines.append(f"end {len(manifests)} {nprog} {nstage}")
    tmp = dest / f"{INSTALL_MANIFEST}.tmp"
    tmp.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")
    tmp.replace(dest / INSTALL_MANIFEST)
    print(f"glslang {glslang_version(tool)}: {nstage} stages of {nprog} programs in {len(manifests)} packages "
          f"({len(controls)} distinct by control key) -> {dest} ({INSTALL_MANIFEST})")
    return 0


# --- main ---------------------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--only", action="append",
                    help="package name(s), comma-separated; repeatable (the compile report then covers only these)")
    ap.add_argument("--compile", action="store_true", help="compile every extracted source with glslang")
    ap.add_argument("--install", metavar="DIR", type=Path,
                    help="the port's build step: compile every stage for Vulkan (vk_shim) into DIR with "
                         "DIR/shaders.manifest; fails unless every stage compiles")
    ap.add_argument("--glslang-path", action="store_true",
                    help="print the glslang this tool runs and exit (the port's CMake depends on the file)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    args = ap.parse_args()
    if args.glslang_path:
        print(Path(glslang_path()).as_posix())
        return 0
    if args.install and args.only:
        ap.error("--install covers every package; with --only it would replace a full install with a partial one")
    only = [n for o in args.only for n in o.split(",") if n] if args.only else None
    out_root = ROOT / "build" / "shaders" / args.version
    out_root.mkdir(parents=True, exist_ok=True)
    manifests = []
    errors = 0
    for path in package_paths(args.version, only):
        try:
            manifests.append(extract(path, out_root))
        except (FormatError, ValueError, UnicodeDecodeError, struct.error) as e:
            errors += 1
            print(f"{path.name}: {e}", file=sys.stderr)
            continue
        if manifests[-1]["unaccounted_bytes"]:
            errors += 1
            print(f"{path.name}: {manifests[-1]['unaccounted_bytes']} bytes the parser does not account for "
                  "(extracted anyway)", file=sys.stderr)
    nprog = sum(len(m["programs"]) for m in manifests)
    nstage = sum(len(p["stages"]) for m in manifests for p in m["programs"])
    unaccounted = sum(m["unaccounted_bytes"] for m in manifests)
    ext = collections.Counter(e for m in manifests for p in m["programs"] for s in p["stages"].values()
                              for e in s["extensions"])
    ver = collections.Counter(s["version"] for m in manifests for p in m["programs"] for s in p["stages"].values())
    print(f"{len(manifests)} packages, {nprog} programs, {nstage} stage sources -> "
          f"{out_root.relative_to(ROOT).as_posix()} ({errors} packages with errors, {unaccounted} bytes unaccounted)")
    print("  #version: " + ", ".join(f"{v} {n}" for v, n in ver.most_common()))
    print("  extensions: " + (", ".join(f"{e} {n}" for e, n in ext.most_common()) or "none"))
    stages = [s for m in manifests for p in m["programs"] for s in p["stages"].values()]
    for kind in ("blocks", "samplers"):
        checks = [s["binding_check"][kind] for s in stages]
        total = sum(c["active"] for c in checks)
        same = sum(c["same"] for c in checks)
        undeclared = sum(len(c["undeclared"]) for c in checks)
        order = f", {sum(c['by_order'] for c in checks)} numbered in declaration order" if kind == "blocks" else ""
        print(f"  active {kind}: {total}; binding = the source's layout number {same}, other {total - same - undeclared}, "
              f"no layout number {undeclared}{order}")
    if args.compile and manifests:
        compile_all(out_root, manifests, args.jobs)
    if args.install:
        if errors:
            print("not installing: a package failed to parse", file=sys.stderr)
            return 1
        return install(out_root, manifests, args.install.resolve(), args.version, args.jobs)
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
