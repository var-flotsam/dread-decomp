"""tools/port_imports.py on made-up symbol names and a hand-built dynamic symbol table (no game data)."""

import struct
import types
import unittest
from unittest import mock

from tools.tests import _path  # noqa: F401
import nso2elf
import port_imports as pi


class Namespace(unittest.TestCase):
    def test_c_name(self):
        self.assertEqual(pi.namespace("memcpy", "memcpy"), "C")

    def test_nn_namespace_is_two_components(self):
        self.assertEqual(pi.namespace("_ZN2nn2os9SleepTimeEv", "nn::os::SleepTime()"), "nn::os")
        self.assertEqual(pi.namespace("_ZN2nn3hid3fooEv", "nn::hid::detail::foo()"), "nn::hid")

    def test_movie_and_nv(self):
        self.assertEqual(pi.namespace("_ZN5movie3BarEv", "movie::Bar()"), "movie")
        self.assertEqual(pi.namespace("_ZN2nv3FooEv", "nv::Foo(int)"), "nv")
        self.assertEqual(pi.namespace("_ZN2nv3FooEv", "nv(int)"), "nv")

    def test_other_mangled_names_are_cxx(self):
        self.assertEqual(pi.namespace("_ZdlPv", "operator delete(void*)"), "C++")
        self.assertEqual(pi.namespace("_ZNSt3__14foo", "std::__1::foo()"), "C++")
        # "nnx::" is not nn::
        self.assertEqual(pi.namespace("_ZN3nnx1fEv", "nnx::f()"), "C++")


class Lp64(unittest.TestCase):
    def test_long_long_read_as_long(self):
        self.assertEqual(pi.lp64("f(long long, unsigned long long)"), "f(long, unsigned long)")

    def test_plain_long_unchanged(self):
        self.assertEqual(pi.lp64("f(long)"), "f(long)")


class LongPatterns(unittest.TestCase):
    """The --windows report's test for a spelled-out 32-bit long in a parameter list."""

    def narrow(self, params: str) -> bool:
        return bool(pi.LONG.search(pi.LONG_LONG.sub("", params)))

    def test_long_flags(self):
        self.assertTrue(self.narrow("unsigned long, int)"))

    def test_long_long_and_long_double_do_not(self):
        self.assertFalse(self.narrow("long long, unsigned long long)"))
        self.assertFalse(self.narrow("long double)"))

    def test_identifier_containing_long(self):
        self.assertFalse(self.narrow("Longitude, prolonged)"))


class TablesRegex(unittest.TestCase):
    def test_asciz_names(self):
        import re
        text = '\t.asciz\t"_ZN2nn2os1fEv"\n\t.quad 0\n\t.asciz\t"memcpy"\n'
        self.assertEqual(re.findall(r'\.asciz\t"([^"]+)"', text), ["_ZN2nn2os1fEv", "memcpy"])


def fake_module(symbols: list[tuple[str, int]]):
    """A module whose dynamic symbol table holds (name, st_shndx) after the null symbol."""
    strtab = b"\0"
    offsets = []
    for name, _ in symbols:
        offsets.append(len(strtab))
        strtab += name.encode() + b"\0"
    symtab = bytes(24)
    for (name, shndx), off in zip(symbols, offsets):
        symtab += struct.pack("<IBBHQQ", off, 0x12, 0, shndx, 0x1000 if shndx else 0, 0)
    sym_off = 0x40
    str_off = sym_off + len(symtab)
    image = bytearray(sym_off) + symtab + strtab
    dt = nso2elf.DT
    return types.SimpleNamespace(image=bytes(image), dyn={dt["SYMTAB"]: sym_off, dt["STRTAB"]: str_off},
                                 dynsym_size=len(symtab))


class Imports(unittest.TestCase):
    def test_undefined_symbols_only(self):
        mod = fake_module([("_ZN2nn2os9SleepTimeEv", 0), ("fn_defined", 5), ("memcpy", 0), ("data_def", 7)])
        with mock.patch.object(pi.nso2elf, "read_module", return_value=mod):
            self.assertEqual(pi.imports(None), ["_ZN2nn2os9SleepTimeEv", "memcpy"])

    def test_empty_table(self):
        mod = fake_module([])
        with mock.patch.object(pi.nso2elf, "read_module", return_value=mod):
            self.assertEqual(pi.imports(None), [])


if __name__ == "__main__":
    unittest.main()
