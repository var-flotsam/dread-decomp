// CMaterialFXComponent unit (tools/tu_map.py): records a material colour change. The change's key is bound
// to the component; while it is live, the record for the same key, slot and kind is overwritten (or a new
// one is added to the table of 50), and when its time is about zero the colour is applied to the key's slot
// at once (kind 0: +0x240 and its target +0x2e0; kind 1: +0x14c and +0x1f0). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "game/CMaterialFXComponent.h"
#include "game/UnkTU1273.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;

// A colour whose assignment is the out-of-line one (fn_710007b518).
struct UnkMaterialFXColor_l926aa0 : CVector4D {
    UnkMaterialFXColor_l926aa0& operator=(const UnkMaterialFXColor_l926aa0& other) {
        Assign(*this, other);
        return *this;
    }
};

struct UnkMaterialFXKey_l926aa0 {
    u64 field_0x0;
    CStrId id;  // 0x8
    u32 index;  // 0x10
    u32 type;   // 0x14
};

struct UnkMaterialFXRecord_l926aa0 {
    u32 field_0x0;
    u32 slot;  // 0x4
    u32 kind;  // 0x8
    UnkMaterialFXColor_l926aa0 color;  // 0xc
    UnkMaterialFXColor_l926aa0 field_0x1c;
    UnkMaterialFXKey_l926aa0 key;  // 0x30
    u32 field_0x48;
    f32 field_0x4c;
    f32 time;  // 0x50
};
static_assert(sizeof(UnkMaterialFXRecord_l926aa0) == 0x58, "UnkMaterialFXRecord_l926aa0 size");

struct UnkMaterialFXTarget_l926aa0 {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkMaterialFXSlots_l926aa0 {
    u8 pad_0x0[0x14c];
    CVector4D field_0x14c[10];
    UnkMaterialFXTarget_l926aa0* field_0x1f0[10];
    CVector4D field_0x240[10];
    UnkMaterialFXTarget_l926aa0* field_0x2e0[10];
};

struct UnkMaterialFXComponent_l926aa0 {
    u8 pad_0x0[0x48];
    u32 count;  // 0x48
    u8 pad_0x4c[0x4];
    UnkMaterialFXRecord_l926aa0* records[50];  // 0x50
};

static inline UnkKey* Key(UnkMaterialFXRecord_l926aa0* record) {
    return reinterpret_cast<UnkKey*>(&record->key);
}

static inline UnkMaterialFXSlots_l926aa0* Slots(UnkKey* key) {
    return reinterpret_cast<UnkMaterialFXSlots_l926aa0*>(fn_710080fa28(key));
}

void fn_7100926aa0(UnkMagnetFx_b0086* fx, UnkMagnetColorChange_b0086* change_) {
    UnkMaterialFXComponent_l926aa0* self = reinterpret_cast<UnkMaterialFXComponent_l926aa0*>(fx);
    UnkMaterialFXRecord_l926aa0* change = reinterpret_cast<UnkMaterialFXRecord_l926aa0*>(change_);
    fn_710080fb00(Key(change), self);
    if (!fn_710080fb08(Key(change)))
        return;

    u32 index = -1;
    for (u32 i = 0; i < self->count; ++i) {
        UnkMaterialFXRecord_l926aa0* r = self->records[i];
        if (AreKindedKeysEqual(Key(r), Key(change)) && r->slot == change->slot && r->kind == change->kind)
            index = i;
    }
    if (index == (u32)-1)
        index = self->count++;
    if (self->count >= 50)
        return;

    UnkMaterialFXRecord_l926aa0* r = self->records[index];
    UnkKey* key = Key(r);
    *r = *change;
    f32 time = r->time;
    if (time + 0.001f >= 0.0f && time - 0.001f <= 0.0f) {
        switch (r->kind) {
        case 0: {
            UnkMaterialFXSlots_l926aa0* s = Slots(key);
            u32 n = r->slot;
            Assign(s->field_0x240[n], r->color);
            if (UnkMaterialFXTarget_l926aa0* t = s->field_0x2e0[n])
                *t->value = r->color;
            break;
        }
        case 1: {
            UnkMaterialFXSlots_l926aa0* s = Slots(key);
            u32 n = r->slot;
            Assign(s->field_0x14c[n], r->color);
            if (UnkMaterialFXTarget_l926aa0* t = s->field_0x1f0[n])
                *t->value = r->color;
            break;
        }
        }
    }
}
