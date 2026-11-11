// Unit 2563 (minimap data): copy constructor of a container holding an array of 0x28-byte entries and
// an array of shared string-data pointers. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkMinimapEntry {
    u32 field_0x0[6];
    UnkStringData* name;
    u16 field_0x20;
    u8 pad_0x22[6];
};

struct UnkMinimapData {
    UnkMinimapEntry* entries;
    UnkStringData** names;
    u32 count;
    u32 pad_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u64 field_0x20;
};

void fn_7100e5ba88(void*, u32);

static inline void AddRef(UnkStringData* d) {
    if (&g_unk_7101d47108->empty != d)
        ++d->ref_count;
}

void fn_7100e5b944(UnkMinimapData* self, const UnkMinimapData* src) {
    self->names = nullptr;
    self->count = 0;
    self->pad_0x14 = 0;
    self->entries = nullptr;
    self->field_0x18 = src->field_0x18;
    self->field_0x1c = src->field_0x1c;
    self->field_0x20 = src->field_0x20;
    fn_7100e5ba88(self, src->count);
    self->count = src->count;
    u64 off = 0;
    for (u64 i = 0; i < self->count; ++i, off += sizeof(UnkMinimapEntry)) {
        const UnkMinimapEntry* s = reinterpret_cast<const UnkMinimapEntry*>(reinterpret_cast<const u8*>(src->entries) + off);
        UnkMinimapEntry* d = reinterpret_cast<UnkMinimapEntry*>(reinterpret_cast<u8*>(self->entries) + off);
        d->field_0x0[0] = s->field_0x0[0];
        d->field_0x0[1] = s->field_0x0[1];
        d->field_0x0[2] = s->field_0x0[2];
        d->field_0x0[3] = s->field_0x0[3];
        d->field_0x0[4] = s->field_0x0[4];
        d->field_0x0[5] = s->field_0x0[5];
        d->name = s->name;
        AddRef(s->name);
        d->field_0x20 = s->field_0x20;
    }
    self->count = src->count;
    for (u64 i = 0; i < self->count; ++i) {
        UnkStringData* n = src->names[i];
        self->names[i] = n;
        AddRef(n);
    }
}
