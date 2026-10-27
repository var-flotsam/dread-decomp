// From unit 2668 of tools/tu_map.py (0x710109c64c-0x710109d670): the engine's script console next to
// the embedded Lua. fn_710109c880 runs a string as a chunk (a string that starts with a space is
// printed: it runs as "print(...)"), with a scratch array pushed for the length of the run, and
// reports an error message through a callback. The start-up settings hand it to the engine
// (src/engine/HostArguments.cpp). Names and layouts are placeholders.
#include <stdio.h>
#include <string.h>

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "base/UnkVector.h"
#include "types.h"

extern lua_State* g_unk_7172a01d18;

// An array of 8-byte elements (base::UnkVector's layout) on the scratch stack below, which packs
// its arrays after its count, so their words are only 4-byte aligned.
struct __attribute__((packed, aligned(4))) UnkLuaScratch {
    u64* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u8 pad_0x15[3];
    u32 flags;
    u32 pad_0x1c;
    base::UnkAllocator* allocator;
};
static_assert(sizeof(UnkLuaScratch) == 0x28, "UnkLuaScratch layout");

struct UnkLuaScratchStack {
    u32 count;
    UnkLuaScratch entries[1];  // the real number is unknown
};
extern UnkLuaScratchStack g_unk_7172a025a0 __attribute__((visibility("hidden")));

// Pops the scratch array the last run pushed (and frees it).
void fn_710109c9e0();

// The error report: the message, then two words (both 2 here).
typedef void (*UnkLuaReportFn)(const char* message, s32 a, s32 b);

// Runs `str`; on an error, pops the error object and reports its message if there is a callback.
void fn_710109c880(const char* str, UnkLuaReportFn report) {
    base::UnkAllocator* allocator = fn_7100083990();
    u32 flags = 6;
    u64* data = static_cast<u64*>(allocator->vf_0x40(0x190 * sizeof(u64), 8, flags));
    u32 top = g_unk_7172a025a0.count;
    UnkLuaScratch& scratch = g_unk_7172a025a0.entries[top];
    scratch.data = data;
    scratch.count = 0;
    scratch.capacity = 0x190;
    scratch.step = 10;
    scratch.field_0x14 = true;
    scratch.flags = flags;
    scratch.allocator = allocator;
    g_unk_7172a025a0.count = top + 1;

    char buf[0x400];
    if (*str == ' ') {
        sprintf(buf, "print(%s)", str);
        str = buf;
    }
    // The state is read again for each call (each could replace it).
    if ((luaL_loadbuffer(g_unk_7172a01d18, str, strlen(str), str) != 0 ||
         lua_pcall(g_unk_7172a01d18, 0, LUA_MULTRET, 0) != 0) &&
        !lua_isnil(g_unk_7172a01d18, -1)) {
        const char* message = lua_tostring(g_unk_7172a01d18, -1);
        lua_pop(g_unk_7172a01d18, 1);
        if (report)
            report(message ? message : "(error object is not a string)", 2, 2);
    }
    fn_710109c9e0();
}
