# libc++ (adapted excerpt)

`sort.h` holds `std::__sort` from libc++'s `<algorithm>`, LLVM 9.0.1
(`libcxx/include/algorithm`, tag `llvmorg-9.0.1`; the body is unchanged through LLVM 12), under
the Apache License v2.0 with LLVM Exceptions (`LICENSE.TXT`, copied from the same tag).

Changes, made so the game's copy can be reconstructed (the owner approved copying it, 2026-10-22):

- It lives in namespace `libcxx_adapted` (as `libcxx_adapted::__sort`), so it doesn't clash with
  the build's own libc++; game code instantiates it explicitly.
- The pivot offsets halve the length as unsigned (`(unsigned long)len / 2`), as the game's
  copy does.
- `__insertion_sort_3` is written out inline (the game's copy inlines it), with the same steps.
- `_VSTD::` and the libc++ attribute macros are dropped; comments are trimmed.
