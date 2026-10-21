# Lua 5.1.4 (vendored)

Upstream Lua 5.1.4 from [lua.org](https://www.lua.org/ftp/lua-5.1.4.tar.gz) (MIT, see `COPYRIGHT`),
as linked into Metroid Dread's `main`. Identified by `tools/libcheck_lua.py`: built as C++ with the
project flags, 81 of 82 standard-library functions are byte-identical to the game's.

Differences from upstream:
- `luaconf.h`: `lua_Number` is `float`, `lua_str2number` is `(float)strtod`, and the long-jump buffer is
  16-byte aligned (marked `dread-decomp:`).
- `lobject.h`: `twoto(x)` is `1L<<(x)` (64-bit node counts and masks; `ltable` matches only with it).
- Build flag: the game's Lua has no type-based alias analysis (`-fno-strict-aliasing`,
  `buildcfg.LUA_FLAGS`); `lcode`, `llex` and `lparser` match completely only with it.
- `lundump.c`: the game's loader is the older one that byte-swaps chunks of the other endianness
  (`LoadState.swap`, `LoadMem` with a swap loop, `LoadHeader` ignores header byte 6) and has no
  `nCcalls` depth check in `LoadFunction`; booleans are stored without `!= 0` (marked `dread-decomp:`).
- The standalone `lua.c`, `luac.c` and `print.c` are omitted.
- The game replaces `print` (`luaB_print` differs); that replacement is game code, not part of
  this directory.

All functions here are status `L` (library) in `data/functions/`. `tools/sigmatch.py` names them
in the binary by matching these compiled objects.
