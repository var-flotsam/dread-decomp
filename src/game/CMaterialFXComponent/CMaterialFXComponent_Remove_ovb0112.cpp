// CMaterialFXComponent's unit (tools/tu_map.py): removes the entry at `index` of the fixed table of 50
// (+0x48 count, +0x50 entries): when its key is still live (fn_710080fb08) the second value at +0x1c goes
// back to the key's slot (kind 0: +0x240 and its target +0x2e0; kind 1: +0x14c and +0x1f0), then the entry
// swaps places with the last one and the count drops. The entry layout is fn_7100926874.cpp's. Names and
// layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkTU1273.h"
#include "types.h"

using base::math::CVector4D;

struct UnkKey;
struct UnkSlots;
bool fn_710080fb08(UnkKey* key);

struct UnkMaterialFXTarget_ovb0112 {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkMaterialFXSlots_ovb0112 {
    u8 pad_0x0[0x14c];
    CVector4D field_0x14c[10];
    UnkMaterialFXTarget_ovb0112* field_0x1f0[10];
    CVector4D field_0x240[10];
    UnkMaterialFXTarget_ovb0112* field_0x2e0[10];
};

struct UnkMaterialFXEntry_ovb0112 {
    u32 field_0x0;
    u32 slot;  // 0x4
    u32 kind;  // 0x8
    CVector4D field_0xc;
    CVector4D field_0x1c;
    u8 pad_0x2c[0x4];
    u8 key[0x18];  // 0x30
};

struct UnkMaterialFX_b49a4;

struct UnkMaterialFXList_ovb0112 {
    u8 pad_0x0[0x48];
    u32 count;
    u8 pad_0x4c[0x4];
    UnkMaterialFXEntry_ovb0112* entries[50];
};

template <class K>
static inline UnkMaterialFXSlots_ovb0112* Slots(K& key) {
    return reinterpret_cast<UnkMaterialFXSlots_ovb0112*>(fn_710080fa28(reinterpret_cast<UnkKey*>(&key)));
}

void fn_7100926494(UnkMaterialFX_b49a4* self_, s32 index_) {
    UnkMaterialFXList_ovb0112* self = reinterpret_cast<UnkMaterialFXList_ovb0112*>(self_);
    u32 index = index_;
    if (index >= 50 || index >= self->count)
        return;
    UnkMaterialFXEntry_ovb0112* e = self->entries[index];
    if (fn_710080fb08(reinterpret_cast<UnkKey*>(e->key))) {
        switch (e->kind) {
        case 0: {
            UnkMaterialFXSlots_ovb0112* s = Slots(e->key);
            u32 n = e->slot;
            fn_710007b518(&s->field_0x240[n], &e->field_0x1c);
            if (UnkMaterialFXTarget_ovb0112* t = s->field_0x2e0[n])
                __builtin_memcpy(t->value, &e->field_0x1c, sizeof(CVector4D));
            break;
        }
        case 1: {
            UnkMaterialFXSlots_ovb0112* s = Slots(e->key);
            u32 n = e->slot;
            fn_710007b518(&s->field_0x14c[n], &e->field_0x1c);
            if (UnkMaterialFXTarget_ovb0112* t = s->field_0x1f0[n])
                __builtin_memcpy(t->value, &e->field_0x1c, sizeof(CVector4D));
            break;
        }
        }
    }
    u32 last = self->count - 1;
    UnkMaterialFXEntry_ovb0112* moved = self->entries[last];
    self->entries[last] = self->entries[index];
    self->entries[index] = moved;
    self->count = last;
}
