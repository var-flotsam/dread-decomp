// Calls a script function by name: strips the spaces from the name, looks the function up and,
// when found, pushes the variadic arguments described by `fmt` and runs it, collecting the
// results in a property bag. The result is whether the call returned 1. Two identical copies.
#include <stdarg.h>

#include "../game_s7/UnkPropBagS7.h"
#include "base/global/CRntString.h"
#include "types.h"

using namespace unk_prop_bag;

struct UnkStrStorage {
    void* vtable;
    u8 pad_0x8[0x100];
    void* field_0x108;
    u32 field_0x110;
};

// A string (header of 0x30 bytes) with 0x100 bytes of inline storage.
struct UnkStrBuffer {
    const char* data;
    u32 length;
    u8 pad_0xc[0x24];
    UnkStrStorage storage;

    // The string part (its constructor and destructor run by hand).
    base::global::CRntString* Str() { return reinterpret_cast<base::global::CRntString*>(this); }

    UnkStrBuffer() {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage));
        storage.vtable = g_unk_7101a194b8 + 2;
        storage.field_0x108 = nullptr;
        storage.field_0x110 = 0;
    }
    ~UnkStrBuffer() {
        fn_710007ed40(Str());
        fn_710007ecd4(Str());
    }
    const char* c_str() const { return data ? data : ""; }
};

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* str);
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args);
s32 fn_71010c0cd0(lua_State* L, s32 count, char* buf, u32 size, UnkPropBag* results);

// The body of both copies (va_start needs the variadic function itself).
#define UNK_CALL_SCRIPT(results, name, fmt)                                          \
    char buf[0x200];                                                                 \
    if (!name)                                                                       \
        return false;                                                                \
    UnkStrBuffer str;                                                                \
    fn_710007e77c(str.Str(), name);                                                  \
    s32 n = 0;                                                                       \
    const char* p = str.data;                                                        \
    for (u64 left = str.length; left != 0; --left, ++p) {                            \
        char c = *p;                                                                 \
        if (c != ' ')                                                                \
            buf[n++] = c;                                                            \
    }                                                                                \
    buf[n] = 0;                                                                      \
    {                                                                                \
        UnkStrBuffer stripped;                                                       \
        fn_710007e77c(stripped.Str(), buf);                                          \
        fn_710007eaa0(str.Str(), stripped.Str());                                    \
    }                                                                                \
    bool result = false;                                                             \
    if (fn_71010c0bb0(g_unk_7172a01d18, str.c_str()) != 0) {                         \
        va_list args;                                                                \
        va_start(args, fmt);                                                         \
        s32 r = fn_71010bf400(g_unk_7172a01d18, fmt, args);                          \
        result = fn_71010c0cd0(g_unk_7172a01d18, r, buf, 0x100, results) == 1;       \
        va_end(args);                                                                \
    }                                                                                \
    return result;

bool fn_7100dfb758(UnkPropBag* results, const char* name, const char* fmt, ...) {
    UNK_CALL_SCRIPT(results, name, fmt)
}

bool fn_7100e03d78(UnkPropBag* results, const char* name, const char* fmt, ...) {
    UNK_CALL_SCRIPT(results, name, fmt)
}
