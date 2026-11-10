// Unit 2676 of tools/tu_map.py (the game's additions to Lua's auxiliary library, src/base/UnkLuaAux.cpp): a printf to
// stdout that returns nothing. Names are placeholders.
#include <stdarg.h>
#include <stdio.h>

void fn_71010a59e0(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}
