"""tools/hostenv.py: cache folder, executable names and llvm-cxxfilt lookup, per platform.

The platform flags, the home folder, the environment and PATH lookup are patched, so every branch
runs on any host and nothing outside a temporary folder is touched.
"""

import os
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import hostenv


class Base(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.home = Path(self.tmp.name) / "home"
        self.home.mkdir()
        for p in (mock.patch.object(Path, "home", lambda: self.home),):
            p.start()
            self.addCleanup(p.stop)

    def platform(self, windows=False, macos=False):
        stack = [mock.patch.object(hostenv, "WINDOWS", windows), mock.patch.object(hostenv, "MACOS", macos)]
        for p in stack:
            p.start()
            self.addCleanup(p.stop)


class CacheDir(Base):
    def test_windows_uses_localappdata(self):
        self.platform(windows=True)
        with mock.patch.dict(os.environ, {"LOCALAPPDATA": str(self.home / "AppData")}):
            self.assertEqual(hostenv.cache_dir(), self.home / "AppData" / "dread-decomp")

    def test_windows_without_localappdata_falls_back_to_home(self):
        self.platform(windows=True)
        env = {k: v for k, v in os.environ.items() if k != "LOCALAPPDATA"}
        with mock.patch.dict(os.environ, env, clear=True):
            self.assertEqual(hostenv.cache_dir(), self.home / "dread-decomp")

    def test_macos(self):
        self.platform(macos=True)
        self.assertEqual(hostenv.cache_dir(), self.home / "Library" / "Caches" / "dread-decomp")

    def test_linux_xdg(self):
        self.platform()
        with mock.patch.dict(os.environ, {"XDG_CACHE_HOME": str(self.home / "xdg")}):
            self.assertEqual(hostenv.cache_dir(), self.home / "xdg" / "dread-decomp")

    def test_linux_default_and_empty_xdg(self):
        self.platform()
        with mock.patch.dict(os.environ, {"XDG_CACHE_HOME": ""}):
            self.assertEqual(hostenv.cache_dir(), self.home / ".cache" / "dread-decomp")

    def test_linux_legacy_folder_wins(self):
        self.platform()
        (self.home / "dread-decomp").mkdir()
        with mock.patch.dict(os.environ, {"XDG_CACHE_HOME": str(self.home / "xdg")}):
            self.assertEqual(hostenv.cache_dir(), self.home / "dread-decomp")


class Exe(Base):
    def test_suffix_only_on_windows(self):
        self.platform(windows=True)
        self.assertEqual(hostenv.exe("clang"), "clang.exe")
        self.platform()
        self.assertEqual(hostenv.exe("clang"), "clang")


class Cxxfilt(Base):
    def setUp(self):
        super().setUp()
        self.platform()
        self.which = mock.patch.object(hostenv.shutil, "which", return_value=None)
        self.which.start()
        self.addCleanup(self.which.stop)
        p = mock.patch.object(hostenv, "_BREW_LLVM", (str(self.home / "brew1"), str(self.home / "brew2")))
        p.start()
        self.addCleanup(p.stop)
        with mock.patch.dict(os.environ, {"XDG_CACHE_HOME": ""}):
            self.toolchains = hostenv.cache_dir() / "toolchains"
        env = mock.patch.dict(os.environ, {"XDG_CACHE_HOME": ""})
        env.start()
        self.addCleanup(env.stop)

    def touch(self, path: Path) -> Path:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text("")
        return path

    def test_path_lookup_first(self):
        self.touch(self.home / "brew1" / "llvm-cxxfilt")
        with mock.patch.object(hostenv.shutil, "which", return_value="/usr/bin/llvm-cxxfilt"):
            self.assertEqual(hostenv.cxxfilt(), "/usr/bin/llvm-cxxfilt")

    def test_homebrew_in_order(self):
        self.touch(self.home / "brew2" / "llvm-cxxfilt")
        self.assertEqual(hostenv.cxxfilt(), str(self.home / "brew2" / "llvm-cxxfilt"))

    def test_project_toolchain_in_reverse_name_order(self):
        self.touch(self.toolchains / "llvm-9.0.1" / "bin" / "llvm-cxxfilt")
        self.touch(self.toolchains / "llvm-10.0.0" / "bin" / "llvm-cxxfilt")
        (self.toolchains / "llvm-9.9.9" / "bin").mkdir(parents=True)  # no cxxfilt: skipped
        # Folders are tried by reverse string order, so "llvm-9.0.1" comes before "llvm-10.0.0".
        self.assertEqual(hostenv.cxxfilt(), str(self.toolchains / "llvm-9.0.1" / "bin" / "llvm-cxxfilt"))

    def test_fallback_bare_name(self):
        self.assertEqual(hostenv.cxxfilt(), "llvm-cxxfilt")


class Platform(unittest.TestCase):
    def test_platform_name_consistent(self):
        expected = "windows" if hostenv.WINDOWS else "macos" if hostenv.MACOS else "linux"
        self.assertEqual(hostenv.PLATFORM, expected)


if __name__ == "__main__":
    unittest.main()
