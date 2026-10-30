// From unit 2668 of tools/tu_map.py (0x710109c64c-0x710109d670): the engine's script console next to
// the embedded Lua. fn_710109c880 runs a string as a chunk (a string that starts with a space is
// printed: it runs as "print(...)"), with a scratch array pushed for the length of the run, and
// reports an error message through a callback; fn_710109c9e0 pops that array again. The start-up settings hand it to the engine
// (src/engine/HostArguments.cpp). Names and layouts are placeholders.
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "base/UnkFormat.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
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

    // The array as a new slot of the stack gets it: empty, with room for 400 elements.
    void Init() {
        data = nullptr;
        count = 0;
        step = 10;
        field_0x14 = true;
        flags = 6;
        allocator = fn_7100083990();
        if (capacity < 400) {
            u64* p = static_cast<u64*>(allocator->vf_0x40(400 * sizeof(u64), 8, flags));
            if (data) {
                memcpy(p, data, (u64)count * sizeof(u64));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = 400;
        }
    }

    // Frees the array (the elements need no destruction).
    void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
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

// Releases the values in the array the last run pushed, then shrinks the stack by one, freeing the
// array. The shrink is the stack's general resize, so a pop of an empty stack would build
// 0xffffffff new slots, as in the original.
// NON_MATCHING(minor): register allocation and addressing only: the original addresses the slots
// through a second base (0x7172a01d20) and keeps the slot pointer at the slot's data rather than
// its count, so the offsets and registers differ.
void fn_710109c9e0() {
    UnkLuaScratchStack& stack = g_unk_7172a025a0;
    UnkLuaScratch& top = (stack.entries + stack.count)[-1];
    for (u64* it = top.data, *end = top.data + top.count; it != end; ++it)
        fn_71000a1c60(reinterpret_cast<base::reflection::UnkReflValue*>(*it));
    u32 old_count = stack.count;
    u32 new_count = old_count - 1;
    if (old_count < new_count) {
        for (u32 i = old_count; i != new_count; ++i)
            stack.entries[i].Init();
    } else {
        for (u64 i = new_count; i < stack.count; ++i)
            stack.entries[i].Free();
    }
    stack.count = new_count;
}

// Formats into a buffer (vsnprintf's arguments), and the console's line output.
s32 fn_710109d4f0(const char* format, ...);

// Prints a formatted line to the console, unless the format is a bare newline.
// NON_MATCHING(minor): the variadic saves and the va_list set-up are scheduled differently (one more move).
void fn_710109ce10(const char* format, ...) {
    if (*format == '\n')
        return;
    char line[0x200];
    va_list args;
    va_start(args, format);
    fn_710007ba20(line, sizeof(line), format, args);
    va_end(args);
    fn_710109d4f0(line);
}

// The console's line output: formats into its 0x400-byte line and keeps a copy with a newline (the
// two buffers follow each other in the .bss). Written by hand: it hands a va_list to fn_710007ba20,
// which on the port must be the host's.
extern char g_unk_7172a01d68[0x400];
extern char g_unk_7172a02168[];
// NON_MATCHING(minor): the variadic register saves and the va_list set-up are laid out differently;
// same behaviour.
s32 fn_710109d4f0(const char* format, ...) {
    va_list args;
    va_start(args, format);
    fn_710007ba20(g_unk_7172a01d68, sizeof(g_unk_7172a01d68), format, args);
    return sprintf(g_unk_7172a02168, "%s\n", g_unk_7172a01d68);
}

// lua_pcall with a scratch array pushed for the call's reflected values and popped after it, as
// fn_710109c880 runs a string. The state is the caller's.
s32 fn_710109cef0(lua_State* L, s32 nargs, s32 nresults, s32 errfunc) {
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
    s32 result = lua_pcall(L, nargs, nresults, errfunc);
    fn_710109c9e0();
    return result;
}
