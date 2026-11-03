"""tools/bshdat.py on a made-up shader package and tiny hand-assembled SPIR-V modules.

The package is built here, field by field, in the layout the tool's docstring describes: invented
GLSL text and names, no bytes from the game. No glslang run.
"""

import struct
import tempfile
import unittest
from pathlib import Path

from tools.tests import _path  # noqa: F401
import bshdat as bs


# --- a synthetic package --------------------------------------------------------------------------

def esc(b: bytes) -> bytes:
    return (b.replace(b"\\", b"\\\\").replace(b'"', b'\\"').replace(b"\n", b"\\n")
            .replace(b"\r", b"\\r").replace(b"\t", b"\\t"))


def align(buf: bytearray, n: int) -> None:
    buf += b"\xff" * (-len(buf) % n)


def make_blob(files: list[tuple[str, int, str, bytes]], stage: int, block: bytes = b"Blk") -> bytes:
    """A stage blob: header, a reflection section (one active uniform block) and a debug section."""
    # Reflection section: magic, one block record (0x6c) at 0x10, then the name table.
    refl = bytearray(0x10 + 0x6C)
    struct.pack_into("<I", refl, 0, bs.REFL_MAGIC)
    names = block + b"\0"
    struct.pack_into("<II", refl, 0x10, 0, len(names))
    struct.pack_into("<III", refl, 0x10 + 0x28, 32, 2, 1 << stage)
    struct.pack_into("<6i", refl, 0x10 + 0x34, 0, 0, -1, -1, -1, -1)
    str_off = len(refl)
    refl += names
    refl += bytes(-len(refl) % 8)
    # Debug section: header, one text part.
    text = b"!!NVxp5.0\n#profile gp5xp\n# 12 instructions, 3 R-regs\n"
    for name, fid, kind, body in files:
        text += b'#MSDB: (%s:%d:%d:%s)"%s"\n' % (name.encode(), fid, len(body), kind.encode(), esc(body))
    text += b"\0"
    dbg = bytearray(0x54) + text
    dbg += bytes(-len(dbg) % 8)
    struct.pack_into("<I", dbg, 0, bs.DEBUG_MAGIC)
    struct.pack_into("<Q", dbg, 0x18, 0x0123456789ABCDEF)
    struct.pack_into("<II", dbg, 0x28, 1, len(dbg))
    struct.pack_into("<9I", dbg, 0x30, len(text), 0x54, 0, 1, stage, 0x00200416, 0, 0, 0)

    first = bs.SECTION_RECORDS + 2 * bs.SECTION_STRIDE
    refl_off, dbg_off = first, first + len(refl)
    size = dbg_off + len(dbg)
    blob = bytearray(first)
    struct.pack_into("<I", blob, 0, bs.BLOB_MAGIC)
    struct.pack_into("<III", blob, 0x44, size, first, 2)
    rec = bs.SECTION_RECORDS
    words = [0] * 36
    words[0:3] = [len(refl), refl_off, 3]
    words[11:25] = [1, 0x10] + [0, str_off] * 6
    words[25:27] = [len(names), str_off]
    struct.pack_into("<36I", blob, rec, *words)
    struct.pack_into("<III", blob, rec + bs.SECTION_STRIDE, len(dbg), dbg_off, 4)
    return bytes(blob + refl + dbg)


VERT_SRC = b'#define HAS_NORMAL\r\n\r\n#version 440 core\r\n#include "common.h"\r\nvoid main() { x(); }\r\n'
FRAG_SRC = b'#define HAS_NORMAL\n\n#version 440 core\nout vec4 c;\nvoid main() { c = vec4(1.0); }\n'
INC_SRC = b'// "quoted"\tand tabbed\nvoid x() {}\n'


def make_package(stray: bool = False) -> bytes:
    vert = make_blob([("_shader1", 13684, "SOURCE_FILE", VERT_SRC), ("common.h", 2, "INCLUDED_FILE", INC_SRC)], 0)
    frag = make_blob([("_shader2", 13684, "SOURCE_FILE", FRAG_SRC)], 1)
    d = bytearray(0x40)
    d[0:8] = b"MSHD" + struct.pack("<HH", 1, 0x13)
    align(d, 0x80)
    vert_off = len(d)
    d += vert
    align(d, 0x80)
    if stray:
        d[-1] = 0x00  # a byte in the padding that is not 0xff
    frag_off = len(d)
    d += frag
    align(d, 8)

    def put(b: bytes) -> int:
        off = len(d)
        d.extend(b)
        align(d, 8)
        return off

    rec = put(struct.pack("<IIQ", 0, 0, 0) + struct.pack("<IIQ", len(vert), 0xFFFFFFFF, vert_off)
              + struct.pack("<IIQ", len(frag), 0xFFFFFFFF, frag_off))
    prog_list = put(struct.pack("<QQ", rec, 0))
    entry = put(struct.pack("<IIQ", 5, 5, rec))
    vmap = put(struct.pack("<QQ", entry, 0))

    def name_list(items: list[tuple[str, int]]) -> int:
        head = 0
        for name, ident in reversed(items):
            n = put(name.encode() + b"\0")
            e = put(struct.pack("<QI", n, ident) + b"\xff" * 4)
            head = put(struct.pack("<QQ", e, head))
        return head

    u1 = name_list([("uView", 0), ("uTime", 116)])
    attrs = name_list([("aPos", 3)])
    struct.pack_into("<7Q", d, 8, prog_list, vmap, rec, 0, u1, 0, attrs)
    return bytes(d)


class Package(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.dir = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def write(self, data: bytes, name: str = "pkg_test") -> Path:
        p = self.dir / f"{name}.bshdat"
        p.write_bytes(data)
        return p

    def test_parse_package(self):
        pkg = bs.parse_package(self.write(make_package()))
        self.assertEqual(pkg.name, "pkg_test")
        self.assertEqual(pkg.version, (1, 0x13))
        self.assertEqual(pkg.unaccounted, 0)
        self.assertEqual(len(pkg.programs), 1)
        p = pkg.programs[0]
        self.assertEqual(p.key, (5, 5))
        self.assertEqual(pkg.last_program, 0)
        self.assertEqual(sorted(p.stages), ["frag", "vert"])
        self.assertEqual(pkg.uniforms1, [("uView", 0), ("uTime", 116)])
        self.assertEqual(pkg.uniforms2, [])
        self.assertEqual(pkg.attributes, [("aPos", 3)])

    def test_stage_contents(self):
        st = bs.parse_package(self.write(make_package())).programs[0].stages["vert"]
        self.assertEqual([(f.name, f.file_id, f.kind) for f in st.files],
                         [("_shader1", 13684, "SOURCE_FILE"), ("common.h", 2, "INCLUDED_FILE")])
        self.assertEqual(st.source.text, VERT_SRC)
        self.assertEqual(st.files[1].text, INC_SRC)
        self.assertEqual(st.profile, "gp5xp")
        self.assertEqual(st.listing_summary, "12 instructions, 3 R-regs")
        self.assertEqual(st.debug_hash, "0123456789abcdef")
        self.assertEqual([s["name"] for s in st.sections], ["reflection", "debug"])
        self.assertEqual(st.reflection["blocks"],
                         [{"name": "Blk", "size": 32, "members": 2, "stages": 1, "binding": 0}])
        self.assertEqual(st.reflection["uniforms"], [])

    def test_unaccounted_byte(self):
        self.assertEqual(bs.parse_package(self.write(make_package(stray=True))).unaccounted, 1)

    def test_bad_magic_and_version(self):
        data = make_package()
        with self.assertRaises(bs.FormatError):
            bs.parse_package(self.write(b"XXXX" + data[4:]))
        with self.assertRaises(bs.FormatError):
            bs.parse_package(self.write(data[:4] + struct.pack("<HH", 2, 0x13) + data[8:]))

    def test_extract(self):
        out = self.dir / "out"
        man = bs.extract(self.write(make_package()), out)
        prog = man["programs"][0]
        self.assertEqual(prog["name"], "p5")
        self.assertEqual(prog["defines"], ["HAS_NORMAL"])
        vert = (out / "pkg_test" / "p5.vert").read_text()
        self.assertTrue(vert.startswith("#version 440 core\n#define HAS_NORMAL\n\n"))
        self.assertIn('// bshdat.py: begin #include "common.h"\n// "quoted"\tand tabbed\nvoid x() {}\n'
                      '// bshdat.py: end #include "common.h"', vert)
        self.assertNotIn("\r", vert)
        self.assertTrue((out / "pkg_test" / "manifest.json").is_file())
        self.assertEqual(prog["stages"]["vert"]["includes"], [{"name": "common.h", "file_id": 2, "length": len(INC_SRC)}])


# --- smaller pieces ------------------------------------------------------------------------------

class CoverAndLists(unittest.TestCase):
    def test_leftover(self):
        data = bytes([0xFF, 1, 2, 0xFF, 3, 0xFF])
        c = bs.Cover(len(data))
        c.add(1, 1, "a")
        c.add(1, 2, "b")  # overlaps a by one byte
        self.assertEqual(c.leftover(data), (1, 1))  # byte 4 unexplained

    def test_add_past_end(self):
        with self.assertRaises(bs.FormatError):
            bs.Cover(8).add(4, 8, "x")

    def test_walk_list(self):
        d = struct.pack("<QQQQ", 0x111, 16, 0x222, 0)
        c = bs.Cover(len(d))
        self.assertEqual(bs.walk_list(d, 0, c, "l"), [])
        self.assertEqual(bs.walk_list(d, 16, c, "l"), [0x222])
        loop = struct.pack("<QQQQ", 0, 16, 1, 16)
        with self.assertRaises(bs.FormatError):
            bs.walk_list(loop, 16, bs.Cover(len(loop)), "loop")

    def test_unescape(self):
        self.assertEqual(bs.unescape(rb'a\nb\t\"\\\r'), b'a\nb\t"\\\r')
        with self.assertRaises(bs.FormatError):
            bs.unescape(rb"\q")


class Glsl(unittest.TestCase):
    def test_flatten_nested_and_missing(self):
        files = {"m": '#include "a.h"\nmain\n', "a.h": '#include "b.h"\na\n', "b.h": "b\n"}
        flat = bs.flatten(files, "m")
        self.assertEqual(flat.split("\n")[2], "b")
        self.assertEqual(flat.count("begin #include"), 2)
        with self.assertRaises(bs.FormatError):
            bs.flatten({"m": '#include "nope.h"\n'}, "m")
        with self.assertRaises(bs.FormatError):
            bs.flatten({"m": '#include "m"\n'}, "m")

    def test_prefix_defines(self):
        self.assertEqual(bs.prefix_defines("#define A\n#define B 1\n\n#version 440\n"), ["A", "B 1"])
        self.assertEqual(bs.prefix_defines("#version 440\n#define A\n"), [])

    def test_glsl_for_repeated_version(self):
        files = [bs.SourceFile("_s", 1, "SOURCE_FILE", b'#define X\n\n#include "i.h"\nvoid main(){}\n'),
                 bs.SourceFile("i.h", 2, "INCLUDED_FILE", b"#version 450\nint a;\n#version 450\n")]
        st = bs.Stage("frag", 0, 0, [], {}, "", None, None, files)
        text, info = bs.glsl_for(st)
        self.assertTrue(text.startswith("#version 450\n#define X\n"))
        self.assertIn("// bshdat.py: repeated #version 450", text)
        self.assertEqual(info["version"], "450")
        self.assertEqual(info["version_from"], "i.h")
        self.assertEqual(info["repeated_version_lines"], 1)
        st_none = bs.Stage("frag", 0, 0, [], {}, "", None, None, [bs.SourceFile("_s", 1, "SOURCE_FILE", b"x\n")])
        with self.assertRaises(bs.FormatError):
            bs.glsl_for(st_none)

    def test_binding_check(self):
        text = ("layout(location = 0) uniform A { vec4 a; };\n// layout(location = 9) uniform Z {\n"
                "layout(location = 1) uniform B { vec4 b; };\nlayout(location = 2) uniform C { vec4 c; };\n"
                "layout(binding = 3) uniform sampler2D tex;\n")
        refl = {"blocks": [{"name": "A", "binding": -1}, {"name": "B", "binding": 0}, {"name": "C", "binding": 1}],
                "uniforms": [{"name": "tex", "block": -1, "binding": 3}, {"name": "u", "block": 0, "binding": -1}]}
        out = bs.binding_check(text, refl)
        self.assertEqual(out["blocks"]["active"], 2)
        self.assertEqual(out["blocks"]["same"], 0)
        self.assertEqual(out["blocks"]["by_order"], 2)
        self.assertEqual(out["samplers"], {"active": 1, "same": 1, "differ": [], "undeclared": []})


class Rewrites(unittest.TestCase):
    def test_declared_components(self):
        for t, n in (("float", 1), ("vec2", 2), ("ivec3", 3), ("float4", 4), ("uint", 1), ("mat4", 0), ("vec", 0)):
            self.assertEqual(bs.declared_components(t), n, t)

    def test_drop_uncovered_inputs(self):
        text = ("layout(location = 0) in vec4 a;\nlayout(location = 1) flat in vec4 b;\n"
                "layout(location = 2) in vec2 c;\nlayout(location = 3) in vec4 d[2];\n")
        out = bs.drop_uncovered_inputs(text, {"a"}, {1: 4, 2: 1, 3: 4}).split("\n")
        self.assertEqual(out[0], "layout(location = 0) in vec4 a;")         # active
        self.assertEqual(out[1], "layout(location = 1) flat in vec4 b;")    # covered
        self.assertIn("inactive input c (location 2) dropped", out[2])      # too narrow
        self.assertIn("inactive input d (location 3) dropped", out[3])      # arrays are never covered

    def test_shim_rewrite_blocks_and_typecast(self):
        text = ("#version 440 core\nlayout(location = 3, std140) uniform A {\n};\n"
                "layout(location = 0) uniform B {\n};\nlayout(binding = 7) uniform C {\n};\n")
        out = bs.shim_rewrite(text, {"A": 0, "B": 1}, ("blocks", "typecast")).split("\n")
        self.assertEqual(out[1], bs.SHIM_TYPECAST)
        self.assertEqual(out[2], "layout(std140, binding = 0) uniform A {")
        self.assertEqual(out[4], "layout(binding = 1) uniform B {")
        self.assertEqual(out[6], "layout(binding = 2) uniform C {")  # inactive: above the active ones

    def test_fixups_must_match_once(self):
        fx = ((r"foo", "bar"),)
        self.assertEqual(bs.apply_fixups("a foo b", fx, "w"), "a bar b")
        with self.assertRaises(bs.FormatError):
            bs.apply_fixups("foo foo", fx, "w")
        with self.assertRaises(bs.FormatError):
            bs.apply_fixups("none", fx, "w")

    def test_fxaa_fixup_patterns(self):
        src = ("half4 FxaaTexOff(sampler2D t, float2 p, int2 o, float2 r) {\n"
               "    return textureLodOffset(t, p.xy, 0.0, o.xy);\n}\n")
        out = bs.apply_fixups(src, bs.FIXUPS[("pp_fxaa_nx", "frag")], "w")
        self.assertIn("#define FxaaTexOff(t, p, o, r) textureLodOffset(t, (p).xy, 0.0, o)", out)
        self.assertIn("half4 FxaaTexOff_bshdat_unused(", out)
        self.assertIn("textureLod(t, p.xy, 0.0)", out)

    def test_categorise(self):
        log = ("ERROR: 0:3: 'location' : cannot apply to uniform or buffer block\n"
               "ERROR: 0:9: 'foo' : undeclared identifier\nERROR: 0:9: 'bar' : undeclared identifier\n"
               "ERROR: 3 compilation errors.  No code generated.\n")
        cats, first = bs.categorise(log)
        self.assertEqual(cats, ["block_location", "undeclared_identifier"])
        self.assertEqual(first, "'location' : cannot apply to uniform or buffer block")
        self.assertEqual(bs.categorise(""), (["other"], "no output"))
        self.assertEqual(bs.categorise("ERROR: something odd\n")[0], ["other"])

    def test_input_rule(self):
        prog = {"stages": {"frag": {"reflection": {"inputs": [{"name": "v[0]"}]}},
                           "vert": {"reflection": {"outputs": [
                               {"location": 0, "type": 37, "array": 1}, {"location": -1, "type": 39, "array": 1},
                               {"location": 2, "type": 999, "array": 1}, {"location": 3, "type": 39, "array": 2}]}}}}
        self.assertIsNone(bs.input_rule("vert", prog))
        self.assertEqual(bs.input_rule("frag", prog), ({"v"}, {0: 2, 2: 4}))


# --- SPIR-V --------------------------------------------------------------------------------------

def op(code: int, *args: int) -> list[int]:
    return [((len(args) + 1) << 16) | code, *args]


def name_words(s: str) -> list[int]:
    b = s.encode() + b"\0"
    b += bytes(-len(b) % 4)
    return list(struct.unpack(f"<{len(b) // 4}I", b))


def module(*instrs: list[int]) -> bytes:
    words = [bs.SPV_MAGIC, 0x10000, 0, 100, 0] + [w for i in instrs for w in i]
    return struct.pack(f"<{len(words)}I", *words)


RESOURCES = [
    op(bs.OP_CAPABILITY, bs.CAP_SHADER),
    op(bs.OP_NAME, 3, *name_words("Blk")),
    op(bs.OP_NAME, 9, *name_words("tex")),
    op(bs.OP_DECORATE, 3, bs.DEC_BLOCK),
    op(bs.OP_MEMBER_DECORATE, 3, 0, bs.DEC_OFFSET, 0),
    op(bs.OP_MEMBER_DECORATE, 3, 1, bs.DEC_OFFSET, 16),
    op(bs.OP_DECORATE, 5, bs.DEC_BINDING, 0),
    op(bs.OP_DECORATE, 5, bs.DEC_SET, 1),
    op(bs.OP_DECORATE, 9, bs.DEC_BINDING, 16),
    op(bs.OP_DECORATE, 9, bs.DEC_SET, 1),
    op(bs.OP_TYPE_FLOAT, 1, 32),
    op(bs.OP_TYPE_VECTOR, 2, 1, 4),
    op(bs.OP_TYPE_STRUCT, 3, 2, 1),
    op(bs.OP_TYPE_POINTER, 4, bs.SC_UNIFORM, 3),
    op(bs.OP_VARIABLE, 4, 5, bs.SC_UNIFORM),
    op(bs.OP_TYPE_IMAGE, 6, 1, 1, 0, 0, 0, 1, 0),
    op(bs.OP_TYPE_SAMPLED_IMAGE, 7, 6),
    op(bs.OP_TYPE_POINTER, 8, bs.SC_UNIFORM_CONSTANT, 7),
    op(bs.OP_VARIABLE, 8, 9, bs.SC_UNIFORM_CONSTANT),
]


class Spirv(unittest.TestCase):
    def test_resources(self):
        res, caps = bs.spirv_resources(module(*RESOURCES), "m")
        self.assertEqual(caps, [bs.CAP_SHADER])
        self.assertEqual(res, [
            {"set": 1, "binding": 0, "kind": "block", "count": 1, "type": "Blk", "name": "", "size": 20},
            {"set": 1, "binding": 16, "kind": "sampler", "count": 1, "type": "", "name": "tex", "size": 0}])

    def test_not_spirv_or_malformed(self):
        with self.assertRaises(bs.FormatError):
            bs.spirv_resources(b"\0" * 20, "m")
        with self.assertRaises(bs.FormatError):
            bs.spirv_resources(module([(9 << 16) | 5, 1]), "m")

    def test_bound_variable_of_another_kind(self):
        bad = module(op(bs.OP_TYPE_FLOAT, 1, 32), op(bs.OP_TYPE_POINTER, 2, bs.SC_UNIFORM_CONSTANT, 1),
                     op(bs.OP_VARIABLE, 2, 3, bs.SC_UNIFORM_CONSTANT), op(bs.OP_DECORATE, 3, bs.DEC_BINDING, 0))
        with self.assertRaises(bs.FormatError):
            bs.spirv_resources(bad, "m")

    def interface(self) -> dict:
        return bs.spirv_interface(module(
            op(bs.OP_ENTRY_POINT, 0, 99), op(bs.OP_FUNCTION, 0, 99, 0, 0),
            op(bs.OP_DECORATE, 11, bs.DEC_LOCATION, 3),
            op(bs.OP_DECORATE, 13, bs.DEC_LOCATION, 0),
            op(bs.OP_DECORATE, 18, bs.DEC_LOCATION, 4),
            op(bs.OP_DECORATE, 20, bs.DEC_BUILTIN, 0),
            op(bs.OP_TYPE_FLOAT, 1, 32),
            op(bs.OP_TYPE_VECTOR, 2, 1, 4),
            op(bs.OP_TYPE_INT, 14, 32, 0),
            op(bs.OP_CONSTANT, 14, 15, 2),
            op(bs.OP_TYPE_ARRAY, 16, 2, 15),
            op(bs.OP_TYPE_POINTER, 10, bs.SC_INPUT, 2),
            op(bs.OP_TYPE_POINTER, 12, bs.SC_OUTPUT, 14),
            op(bs.OP_TYPE_POINTER, 17, bs.SC_OUTPUT, 16),
            op(bs.OP_VARIABLE, 10, 11, bs.SC_INPUT),
            op(bs.OP_VARIABLE, 12, 13, bs.SC_OUTPUT),
            op(bs.OP_VARIABLE, 17, 18, bs.SC_OUTPUT),
            op(bs.OP_VARIABLE, 10, 20, bs.SC_INPUT)), "m")

    def test_interface(self):
        self.assertEqual(self.interface(), {"in": {3: ("float", 4)},
                                            "out": {0: ("uint", 1), 4: ("float", 4), 5: ("float", 4)}})

    def test_interface_needs_entry_point(self):
        with self.assertRaises(bs.FormatError):
            bs.spirv_interface(module(op(bs.OP_TYPE_FLOAT, 1, 32)), "m")

    def test_interface_problems(self):
        vert = {"out": {0: ("float", 4), 1: ("int", 2), 2: ("float", 2)}}
        frag = {"in": {0: ("float", 3), 1: ("float", 2), 2: ("float", 4), 7: ("float", 1)}}
        problems = bs.interface_problems(vert, frag)
        self.assertEqual(len(problems), 3)
        self.assertIn("location 1: the vertex stage writes int", problems[0])
        self.assertIn("location 2: the vertex stage writes 2 components", problems[1])
        self.assertIn("location 7 is not written", problems[2])


class Install(unittest.TestCase):
    def test_fnv_and_round16(self):
        self.assertEqual(bs.fnv1a64(b""), 0xCBF29CE484222325)
        self.assertEqual(bs.fnv1a64(b"a"), 0xAF63DC4C8601EC8C)
        self.assertEqual([bs.round16(n) for n in (0, 1, 16, 17)], [0, 16, 16, 32])

    def test_stage_control(self):
        blob = 0x100
        pkg = bytearray(0x400)
        rec = blob + bs.SECTION_RECORDS
        struct.pack_into("<III", pkg, rec, 0x80, 0x200, 0)
        struct.pack_into("<4I", pkg, rec + 0x2C, 0, 0, 0x20, 0x40)
        pkg[blob + 0x200:blob + 0x220] = b"C" * 0x20
        pkg[blob + 0x220:blob + 0x260] = b"X" * 0x40
        control, code, off = bs.stage_control(bytes(pkg), blob, "w")
        self.assertEqual((control, code, off), (b"C" * 0x20, b"X" * 0x40, blob + 0x200))
        struct.pack_into("<4I", pkg, rec + 0x2C, 0, 0, 0x20, 0x100)  # code past the section
        with self.assertRaises(bs.FormatError):
            bs.stage_control(bytes(pkg), blob, "w")

    def test_stage_layout(self):
        st = {"reflection": {"blocks": [{"name": "Blk", "binding": 0, "size": 32}, {"name": "Off", "binding": -1}],
                             "uniforms": [{"name": "tex[0]", "block": -1, "binding": 2, "array": 2},
                                          {"name": "tex[1]", "block": -1, "binding": 2, "array": 2}]}}
        res = [{"set": 1, "binding": 0, "kind": "block", "count": 1, "type": "Blk", "name": "", "size": 20},
               {"set": 1, "binding": 1, "kind": "block", "count": 1, "type": "Off", "name": "", "size": 4},
               {"set": 1, "binding": 18, "kind": "sampler", "count": 2, "type": "", "name": "tex", "size": 0}]
        blocks, samplers = bs.stage_layout(st, res, "frag", "w")
        self.assertEqual(blocks, [(0, 0, 32, "Blk"), (-1, 1, 16, "Off")])
        self.assertEqual(samplers, [(2, 18, 2, "tex")])
        with self.assertRaises(bs.FormatError):
            bs.stage_layout(st, res, "vert", "w")  # wrong descriptor set
        res[0]["size"] = 40
        with self.assertRaises(bs.FormatError):
            bs.stage_layout(st, res, "frag", "w")  # size disagrees with NVN's


if __name__ == "__main__":
    unittest.main()
