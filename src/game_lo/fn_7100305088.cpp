// Formats "<prefix>.<name>" (the prefix at +0x98 may be null) and looks it up in a global
// context. Names already recorded as missing return false; a name the lookup rejects is recorded
// by its 64-bit hash. Otherwise the variadic arguments are passed on with `fmt`, and the result
// is whether the final call returns 1.
#include <stdarg.h>
#include <stdio.h>

#include "base/UnkVector.h"

// A 64-bit name hash (CRC-64 of the string), built out of line. The value is a union member:
// the original reloads the array's count after each element store.
struct UnkNameHash {
    union {
        u64 value;
    };
    explicit UnkNameHash(const char* str);  // fn_710006663c
    UnkNameHash(UnkNameHash&& other) : value(other.value) {}
};

struct UnkScriptCaller {
    u8 field_0x0[0x30];
    void* field_0x30;
    u8 field_0x38[0x8];
    base::UnkVector<UnkNameHash> missing;  // +0x40
    u8 field_0x68[0x30];
    const char* prefix;  // +0x98
};

// The global context object, reached through the GOT (named after its GOT slot).
extern void* g_unk_7101cba098;

s32 fn_71010c0bb0(void* ctx, const char* name);
void fn_7100fbc4e0(void* ctx, void* arg);
s32 fn_71010bf400(void* ctx, const char* fmt, va_list args);
s32 fn_71010c0d80(void* ctx, s32 count, char* out, s32 out_size, void* a, void* b);

bool fn_7100305088(UnkScriptCaller* self, const char* name, const char* fmt, ...) {
    if (!name)
        return false;
    char full[0x100];
    sprintf(full, "%s.%s", self->prefix ? self->prefix : "", name);
    UnkNameHash id(full);
    const UnkNameHash* it = self->missing.data;
    for (u64 n = self->missing.count; n != 0; --n, ++it)
        if (it->value == id.value)
            return false;
    bool result = false;
    if (fn_71010c0bb0(g_unk_7101cba098, full) != 0) {
        fn_7100fbc4e0(g_unk_7101cba098, self->field_0x30);
        va_list args;
        va_start(args, fmt);
        s32 n = fn_71010bf400(g_unk_7101cba098, fmt, args) + 1;
        va_end(args);
        char out[0x100];
        result = fn_71010c0d80(g_unk_7101cba098, n, out, 0x100, nullptr, nullptr) == 1;
    } else {
        self->missing.EmplaceBack(static_cast<UnkNameHash&&>(id));
    }
    return result;
}
