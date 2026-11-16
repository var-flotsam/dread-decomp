// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): the add-copy of the reflected keyed
// array of 0x118-byte records by u32 key: grows by its step when full (fn_7100eae334), then appends the
// key and a copy of the record. Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

// A word copied on its own (a value type with its own copy constructor).
struct UnkWord118 {
    u32 value;

    UnkWord118(const UnkWord118& other) : value(other.value) {}
};

extern void* g_unk_7101a1b5a0[];
using UnkString20_118 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

struct UnkRecord118_adb34 {
    u64 field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    UnkWord118 field_0x20[7];
    u64 field_0x40;
    u64 field_0x48;
    u32 field_0x50;
    CRntString field_0x58;  // copied by fn_710007ea60
    UnkWord118 field_0x88;
    u64 field_0x90;
    u32 field_0x98;
    UnkString20_118 field_0xa0;
    UnkWord118 field_0x108;
    alignas(8) u32 field_0x110;
    u16 field_0x114;
};
static_assert(sizeof(UnkRecord118_adb34) == 0x118, "UnkRecord118_adb34 layout");

// As src/game/CMinimapManager/CTunableMinimapManager_Reserve118_b49.cpp defines it.
struct UnkKeyed118Array_b49;
void fn_7100eae334(UnkKeyed118Array_b49* self, u32 capacity);

struct UnkKeyed118Array_adb34 {
    UnkRecord118_adb34* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_7100eadb34(UnkKeyed118Array_adb34* self, const u32* key, const UnkRecord118_adb34* value) {
    if (self->count == self->capacity)
        fn_7100eae334(reinterpret_cast<UnkKeyed118Array_b49*>(self), self->step + self->count);
    new (&self->keys[self->count]) u32(*key);
    new (&self->values[self->count]) UnkRecord118_adb34(*value);
    self->count++;
}
