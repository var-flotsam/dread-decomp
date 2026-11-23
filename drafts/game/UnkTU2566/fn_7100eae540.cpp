// Unit 2566 (tools/tu_map.py; hint CMinimapManager::CTunableMinimapManager): the copy assignment of the keyed
// array of 0x118-byte records and their u32 keys: the records destroyed, the capacity set to the other's
// count, the records copied one by one and the keys as a block. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2566.h"
#include "types.h"

using base::global::CStrId;

// As src/game/CMinimapManager/CTunableMinimapManager_Reserve118_b49.cpp and
// src/game_aca57c/fn_7100eae77c.cpp define them.
struct UnkKeyed118Array_b49;
struct UnkRecord118Table;
void fn_7100eae334(UnkKeyed118Array_b49* self, u32 capacity);
void fn_7100eae77c(UnkRecord118Table* self);

extern void* g_unk_7101a1b5a0[];

namespace {

// A string copied by fn_710007ea60.
struct UnkCopiedString_leae540 {
    base::UnkNamedEntry entry;

    UnkCopiedString_leae540(const UnkCopiedString_leae540& other) { fn_710007ea60(&entry, &other.entry); }
};

struct UnkWords5_leae540 {
    u32 field_0x0[5];
};
struct UnkWords3_leae540 {
    u32 field_0x0[3];
};
struct UnkHalves3_leae540 {
    u16 field_0x0[3];
};

struct UnkRecord118_leae540 {
    u64 field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    u32 field_0x20;
    u32 field_0x24;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
    u32 unk_0x3c;
    UnkWords5_leae540 field_0x40;
    u32 unk_0x54;
    UnkCopiedString_leae540 field_0x58;
    u32 field_0x88;
    u32 unk_0x8c;
    UnkWords3_leae540 field_0x90;
    UnkBufferString<0x20, g_unk_7101a1b5a0> field_0xa0;
    u32 field_0x108;
    u32 unk_0x10c;
    UnkHalves3_leae540 field_0x110;

    UnkRecord118_leae540(const UnkRecord118_leae540& other)
        : field_0x0(other.field_0x0), field_0x8(other.field_0x8), field_0x10(other.field_0x10),
          field_0x18(other.field_0x18), field_0x20(other.field_0x20), field_0x24(other.field_0x24),
          field_0x28(other.field_0x28), field_0x2c(other.field_0x2c), field_0x30(other.field_0x30),
          field_0x34(other.field_0x34), field_0x38(other.field_0x38), field_0x40(other.field_0x40),
          field_0x58(other.field_0x58), field_0x88(other.field_0x88), field_0x90(other.field_0x90),
          field_0xa0(other.field_0xa0), field_0x108(other.field_0x108), field_0x110(other.field_0x110) {}
};
static_assert(sizeof(UnkRecord118_leae540) == 0x118, "UnkRecord118_leae540 layout");

}  // namespace

// The records and their keys in parallel arrays, with one count and capacity.
struct UnkKeyed118_leae540 {
    UnkRecord118_leae540* records;
    u32* keys;
    u32 count;
    u32 capacity;
};

UnkKeyed118_leae540* fn_7100eae540(UnkKeyed118_leae540* self, const UnkKeyed118_leae540* other) {
    if (self == other)
        return self;
    if (self->records) {
        for (u64 i = 0; i < self->count; ++i)
            fn_7100ea7034(reinterpret_cast<UnkRecord118*>(&self->records[i]));
    }
    self->count = 0;
    if (other->count != self->capacity) {
        fn_7100eae77c(reinterpret_cast<UnkRecord118Table*>(self));
        fn_7100eae334(reinterpret_cast<UnkKeyed118Array_b49*>(self), other->count);
    }
    self->count = other->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkRecord118_leae540* record = &self->records[i];
        new (record) UnkRecord118_leae540(other->records[i]);
    }
    memcpy(self->keys, other->keys, (u64)other->count * sizeof(u32));
    self->count = other->count;
    return self;
}
