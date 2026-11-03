"""tools/fetch_sources.py with urllib replaced by in-memory archives (no network).

DEST and PINS are patched to a temporary folder, so neither the cache nor data/sources.json is
touched.
"""

import contextlib
import hashlib
import io
import json
import tarfile
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.tests import _path  # noqa: F401
import fetch_sources


def tarball(files: dict[str, bytes]) -> bytes:
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        for name, data in files.items():
            info = tarfile.TarInfo(name)
            info.size = len(data)
            tar.addfile(info, io.BytesIO(data))
    return buf.getvalue()


class Sources(unittest.TestCase):
    def test_lua_releases(self):
        lua = sorted(n for n in fetch_sources.SOURCES if n.startswith("lua-"))
        self.assertEqual(lua, ["lua-5.1", "lua-5.1.1", "lua-5.1.2", "lua-5.1.3", "lua-5.1.4", "lua-5.1.5"])
        self.assertEqual(fetch_sources.SOURCES["lua-5.1.4"], "https://www.lua.org/ftp/lua-5.1.4.tar.gz")

    def test_all_urls_https(self):
        for name, url in fetch_sources.SOURCES.items():
            self.assertTrue(url.startswith("https://"), name)

    def test_single_files_are_sources(self):
        self.assertLessEqual(set(fetch_sources.SINGLE_FILES), set(fetch_sources.SOURCES))

    def test_pinned_names_known(self):
        if fetch_sources.PINS.is_file():
            pins = json.loads(fetch_sources.PINS.read_text())
            self.assertLessEqual(set(pins), set(fetch_sources.SOURCES))
            for v in pins.values():
                self.assertRegex(v["sha256"], r"^[0-9a-f]{64}$")


class Fetch(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.dest = Path(self.tmp.name) / "sources"
        self.pins_file = Path(self.tmp.name) / "sources.json"
        self.payload = {}
        self.urls = []
        for p in (mock.patch.object(fetch_sources, "DEST", self.dest),
                  mock.patch.object(fetch_sources, "PINS", self.pins_file),
                  mock.patch.object(fetch_sources.urllib.request, "urlopen", self.fake_urlopen)):
            p.start()
            self.addCleanup(p.stop)

    def fake_urlopen(self, url, timeout=None):
        self.urls.append(url)
        return io.BytesIO(self.payload[url])

    def fetch(self, name, pins):
        with contextlib.redirect_stdout(io.StringIO()):
            fetch_sources.fetch(name, pins)

    def test_source_dir(self):
        self.assertEqual(fetch_sources.source_dir("lua-5.1.4"), self.dest / "lua-5.1.4")

    def test_tarball_extracted_and_pinned(self):
        url = fetch_sources.SOURCES["lua-5.1.4"]
        self.payload[url] = tarball({"lua-5.1.4/README": b"made up\n"})
        pins = {}
        self.fetch("lua-5.1.4", pins)
        self.assertEqual((self.dest / "lua-5.1.4" / "README").read_bytes(), b"made up\n")
        self.assertEqual(pins["lua-5.1.4"], {"url": url, "sha256": hashlib.sha256(self.payload[url]).hexdigest()})

    def test_already_present_not_downloaded(self):
        (self.dest / "lua-5.1.4").mkdir(parents=True)
        self.fetch("lua-5.1.4", {})
        self.assertEqual(self.urls, [])

    def test_pin_mismatch_rejected(self):
        url = fetch_sources.SOURCES["lua-5.1.4"]
        self.payload[url] = tarball({"lua-5.1.4/README": b"x"})
        with self.assertRaisesRegex(SystemExit, "does not match pinned"):
            self.fetch("lua-5.1.4", {"lua-5.1.4": {"sha256": "0" * 64}})
        self.assertFalse((self.dest / "lua-5.1.4").exists())

    def test_archive_without_expected_folder(self):
        url = fetch_sources.SOURCES["lua-5.1.3"]
        self.payload[url] = tarball({"other/README": b"x"})
        with self.assertRaisesRegex(SystemExit, "expected"):
            self.fetch("lua-5.1.3", {})

    def test_unsafe_archive_member_refused(self):
        url = fetch_sources.SOURCES["lua-5.1.2"]
        self.payload[url] = tarball({"../escape.txt": b"x"})
        with self.assertRaises(tarfile.TarError):
            self.fetch("lua-5.1.2", {})
        self.assertFalse((Path(self.tmp.name) / "escape.txt").exists())

    def test_single_file_saved_under_its_name(self):
        url = fetch_sources.SOURCES["dlmalloc-2.8.4"]
        self.payload[url] = b"/* made-up allocator */\n"
        self.fetch("dlmalloc-2.8.4", {})
        self.assertEqual((self.dest / "dlmalloc-2.8.4" / "malloc.c").read_bytes(), self.payload[url])

    def test_main_writes_pins_even_on_failure(self):
        url = fetch_sources.SOURCES["lua-5.1.5"]
        self.payload[url] = tarball({"lua-5.1.5/x": b"1"})
        with mock.patch("sys.argv", ["fetch_sources.py", "lua-5.1.5", "lua-5.1.1"]), \
                contextlib.redirect_stdout(io.StringIO()), self.assertRaises(KeyError):
            fetch_sources.main()  # lua-5.1.1 has no payload: the fake download raises
        pins = json.loads(self.pins_file.read_text())
        self.assertEqual(set(pins), {"lua-5.1.5"})


if __name__ == "__main__":
    unittest.main()
