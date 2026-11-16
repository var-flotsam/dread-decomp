// CBreakableTileGroupComponent's unit (tools/tu_map.py): removes one record from an array of 0x48-byte
// records (each a name id and an array of polymorphic 0x40-byte entries): it is destroyed and the later
// ones move down. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// An entry of a record's array; destroyed through its vtable.
struct UnkTileEntry_l758e8c {
    virtual ~UnkTileEntry_l758e8c();
    u8 pad_0x8[0x38];
};

// The record's entries, freed with the record.
struct UnkTileEntries_l758e8c {
    base::global::CRntVector<UnkTileEntry_l758e8c> entries;

    UnkTileEntries_l758e8c(UnkTileEntries_l758e8c&&) = default;
    ~UnkTileEntries_l758e8c() { entries.Free(); }
};

struct UnkTileRecord_l758e8c {
    u64 field_0x0;
    u32 field_0x8;
    CStrId id;                       // 0x10
    UnkTileEntries_l758e8c entries;  // 0x18
    bool field_0x40;

    UnkTileRecord_l758e8c(UnkTileRecord_l758e8c&& other)
        : field_0x0(other.field_0x0), field_0x8(other.field_0x8), id(other.id),
          entries(static_cast<UnkTileEntries_l758e8c&&>(other.entries)), field_0x40(other.field_0x40) {}
};
static_assert(sizeof(UnkTileRecord_l758e8c) == 0x48, "UnkTileRecord_l758e8c layout");

using UnkTileRecords_l758e8c = base::global::CRntVector<UnkTileRecord_l758e8c>;

// Destroys the record at `index` and moves the later ones down by one.
void fn_7100758e8c(UnkTileRecords_l758e8c* self, u32 index) {
    if (index < self->count) {
        self->data[index].~UnkTileRecord_l758e8c();
        for (u64 i = index; i < self->count - 1; ++i) {
            UnkTileRecord_l758e8c* at = &self->data[i];
            new (at) UnkTileRecord_l758e8c(static_cast<UnkTileRecord_l758e8c&&>(at[1]));
            (&self->data[i])[1].~UnkTileRecord_l758e8c();
        }
        self->count = self->count - 1;
    }
}
