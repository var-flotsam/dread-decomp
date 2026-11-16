// CMaterialFXComponent unit (tools/tu_map.py): stops the effects a request names. With a named id the
// id's index is looked up and the first kind-1 entry for it and the request's slot and kind is removed;
// with no name every index is tried in turn. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU218.h"
#include "types.h"

using base::global::CStrId;

struct UnkMaterialFX_b49a4;
void fn_7100926494(UnkMaterialFX_b49a4* self, s32 index);
bool fn_710080fb08(struct UnkKey* key);

struct UnkMaterialFXEntry_l926cd4 {
    u32 field_0x0;
    u32 slot;  // 0x4
    u32 kind;  // 0x8
    u8 pad_0xc[0x24];
    u8 key[0x10];  // 0x30
    u32 index;     // 0x40
    u32 type;      // 0x44
};

struct UnkMaterialFXTable_l926cd4 {
    u8 pad_0x0[0x2c];
    u32 count;  // 0x2c
};

struct UnkMaterialFXLookup_l926cd4 {
    u8 pad_0x0[0x78];
    UnkMaterialFXTable_l926cd4* table;  // 0x78
};

struct UnkMaterialFXSource_l926cd4 {
    u8 pad_0x0[0x138];
    UnkMaterialFXLookup_l926cd4* lookup;  // 0x138
};

struct UnkMaterialFXHolder_l926cd4 {
    u8 pad_0x0[0x50];
    UnkMaterialFXSource_l926cd4* source;  // 0x50
};

struct UnkMaterialFXOwner_l926cd4 {
    u8 pad_0x0[0x228];
    UnkMaterialFXHolder_l926cd4* holder;  // 0x228
};

struct UnkMaterialFXComponent_l926cd4 {
    u8 pad_0x0[0x30];
    UnkMaterialFXOwner_l926cd4* owner;  // 0x30
    u8 pad_0x38[0x10];
    u32 count;  // 0x48
    u8 pad_0x4c[0x4];
    UnkMaterialFXEntry_l926cd4* entries[50];  // 0x50
};

struct UnkMaterialFXRequest_l926cd4 {
    u8 pad_0x0[0x10];
    CStrId id;  // 0x10
    u32 slot;   // 0x18
    u32 kind;   // 0x1c
};

// Removes the first kind-1 entry for this index, slot and kind, if its key is still live.
static inline void RemoveFirst(UnkMaterialFXComponent_l926cd4* self, u32 index, u32 slot, u32 kind) {
    for (u32 i = 0; i < self->count; ++i) {
        UnkMaterialFXEntry_l926cd4* e = self->entries[i];
        if (e->type == 1 && e->index == index && e->slot == slot && e->kind == kind) {
            if (fn_710080fb08(reinterpret_cast<UnkKey*>(e->key)))
                fn_7100926494(reinterpret_cast<UnkMaterialFX_b49a4*>(self), i);
            return;
        }
    }
}

void fn_7100926cd4(UnkMaterialFXComponent_l926cd4* self, const UnkMaterialFXRequest_l926cd4* request) {
    u32 slot = request->slot;
    u32 kind = request->kind;
    CStrId id = request->id;
    UnkMaterialFXHolder_l926cd4* holder = self->owner->holder;
    if (!holder || !holder->source)
        return;
    UnkMaterialFXSource_l926cd4* source = holder->source;
    if (id.data != &g_unk_7101d47108->empty) {
        u32 index = -1;
        u64 key = id.data->Key();
        if (fn_7100199d88(source->lookup->table, &key, &index))
            RemoveFirst(self, index, slot, kind);
    } else {
        u32 n = source->lookup->table->count;
        for (u32 index = 0; index < n; ++index)
            RemoveFirst(self, index, slot, kind);
    }
}
