// Unit 2668 (no reliable name; next to src/base/UnkLuaDoString.cpp, the engine's script console and embedded Lua):
// the scratch-array stack the script runs push (clearing it, a load wrapped in a scratch array, appending a word
// to the top array), the console's line output (fn_710109d450), a name resolved into a string with inline storage
// (fn_710109c6d0) and the Lua start-up that creates the state and the script manager (fn_710109cc50). Names and
// layouts are placeholders.
#include <new>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "base/UnkFormat.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2669.h"
#include "types.h"

struct lua_State;
#include "base/UnkFormat.h"
#include "base/UnkVector.h"
#include "types.h"

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

    // Frees the array (the elements need no destruction).
    void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    void PushBack(u64 value) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        u64* p;
        if (old_capacity < new_capacity) {
            p = static_cast<u64*>(allocator->vf_0x40((u64)new_capacity * sizeof(u64), 8, flags));
            if (data) {
                memcpy(p, data, (u64)count * sizeof(u64));
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        // Untyped store: the original reloads the count after the element is written.
        __builtin_memcpy(&p[n], &value, sizeof(value));
        ++count;
    }
};
static_assert(sizeof(UnkLuaScratch) == 0x28, "UnkLuaScratch layout");

struct UnkLuaScratchStack {
    u32 count;
    UnkLuaScratch entries[1];  // the real number is unknown
};

// Pops the scratch array the last run pushed (and frees it).
s32 fn_71010a59a0(lua_State* L, const char* s);

// Hands the pointer to the scratch array on top of the stack.
void fn_710109cfb0(u64 value) {
    UnkLuaScratchStack& stack = g_unk_7172a025a0;
    (stack.entries + stack.count)[-1].PushBack(value);
}

// Loads a chunk with a scratch array pushed for the load, popped after it; true when it loaded.
bool fn_710109cba0(const char* str) {
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
    s32 result = fn_71010a59a0(g_unk_7172a01d18, str);
    fn_710109c9e0();
    return result == 0;
}

// Frees every scratch array and empties the stack.
void fn_710109c64c(UnkLuaScratchStack* stack) {
    for (u64 i = 0; i < stack->count; ++i)
        stack->entries[i].Free();
    stack->count = 0;
}

s32 fn_710109d450(const char* format, ...);

extern char g_unk_7172a01d68[0x400] __attribute__((visibility("hidden")));

s32 fn_710109d450(const char* format, ...) {
    va_list args;
    va_start(args, format);
    fn_710007ba20(g_unk_7172a01d68, sizeof(g_unk_7172a01d68), format, args);
    va_end(args);
    return sprintf(g_unk_7172a01d68 + 0x400, "%s\n", g_unk_7172a01d68);
}

using base::global::CRntString;

extern const char g_unk_7101602004[] __attribute__((visibility("hidden")));

s32 fn_710007fb10(const base::global::CRntString* self, char c, s32 start);
void fn_710007f564(void* self, const char* str);

// A string with 0x100 bytes of inline storage (0x148 bytes), as in src/base/UnkTunableRegistry.cpp.
struct UnkInlineString_710109c6d0 {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkInlineString_710109c6d0() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        field_0x138 = 0;
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x140 = 0;
    }
    ~UnkInlineString_710109c6d0() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
};

void fn_710109c6d0(CRntString* name, UnkInlineString_710109c6d0* out) {
    fn_710007e77c(out->Str(), g_unk_710162e2ed);
    s32 dot = fn_710007fb10(name, '.', -1);
    if (dot < 0)
        return;
    const char* chars = name->data;
    char lower[0x100];
    alignas(8) u8 text_raw[sizeof(UnkInlineString_710109c6d0)];
    {
        UnkInlineString_710109c6d0 stem;
        fn_710007e7f8(stem.Str(), chars, dot);
        lower[0] = 0;
        const char* src = stem.Str()->data;
        u32 count = strlen(src);
        if (count) {
            u64 length = count;
            u64 i = 0;
            do {
                char c = src[i];
                lower[i] = (u8)(c - 'A') < 26 ? c + 0x20 : c;
            } while (++i < length);
            lower[length] = 0;
        }
        UnkInlineString_710109c6d0& text = *new (text_raw) UnkInlineString_710109c6d0;
        fn_710007e77c(text.Str(), lower);
        fn_710007eaa0(out->Str(), text.Str());
        text.~UnkInlineString_710109c6d0();
    }
    fn_710007f564(out->Str(), g_unk_7101602004);
}

struct UnkScriptManager_ovb0085;

extern "C" lua_State* luaL_newstate(void);
extern "C" int (*lua_atpanic(lua_State* L, int (*panicf)(lua_State*)))(lua_State*);

extern void* (*g_unk_7101caf820)(u64 size);
extern void (*g_unk_7101caf828)(void* ptr);
extern void (*g_unk_7101caf830)(const char* format, ...);
extern int (*g_unk_7101caf838)(const char* fmt, ...);

void* fn_7100083a70(u64 size);
void fn_7100083a50(void* ptr);
void fn_710109cd70(const char* format, ...) __attribute__((visibility("hidden")));
void fn_710109ce10(const char* format, ...) __attribute__((visibility("hidden")));
int fn_710109ceb0(lua_State* L) __attribute__((visibility("hidden")));
void fn_71010aed50(lua_State* L);
void fn_71010c10c0(lua_State* L);
void fn_7101075e40(lua_State* L);
void fn_7100fbc760(lua_State* L);
void fn_7100fbf270(lua_State* L);
void fn_7101076ed0(lua_State* L);
void fn_71010741b0(lua_State* L);
void fn_710109ff40(lua_State* L);
void fn_710109f390(lua_State* L);
void fn_7101097e40(lua_State* L);
void fn_7100fb96a0(lua_State* L);
void fn_710109e268(UnkScriptManager_ovb0085* self);

// Creates the Lua state with the game's allocation and print hooks, opens its libraries and
// returns the script manager, created on first use.
void fn_710109cc50() {
    g_unk_7101caf820 = fn_7100083a70;
    g_unk_7101caf828 = fn_7100083a50;
    g_unk_7172a01d18 = luaL_newstate();
    g_unk_7101caf830 = fn_710109cd70;
    g_unk_7101caf838 = reinterpret_cast<int (*)(const char*, ...)>(fn_710109ce10);
    lua_atpanic(g_unk_7172a01d18, fn_710109ceb0);
    fn_71010aed50(g_unk_7172a01d18);
    fn_71010c10c0(g_unk_7172a01d18);
    fn_7101075e40(g_unk_7172a01d18);
    fn_7100fbc760(g_unk_7172a01d18);
    fn_7100fbf270(g_unk_7172a01d18);
    fn_7101076ed0(g_unk_7172a01d18);
    fn_71010741b0(g_unk_7172a01d18);
    fn_710109ff40(g_unk_7172a01d18);
    fn_710109f390(g_unk_7172a01d18);
    fn_7101097e40(g_unk_7172a01d18);
    fn_7100fb96a0(g_unk_7172a01d18);
    UnkScriptManager_ovb0085* manager = g_unk_71727bed90;
    if (!manager) {
        manager = static_cast<UnkScriptManager_ovb0085*>(fn_71000839a0(0x88, 0x3f));
        fn_710109d810(manager);
        g_unk_71727bed90 = manager;
    }
    fn_710109e268(manager);
}
