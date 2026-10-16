#!/usr/bin/env python3
"""Copy a user-supplied ExeFS/RomFS dump into orig/<version>/ and verify it.

Usage:
    python tools/setup.py --version 2.1.0 --exefs <dir> --romfs <dir>
    python tools/setup.py --version 2.1.0 --verify-only

On the first run for a version, data/hashes.json gets a new entry generated
from the dump. The entry is marked "confirmed": false until you check the
printed hashes and re-run with --confirm. Later runs must match exactly.

Only hashes and the NSO build ID are written to tracked files. Game data is
copied into orig/, which is gitignored.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
ORIG = ROOT / "orig"
HASHES = ROOT / "data" / "hashes.json"

# ExeFS files every dump must contain. subsdk* files are optional and vary.
REQUIRED_EXEFS = ("main", "main.npdm", "rtld", "sdk")

NSO_MAGIC = b"NSO0"
NSO_BUILD_ID_OFFSET = 0x40
NSO_BUILD_ID_SIZE = 0x20


class SetupError(Exception):
    pass


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def nso_build_id(path: Path) -> str:
    with path.open("rb") as f:
        header = f.read(NSO_BUILD_ID_OFFSET + NSO_BUILD_ID_SIZE)
    if header[:4] != NSO_MAGIC:
        raise SetupError(f"{path} is not an NSO (bad magic {header[:4]!r})")
    return header[NSO_BUILD_ID_OFFSET:NSO_BUILD_ID_OFFSET + NSO_BUILD_ID_SIZE].hex()


def exefs_files(exefs: Path) -> list[str]:
    missing = [n for n in REQUIRED_EXEFS if not (exefs / n).is_file()]
    if missing:
        raise SetupError(f"ExeFS dump {exefs} is missing: {', '.join(missing)}")
    subsdks = sorted(p.name for p in exefs.glob("subsdk*") if p.is_file())
    return list(REQUIRED_EXEFS) + subsdks


def hash_exefs(exefs: Path) -> dict:
    files = exefs_files(exefs)
    return {
        "main_build_id": nso_build_id(exefs / "main"),
        "files": {name: sha256_file(exefs / name) for name in files},
    }


def hash_romfs(romfs: Path) -> tuple[dict, list[str]]:
    """Return a compact summary plus the full manifest lines.

    The manifest (one "sha256  relpath" line per file, sorted) is hashed as a
    whole so hashes.json stays small no matter how many files the RomFS has.
    """
    if not romfs.is_dir():
        raise SetupError(f"RomFS dump {romfs} is not a directory")
    lines = []
    total = 0
    for p in sorted(romfs.rglob("*")):
        if p.is_file():
            rel = p.relative_to(romfs).as_posix()
            lines.append(f"{sha256_file(p)}  {rel}")
            total += p.stat().st_size
    if not lines:
        raise SetupError(f"RomFS dump {romfs} is empty")
    manifest = "\n".join(lines) + "\n"
    summary = {
        "file_count": len(lines),
        "total_bytes": total,
        "manifest_sha256": hashlib.sha256(manifest.encode()).hexdigest(),
    }
    return summary, lines


def load_hashes() -> dict:
    if HASHES.is_file():
        return json.loads(HASHES.read_text(encoding="utf-8"))
    return {}


def save_hashes(db: dict) -> None:
    HASHES.parent.mkdir(parents=True, exist_ok=True)
    HASHES.write_text(json.dumps(db, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")


def which_version(db: dict, build_id: str) -> str | None:
    for ver, entry in db.items():
        if entry.get("exefs", {}).get("main_build_id") == build_id:
            return ver
    return None


def compare(expected: dict, actual: dict, label: str) -> list[str]:
    errors = []
    if label == "exefs":
        if expected["main_build_id"] != actual["main_build_id"]:
            errors.append(
                f"main build ID {actual['main_build_id']} != expected {expected['main_build_id']}"
            )
        exp_files, act_files = expected["files"], actual["files"]
        for name in sorted(set(exp_files) | set(act_files)):
            if name not in act_files:
                errors.append(f"exefs/{name}: missing")
            elif name not in exp_files:
                errors.append(f"exefs/{name}: unexpected file")
            elif exp_files[name] != act_files[name]:
                errors.append(f"exefs/{name}: sha256 mismatch")
    else:
        for key in ("file_count", "total_bytes", "manifest_sha256"):
            if expected[key] != actual[key]:
                errors.append(f"romfs {key}: {actual[key]} != expected {expected[key]}")
    return errors


def copy_tree(src: Path, dst: Path) -> None:
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(src, dst)


def print_entry(version: str, entry: dict) -> None:
    ex = entry["exefs"]
    print(f"\nHashes for version {version}:")
    print(f"  main build ID   {ex['main_build_id']}")
    for name, digest in ex["files"].items():
        print(f"  exefs/{name:<10} {digest}")
    ro = entry["romfs"]
    print(f"  romfs           {ro['file_count']} files, {ro['total_bytes']} bytes")
    print(f"  romfs manifest  {ro['manifest_sha256']}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True, help="game version, e.g. 2.1.0")
    ap.add_argument("--exefs", type=Path, help="ExeFS dump directory")
    ap.add_argument("--romfs", type=Path, help="RomFS dump directory")
    ap.add_argument("--verify-only", action="store_true",
                    help="verify orig/<version>/ against data/hashes.json without copying")
    ap.add_argument("--confirm", action="store_true",
                    help="mark this version's hashes as confirmed by the user")
    args = ap.parse_args()

    dest = ORIG / args.version
    if args.verify_only:
        exefs, romfs = dest / "exefs", dest / "romfs"
    else:
        if not args.exefs or not args.romfs:
            ap.error("--exefs and --romfs are required unless --verify-only is given")
        exefs, romfs = args.exefs.resolve(), args.romfs.resolve()

    db = load_hashes()

    print(f"Hashing ExeFS at {exefs} ...")
    ex = hash_exefs(exefs)

    # Catch a dump of the wrong version before hashing the (large) RomFS.
    known = which_version(db, ex["main_build_id"])
    if known is not None and known != args.version:
        raise SetupError(
            f"version mismatch: this dump's main build ID matches version {known}, "
            f"not {args.version}. Pass --version {known} or supply the {args.version} dump."
        )
    if args.version in db and known is None:
        raise SetupError(
            f"version mismatch: main build ID {ex['main_build_id']} does not match the "
            f"recorded build ID for {args.version} "
            f"({db[args.version]['exefs']['main_build_id']}). Wrong version or patched dump?"
        )

    print(f"Hashing RomFS at {romfs} (this can take a while) ...")
    ro, manifest = hash_romfs(romfs)
    actual = {"exefs": ex, "romfs": ro}

    if args.version in db:
        expected = db[args.version]
        errors = compare(expected["exefs"], ex, "exefs") + compare(expected["romfs"], ro, "romfs")
        if errors:
            raise SetupError("hash verification failed:\n  " + "\n  ".join(errors))
        print("All hashes match data/hashes.json.")
        if args.confirm and not expected.get("confirmed"):
            expected["confirmed"] = True
            save_hashes(db)
            print(f"Marked {args.version} as confirmed.")
        elif not expected.get("confirmed"):
            print(f"Note: {args.version} hashes are not confirmed yet; re-run with --confirm once checked.")
    else:
        if args.verify_only:
            raise SetupError(f"no recorded hashes for {args.version}; run without --verify-only first")
        actual["confirmed"] = False
        db[args.version] = actual
        save_hashes(db)
        print_entry(args.version, actual)
        print(f"\nFirst run for {args.version}: wrote data/hashes.json (unconfirmed).")
        print("Check these hashes against a trusted source, then re-run with --confirm.")

    if not args.verify_only:
        print(f"Copying into {dest} ...")
        dest.mkdir(parents=True, exist_ok=True)
        copy_tree(exefs, dest / "exefs")
        copy_tree(romfs, dest / "romfs")

    # The full per-file manifest goes to build/, never to tracked files.
    out = ROOT / "build" / args.version
    out.mkdir(parents=True, exist_ok=True)
    (out / "romfs_manifest.txt").write_text("\n".join(manifest) + "\n", encoding="utf-8")
    print("Done.")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except SetupError as e:
        print(f"error: {e}", file=sys.stderr)
        sys.exit(1)
