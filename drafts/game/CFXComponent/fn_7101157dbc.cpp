// CFXComponent's unit: removes one entry from the component's parallel arrays (entries of 0x20 bytes at
// +0x0, their string-id names at +0x8, count at +0x10): the name is released, the last entry and name move
// into the hole, and the entry now at that index is returned. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkFXListEntry_l157dbc {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkFXEntryList_l157dbc {
    UnkFXListEntry_l157dbc* values;  // 0x0
    CStrId* names;                   // 0x8
    u32 count;                       // 0x10
};

UnkFXListEntry_l157dbc* fn_7101157dbc(UnkFXEntryList_l157dbc* self, UnkFXListEntry_l157dbc* entry) {
    u32 index = entry - self->values;
    self->names[index].Reset();
    u32 last = self->count - 1;
    if (last != index) {
        self->values[index] = self->values[last];
        self->names[index].ReleaseAndTake(self->names[last]);
        self->names[last].Reset();
    }
    --self->count;
    return &self->values[index];
}
