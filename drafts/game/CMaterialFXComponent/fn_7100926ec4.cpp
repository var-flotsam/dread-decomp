// CMaterialFXComponent unit (tools/tu_map.py): stops the effects a request names on one part of the world.
// The part is found by its name's hash (fn_7101152b90); with a named id the part gives the id's index and
// the first kind-0 entry for that part, index, slot and kind is removed; with no name every index the
// part has is tried in turn. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CFXComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkKey;
struct UnkMaterialFX_b49a4;
void fn_7100926494(UnkMaterialFX_b49a4* self, s32 index);
bool fn_710080fb08(UnkKey* key);

struct UnkMaterialFXPart_l926ec4;

struct UnkMaterialFXPartVtable_l926ec4 {
    void* pad_0x0[30];
    u32 (*IndexCount)(UnkMaterialFXPart_l926ec4* self);  // 0xf0
    void* pad_0xf8[2];
    bool (*FindIndex)(UnkMaterialFXPart_l926ec4* self, u64 hash, u32* index);  // 0x108
};

struct UnkMaterialFXPart_l926ec4 {
    const UnkMaterialFXPartVtable_l926ec4* vtable;
};

struct UnkMaterialFXKey_l926ec4 {
    u64 field_0x0;
    CStrId part;  // 0x8
    u32 index;    // 0x10
    u32 type;     // 0x14
};

struct UnkMaterialFXEntry_l926ec4 {
    u32 field_0x0;
    u32 slot;  // 0x4
    u32 kind;  // 0x8
    u8 pad_0xc[0x24];
    UnkMaterialFXKey_l926ec4 key;  // 0x30
};

struct UnkMaterialFXOwner_l926ec4 {
    u8 pad_0x0[0x280];
    UnkWorld* world;  // 0x280
};

struct UnkMaterialFXComponent_l926ec4 {
    u8 pad_0x0[0x30];
    UnkMaterialFXOwner_l926ec4* owner;  // 0x30
    u8 pad_0x38[0x10];
    u32 count;  // 0x48
    u8 pad_0x4c[0x4];
    UnkMaterialFXEntry_l926ec4* entries[50];  // 0x50
};

struct UnkMaterialFXRequest_l926ec4 {
    u8 pad_0x0[0x10];
    CStrId part;  // 0x10
    CStrId id;    // 0x18
    u32 slot;     // 0x20
    u32 kind;     // 0x24
};

// Removes the first kind-0 entry for this part, index, slot and kind, if its key is still live.
static inline void RemoveFirst(UnkMaterialFXComponent_l926ec4* self, CStrId part, u32 index, u32 slot,
                               u32 kind) {
    for (u32 i = 0; i < self->count; ++i) {
        UnkMaterialFXEntry_l926ec4* e = self->entries[i];
        if (e->key.type == 0 && e->key.part.data == part.data && e->key.index == index && e->slot == slot &&
            e->kind == kind) {
            if (fn_710080fb08(reinterpret_cast<UnkKey*>(&e->key)))
                fn_7100926494(reinterpret_cast<UnkMaterialFX_b49a4*>(self), i);
            return;
        }
    }
}

void fn_7100926ec4(UnkMaterialFXComponent_l926ec4* self, const UnkMaterialFXRequest_l926ec4* request) {
    u32 slot = request->slot;
    u32 kind = request->kind;
    CStrId id = request->id;
    UnkWorld* world = self->owner->world;
    if (!world)
        return;
    UnkMaterialFXPart_l926ec4* part =
        reinterpret_cast<UnkMaterialFXPart_l926ec4*>(fn_7101152b90(world, request->part.data->Key()));
    if (!part)
        return;
    if (id.data != &g_unk_7101d47108->empty) {
        u32 index = -1;
        if (part->vtable->FindIndex(part, id.data->Key(), &index))
            RemoveFirst(self, request->part, index, slot, kind);
    } else {
        u32 n = part->vtable->IndexCount(part);
        for (u32 index = 0; index < n; ++index)
            RemoveFirst(self, request->part, index, slot, kind);
    }
}
