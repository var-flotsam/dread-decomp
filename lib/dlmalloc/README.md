# dlmalloc 2.8.4 (vendored)

Doug Lea's malloc, version 2.8.4 (public domain; see the header of `malloc.c`), unmodified. It is
the heap behind the default allocator's sub-allocators (`include/base/UnkDefaultAllocator.h`). The
author's copy is <https://gee.cs.oswego.edu/pub/misc/malloc-2.8.4.c>; this one comes from a mirror
of every release, pinned to a commit and a sha256 in `data/sources.json`
(`python tools/fetch_sources.py dlmalloc-2.8.4` downloads it again for comparison).

The version is 2.8.4 because the mspace layout fits it, and not 2.8.3 or 2.8.5 and later.

`mspace.c` holds the game's build settings, inferred from the code, and includes `malloc.c`:
- mspaces only, with no locks, no `mmap` and no `MORECORE`, and no trimming. Each space is created
  over a range the caller owns. The game's `mparams` hold a 4 KiB page, a 64 KiB granularity, and
  no mmap or trim thresholds.
- 16-byte alignment.
- The error actions do nothing, and allocation failure doesn't set `errno`. The game's functions
  call nothing but `memset`.
- `mparams.magic` is the constant `0x58585858`, with no call to `time`.
- No type-based alias analysis (`-fno-strict-aliasing`, as the game's Lua).

With these settings, `create_mspace_with_base`, `mspace_malloc` and `mspace_memalign` are
identical to the game's. In `mspace_free`, the game's `unlink_large_chunk` steps from `child[1]`
to `child[0]` with a separate subtraction (8 bytes longer). The game links only these four
functions.

All functions here are status `L` (library) in `data/functions/`. `tools/sigmatch.py --library
dlmalloc` names them in the binary by matching these compiled objects.
