# Lua 5.1.4 (vendored)

Upstream Lua 5.1.4 from [lua.org](https://www.lua.org/ftp/lua-5.1.4.tar.gz) (MIT, see `COPYRIGHT`),
as linked into Metroid Dread's `main`. Identified by `tools/libcheck_lua.py`: built as C++ with the
project flags, 81 of 82 standard-library functions are byte-identical to the game's.

Differences from upstream:
- `luaconf.h`: `lua_Number` is `float` (marked `dread-decomp:`).
- The standalone `lua.c`, `luac.c` and `print.c` are omitted.
- The game replaces `print` (`luaB_print` differs); that replacement is game code, not part of
  this directory.

All functions here are status `L` (library) in `data/functions/`. `tools/sigmatch.py` names them
in the binary by matching these compiled objects.
