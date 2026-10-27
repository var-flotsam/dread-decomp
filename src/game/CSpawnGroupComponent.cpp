// The spawn group component's unit (CSpawnGroupComponent; translation unit 2703 of
// data/tu_map.csv): fn_710111c0fc calls a script function of the group by name, remembering the
// names the script doesn't define so they aren't looked up again.
#include <stdarg.h>

#include "../game_aca57c/UnkBufferString.h"
#include "../game_s7/UnkPropBagS7.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "types.h"

using namespace unk_prop_bag;

extern void* g_unk_7101a1b750[];
using UnkString80 = UnkBufferString<0x80, g_unk_7101a1b750>;

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* str);
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args);
s32 fn_71010c0cd0(lua_State* L, s32 count, char* buf, u32 size, UnkPropBag* results);

typedef s64 __attribute__((may_alias)) a_s64;

// base::UnkVector's layout with the count in a union (no type-based alias info: the original
// reloads it after every key it stores).
struct UnkKeyVector {
    s64* data;
    union {
        u32 count;
    };
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Appends a key, growing like base::UnkVector::PushBack but copying the old keys one at a time
    // (stores that may alias anything: the original reloads the count and the array after them).
    void PushBack(s64 key) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        s64* p;
        if (old_capacity < new_capacity) {
            p = static_cast<s64*>(allocator->vf_0x40((u64)new_capacity * sizeof(s64), 8, flags));
            if (s64* old = data) {
                for (u32 i = 0; i < count; ++i)
                    reinterpret_cast<a_s64*>(p)[i] = old[i];
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        p[n] = key;
        ++count;
    }
};
static_assert(sizeof(UnkKeyVector) == 0x28, "UnkKeyVector layout");

struct UnkSpawnGroupScript {
    u8 unk_0x0[0x40];
    UnkKeyVector missing;  // the keys (CRntString::key) of names the script lacks
};

// Calls the script function `name` (spaces removed) with the arguments `fmt` describes, the
// results going to `results`; whether the call returned 1. False for no name or a name already
// known to be missing.
// NON_MATCHING(minor): scheduling: the Lua state's GOT load comes after the empty-string address
// (the original loads it first), the copy loop compares its counter in 64 bits (the original in
// 32), and the result's clearing and the count's increment swap places.
bool fn_710111c0fc(UnkSpawnGroupScript* self, UnkPropBag* results, const char* name, const char* fmt, ...) {
    char buf[0x200];
    if (!name)
        return false;
    UnkString80 str(name);
    s32 n = 0;
    const char* p = str.Str()->data;
    for (u64 left = str.Str()->length; left != 0; --left, ++p) {
        char c = *p;
        if (c != ' ')
            buf[n++] = c;
    }
    buf[n] = 0;
    {
        UnkString80 stripped(buf);
        fn_710007eaa0(str.Str(), stripped.Str());
    }
    s64 key = str.Str()->key;
    const s64* it = self->missing.data;
    for (u64 left = self->missing.count; left != 0; --left, ++it) {
        if (*it == key)
            return false;
    }
    const char* s = str.Str()->data ? str.Str()->data : g_unk_710162e2ed;
    if (fn_71010c0bb0(g_unk_7172a01d18, s) == 0) {
        self->missing.PushBack(key);
        return false;
    }
    va_list args;
    va_start(args, fmt);
    s32 r = fn_71010bf400(g_unk_7172a01d18, fmt, args);
    bool result = fn_71010c0cd0(g_unk_7172a01d18, r, buf, 0x100, results) == 1;
    va_end(args);
    return result;
}
