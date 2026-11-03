// CMinimapManager::CTunableMinimapManager's unit (tools/tu_map.py): the Reserve of the keyed array of
// 0x118-byte records by u32 key (src/game/CMinimapManager/CTunableMinimapManager_Keyed118_b45.cpp):
// the records are moved over and destroyed (fn_7100ea7034), the keys copied, the old arrays freed.
// A record's move: a word, three string ids taken over, plain words and blocks, a string moved
// (fn_710007eab4), and a string with 0x20 bytes of storage copied. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;

// As src/game/logic/collision/CCollider/fn_7100d3d424.cpp declares it.
struct UnkRecord118;
void fn_7100ea7034(UnkRecord118* self);

struct UnkMovedStrId118 {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId118(UnkMovedStrId118&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

// Copied as a block of N bytes.
template <u32 N>
struct UnkBlock118 {
    u8 data[N];

    __attribute__((always_inline)) UnkBlock118(const UnkBlock118& other) { __builtin_memcpy(this, &other, N); }
};

// A string moved by its move constructor (fn_710007eab4).
struct UnkMovedString118 {
    u8 data[0x30];

    __attribute__((always_inline)) UnkMovedString118(UnkMovedString118&& other) {
        fn_710007eab4(reinterpret_cast<CRntString*>(this), reinterpret_cast<CRntString*>(&other));
    }
};

// Padding a move leaves alone.
struct UnkPad118 {
    u32 data;

    __attribute__((always_inline)) UnkPad118(UnkPad118&&) {}
};

extern void* g_unk_7101a1b5a0[];
using UnkString20_118 = UnkBufferString<0x20, g_unk_7101a1b5a0>;

struct UnkRecord118_b49 {
    u64 field_0x0;
    UnkMovedStrId118 field_0x8;
    UnkMovedStrId118 field_0x10;
    UnkMovedStrId118 field_0x18;
    UnkBlock118<4> field_0x20[7];
    UnkPad118 pad_0x3c;
    UnkBlock118<0x14> field_0x40;
    UnkPad118 pad_0x54;
    UnkMovedString118 field_0x58;
    UnkBlock118<4> field_0x88;
    UnkPad118 pad_0x8c;
    UnkBlock118<0xc> field_0x90;
    UnkPad118 pad_0x9c;
    UnkString20_118 field_0xa0;
    UnkBlock118<4> field_0x108;
    UnkPad118 pad_0x10c;
    UnkBlock118<6> field_0x110;

    UnkRecord118_b49(UnkRecord118_b49&& other) = default;
};
static_assert(sizeof(UnkRecord118_b49) == 0x118, "UnkRecord118_b49 layout");
static_assert(__builtin_offsetof(UnkRecord118_b49, field_0x110) == 0x110, "UnkRecord118_b49 layout");

struct UnkKeyed118Array_b49 {
    UnkRecord118_b49* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100eae334(UnkKeyed118Array_b49* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    auto* values = static_cast<UnkRecord118_b49*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkRecord118_b49), 8, self->flags));
    auto* keys = static_cast<u32*>(self->allocator->vf_0x40((u64)capacity * sizeof(u32), 8, self->flags));
    if (UnkRecord118_b49* src = self->values) {
        UnkRecord118_b49* dst = values;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            new (dst) UnkRecord118_b49(static_cast<UnkRecord118_b49&&>(*src));
            fn_7100ea7034(reinterpret_cast<UnkRecord118*>(src));
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        memcpy(keys, self->keys, (u64)self->count * sizeof(u32));
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = capacity;
}
