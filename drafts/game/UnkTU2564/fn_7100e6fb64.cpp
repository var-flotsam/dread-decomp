// CMinimapDef's unit (2564): the copy assignment of the keyed array of 0xc8-byte records (three string
// ids, a word, a string with 0x40 bytes of storage and plain values). Names and layouts are placeholders.
#include <new>

#include "../../../src/game_aca57c/UnkBufferString.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2564.h"
#include "types.h"

using base::global::CStrId;

// Five words copied together.
struct UnkWords5_le6fb64 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};

// The 0xc8-byte record.
struct UnkMinimapRecord_le6fb64 {
    CStrId field_0x0;
    u64 field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    UnkBufferString<0x40, g_unk_7101a1b678> field_0x20;
    u32 field_0xa8;
    u32 field_0xac;
    UnkWords5_le6fb64 field_0xb0;

    UnkMinimapRecord_le6fb64(const UnkMinimapRecord_le6fb64& other)
        : field_0x0(other.field_0x0), field_0x8(other.field_0x8), field_0x10(other.field_0x10),
          field_0x18(other.field_0x18), field_0x20(other.field_0x20) {
        field_0xa8 = other.field_0xa8;
        field_0xac = other.field_0xac;
        field_0xb0 = other.field_0xb0;
    }
    ~UnkMinimapRecord_le6fb64() { fn_7100e6c960(reinterpret_cast<char*>(this)); }
};
static_assert(sizeof(UnkMinimapRecord_le6fb64) == 0xc8, "UnkMinimapRecord_le6fb64 layout");

typedef base::global::CRntSmallDictionary<CStrId, UnkMinimapRecord_le6fb64> UnkMinimapRecords_le6fb64;

struct UnkIdsString40_b49;
void fn_7100e6fdf4(base::global::CRntSmallDictionary<CStrId, UnkIdsString40_b49>* self);
void fn_7100e6f940(void* array, u32 capacity);

// Empties the array, resizes it to the other's count, then copies the other's records and keys.
UnkMinimapRecords_le6fb64* fn_7100e6fb64(UnkMinimapRecords_le6fb64* self, const UnkMinimapRecords_le6fb64* other) {
    if (self != other) {
        self->Clear();
        if (other->count != self->capacity) {
            fn_7100e6fdf4(reinterpret_cast<base::global::CRntSmallDictionary<CStrId, UnkIdsString40_b49>*>(self));
            fn_7100e6f940(self, other->count);
        }
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            new (&self->values[i]) UnkMinimapRecord_le6fb64(other->values[i]);
        self->count = other->count;
        for (u64 i = 0; i < self->count; ++i)
            new (&self->keys[i]) CStrId(other->keys[i]);
    }
    return self;
}
