// Unit 2566 (CMinimapManager::CTunableMinimapManager): erases a record from the keyed array of
// 0x118-byte records by u32 key: the record is destroyed (fn_7100ea7034) and the last one, with its
// key, moved into its place; returns the record now at that place. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::CRntString;

// As include/game/UnkTU2566.h declares it.
struct UnkRecord118;
void fn_7100ea7034(UnkRecord118* self);

// The record and the array as src/game/CMinimapManager/CTunableMinimapManager_Reserve118_b49.cpp
// define them.

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
        CRntString_MoveConstruct(reinterpret_cast<CRntString*>(this), reinterpret_cast<CRntString*>(&other));
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
    UnkMovedStrId field_0x8;
    UnkMovedStrId field_0x10;
    UnkMovedStrId field_0x18;
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

// `pos` is a record of the array `self`; the declaration in include/game/UnkTU2566.h lacks the result.
void* fn_7100ea5d7c(char* self, void* pos) {
    auto* array = reinterpret_cast<UnkKeyed118Array_b49*>(self);
    u32 index = static_cast<UnkRecord118_b49*>(pos) - array->values;
    fn_7100ea7034(reinterpret_cast<UnkRecord118*>(&array->values[index]));
    u32 last = array->count - 1;
    if (last != index) {
        new (&array->values[index]) UnkRecord118_b49(static_cast<UnkRecord118_b49&&>(array->values[last]));
        fn_7100ea7034(reinterpret_cast<UnkRecord118*>(&array->values[array->count - 1]));
        array->keys[index] = array->keys[array->count - 1];
        last = array->count - 1;
    }
    array->count = last;
    return &array->values[index];
}
