// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): the copy constructor of the reflected keyed
// array of 0x118-byte records by u32 key: takes the other's settings, reserves its count (fn_7100eae334), copies
// each record and then the keys. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/UnkNamedRegistry.h"
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

struct UnkRecord118_ae14c {
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
static_assert(sizeof(UnkRecord118_ae14c) == 0x118, "UnkRecord118_ae14c layout");

// As src/game/CMinimapManager/CTunableMinimapManager_Reserve118_b49.cpp defines it.
struct UnkKeyed118Array_b49;
void fn_7100eae334(UnkKeyed118Array_b49* self, u32 capacity);

struct UnkKeyed118Array_ae14c {
    UnkRecord118_ae14c* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::IAllocator* allocator;
};

void fn_7100eae14c(UnkKeyed118Array_ae14c* self, const UnkKeyed118Array_ae14c* other) {
    self->values = nullptr;
    self->keys = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x18 = other->field_0x18;
    self->flags = other->flags;
    self->allocator = other->allocator;
    fn_7100eae334(reinterpret_cast<UnkKeyed118Array_b49*>(self), other->count);
    self->count = other->count;
    for (u32 i = 0; i < self->count; ++i)
        new (&self->values[i]) UnkRecord118_ae14c(other->values[i]);
    memcpy(self->keys, other->keys, (u64)other->count * sizeof(u32));
    self->count = other->count;
}
