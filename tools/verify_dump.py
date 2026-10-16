#!/usr/bin/env python3
"""Check orig/<version>/ against the game's own internal records.

Nintendo doesn't publish dump hashes, so this checks the dump against data
inside the game itself:
  1. every NSO's text/rodata/data segments match the SHA-256 in its header;
  2. main.npdm names Metroid Dread's title ID;
  3. every RomFS file (loose, or packed inside a .pkg) is listed in
     system/files.toc with exactly its size.

files.toc is a master index with many more entries than the RomFS ships
(about 118k entries, about 40k of them present in 2.1.0), so entries with no
file are reported but are not errors. Loose textures are indexed without
their "textures/" prefix.

Usage: python tools/verify_dump.py --version 2.1.0
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import nso  # noqa: E402

from mercury_engine_data_structures.base_resource import resolve_asset_id  # noqa: E402
from mercury_engine_data_structures.formats.pkg import Pkg  # noqa: E402
from mercury_engine_data_structures.formats.toc import Toc  # noqa: E402
from mercury_engine_data_structures.game_check import Game  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
DREAD_TITLE_ID = 0x010093801237C000
NSO_FILES = ("main", "rtld", "sdk", "subsdk0")


def check_nsos(exefs: Path) -> list[str]:
    errors = []
    for name in NSO_FILES:
        path = exefs / name
        if not path.is_file():
            continue
        module = nso.load(path)
        errs = module.verify()
        errors += errs
        checked = [s.name for s in module.segments if s.check_hash]
        status = "FAIL" if errs else "ok"
        print(f"  {name:<8} {status}  build ID {module.build_id[:20].hex().upper()}  "
              f"segments hash-checked: {', '.join(checked) or 'none'}")
    return errors


def check_npdm(path: Path) -> list[str]:
    raw = path.read_bytes()
    if raw[:4] != b"META":
        return [f"main.npdm: bad magic {raw[:4]!r}"]
    name = raw[0x20:0x30].split(b"\0", 1)[0].decode("ascii", "replace")
    aci_offset, _ = struct.unpack_from("<II", raw, 0x70)
    if raw[aci_offset:aci_offset + 4] != b"ACI0":
        return ["main.npdm: ACI0 section not found"]
    (program_id,) = struct.unpack_from("<Q", raw, aci_offset + 0x10)
    print(f"  main.npdm name {name!r}, program ID {program_id:016X}")
    if program_id != DREAD_TITLE_ID:
        return [f"main.npdm: program ID {program_id:016X} is not Metroid Dread ({DREAD_TITLE_ID:016X})"]
    return []


def check_romfs(romfs: Path) -> list[str]:
    """Every files.toc asset must exist, as a loose file or inside a .pkg, with the listed size.

    Assets can appear in several packs. Each copy is checked.
    """
    toc = Toc.parse((romfs / Toc.system_files_name()).read_bytes(), Game.DREAD)
    expected = toc.raw.files
    pkg_header = Pkg.header_class(Game.DREAD)
    errors = []
    seen = set()
    loose = packed = unlisted = pkgs = 0

    def check(asset_id: int, size: int, where: str) -> bool:
        want = expected.get(asset_id)
        if want is None:
            return False
        if want != size:
            errors.append(f"{where}: size {size} != files.toc {want}")
        seen.add(asset_id)
        return True

    for p in romfs.rglob("*"):
        if not p.is_file():
            continue
        rel = p.relative_to(romfs).as_posix()
        size = p.stat().st_size
        asset_path = rel.removeprefix("textures/") if p.suffix == ".bctex" else rel
        if check(resolve_asset_id(asset_path, Game.DREAD), size, f"romfs/{rel}"):
            loose += 1
        else:
            unlisted += 1
        if p.suffix == ".pkg":
            pkgs += 1
            with p.open("rb") as f:
                header = pkg_header.parse_stream(f)
            for e in header.file_entries:
                if e.end_offset > size or e.start_offset > e.end_offset:
                    errors.append(f"romfs/{rel}: entry {e.asset_id:016X} out of bounds")
                elif check(e.asset_id, e.end_offset - e.start_offset, f"romfs/{rel}:{e.asset_id:016X}"):
                    packed += 1
    print(f"  files.toc lists {len(expected)} assets; {len(seen)} present "
          f"({loose} loose files, {packed} entries across {pkgs} .pkg files), "
          f"{len(expected) - len(seen)} not shipped (informational)")
    print(f"  {unlisted} on-disk files not in files.toc; {len(errors)} size/bounds errors")
    # files.toc doesn't list itself; anything else unlisted is suspicious.
    if unlisted > 1:
        errors.append(f"romfs: {unlisted} files on disk are not listed in files.toc")
    return errors


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    base = ROOT / "orig" / args.version

    errors = []
    print("ExeFS NSO segment hashes:")
    errors += check_nsos(base / "exefs")
    print("Title ID:")
    errors += check_npdm(base / "exefs" / "main.npdm")
    print("RomFS vs system/files.toc:")
    errors += check_romfs(base / "romfs")

    if errors:
        print("\nFAILED:\n  " + "\n  ".join(errors[:50]))
        return 1
    print("\nAll internal consistency checks passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
